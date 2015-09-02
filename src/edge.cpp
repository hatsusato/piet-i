#include "edge.hpp"
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
