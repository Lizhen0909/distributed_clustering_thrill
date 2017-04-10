#include "thrill_local_moving.hpp"
#include "thrill_louvain.hpp"

int main(int argc, char const *argv[]) {
  return Louvain::performAndEvaluate(argc, argv, "thrill node based fully distributed local moving", [](const auto& graph) {
    return Louvain::louvain(graph, [](const auto& graph) {
      return LocalMoving::distributedLocalMoving(graph, 32);
    });
  });
}

