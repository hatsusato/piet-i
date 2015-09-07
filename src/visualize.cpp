#include "visualize.hpp"
#include <cassert>
#include "codel.hpp"
#include "codel_table.hpp"
#include "utility.hpp"

namespace /* unnamed */ {
enum class Index {
  K, R, G, Y, B, M, C, W
};
enum class LightnessIndex {
  W, L, N, D, K
};
}  // namespace /* unnamed */
template <>
struct enumeration_traits<Index> : enumeration_trait_indexing {};
template <>
struct enumeration_traits<LightnessIndex> : enumeration_trait_indexing {};

Index hue_index(Hue hue) {
  switch (hue) {
    case Hue::RED:
      return Index::R;
    case Hue::YELLOW:
      return Index::Y;
    case Hue::GREEN:
      return Index::G;
    case Hue::CYAN:
      return Index::C;
    case Hue::BLUE:
      return Index::B;
    case Hue::MAGENTA:
      return Index::M;
    default:
      assert(false);
  }
  return Index::K;
}

Index fore_index(const Codel& codel) {
  switch (codel.type()) {
    case ColourType::BLACK:
      return Index::K;
    case ColourType::WHITE:
      return Index::W;
    case ColourType::COLOUR:
      switch (codel.colour().lightness()) {
        case Lightness::LIGHT:
          return Index::W;
        case Lightness::DARK:
          return Index::K;
        case Lightness::NORMAL:
          return hue_index(codel.colour().hue());
        default:
          assert(false);
      }
    default:
      assert(false);
  }
  return Index::K;
}

Index back_index(const Codel& codel) {
  switch (codel.type()) {
    case ColourType::BLACK:
      return Index::K;
    case ColourType::WHITE:
      return Index::W;
    case ColourType::COLOUR:
      return hue_index(codel.colour().hue());
    default:
      assert(false);
  }
  return Index::K;
}

void show_pixel(const Codel& codel) {
  static const char* fore[] =
      {"\x1b[30;1m", "\x1b[31;1m", "\x1b[32;1m", "\x1b[33;1m",
       "\x1b[34;1m", "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};
  static const char* back[] =
      {"\x1b[40;1m", "\x1b[41;1m", "\x1b[42;1m", "\x1b[43;1m",
       "\x1b[44;1m", "\x1b[45;1m", "\x1b[46;1m", "\x1b[47;1m"};
  static const char text[] = {'K', 'R', 'G', 'Y', 'B', 'M', 'C', 'W'};
  static const char* reset = "\x1b[0m";
  if (codel) {
    std::cout << back[index(back_index(codel))]
              << fore[index(fore_index(codel))]
              << text[index(back_index(codel))]
              << reset;
  } else {
    std::cout << "?";
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
