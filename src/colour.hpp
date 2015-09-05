#ifndef PIET_I_COLOUR_HPP
#define PIET_I_COLOUR_HPP

#include "png.hpp"
#include "utility.hpp"

enum class Hue : int {
  RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, COUNT
};
ENUM_INDEXING(Hue);

enum class Lightness : int {
  LIGHT, NORMAL, DARK, COUNT
};
ENUM_INDEXING(Lightness);

struct Colour {
 public:
  Colour();
  Colour(Hue hue, Lightness lightness);
  explicit operator bool() const;
  Hue hue() const;
  Lightness lightness() const;
 public:
  static const Colour unknown;
  static constexpr int count = index(Hue::COUNT) * index(Lightness::COUNT);
  friend bool operator==(const Colour&, const Colour&);
 private:
  Hue hue_;
  Lightness lightness_;
};

bool operator==(const Colour& lhs, const Colour& rhs);
bool operator!=(const Colour& lhs, const Colour& rhs);

Colour make_colour(const Pixel& pixel);
int difference(const Colour& current, const Colour& next);

#endif  // PIET_I_COLOUR_HPP
