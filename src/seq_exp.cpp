#include "data/graph.hpp"
#include "data/cluster_store.hpp"
#include "algo/louvain.hpp"
#include "algo/similarity.hpp"
#include "algo/partitioning.hpp"
#include "util/logging.hpp"
#include "util/input.hpp"

#include <iostream>
#include <string>
#include <numeric>
#include <assert.h>
#include <random>

int main(int argc, char const *argv[]) {
  Logging::Id run_id = Logging::getUnusedId();
  Input input(argc, argv, run_id);
  input.initialize();

  if (!input.shouldRun()) {
    return input.getExitCode();
  }

  Modularity::rng = std::default_random_engine(input.getSeed());
  const Graph& graph = input.getGraph();

  ClusterStore base_clusters(graph.getNodeCount());
  ClusterStore compare_clusters(graph.getNodeCount());

  Logging::Id ground_proof_logging_id = 0;
  if (input.isGroundProofAvailable()) {
    ground_proof_logging_id = Louvain::log_clustering(graph, input.getGroundProof());
    Logging::report("clustering", ground_proof_logging_id, "source", "ground_proof");
    Logging::report("clustering", ground_proof_logging_id, "program_run_id", run_id);
  }

  Logging::Id base_algo_run_logging_id = Logging::getUnusedId();
  Logging::report("algorithm_run", base_algo_run_logging_id, "program_run_id", run_id);
  Logging::report("algorithm_run", base_algo_run_logging_id, "algorithm", "sequential louvain");

  Louvain::louvainModularity(graph, base_clusters, base_algo_run_logging_id);
  Logging::Id base_cluster_logging_id = Louvain::log_clustering(graph, base_clusters);
  Logging::report("clustering", base_cluster_logging_id, "source", "computation");
  Logging::report("clustering", base_cluster_logging_id, "algorithm_run_id", base_algo_run_logging_id);

  if (input.isGroundProofAvailable()) {
    Logging::log_comparison_results(ground_proof_logging_id, input.getGroundProof(), base_cluster_logging_id, base_clusters);
  }

  Logging::Id compare_algo_run_logging_id = Logging::getUnusedId();
  Logging::report("algorithm_run", compare_algo_run_logging_id, "program_run_id", run_id);
  Logging::report("algorithm_run", compare_algo_run_logging_id, "algorithm", "sequential louvain");

  Louvain::louvainModularity(graph, compare_clusters, compare_algo_run_logging_id);
  Logging::Id compare_cluster_logging_id = Louvain::log_clustering(graph, compare_clusters);
  Logging::report("clustering", compare_cluster_logging_id, "source", "computation");
  Logging::report("clustering", compare_cluster_logging_id, "algorithm_run_id", compare_algo_run_logging_id);

  Logging::log_comparison_results(base_cluster_logging_id, base_clusters, compare_cluster_logging_id, compare_clusters);
  if (input.isGroundProofAvailable()) {
    Logging::log_comparison_results(ground_proof_logging_id, input.getGroundProof(), compare_cluster_logging_id, compare_clusters);
  }

  std::vector<uint32_t> partitions(graph.getNodeCount());
  auto run_and_log_partitioned_louvain = [&](auto calculate_partition) {
    Logging::Id partition_logging_id = calculate_partition(partitions);
    std::vector<Logging::Id> partition_element_logging_ids = Partitioning::analyse(graph, partitions, partition_logging_id);

    for (bool allow_move_to_ghosts : { true, false }) {
      compare_algo_run_logging_id = Logging::getUnusedId();
      Logging::report("algorithm_run", compare_algo_run_logging_id, "program_run_id", run_id);
      Logging::report("algorithm_run", compare_algo_run_logging_id, "algorithm", "partitioned louvain");
      Logging::report("algorithm_run", compare_algo_run_logging_id, "partition_id", partition_logging_id);
      Logging::report("algorithm_run", compare_algo_run_logging_id, "allow_move_to_ghosts", allow_move_to_ghosts);

      if (allow_move_to_ghosts) {
        Louvain::partitionedLouvain<true>(graph, compare_clusters, partitions, compare_algo_run_logging_id, partition_element_logging_ids);
      } else {
        Louvain::partitionedLouvain<false>(graph, compare_clusters, partitions, compare_algo_run_logging_id, partition_element_logging_ids);
      }
      compare_cluster_logging_id = Louvain::log_clustering(graph, compare_clusters);
      Logging::report("clustering", compare_cluster_logging_id, "source", "computation");
      Logging::report("clustering", compare_cluster_logging_id, "algorithm_run_id", compare_algo_run_logging_id);

      Logging::log_comparison_results(base_cluster_logging_id, base_clusters, compare_cluster_logging_id, compare_clusters);
      if (input.isGroundProofAvailable()) {
        Logging::log_comparison_results(ground_proof_logging_id, input.getGroundProof(), compare_cluster_logging_id, compare_clusters);
      }
    }
  };

  for (uint32_t i : { 4, 16, 64, 256, 1024 }) {
    run_and_log_partitioned_louvain([&](std::vector<uint32_t>& partitions) {
      return Partitioning::chunk(graph, i, partitions);
    });
    run_and_log_partitioned_louvain([&](std::vector<uint32_t>& partitions) {
      return Partitioning::deterministicGreedyWithLinearPenalty(graph, i, partitions);
    });
    // run_and_log_partitioned_louvain([&](std::vector<uint32_t>& partitions) {
    //   return Partitioning::deterministicGreedyWithLinearPenalty(graph, i, partitions, true);
    // });
    // run_and_log_partitioned_louvain([&](std::vector<uint32_t>& partitions) {
    //   return Partitioning::clusteringBased(graph, i, partitions, input.isGroundProofAvailable() ? input.getGroundProof() : base_clusters); // TODO log cluster id?
    // });
    // run_and_log_partitioned_louvain([&](std::vector<uint32_t>& partitions) {
    //   return Partitioning::random(graph, i, partitions);
    // });
  }

  input.forEachPartition(partitions, [&](const std::vector<uint32_t>&, const std::string& logging_id) {
    run_and_log_partitioned_louvain([&](std::vector<uint32_t>&) {
      Logging::Id partition_logging_id = Logging::getUnusedId();
      Logging::report("partition", partition_logging_id, "algorithm", "external");
      Logging::report("partition", partition_logging_id, "program_run_id", logging_id);
      return partition_logging_id;
    });
  });
}
