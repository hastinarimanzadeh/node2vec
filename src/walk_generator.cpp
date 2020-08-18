#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "graph.tpp"

using GraphType = graph<int, float>;
GraphType read_edgelist(std::string filename) {
  std::vector<typename GraphType::edge_type> graph_edges;

  std::ifstream edgelist_file(filename);
  if (!edgelist_file.is_open())
    throw std::runtime_error("bad input file");

  std::string line;
  while(std::getline(edgelist_file, line)) {
    std::istringstream iss(line);
    GraphType::vertex_type v1, v2;
    GraphType::weight_type w;
    if (!(iss >> v1 >> v2 >> w))
      throw std::runtime_error("bad input file");
    graph_edges.emplace_back(v1, v2, w);
  }

  return graph_edges;
}

int main(int /*argc*/, char const ** /*argv*/) {

  GraphType g = read_edgelist("graph.edges");
  
  return 0;
}
