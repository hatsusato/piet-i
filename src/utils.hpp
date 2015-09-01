#ifndef PIET_I_UTILS_HPP
#define PIET_I_UTILS_HPP

#include <tuple>
#include <cassert>
#include "png.hpp"

using Coord = std::tuple<int, int>;

Coord next_coordinate(const Coord& coord, Direction dir);

template <size_t N>
bool same_element(const Coord& coord, int value) {
  return std::get<N>(coord) == value;
}
template <size_t N>
bool less_element(const Coord& lhs, const Coord& rhs) {
  return std::get<N>(lhs) < std::get<N>(rhs);
}
template <size_t N>
bool greater_element(const Coord& lhs, const Coord& rhs) {
  return std::get<N>(lhs) > std::get<N>(rhs);
}

#endif  // PIET_I_UTILS_HPP
