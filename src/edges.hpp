#ifndef PIET_I_EDGES_HPP
#define PIET_I_EDGES_HPP

#include <array>
#include "coord.hpp"
#include "direction.hpp"

template <typename T>
class Edges {
 public:
  using BothChoose = std::array<T, 2>;
  T& edge(DP dp, CC cc) {
    return edges_[index(dp)][index(cc)];
  }
  const T& edge(DP dp, CC cc) const {
    return edges_[index(dp)][index(cc)];
  }
 private:
  std::array<BothChoose, 4> edges_;
};

Edges<Coord> make_edges(const Coordinates& coords);

#endif  // PIET_I_EDGES_HPP
