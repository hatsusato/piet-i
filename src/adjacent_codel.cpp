#include "adjacent_codel.hpp"
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
const Coord& AdjacentCodel::edge(Direction direction, Choose choose) const {
  return edges_.edge(direction, choose);
}
bool AdjacentCodel::includes(const Coord& coord) const {
  return coords_.includes(coord);
}
Coord AdjacentCodel::find_out_of_range(const Coord& coord,
                                       Direction direction) const {
  return coords_.find_out_of_range(coord, direction);
}

void search_connected_codel(CodelTable& table, Coordinates& coords,
                            const Codel& codel, const Coord& current) {
  coords.push_back(current);
  table.at(current) = Codel::unknown;
  for (Direction direction; direction; ++direction) {
    const auto next = current.next(direction);
    if (next.inside(0, 0, table.width(), table.height()) &&
        table.at(next) == codel) {
      search_connected_codel(table, coords, codel, next);
    }
  }
}

std::vector<AdjacentCodel> extract_connected_codels(const CodelTable& table) {
  const auto w = static_cast<int>(table.width());
  const auto h = static_cast<int>(table.height());
  auto tmp = table.clone();
  std::vector<AdjacentCodel> result;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const auto current = Coord(x, y);
      if (const auto codel = tmp.at(current)) {
        Coordinates coords;
        search_connected_codel(tmp, coords, codel, current);
        result.emplace_back(codel, coords);
      }
    }
  }
  return result;
}
