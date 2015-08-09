#ifndef PIET_I_VISUALIZE_HPP
#define PIET_I_VISUALIZE_HPP

#include "utils.hpp"

Color what_color(const Pixel& pixel);
Color how_bright(const Pixel& pixel);
void show_pixel(const Pixel& pixel);
void visualize(const Image& image);

#endif  // PIET_I_VISUALIZE_HPP
