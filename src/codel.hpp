#ifndef PIET_I_CODEL_HPP
#define PIET_I_CODEL_HPP

#include "png.hpp"
#include "utils.hpp"

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
  struct {
    unsigned color_part_ : 4;
    unsigned brightness_part_ : 4;
  };
 public:
  static const Codel unknown;
};
bool operator==(const Codel& lhs, const Codel& rhs);
bool operator!=(const Codel& lhs, const Codel& rhs);

#endif  // PIET_I_CODEL_HPP
