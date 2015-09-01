#include "coord.hpp"
#include <cassert>
#include <algorithm>
#include <functional>

namespace /* unnamed */ {
std::tuple<std::function<std::vector<int>(const Coordinates&)>,
           std::function<Coord(int)>,
           std::function<bool(const int&, const int&)> >
generate_utilities(Direction direction, int value) {
  const auto filter_horz =
      [value](const Coordinates& coords) -> std::vector<int> {
    std::vector<int> result;
    for (auto&& coord : coords) {
      if (coord.x() == value) {
        result.push_back(coord.y());
      }
    }
    return result;
  };
  const auto filter_vert =
      [value](const Coordinates& coords) -> std::vector<int> {
    std::vector<int> result;
    for (auto&& coord : coords) {
      if (coord.y() == value) {
        result.push_back(coord.x());
      }
    }
    return result;
  };
  const auto make_horz = [value](int minmax) -> Coord {
    return Coord(value, minmax);
  };
  const auto make_vert = [value](int minmax) -> Coord {
    return Coord(minmax, value);
  };
  const auto less = std::less<int>();
  const auto greater = std::greater<int>();
  switch (direction) {
    case DP::RIGHT:
      return std::make_tuple(filter_horz, make_horz, less);
    case DP::DOWN:
      return std::make_tuple(filter_vert, make_vert, greater);
    case DP::LEFT:
      return std::make_tuple(filter_horz, make_horz, greater);
    case DP::UP:
      return std::make_tuple(filter_vert, make_vert, less);
    default:
      assert(false);
      return std::make_tuple(nullptr, nullptr, nullptr);
  }
}
}  // namespace /* unnamed */

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
bool Coord::inside(int left, int top, int right, int bottom) const {
  return (left <= x() && top <= y() && x() < right && y() < bottom);
}

std::array<int, 4> Coordinates::range() const {
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

Coord& Edges::edge(Direction direction, Choose choose) {
  return edges_[direction.value()][choose.value()];
}
const Coord& Edges::edge(Direction direction, Choose choose) const {
  return edges_[direction.value()][choose.value()];
}
