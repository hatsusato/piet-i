#ifndef PIET_I_COORD_HPP
#define PIET_I_COORD_HPP

#include <array>
#include <tuple>
#include <vector>
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
  bool inside(int left, int top, int right, int bottom) const;
};

class Coordinates : public std::vector<Coord> {
 public:
  std::array<int, 4> range() const;
};

#endif  // PIET_I_COORD_HPP
