#ifndef PIET_I_PNG_HPP
#define PIET_I_PNG_HPP

#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>

using Pixel = png::rgb_pixel;
using Image = png::image<Pixel>;
using Byte = png::byte;

bool operator==(const Pixel& lhs, const Pixel& rhs);
bool operator!=(const Pixel& lhs, const Pixel& rhs);

#endif  // PIET_I_PNG_HPP
