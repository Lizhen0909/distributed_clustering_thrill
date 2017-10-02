#include "data/graph.hpp"
#include "data/cluster_store.hpp"
#include "util/logging.hpp"
#include "util/input.hpp"
#include "algo/louvain.hpp"

#include <thrill/common/stats_timer.hpp>

#include <Infomap.h>


int main(int argc, char const *argv[]) {
  Logging::Id run_id = Logging::getUnusedId();
  Input input(argc, argv, run_id);
  input.initialize();

  if (!input.shouldRun()) {
    return input.getExitCode();
  }

  const Graph& graph = input.getGraph();

  infomap::Infomap infomap_wrapper("--clu -2 -s" + std::to_string(input.getSeed()));

  for (NodeId node = 0; node < graph.getNodeCount(); node++) {
    graph.forEachAdjacentNode(node, [&](NodeId neighbor, auto) {
      infomap_wrapper.addLink(node, neighbor);
    });
  }

  Logging::Id algo_run_logging_id = Logging::getUnusedId();
  Logging::report("algorithm_run", algo_run_logging_id, "program_run_id", run_id);
  Logging::report("algorithm_run", algo_run_logging_id, "algorithm", "sequential infomap");

  thrill::common::StatsTimerBase<true> timer(/* autostart */ true);
  infomap_wrapper.run();
  Logging::report("algorithm_run", algo_run_logging_id, "runtime", timer.Microseconds() / 1000000.0);

  ClusterStore clusters(graph.getNodeCount());
  for (auto leafIt = infomap_wrapper.tree.leafIter(); !leafIt.isEnd(); ++leafIt) {
    clusters.set(leafIt->originalLeafIndex, leafIt.moduleIndex());
  }

  Logging::Id cluster_logging_id = Louvain::log_clustering(graph, clusters);
  Logging::report("clustering", cluster_logging_id, "source", "computation");
  Logging::report("clustering", cluster_logging_id, "algorithm_run_id", algo_run_logging_id);

  if (input.shouldWriteOutput()) {
    Logging::report("clustering", cluster_logging_id, "path", input.outputFile());
    IO::write_clustering(input.outputFile(), clusters);
  }
}