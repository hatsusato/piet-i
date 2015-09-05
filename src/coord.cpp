#include "coord.hpp"
#include <cassert>
#include <algorithm>
#include <iterator>

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
Coord Coord::next(DP dp) const {
  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, 1, 0, -1};
  auto result = *this;
  result.x() += dx[index(dp)];
  result.y() += dy[index(dp)];
  return result;
}
bool Coord::inside(int left, int top, int right, int bottom) const {
  return (left <= x() && top <= y() && x() < right && y() < bottom);
}

std::array<int, DP::count> Coordinates::range() const {
  assert(!empty());
  int right, down, left, up;
  right = left = front().x();
  up = down = front().y();
  for (auto&& coord : *this) {
    right = std::max(right, coord.x());
    down = std::max(down, coord.y());
    left = std::min(left, coord.x());
    up = std::min(up, coord.y());
  }
  return {{right, down, left, up}};
}
bool Coordinates::includes(const Coord& coord) const {
  using std::begin;
  using std::end;
  return std::find(begin(*this), end(*this), coord) != end(*this);
}
Coord Coordinates::find_out_of_range(const Coord& coord, DP dp) const {
  Coord current = coord;
  while (includes(current)) {
    current = current.next(dp);
  }
  return current;
}
