#ifndef PIET_I_UTILS_HPP
#define PIET_I_UTILS_HPP

#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>

using Pixel = png::rgb_pixel;
using Image = png::image<Pixel>;
using Byte = png::byte;

enum class Color {
  BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, UNKNOWN
};

bool operator==(const Pixel& lhs, const Pixel& rhs);
bool operator!=(const Pixel& lhs, const Pixel& rhs);
size_t gcd(size_t a, size_t b);

#endif  // PIET_I_UTILS_HPP
