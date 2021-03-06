#pragma once

#include "data/graph.hpp"
#include "data/cluster_store.hpp"
#include "util/io.hpp"
#include "util/logging.hpp"

#include <memory>
#include <iostream>
#include <chrono>
#include <set>

#include <tlx/cmdline_parser.hpp>

using PartitionInput = std::pair<std::string, std::string>;

class Input {
private:

  int exit = 0;
  bool initialized = false;
  Logging::Id run_id;

  std::unique_ptr<Graph> graph;
  std::unique_ptr<ClusterStore> ground_proof;
  std::unordered_map<Graph::NodeId, Graph::NodeId> id_mapping;

  std::string graph_file = "";
  std::string ground_proof_file = "";
  std::string output_file = "";
  std::vector<std::string> partitions_strings;
  std::vector<PartitionInput> partitions;
  bool snap_format = false;
  bool binary_format = false;
  unsigned seed;

public:
  Input(int argc, char const *argv[], Logging::Id run_id) :
    run_id(run_id),
    seed(std::chrono::system_clock::now().time_since_epoch().count()) {

    tlx::CmdlineParser cp;

    cp.add_string('g', "ground-proof", "file", ground_proof_file, "A ground proof clustering to compare to");
    cp.add_string('o', "output", "file", output_file, "The file to write the clustering to");
    cp.add_unsigned('s', "seed", "unsigned int", seed, "Fix random seed");
    cp.add_flag('f', "snap-format", "bool", snap_format, "Graph is in SNAP Edge List Format rather than DIMACS graph");
    cp.add_flag('b', "binary-format", "bool", binary_format, "Graph is in Thrill binary format rather than DIMACS graph");
    cp.add_param_string("graph", graph_file, "The graph to perform clustering on, in metis format");
    cp.add_opt_param_stringlist("partitions", partitions_strings, "Partition with reporting UUID (comma seperated)");

    if (!cp.process(argc, argv)) {
      exit = 1;
    }

    Logging::report("program_run", run_id, "binary", argv[0]);
  }

  void initialize() {
    if (graph_file.empty()) {
      exit = 1;
      return;
    }
    std::vector<std::vector<Graph::NodeId>> neighbors;
    Graph::EdgeId edge_count = 0;
    if (snap_format) {
      establishIdMapping(graph_file);
      neighbors.resize(id_mapping.size());
      edge_count = IO::read_graph_txt(graph_file, neighbors, id_mapping);
    } else if (binary_format) {
      edge_count = IO::read_graph_bin(graph_file, neighbors);
    } else {
      edge_count = IO::read_graph(graph_file, neighbors);
    }
    graph = std::make_unique<Graph>(neighbors.size(), edge_count);
    graph->setEdgesByAdjacencyLists(neighbors);


    Logging::report("program_run", run_id, "graph", graph_file);
    Logging::report("program_run", run_id, "node_count", graph->getNodeCount());
    Logging::report("program_run", run_id, "edge_count", graph->getEdgeCount());
    Logging::report("program_run", run_id, "seed", seed);

    if (!ground_proof_file.empty()) {
      ground_proof = std::make_unique<ClusterStore>(graph->getNodeCount());
      if (snap_format) {
        IO::read_snap_clustering(ground_proof_file, *ground_proof, id_mapping);
      } else {
        IO::read_clustering(ground_proof_file, *ground_proof);
      }
      Logging::report("program_run", run_id, "ground_proof", ground_proof_file);
    }

    initialized = true;
  }

  int getExitCode() { return exit; }
  bool shouldRun() { return initialized; }
  unsigned getSeed() { return seed; }
  const Graph& getGraph() { return *graph; }
  bool isGroundProofAvailable() { if (ground_proof) { return true; } else { return false; } }
  const ClusterStore& getGroundProof() { return *ground_proof; }
  bool shouldWriteOutput() { return !output_file.empty(); }
  const std::string& outputFile() { return output_file; }

  template<class F>
  void forEachPartition(std::vector<uint32_t>& node_partition_elements, F f) {
    assert(node_partition_elements.size() == graph->getNodeCount());
    for (const auto& partitions_string : partitions_strings) {
      PartitionInput partition_input = Logging::parse_input_with_logging_id(partitions_string);
      IO::read_partition(partition_input.first, node_partition_elements);
      f(node_partition_elements, partition_input.second);
    }
  }

private:

  void establishIdMapping(const std::string& graph_file) {
    std::set<NodeId> node_ids;
    IO::open_file(graph_file, [&](auto& file) {
      std::string line;

      while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
          std::istringstream line_stream(line);
          Graph::NodeId tail, head;
          if (line_stream >> tail >> head) {
            node_ids.insert(tail);
            node_ids.insert(head);
          }
        }
      }
    });

    NodeId new_id = 0;
    for (NodeId old_id : node_ids) {
      id_mapping[old_id] = new_id++;
    }
  }
};
