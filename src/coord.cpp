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
std::tuple<Coord, Coord> edge_minmax(const Coordinates& coords,
                                     Direction direction, int border) {
  using std::begin;
  using std::end;
  assert(!coords.empty());
  const auto functions = generate_utilities(direction, border);
  const auto filter = std::get<0>(functions);
  const auto make = std::get<1>(functions);
  const auto compare = std::get<2>(functions);
  const auto filtered = filter(coords);
  decltype(begin(filtered)) minimum, maximum;
  std::tie(minimum, maximum) =
      std::minmax_element(begin(filtered), end(filtered), compare);
  assert(minimum != end(filtered) && maximum != end(filtered));
  return std::make_tuple(make(*minimum), make(*maximum));
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

Edges::Edges(const Coordinates& coords)
    : edges_() {
  assert(!coords.empty());
  const auto range = coords.range();
  for (Direction direction; direction; ++direction) {
    const auto minmax =
        edge_minmax(coords, direction, range[direction.value()]);
    edge(direction, CC::LEFT) = std::get<0>(minmax);
    edge(direction, CC::RIGHT) = std::get<1>(minmax);
  }
}
Coord& Edges::edge(Direction direction, Choose choose) {
  return edges_[direction.value()][choose.value()];
}
const Coord& Edges::edge(Direction direction, Choose choose) const {
  return edges_[direction.value()][choose.value()];
}
