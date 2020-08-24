#include "../include/graph.hpp"

template <class NodeType, class WeightType>
graph<NodeType, WeightType>::graph(std::vector<edge_type> edges) {
  _edge_list = edges;
}

template <class NodeType, class WeightType>
std::unordered_set<std::pair<NodeType, WeightType>, pair_hash>
graph<NodeType, WeightType>::neighbours(NodeType v) const {
  std::unordered_set<std::pair<NodeType, WeightType>, pair_hash> neighbours;

  for (const edge_type& edge: _edge_list)
    if (std::get<0>(edge) == v)
      neighbours.insert(std::make_pair(std::get<1>(edge), std::get<2>(edge)));

  return neighbours;
}

template <class NodeType, class WeightType>
std::unordered_set<NodeType> graph<NodeType, WeightType>::nodes() const {
  std::unordered_set<NodeType> nodes;
  for (const edge_type& edge: _edge_list) {
    nodes.insert(std::get<0>(edge));
    nodes.insert(std::get<1>(edge));
  }

  return nodes;
}

template<class NodeType, class WeightType>
std::vector<typename graph<NodeType, WeightType>::edge_type>
graph<NodeType, WeightType>::edge_list() const {
  return _edge_list;
}
