#ifndef PIET_I_CODEL_HPP
#define PIET_I_CODEL_HPP

#include "utils.hpp"

struct Codel {
 public:
  Codel();
  Codel(Color color, Brightness brightness);
  bool is_valid() const;
  Color color() const;
  Brightness brightness() const;
  void set_color(Color color);
  void set_brightness(Brightness brightness);
 private:
  Byte color_part() const;
  void color_part(Byte color);
  Byte brightness_part() const;
  void brightness_part(Byte brightness);
  // Lower 4bits represent color
  // Next 2bits represent brightness
  Byte data;
};

size_t codel_size(const Image& image);

#endif  // PIET_I_CODEL_HPP
