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
