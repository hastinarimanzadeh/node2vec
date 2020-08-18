#include <vector>
#include <unordered_set>
#include <tuple>
#include <utility>


template <class T>
inline size_t combine_hash(const size_t seed, const T& other) {
  return seed ^ (std::hash<T>{}(other) +
      0x9E3779B97F4A7C15 + (seed << 6) + (seed >> 2));
}

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const {
		return combine_hash(std::hash<T1>()(pair.first), pair.second);
	}
};

template <class NodeType, class WeightType>
class graph {
  public:
    using edge_type = std::tuple<NodeType, NodeType, WeightType>;
    using vertex_type = NodeType;
    using weight_type = WeightType;


    graph() = default;
    graph(std::vector<edge_type> edges);

    std::unordered_set<std::pair<NodeType, WeightType>, pair_hash>
    neighbours(NodeType v) const;

    std::unordered_set<NodeType>
    nodes() const;
  private:
    std::vector<edge_type> edge_list;
};
