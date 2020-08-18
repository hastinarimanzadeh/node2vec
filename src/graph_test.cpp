#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "graph.tpp"

using GraphType = graph<int, float>;

TEST_CASE("Graph class", "[graph]") {
  std::vector<typename GraphType::edge_type> graph_edges = {
    {1, 2, 0.5}, {2, 3, 0.1},
    {4, 5, 80.0}, {4, 6, 0.5}};
  GraphType g(graph_edges);

  REQUIRE(g.nodes() == std::unordered_set<typename GraphType::vertex_type>(
    {1, 2, 3, 4, 5, 6}));

  REQUIRE(g.neighbours(4) == std::unordered_set<
    std::pair<typename GraphType::vertex_type, typename GraphType::weight_type>, pair_hash>(
      {{5, 80.0}, {6, 0.5}}));
}
