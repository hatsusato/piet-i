#ifndef PIET_I_EDGE_HPP
#define PIET_I_EDGE_HPP

#include <array>
#include "coord.hpp"
#include "direction.hpp"

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

#endif  // PIET_I_EDGE_HPP
