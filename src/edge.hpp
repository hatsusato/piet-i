#ifndef PIET_I_EDGE_HPP
#define PIET_I_EDGE_HPP

#include <array>
#include "coord.hpp"
#include "direction.hpp"

class Edges {
  using BothChoose = std::array<Coord, 2>;
 public:
  Edges(const Coordinates& coords);
  Coord& edge(Direction direction, Choose choose);
  const Coord& edge(Direction direction, Choose choose) const;
 private:
  std::array<BothChoose, 4> edges_;
};

#endif  // PIET_I_EDGE_HPP
