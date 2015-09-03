#include "visualize.hpp"
#include "codel.hpp"
#include "codel_table.hpp"

void show_pixel(const Codel& codel) {
  static const char* fore[] =
      {"\x1b[30;1m", "\x1b[31;1m", "\x1b[33;1m", "\x1b[32;1m",
       "\x1b[36;1m", "\x1b[34;1m", "\x1b[35;1m", "\x1b[37;1m"};
  static const char* back[] =
      {"\x1b[40;1m", "\x1b[41;1m", "\x1b[43;1m", "\x1b[42;1m",
       "\x1b[46;1m", "\x1b[44;1m", "\x1b[45;1m", "\x1b[47;1m"};
  static const char* reset = "\x1b[0m";
  static const char text[] = {'K', 'R', 'Y', 'G', 'C', 'B', 'M', 'W'};
  const auto color = codel.color();
  const auto bright = codel.brightness();
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
  const auto table = make_codel_table(image, RegardUnknownAs::UNKNOWN);
  for (size_t y = 0; y < table.height(); ++y) {
    const auto& row = table[y];
    for (size_t x = 0; x < table.width(); ++x) {
      show_pixel(row[x]);
    }
    std::cout << std::endl;
  }
}
