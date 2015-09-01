#include "codel.hpp"

Codel::Codel()
    : colour_(), type_(ColourType::UNKNOWN)
{}
Codel::Codel(Hue hue, Lightness lightness)
    : colour_(hue, lightness),
      type_(colour_ ? ColourType::COLOUR : ColourType::UNKNOWN)
{}
Codel::Codel(Colour colour)
    : colour_(colour), type_(ColourType::COLOUR)
{}
Codel::Codel(ColourType type)
    : colour_(),
      type_(type == ColourType::COLOUR ? ColourType::UNKNOWN : type)
{}
Colour Codel::colour() const {
  return colour_;
}
ColourType Codel::type() const {
  return type_;
}

const Codel Codel::black = Codel(ColourType::BLACK);
const Codel Codel::white = Codel(ColourType::WHITE);
const Codel Codel::unknown = Codel(ColourType::UNKNOWN);

bool operator==(const Codel& lhs, const Codel& rhs) {
  return (lhs.colour_ == rhs.colour_ && lhs.type_ == rhs.type_);
}
bool operator!=(const Codel& lhs, const Codel& rhs) {
  return !(lhs == rhs);
}

Codel make_codel(const Pixel& pixel) {
  const auto colour = make_colour(pixel);
  if (colour) {
    return Codel(colour);
  } else {
    const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
    const Byte z = 0x00, f = 0xFF;
    if (r == z && g == z && b == z) {
      return Codel::black;
    } else if (r == f && g == f && b == f) {
      return Codel::white;
    } else {
      return Codel::unknown;
    }
  }
}
