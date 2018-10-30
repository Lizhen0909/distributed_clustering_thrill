#pragma once

#include <thrill/api/cache.hpp>
#include <thrill/api/collapse.hpp>
#include <thrill/api/inner_join.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/size.hpp>
#include <thrill/api/sum.hpp>
#include <thrill/api/write_lines.hpp>
#include <thrill/api/write_binary.hpp>
#include <thrill/api/zip_with_index.hpp>

#include <vector>
#include <string>
#include <cstdlib>
#include <sparsepp/spp.h>

#include "util/thrill/input.hpp"
#include "util/util.hpp"
#include "util/logging.hpp"
#include "algo/thrill/clustering_quality.hpp"
#include "algo/thrill/contraction.hpp"

namespace Louvain {

template<class NodeType, class F>
auto louvain(const DiaNodeGraph<NodeType>& graph, Logging::Id algorithm_run_id, uint32_t seed, const F& local_moving, uint32_t level = 0) {
  Logging::Id level_logging_id = 0;
  if (graph.nodes.context().my_rank() == 0) {
    level_logging_id = Logging::getUnusedId();
  }

  if (level == 0) {
    graph.nodes.Execute();
    if (graph.nodes.context().my_rank() == 0) {
      Logging::report_timestamp("algorithm_run", algorithm_run_id, "start_ts");
    }
  }

  auto lm_result = local_moving(graph, seed, level_logging_id);

  if (lm_result.second) {
    if (graph.nodes.context().my_rank() == 0) {
      Logging::report_timestamp("algorithm_run", algorithm_run_id, "done_ts");
    }
    return lm_result.first.Map([](const std::pair<NodeType, ClusterId>& node_cluster) { return NodeCluster(node_cluster.first.id, node_cluster.second); }).Collapse();
  } else {
    if (graph.nodes.context().my_rank() == 0) {
      Logging::report_timestamp("algorithm_level", level_logging_id, "local_moving_done_ts");
    }
  }

  auto clusters_with_nodes = lm_result.first
    .template FoldByKey<std::vector<NodeType>>(thrill::NoDuplicateDetectionTag,
      [](const std::pair<NodeType, ClusterId>& node_cluster) { return node_cluster.second; },
      [](std::vector<NodeType>&& acc, const std::pair<NodeType, ClusterId>& node_cluster) {
        acc.push_back(node_cluster.first);
        return std::move(acc);
      })
    .ZipWithIndex([](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, ClusterId new_id) { return std::make_pair(new_id, cluster_nodes.second); });

  size_t cluster_count = clusters_with_nodes.Keep().Size();

  if (clusters_with_nodes.context().my_rank() == 0) {
    Logging::report("algorithm_level", level_logging_id, "algorithm_run_id", algorithm_run_id);
    Logging::report("algorithm_level", level_logging_id, "node_count", graph.node_count);
    Logging::report("algorithm_level", level_logging_id, "cluster_count", cluster_count);
    Logging::report("algorithm_level", level_logging_id, "level", level);
  }

  if (graph.node_count == cluster_count) {
    if (graph.nodes.context().my_rank() == 0) {
      Logging::report_timestamp("algorithm_run", algorithm_run_id, "done_ts");
    }
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

  auto clusters_with_node_ids = clusters_with_nodes
    .Map(
      [](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes) {
        std::vector<NodeId> ids;
        ids.reserve(cluster_nodes.second.size());
        for (const auto& node : cluster_nodes.second) {
          ids.push_back(node.id);
        }
        return std::make_pair(cluster_nodes.first, ids);
      })
    .Cache();

  auto half_meta_edges = clusters_with_nodes
    // Build Meta Graph
    .template FlatMap<WeightedEdge>(
      [](const std::pair<ClusterId, std::vector<NodeType>>& cluster_nodes, auto emit) {
        spp::sparse_hash_map<NodeId, Weight> cluster_links;

        for (const NodeType& node : cluster_nodes.second) {
          for (const typename NodeType::LinkType& link : node.links) {
            cluster_links[link.target] += link.getWeight();
          }
        }

        for (const auto& cluster_link : cluster_links) {
          emit(WeightedEdge { cluster_link.first, cluster_nodes.first, cluster_link.second });
        }
      });
  auto meta_nodes = edgesToNodes(half_meta_edges, graph.node_count)
    .Zip(thrill::NoRebalanceTag, mapping,
      [](const NodeWithWeightedLinks& node, const NodeCluster& node_cluster) {
        assert(node.id == node_cluster.first);
        return std::make_pair(node_cluster.second, node);
      })
    .template GroupToIndex<std::pair<ClusterId, std::vector<NodeWithWeightedLinks>>>(
      [](const std::pair<ClusterId, NodeWithWeightedLinks>& cluster_node) { return cluster_node.first; },
      [](auto& iterator, const ClusterId cluster) {
        std::vector<NodeWithWeightedLinks> nodes;
        while (iterator.HasNext()) {
          nodes.push_back(iterator.Next().second);
        }
        return std::make_pair(cluster, nodes);
      }, cluster_count)
    .Map(
      [cluster_count](const std::pair<ClusterId, std::vector<NodeWithWeightedLinks>>& cluster_nodes) {
        Weight cluster_weight = 0;
        for (const NodeWithWeightedLinks& node : cluster_nodes.second) {
          for (const WeightedEdgeTarget& link : node.links) {
            cluster_weight += link.weight;
          }
        }
        auto links = Contraction::sort_and_merge_links(cluster_nodes.second, cluster_nodes.first, cluster_count);
        return NodeWithWeightedLinks { cluster_nodes.first, std::move(links), cluster_weight };
      })
    .Cache();

  assert(nodesToEdges(meta_nodes.Keep()).Map([](const WeightedEdge& edge) { return edge.getWeight(); }).Sum() / 2 == graph.total_weight);
  size_t num_meta_edges = nodesToEdges(meta_nodes.Keep()).Size() / 2;

  if (clusters_with_nodes.context().my_rank() == 0) {
    Logging::report("algorithm_level", level_logging_id, "meta_edge_count", num_meta_edges);
    Logging::report_timestamp("algorithm_level", level_logging_id, "contraction_done_ts");
  }

  auto meta_result = louvain(DiaNodeGraph<NodeWithWeightedLinks> { meta_nodes, cluster_count, graph.total_weight }, algorithm_run_id, seed, local_moving, level + 1);
  return meta_result
    .Zip(clusters_with_node_ids,
      [](const NodeCluster& meta_cluster, const std::pair<ClusterId, std::vector<NodeId>>& cluster_node_ids) {
        assert(meta_cluster.first == cluster_node_ids.first);
        return std::make_pair(meta_cluster.second, cluster_node_ids.second);
      })
    .template FlatMap<NodeCluster>(
      [](const std::pair<ClusterId, std::vector<NodeId>>& cluster_node_ids, auto emit) {
        for (const NodeId& id : cluster_node_ids.second) {
          emit(NodeCluster(id, cluster_node_ids.first));
        }
      })
    .ReducePairToIndex([](const ClusterId id, const ClusterId) { assert(false); return id; }, graph.node_count);
}

template<class F>
auto performAndEvaluate(int argc, char const *argv[], const std::string& algo, const F& run) {
  return thrill::Run([&](thrill::Context& context) {
    context.enable_consume();

    auto graph = Input::readToNodeGraph(argv[1], context);

    uint32_t seed = 42;
    if (getenv("SEED")) {
      seed = std::stoi(getenv("SEED"));
    }

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
      Logging::report("program_run", program_run_logging_id, "seed", seed);
      if (getenv("MOAB_JOBID")) {
        Logging::report("program_run", program_run_logging_id, "job_id", getenv("MOAB_JOBID"));
      }

      #if defined(FIXED_RATIO)
        Logging::report("program_run", program_run_logging_id, "local_moving_node_ratio", FIXED_RATIO);
      #else
        Logging::report("program_run", program_run_logging_id, "local_moving_node_ratio", "dynamic");
      #endif
      #if defined(STOP_MOVECOUNT)
        Logging::report("program_run", program_run_logging_id, "local_moving_stopping_criterion", "moved_count");
      #else
        Logging::report("program_run", program_run_logging_id, "local_moving_stopping_criterion", "cluster_count");
      #endif
      #if defined(MAX_ITERATIONS)
        Logging::report("program_run", program_run_logging_id, "max_iterations", MAX_ITERATIONS);
      #endif
      #if defined(SWITCH_TO_SEQ)
        Logging::report("program_run", program_run_logging_id, "switch_to_seq", true);
      #else
        Logging::report("program_run", program_run_logging_id, "switch_to_seq", false);
      #endif
      #if defined(NO_CONTRACTION)
        Logging::report("program_run", program_run_logging_id, "contraction", false);
      #else
        Logging::report("program_run", program_run_logging_id, "contraction", true);
      #endif
    }

    Logging::Id algorithm_run_id = 0;
    if (context.my_rank() == 0) {
      algorithm_run_id = Logging::getUnusedId();
      Logging::report("algorithm_run", algorithm_run_id, "program_run_id", program_run_logging_id);
      Logging::report("algorithm_run", algorithm_run_id, "algorithm", algo);
    }
    auto node_clusters = run(graph, algorithm_run_id, seed);
    node_clusters.Execute();
    if (argc > 2) {
      auto clustering_input = Logging::parse_input_with_logging_id(argv[2]);
      node_clusters.Keep().WriteBinary(clustering_input.first);
    }

    size_t cluster_count = node_clusters.Keep().Map([](const NodeCluster& node_cluster) { return node_cluster.second; }).Uniq().Size();
    
    auto fname =std::string("output.cluster");
    node_clusters.Keep().Map([](const NodeCluster& node_cluster) { return std::to_string(node_cluster.first) + " " + std::to_string(node_cluster.second); }).WriteLines(fname);
    
    auto eval_graph = Input::readToNodeGraph(argv[1], context);
    eval_graph.nodes.Keep();
    double modularity = ClusteringQuality::modularity(eval_graph, node_clusters.Keep());
    double map_eq = ClusteringQuality::mapEquation(eval_graph, node_clusters);

    if (context.my_rank() == 0) {
      if (argc > 2) {
        auto clustering_input = Logging::parse_input_with_logging_id(argv[2]);
        Logging::report("clustering", clustering_input.second, "path", clustering_input.first);
        Logging::report("clustering", clustering_input.second, "source", "computation");
        Logging::report("clustering", clustering_input.second, "algorithm_run_id", algorithm_run_id);
        Logging::report("clustering", clustering_input.second, "modularity", modularity);
        Logging::report("clustering", clustering_input.second, "map_equation", map_eq);
        Logging::report("clustering", clustering_input.second, "cluster_count", cluster_count);
      } else {
        Logging::Id clusters_logging_id = Logging::getUnusedId();
        Logging::report("clustering", clusters_logging_id, "source", "computation");
        Logging::report("clustering", clusters_logging_id, "algorithm_run_id", algorithm_run_id);
        Logging::report("clustering", clusters_logging_id, "modularity", modularity);
        Logging::report("clustering", clusters_logging_id, "map_equation", map_eq);
        Logging::report("clustering", clusters_logging_id, "cluster_count", cluster_count);
      }
    }
  });
}

} // Louvain
