#ifndef PIET_I_COORD_HPP
#define PIET_I_COORD_HPP

#include <tuple>
#include "direction.hpp"

class Coord : public std::tuple<int, int> {
  using Base = std::tuple<int, int>;
 public:
  Coord();
  Coord(int x, int y);
  int& x();
  const int& x() const;
  int& y();
  const int& y() const;
  Coord& next(Direction direction);
};

#endif  // PIET_I_COORD_HPP
