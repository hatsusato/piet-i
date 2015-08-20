#ifndef PIET_I_UTILS_HPP
#define PIET_I_UTILS_HPP

#include <tuple>
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

Color what_color(const Pixel& pixel);
Brightness how_bright(const Pixel& pixel);
bool operator==(const Pixel& lhs, const Pixel& rhs);
bool operator!=(const Pixel& lhs, const Pixel& rhs);
size_t gcd(size_t a, size_t b);

#endif  // PIET_I_UTILS_HPP
