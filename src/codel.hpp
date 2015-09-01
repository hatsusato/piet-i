#ifndef PIET_I_CODEL_HPP
#define PIET_I_CODEL_HPP

#include "colour.hpp"

enum class ColourType {
  BLACK, WHITE, COLOUR, UNKNOWN
};

struct Codel {
 public:
  Codel();
  Codel(Color color, Brightness brightness);
  explicit Codel(const Pixel& pixel);
  bool is_valid() const;
  bool is_colored() const;
  Color color() const;
  Brightness brightness() const;
  void set_color(Color color);
  void set_brightness(Brightness brightness);
 private:
  Colour colour_;
  ColourType type_;
};
bool operator==(const Codel& lhs, const Codel& rhs);
bool operator!=(const Codel& lhs, const Codel& rhs);

#endif  // PIET_I_CODEL_HPP
