#ifndef PIET_I_COLOUR_HPP
#define PIET_I_COLOUR_HPP

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
 private:
  Hue hue_;
  Lightness lightness_;
};

#endif  // PIET_I_COLOUR_HPP
