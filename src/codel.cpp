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

bool operator==(const Codel& lhs, const Codel& rhs) {
  return (lhs.colour_ == rhs.colour_ && lhs.type_ == rhs.type_);
}
bool operator!=(const Codel& lhs, const Codel& rhs) {
  return !(lhs == rhs);
}
