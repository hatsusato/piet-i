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
bool Codel::is_valid() const {
  return (color_part_ < static_cast<unsigned>(Color::UNKNOWN) &&
          brightness_part_ < static_cast<unsigned>(Brightness::UNKNOWN));
}
bool Codel::is_colored() const {
  return (static_cast<unsigned>(Color::BLACK) < color_part_ &&
          color_part_ < static_cast<unsigned>(Color::WHITE));
}
Color Codel::color() const {
  return static_cast<Color>(color_part_);
}
Brightness Codel::brightness() const {
  return static_cast<Brightness>(brightness_part_);
}
void Codel::set_color(Color color) {
  color_part_ = static_cast<unsigned>(color);
}
void Codel::set_brightness(Brightness brightness) {
  brightness_part_ = static_cast<unsigned>(brightness);
}
bool operator==(const Codel& lhs, const Codel& rhs) {
  return (lhs.is_valid() && rhs.is_valid() &&
          lhs.color() == rhs.color() &&
          lhs.brightness() == rhs.brightness());
}
bool operator!=(const Codel& lhs, const Codel& rhs) {
  return !(lhs == rhs);
}
