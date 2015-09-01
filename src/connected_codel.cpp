#include "connected_codel.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>

namespace /* unnamed */ {
using UnaryPredicate = std::function<bool(const Coord&)>;
using BinaryPredicate = std::function<bool(const Coord&, const Coord&)>;

std::array<int, 4> coordinates_range(const std::vector<Coord>& coords) {
  assert(!coords.empty());
  int right, up, left, down;
  right = left = std::get<0>(coords.front());
  up = down = std::get<1>(coords.front());
  for (auto&& coord : coords) {
    const auto x = std::get<0>(coord);
    const auto y = std::get<1>(coord);
    right = std::max(right, x);
    down = std::max(down, y);
    left = std::min(left, x);
    up = std::min(up, y);
  }
  return {{right, down, left, up}};
}
UnaryPredicate generate_same_predicate(Direction dir, int value) {
  using namespace std::placeholders;
  switch (dir) {
    case Direction::RIGHT:
    case Direction::LEFT:
      return std::bind(same_element<0>, _1, value);
    case Direction::UP:
    case Direction::DOWN:
      return std::bind(same_element<1>, _1, value);
    default:
      assert(false);
      return nullptr;
  }
}
BinaryPredicate generate_compare_predicate(Direction dir) {
  // Choose::LEFT side is less than Choose::RIGHT side
  switch (dir) {
    case Direction::RIGHT:
      return less_element<1>;
    case Direction::UP:
      return less_element<0>;
    case Direction::LEFT:
      return greater_element<1>;
    case Direction::DOWN:
      return greater_element<0>;
    default:
      assert(false);
      return nullptr;
  }
}
std::tuple<Coord, Coord> range_edge(const std::vector<Coord>& coords,
                                    UnaryPredicate same,
                                    BinaryPredicate compare) {
  using std::begin;
  using std::end;
  assert(!coords.empty());
  std::vector<Coord> filtered;
  std::copy_if(begin(coords), end(coords), std::back_inserter(filtered), same);
  assert(!filtered.empty());
  const auto minmax =
      std::minmax_element(begin(filtered), end(filtered), compare);
  return std::make_tuple(*std::get<0>(minmax), *std::get<1>(minmax));
}
}  // namespace /* unnamed */

ConnectedCodel::ConnectedCodel(const Codel& codel,
                               const std::vector<Coord>& coords)
    : codel_(codel), coords_(coords), boundary_() {
  calculate_boundary();
}
const Codel& ConnectedCodel::codel() const {
  return codel_;
}
size_t ConnectedCodel::size() const {
  return coords_.size();
}
const Coord& ConnectedCodel::edge(Direction direction, Choose choose) const {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  return boundary_[d][c];
}
bool ConnectedCodel::includes(const Coord& coord) const {
  using std::begin;
  using std::end;
  return std::find(begin(coords_), end(coords_), coord) != end(coords_);
}
Coord ConnectedCodel::find_out_of_range(const Coord& coord,
                                        Direction direction) const {
  Coord current = coord;
  while (includes(current)) {
    current = next_coordinate(current, direction);
  }
  return current;
}
void ConnectedCodel::calculate_boundary() {
  using std::begin;
  using std::end;
  assert(!coords_.empty());
  const auto range = coordinates_range(coords_);
  Coord left, right;
  for (int i = 0; i < 4; ++i) {
    const auto dir = static_cast<Direction>(i);
    const auto same = generate_same_predicate(dir, range[i]);
    const auto compare = generate_compare_predicate(dir);
    std::tie(left, right) = range_edge(coords_, same, compare);
    boundary_[i][0] = next_coordinate(left, dir);
    boundary_[i][1] = next_coordinate(right, dir);
  }
}

void search_connected_codel(CodelTable& table, std::vector<Coord>& coords,
                            const Codel& codel, int x, int y) {
  const auto w = static_cast<int>(table.width());
  const auto h = static_cast<int>(table.height());
  coords.emplace_back(x, y);
  table[y][x] = Codel::unknown;
  int next_x, next_y;
  for (int i = 0; i < 4; ++i) {
    const auto dir = static_cast<Direction>(i);
    std::tie(next_x, next_y) = next_coordinate(Coord(x, y), dir);
    if (0 <= next_x && next_x < w && 0 <= next_y && next_y < h) {
      const auto& next_codel = table[next_y][next_x];
      if (next_codel.is_valid() && codel == next_codel) {
        search_connected_codel(table, coords, codel, next_x, next_y);
      }
    }
  }
}

std::vector<ConnectedCodel> extract_connected_codels(const CodelTable& table) {
  const auto w = static_cast<int>(table.width());
  const auto h = static_cast<int>(table.height());
  auto tmp = table.clone();
  std::vector<ConnectedCodel> result;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const auto codel = tmp[y][x];
      if (codel.is_valid()) {
        std::vector<Coord> coords;
        search_connected_codel(tmp, coords, codel, x, y);
        result.emplace_back(codel, coords);
      }
    }
  }
  return result;
}
