#include <iostream>
#include <random>
#include <unordered_map>
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
  while (std::getline(edgelist_file, line)) {
    std::istringstream iss(line);
    GraphType::vertex_type v1, v2;
    GraphType::weight_type w;
    if (!(iss >> v1 >> v2 >> w))
      throw std::runtime_error("bad input file");
    graph_edges.emplace_back(v1, v2, w);
  }

  return graph_edges;
}

std::vector<typename GraphType::vertex_type> generate_walk(
     typename GraphType::vertex_type u, typename GraphType::vertex_type s,
     const GraphType& g,
     const std::unordered_map<std::pair<typename GraphType::vertex_type,
         typename GraphType::vertex_type>, std::vector<double>, pair_hash>& next_hop,
          size_t l, std::mt19937_64& gen) {
  std::vector<typename GraphType::vertex_type> walk;
  walk.reserve(l);
  walk.push_back(u);
  walk.push_back(s);

  for (size_t hop = 1; hop < l; hop++){
    typename GraphType::vertex_type curr = walk[hop];
    typename GraphType::vertex_type last = walk[hop - 1];
    auto n_curr = g.neighbours(curr);
    std::vector<std::pair<typename GraphType::vertex_type,
      typename GraphType::weight_type>> curr_neighbours_vec(n_curr.begin(), n_curr.end());
    std::sort(curr_neighbours_vec.begin(), curr_neighbours_vec.end());
    std::vector<double> probs = next_hop.at(std::make_pair(last, curr));
    std::discrete_distribution<size_t> dd(probs.begin(), probs.end());
    typename GraphType::vertex_type next_s = curr_neighbours_vec[dd(gen)].first;
    walk.push_back(next_s);
  }
  return walk;
}

std::unordered_map<std::pair<typename GraphType::vertex_type,
  typename GraphType::vertex_type>, std::vector<double>, pair_hash>
next_hop_map(const GraphType& g, double p, double q) {
  std::unordered_map<std::pair<typename GraphType::vertex_type,
    typename GraphType::vertex_type>, std::vector<double>, pair_hash> next_hop;

  for (auto [t, v, w1]: g.edge_list()) {
    std::vector<double> probs;
    std::unordered_set<typename GraphType::vertex_type> adj_t;
    auto n_t = g.neighbours(t);
    std::transform(n_t.begin(), n_t.end(), std::inserter(adj_t, adj_t.begin()),
      [](std::pair<typename GraphType::vertex_type,
        typename GraphType::weight_type> neigh) { return neigh.first; });

    auto n_v = g.neighbours(v);
    std::vector<std::pair<typename GraphType::vertex_type,
      typename GraphType::weight_type>> neighbours_vec(n_v.begin(), n_v.end());
    std::sort(neighbours_vec.begin(), neighbours_vec.end());
    for (auto [x, w2]: neighbours_vec)
      if (t == x)
        probs.push_back(w2/p);
      else if (adj_t.find(x) != adj_t.end())
        probs.push_back(w2);
      else
        probs.push_back(w2/q);

    next_hop[std::make_pair(t, v)] = probs;
  }

  return next_hop;
}


int main(int /*argc*/, char const ** /*argv*/) {
  size_t seed = 42;
  double p = 0.5, q = 0.5;
  GraphType g = read_edgelist("graph.edges");
  size_t l = 5, r = 1;

  std::mt19937_64 gen(seed);

  auto next_hop = next_hop_map(g, p, q);

  for (auto& u: g.nodes()) {
    auto n_u = g.neighbours(u);
    std::vector<std::pair<typename GraphType::vertex_type,
      typename GraphType::weight_type>> neighbours_vec(n_u.begin(), n_u.end());
    std::vector<typename GraphType::weight_type> weights;
    std::transform(neighbours_vec.begin(), neighbours_vec.end(),
      std::back_inserter(weights),
      [](std::pair<typename GraphType::vertex_type,
        typename GraphType::weight_type> neigh) { return neigh.second; });
    std::discrete_distribution<size_t> random_neighbour(weights.begin(), weights.end());

    for (size_t walks = 0; walks < r; walks++) {
      typename GraphType::vertex_type s = neighbours_vec[random_neighbour(gen)].first;
      auto walk = generate_walk(u, s, g, next_hop, l, gen);
      
      for(auto& n: walk)
        std::cout << n << " ";
      std::cout << "\n";
    }
  }

  return 0;
}
