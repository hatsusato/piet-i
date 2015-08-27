#ifndef PIET_I_UTILS_HPP
#define PIET_I_UTILS_HPP

#include <memory>
#include <tuple>
#include <cassert>
#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>

using Pixel = png::rgb_pixel;
using Image = png::image<Pixel>;
using Byte = png::byte;
using Coord = std::tuple<int, int>;

enum class Color {
  BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, UNKNOWN
};

enum class Brightness {
  LIGHT, NORMAL, DARK, UNKNOWN
};

enum class Direction {
  RIGHT, UP, LEFT, DOWN
};

enum class Choose {
  LEFT, RIGHT
};

Color what_color(const Pixel& pixel);
Brightness how_bright(const Pixel& pixel);
bool operator==(const Pixel& lhs, const Pixel& rhs);
bool operator!=(const Pixel& lhs, const Pixel& rhs);
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
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif  // PIET_I_UTILS_HPP
