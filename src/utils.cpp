#include "utils.hpp"

Color what_color(const Pixel& pixel) {
  const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
  const Byte z = 0x00, c = 0xC0, f = 0xFF;
  if ((r == z || r == c || r == f) &&
      (g == z || g == c || g == f) &&
      (b == z || b == c || b == f)) {
    if (r == z && g == z && b == z) {
      return Color::BLACK;
    } else if (r == f && g == f && b == f) {
      return Color::WHITE;
    } else if (g == z && b == z) {
      return Color::RED;
    } else if (r == z && b == z) {
      return Color::GREEN;
    } else if (r == z && g == z) {
      return Color::BLUE;
    } else if (r == f && g == f) {
      return Color::YELLOW;
    } else if (g == f && b == f) {
      return Color::CYAN;
    } else if (r == f && b == f) {
      return Color::MAGENTA;
    } else if (r == f) {
      return Color::RED;
    } else if (g == f) {
      return Color::GREEN;
    } else if (b == f) {
      return Color::BLUE;
    } else if (b == z) {
      return Color::YELLOW;
    } else if (r == z) {
      return Color::CYAN;
    } else if (g == z) {
      return Color::MAGENTA;
    }
  }
  return Color::UNKNOWN;
}

Brightness how_bright(const Pixel& pixel) {
  const Byte r = pixel.red, g = pixel.green, b = pixel.blue;
  const Byte c = 0xC0;
  const auto mini = std::min(r, std::min(g, b));
  const auto maxi = std::max(r, std::max(g, b));
  if (mini == c) {
    return Brightness::LIGHT;
  } else if (maxi == c) {
    return Brightness::DARK;
  } else {
    return Brightness::NORMAL;
  }
}

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

Coord next_coordinate(const Coord& coord, Direction dir) {
  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, -1, 0, 1};
  const auto d = static_cast<int>(dir);
  int x, y;
  std::tie(x, y) = coord;
  return Coord(x + dx[d], y + dy[d]);
}
