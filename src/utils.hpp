#ifndef PIET_I_UTILS_HPP
#define PIET_I_UTILS_HPP

#include <tuple>
#include <cassert>

using Coord = std::tuple<int, int>;

enum class Color {
  BLACK, RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE, UNKNOWN
};

enum class Brightness {
  LIGHT, NORMAL, DARK, UNKNOWN
};

enum class Direction {
  RIGHT, DOWN, LEFT, UP
};

enum class Choose {
  LEFT, RIGHT
};

Color what_color(const Pixel& pixel);
Brightness how_bright(const Pixel& pixel);
size_t gcd(size_t a, size_t b);
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
