#ifndef PIET_I_EDGES_HPP
#define PIET_I_EDGES_HPP

#include <array>
#include "direction.hpp"

class Coord;
class Coordinates;

template <typename T>
class Edges {
  using BothChoose = std::array<T, 2>;
 public:
  T& edge(Direction direction, Choose choose) {
    return edges_[direction.value()][choose.value()];
  }
  const T& edge(Direction direction, Choose choose) const {
    return edges_[direction.value()][choose.value()];
  }
 private:
  std::array<BothChoose, 4> edges_;
};

Edges<Coord> make_edges(const Coordinates& coords);

#endif  // PIET_I_EDGES_HPP
