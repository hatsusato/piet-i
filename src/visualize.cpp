#include "visualize.hpp"
#include "codel.hpp"

Color what_color(const Pixel& pixel) {
  const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
  const Byte z = 0x00, c = 0xC0, f = 0xFF;
  if ((r == z || r == c || r == f) &&
      (g == z || g == c || g == f) &&
      (b == z || b == c || b == f)) {
    if (r == z && g == z && b == z) {
      return Color::BLACK;
    } else if (r == f && g == f && b == f) {
      return Color::WHITE;
    } else if (g == z && b == z) {
      return Color::RED;
    } else if (r == z && b == z) {
      return Color::GREEN;
    } else if (r == z && g == z) {
      return Color::BLUE;
    } else if (r == f && g == f) {
      return Color::YELLOW;
    } else if (g == f && b == f) {
      return Color::CYAN;
    } else if (r == f && b == f) {
      return Color::MAGENTA;
    } else if (r == f) {
      return Color::RED;
    } else if (g == f) {
      return Color::GREEN;
    } else if (b == f) {
      return Color::BLUE;
    } else if (b == z) {
      return Color::YELLOW;
    } else if (r == z) {
      return Color::CYAN;
    } else if (g == z) {
      return Color::MAGENTA;
    }
  }
  return Color::UNKNOWN;
}

Color how_bright(const Pixel& pixel) {
  const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
  const Byte c = 0xC0;
  const auto mini = std::min(r, std::min(g, b));
  const auto maxi = std::max(r, std::max(g, b));
  if (mini == c) {
    return Color::WHITE;
  } else if (maxi == c) {
    return Color::BLACK;
  } else {
    return what_color(pixel);
  }
}

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
    const auto b = static_cast<int>(bright);
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

