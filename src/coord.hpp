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
  Coord next(DP dp) const;
  bool inside(int left, int top, int right, int bottom) const;
};

class Coordinates : public std::vector<Coord> {
 public:
  std::array<int, DP::count> range() const;
  bool includes(const Coord& coord) const;
  Coord find_out_of_range(const Coord& coord, DP dp) const;
};

#endif  // PIET_I_COORD_HPP
