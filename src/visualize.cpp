#include "visualize.hpp"
#include "codel.hpp"

void show_pixel(const Pixel& pixel) {
  static const char* fore[] =
      {"\x1b[30;1m", "\x1b[31;1m", "\x1b[32;1m", "\x1b[33;1m",
       "\x1b[34;1m", "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};
  static const char* back[] =
      {"\x1b[40;1m", "\x1b[41;1m", "\x1b[42;1m", "\x1b[43;1m",
       "\x1b[44;1m", "\x1b[45;1m", "\x1b[46;1m", "\x1b[47;1m"};
  static const auto reset = "\x1b[0m";
  static const char text[] = {'K', 'R', 'G', 'Y', 'B', 'M', 'C', 'W'};
  const auto color = what_color(pixel);
  const auto bright = how_bright(pixel);
  if (color == Color::UNKNOWN) {
    std::cout << "?";
  } else {
    const auto c = static_cast<int>(color);
    const auto b = static_cast<int>(
        bright == Brightness::LIGHT ? Color::WHITE :
        bright == Brightness::DARK ? Color::BLACK : color);
    std::cout << back[c] << fore[b] << text[c] << reset;
  }
}

void visualize(const Image& image) {
  const auto step = codel_size(image);
  for (size_t row = 0; row < image.get_height(); row += step) {
    for (size_t col = 0; col < image.get_width(); col += step) {
      show_pixel(image[row][col]);
    }
    std::cout << std::endl;
  }
}

