#include "edges.hpp"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <functional>
#include <tuple>
#include <vector>
#include "coord.hpp"

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
  decltype(begin(filtered)) left, right;
  std::tie(left, right) =
      std::minmax_element(begin(filtered), end(filtered), compare);
  assert(left != end(filtered) && right != end(filtered));
  return std::make_tuple(make(*left), make(*right));
}
}  // namespace /* unnamed */

Edges<Coord> make_edges(const Coordinates& coords) {
  assert(!coords.empty());
  Edges<Coord> result;
  const auto range = coords.range();
  Coord left, right;
  for (Direction direction; direction; ++direction) {
    std::tie(left, right) =
        edge_minmax(coords, direction, range[direction.value()]);
    result.edge(direction, CC::LEFT) = left.next(direction);
    result.edge(direction, CC::RIGHT) = right.next(direction);
  }
  return result;
}
