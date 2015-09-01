#ifndef PIET_I_CODEL_HPP
#define PIET_I_CODEL_HPP

#include "colour.hpp"

enum class ColourType {
  BLACK, WHITE, COLOUR, UNKNOWN
};

struct Codel {
 public:
  Codel();
  Codel(Hue hue, Lightness lightness);
  explicit Codel(Colour colour);
  explicit Codel(ColourType type);
 public:
  friend bool operator==(const Codel&, const Codel&);
 private:
  Colour colour_;
  ColourType type_;
};

bool operator==(const Codel& lhs, const Codel& rhs);
bool operator!=(const Codel& lhs, const Codel& rhs);

#endif  // PIET_I_CODEL_HPP
