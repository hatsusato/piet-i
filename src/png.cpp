#include "png.hpp"

bool operator==(const Pixel& lhs, const Pixel& rhs) {
  return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}
bool operator!=(const Pixel& lhs, const Pixel& rhs) {
  return !(lhs == rhs);
}
