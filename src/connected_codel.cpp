#include "connected_codel.hpp"
#include "codel_table.hpp"

ConnectedCodel::ConnectedCodel(const Codel& codel, const Coordinates& coords)
    : codel_(codel), coords_(coords), edges_(coords)
{}
const Codel& ConnectedCodel::codel() const {
  return codel_;
}
size_t ConnectedCodel::size() const {
  return coords_.size();
}
const Coord& ConnectedCodel::edge(Direction direction, Choose choose) const {
  return edges_.edge(direction, choose);
}
bool ConnectedCodel::includes(const Coord& coord) const {
  return coords_.includes(coord);
}
Coord ConnectedCodel::find_out_of_range(const Coord& coord,
                                        Direction direction) const {
  return coords_.find_out_of_range(coord, direction);
}

void search_connected_codel(CodelTable& table, Coordinates& coords,
                            const Codel& codel, Coord current) {
  coords.push_back(current);
  table.at(current) = Codel::unknown;
  for (Direction direction; direction; ++direction) {
    const auto next = current.next(direction);
    if (next.inside(0, 0, table.width(), table.height())) {
      if (table.at(next) == codel) {
        search_connected_codel(table, coords, codel, next);
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
