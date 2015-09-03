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

Colour make_colour(const Pixel& pixel) {
  const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
  const Byte z = 0x00, c = 0xC0, f = 0xFF;
  const auto which_hue = [r, g, b](Byte lower, Byte upper) -> Hue {
    if (b == lower) {
      if (g == lower) {
        return Hue::RED;
      } else if (r == upper) {
        return Hue::YELLOW;
      } else {
        return Hue::GREEN;
      }
    } else {
      if (g == upper) {
        return Hue::CYAN;
      } else if(r == lower) {
        return Hue::BLUE;
      } else {
        return Hue::MAGENTA;
      }
    }
  };
  if ((r == z || r == c || r == f) &&
      (g == z || g == c || g == f) &&
      (b == z || b == c || b == f)) {
    const bool has_z = (r == z || g == z || b == z);
    const bool has_c = (r == c || g == c || b == c);
    const bool has_f = (r == f || g == f || b == f);
    if (!has_z && has_c && has_f) {
      return Colour(which_hue(c, f), Lightness::LIGHT);
    } else if (has_z && !has_c && has_f) {
      return Colour(which_hue(z, f), Lightness::NORMAL);
    } else if (has_z && has_c && !has_f) {
      return Colour(which_hue(z, c), Lightness::DARK);
    }
  }
  return Colour::unknown;
}

int difference(const Colour& current, const Colour& next) {
  const int current_hue = static_cast<int>(current.hue());
  const int next_hue = static_cast<int>(next.hue());
  const int hue_count = static_cast<int>(Hue::COUNT);
  const int current_lightness = static_cast<int>(current.lightness());
  const int next_lightness = static_cast<int>(next.lightness());
  const int lightness_count = static_cast<int>(Lightness::COUNT);
  const int hue_diff = (hue_count + next_hue - current_hue) % hue_count;
  const int lightness_diff =
      (lightness_count + next_lightness - current_lightness) % lightness_count;
  return hue_diff * lightness_count + lightness_diff;
}
