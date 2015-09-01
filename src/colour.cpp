#include "colour.hpp"

Colour::Colour()
    : hue_(Hue::COUNT), lightness_(Lightness::COUNT)
{}
Colour::Colour(Hue hue, Lightness lightness)
    : hue_(lightness == Lightness::COUNT ? Hue::COUNT : hue),
      lightness_(hue == Hue::COUNT ? Lightness::COUNT : lightness)
{}
