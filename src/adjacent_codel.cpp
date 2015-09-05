#include "adjacent_codel.hpp"
#include <cassert>
#include "codel_table.hpp"

AdjacentCodel::AdjacentCodel(const Codel& codel, const Coordinates& coords)
    : codel_(codel), coords_(coords), edges_(make_edges(coords))
{}
const Codel& AdjacentCodel::codel() const {
  return codel_;
}
size_t AdjacentCodel::size() const {
  return coords_.size();
}
const Coord& AdjacentCodel::edge(DP dp, CC cc) const {
  return edges_.edge(dp, cc);
}
bool AdjacentCodel::includes(const Coord& coord) const {
  return coords_.includes(coord);
}
Coord AdjacentCodel::find_out_of_range(const Coord& coord, DP dp) const {
  return coords_.find_out_of_range(coord, dp);
}

void search_adjacent_codel(CodelTable& table, Coordinates& coords,
                           const Codel& codel, const Coord& current) {
  coords.push_back(current);
  table.at(current) = Codel::unknown;
  for (DP dp; dp; dp.next()) {
    const auto next = current.next(dp);
    if (next.inside(0, 0, table.width(), table.height()) &&
        table.at(next) == codel) {
      search_adjacent_codel(table, coords, codel, next);
    }
  }
}

std::vector<AdjacentCodel> make_adjacent_codels(const CodelTable& table) {
  const auto width = static_cast<int>(table.width());
  const auto height = static_cast<int>(table.height());
  auto tmp = table.clone();
  std::vector<AdjacentCodel> result;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      const auto current = Coord(col, row);
      if (const auto codel = tmp.at(current)) {
        Coordinates coords;
        search_adjacent_codel(tmp, coords, codel, current);
        result.emplace_back(codel, coords);
      }
    }
  }
  assert(result.front().includes(Coord(0, 0)));
  return result;
}
