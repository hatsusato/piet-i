#include "utils.hpp"

Coord next_coordinate(const Coord& coord, Direction dir) {
  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, 1, 0, -1};
  const auto d = static_cast<int>(dir);
  int x, y;
  std::tie(x, y) = coord;
  return Coord(x + dx[d], y + dy[d]);
}
