#include "colour.hpp"

Colour::Colour()
    : hue_(Hue::COUNT), lightness_(Lightness::COUNT)
{}
Colour::Colour(Hue hue, Lightness lightness)
    : hue_(lightness == Lightness::COUNT ? Hue::COUNT : hue),
      lightness_(hue == Hue::COUNT ? Lightness::COUNT : lightness)
{}
Colour::operator bool() const {
  return *this != unknown;
}
Hue Colour::hue() const {
  return hue_;
}
Lightness Colour::lightness() const {
  return lightness_;
}

const Colour Colour::unknown;

bool operator==(const Colour& lhs, const Colour& rhs) {
  return (lhs.hue_ == rhs.hue_ && lhs.lightness_ == rhs.lightness_);
}
bool operator!=(const Colour& lhs, const Colour& rhs) {
  return !(lhs == rhs);
}
