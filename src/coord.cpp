#include "coord.hpp"

Coord::Coord()
    : Base(0, 0)
{}
Coord::Coord(int x, int y)
    : Base(x, y)
{}
int& Coord::x() {
  return std::get<0>(*this);
}
const int& Coord::x() const {
  return std::get<0>(*this);
}
int& Coord::y() {
  return std::get<1>(*this);
}
const int& Coord::y() const {
  return std::get<1>(*this);
}
Coord& Coord::next(Direction direction) {
  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, 1, 0, -1};
  x() += dx[direction.value()];
  y() += dy[direction.value()];
  return *this;
}
