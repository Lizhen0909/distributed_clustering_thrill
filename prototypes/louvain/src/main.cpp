#include "graph.hpp"
#include "modularity.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>

template<class F>
void open_file(std::string filename, F callback, std::ios_base::openmode mode = std::ios::in) {
    std::cout << "Opening file " << filename << "\n";
    std::ifstream f(filename, mode);
    if(!f.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    callback(f);

    std::cout << "done reading " << filename << "\n";
}

int read_graph(const std::string filename, std::vector<std::vector<int>> &neighbors) {
  int edge_count;
  open_file(filename, [&](auto& file) {
    std::string line;
    int node_count;
    std::getline(file, line);
    std::istringstream header_stream(line);
    header_stream >> node_count >> edge_count;
    neighbors.resize(node_count);

    int i = 0;
    while (std::getline(file, line)) {
      std::istringstream line_stream(line);
      int neighbor;
      while (line_stream >> neighbor) {
        neighbors[i].push_back(neighbor - 1);
      }
      i++;
    }
  });
  return edge_count;
}

int main(int, char const *argv[]) {
  std::vector<std::vector<int>> neighbors;
  int edge_count = read_graph(argv[1], neighbors);
  Graph graph(neighbors.size(), edge_count);
  graph.setEdgesByAdjacencyLists(neighbors);

  std::vector<int> clusters(neighbors.size());
  Modularity::louvain(graph, clusters);

  std::cout << graph.modularity(clusters) << "\n";
}

