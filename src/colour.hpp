#ifndef PIET_I_COLOUR_HPP
#define PIET_I_COLOUR_HPP

#include "png.hpp"

enum class Hue {
  RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, COUNT
};

enum class Lightness {
  LIGHT, NORMAL, DARK, COUNT
};

struct Colour {
 public:
  Colour();
  Colour(Hue hue, Lightness lightness);
  explicit operator bool() const;
  Hue hue() const;
  Lightness lightness() const;
 public:
  static const Colour unknown;
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
