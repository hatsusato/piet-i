#ifndef PIET_I_COORD_HPP
#define PIET_I_COORD_HPP

#include <tuple>

class Coord : public std::tuple<int, int> {
  using Base = std::tuple<int, int>;
 public:
  Coord();
  Coord(int x, int y);
};

#endif  // PIET_I_COORD_HPP
