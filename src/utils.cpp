#include "utils.hpp"

bool operator==(const Pixel& lhs, const Pixel& rhs) {
  return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}

bool operator!=(const Pixel& lhs, const Pixel& rhs) {
  return !(lhs == rhs);
}

size_t gcd(size_t a, size_t b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}
