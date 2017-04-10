#pragma once

#include <thrill/api/cache.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/inner_join.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/size.hpp>
#include <thrill/api/sum.hpp>
#include <thrill/api/write_binary.hpp>
#include <thrill/api/zip_with_index.hpp>

#include <vector>

#include "input.hpp"
#include "util.hpp"
#include "logging.hpp"

namespace Louvain {

template<class NodeType, class F>
thrill::DIA<NodeCluster> louvain(const DiaNodeGraph<NodeType>& graph, const F& local_moving) {
  using EdgeType = typename NodeType::LinkType::EdgeType;

  auto clusters_with_nodes = local_moving(graph)
    .template GroupByKey<std::pair<ClusterId, std::vector<NodeType>>>(
      [](const std::pair<NodeType, ClusterId>& node_cluster) { return node_cluster.second; },
      [](auto& iterator, const ClusterId cluster) {
        std::vector<NodeType> nodes;
        while (iterator.HasNext()) {
          nodes.push_back(iterator.Next().first);
        }
        return std::make_pair(cluster, nodes);
      })
    .ZipWithIndex([](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, ClusterId new_id) { return std::make_pair(new_id, cluster_nodes.second); });

  size_t cluster_count = clusters_with_nodes.Keep().Size();

  if (graph.node_count == cluster_count) {
    return clusters_with_nodes.Map([](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes) { return NodeCluster(cluster_nodes.first, cluster_nodes.first); }).Collapse();
  }

  auto mapping = clusters_with_nodes
    .template FlatMap<NodeCluster>(
      [](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, auto emit) {
        for (const NodeType& node : cluster_nodes.second) {
          emit(NodeCluster(node.id, cluster_nodes.first));
        }
      })
    .ReduceToIndex(
      [](const NodeCluster& node_cluster) -> size_t { return node_cluster.first; },
      [](const NodeCluster& node_cluster, const NodeCluster&) { assert(false); return node_cluster; },
      graph.node_count);

  auto half_meta_edges = clusters_with_nodes
    .Keep()
    // Build Meta Graph
    .template FlatMap<EdgeType>(
      [](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, auto emit) {
        for (const NodeType& node : cluster_nodes.second) {
          for (const typename NodeType::LinkType& link : node.links) {
            EdgeType edge = EdgeType::fromLink(cluster_nodes.first, link);
            edge.flip();
            emit(edge);
          }
        }
      });
  auto meta_graph_edges = edgesToNodes(half_meta_edges, graph.node_count)
    .Zip(thrill::NoRebalanceTag, mapping,
      [](const NodeType& node, const NodeCluster& node_cluster) {
        assert(node.id == node_cluster.first);
        return std::make_pair(node_cluster.second, node);
      })
    .template FlatMap<EdgeType>(
      [](const std::pair<ClusterId, NodeType>& cluster_node, auto emit) {
        for (const typename NodeType::LinkType& link : cluster_node.second.links) {
          emit(EdgeType::fromLink(cluster_node.first, link));
        }
      })
    .Map([](const EdgeType& edge) { return WeightedEdge { edge.tail, edge.head, edge.getWeight() }; })
    .ReduceByKey(
      [](const WeightedEdge& edge) { return Util::combine_u32ints(edge.tail, edge.head); },
      [](const WeightedEdge& edge1, const WeightedEdge& edge2) { return WeightedEdge { edge1.tail, edge1.head, edge1.weight + edge2.weight }; })
    // turn loops into forward and backward arc
    .template FlatMap<WeightedEdge>(
      [](const WeightedEdge & edge, auto emit) {
        if (edge.tail == edge.head) {
          assert(edge.weight % 2 == 0);
          emit(WeightedEdge { edge.tail, edge.head, edge.weight / 2 });
          emit(WeightedEdge { edge.tail, edge.head, edge.weight / 2 });
        } else {
          emit(edge);
        }
      })
    .Cache();

  auto nodes = edgesToNodes(meta_graph_edges, cluster_count).Collapse();
  assert(meta_graph_edges.Map([](const WeightedEdge& edge) { return edge.getWeight(); }).Sum() / 2 == graph.total_weight);

  return louvain(DiaNodeGraph<NodeWithWeightedLinks> { nodes, cluster_count, graph.total_weight }, local_moving)
    .Zip(clusters_with_nodes,
      [](const NodeCluster& meta_cluster, const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes) {
        assert(meta_cluster.first == cluster_nodes.first);
        return std::make_pair(meta_cluster.second, cluster_nodes.second);
      })
    .template FlatMap<NodeCluster>(
      [](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, auto emit) {
        for (const NodeType& node : cluster_nodes.second) {
          emit(NodeCluster(node.id, cluster_nodes.first));
        }
      })
    .ReducePairToIndex([](const ClusterId id, const ClusterId) { assert(false); return id; }, graph.node_count);
}

template<class F>
auto performAndEvaluate(int argc, char const *argv[], const std::string& algo, const F& run) {
  return thrill::Run([&](thrill::Context& context) {
    context.enable_consume();

    auto graph = Input::readToNodeGraph(argv[1], context);

    Logging::Id program_run_logging_id;
    if (context.my_rank() == 0) {
      program_run_logging_id = Logging::getUnusedId();
      Logging::report("program_run", program_run_logging_id, "binary", argv[0]);
      Logging::report("program_run", program_run_logging_id, "hosts", context.num_hosts());
      Logging::report("program_run", program_run_logging_id, "total_workers", context.num_workers());
      Logging::report("program_run", program_run_logging_id, "workers_per_host", context.workers_per_host());
      Logging::report("program_run", program_run_logging_id, "graph", argv[1]);
      Logging::report("program_run", program_run_logging_id, "node_count", graph.node_count);
      Logging::report("program_run", program_run_logging_id, "edge_count", graph.total_weight);
    }

    auto node_clusters = run(graph);
    node_clusters.Execute();

    if (context.my_rank() == 0) {
      Logging::Id algorithm_run_id = Logging::getUnusedId();
      Logging::report("algorithm_run", algorithm_run_id, "program_run_id", program_run_logging_id);
      Logging::report("algorithm_run", algorithm_run_id, "algorithm", algo);

      if (argc > 2) {
        auto clustering_input = Logging::parse_input_with_logging_id(argv[2]);
        Logging::report("clustering", clustering_input.second, "source", "computation");
        Logging::report("clustering", clustering_input.second, "algorithm_run_id", algorithm_run_id);
      } else {
        Logging::Id clusters_logging_id = Logging::getUnusedId();
        Logging::report("clustering", clusters_logging_id, "source", "computation");
        Logging::report("clustering", clusters_logging_id, "algorithm_run_id", algorithm_run_id);
      }
    }

    if (argc > 2) {
      auto clustering_input = Logging::parse_input_with_logging_id(argv[2]);
      node_clusters.WriteBinary(clustering_input.first);
    }
  });
}

} // Louvain
