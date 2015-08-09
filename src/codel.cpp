#include "codel.hpp"

Codel::Codel()
    : data(0)
{}
Codel::Codel(Color color, Brightness brightness)
    : data(static_cast<Byte>(color) + static_cast<Byte>(brightness) * 16)
{}
bool Codel::is_valid() const {
  return (color_part() < static_cast<Byte>(Color::UNKNOWN) &&
          brightness_part() < static_cast<Byte>(Brightness::UNKNOWN));
}
Color Codel::color() const {
  return static_cast<Color>(color_part());
}
Brightness Codel::brightness() const {
  return static_cast<Brightness>(brightness_part());
}
void Codel::set_color(Color color) {
  color_part(static_cast<Byte>(color));
}
void Codel::set_brightness(Brightness brightness) {
  brightness_part(static_cast<Byte>(brightness));
}
Byte Codel::color_part() const {
  return data % 16;
}
void Codel::color_part(Byte color) {
  data = brightness_part() * 16 + color;
}
Byte Codel::brightness_part() const {
  return data / 16;
}
void Codel::brightness_part(Byte brightness) {
  data = brightness * 16 + color_part();
}

size_t codel_size(const Image& image) {
  const size_t width = image.get_width();
  const size_t height = image.get_height();
  Pixel prev;
  size_t minimum = gcd(width, height);
  size_t count = 0;
  // vertical iterate
  for (size_t row = 0; row < height; ++row) {
    for (size_t col = 0; col < width; ++col) {
      const auto pixel = image[row][col];
      if (0 < count && prev != pixel) {
        minimum = gcd(minimum, count);
        count = 0;
      }
      ++count;
      prev = pixel;
    }
  }
  count = 0;
  // horizontal iterate
  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < height; ++row) {
      const auto pixel = image[row][col];
      if (0 < count && prev != pixel) {
        minimum = gcd(minimum, count);
        count = 0;
      }
      ++count;
      prev = pixel;
    }
  }
  return minimum;
}
