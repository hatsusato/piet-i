#ifndef PIET_I_CONNECTED_CODEL_HPP
#define PIET_I_CONNECTED_CODEL_HPP

#include "codel.hpp"
#include "coord.hpp"

class CodelTable;

class ConnectedCodel {
 public:
  ConnectedCodel(const Codel& codel, const Coordinates& coords);
  const Codel& codel() const;
  size_t size() const;
  const Coord& edge(Direction direction, Choose choose) const;
  bool includes(const Coord& coord) const;
  Coord find_out_of_range(const Coord& coord, Direction direction) const;
 private:
  Codel codel_;
  Coordinates coords_;
  Edges edges_;
};

void search_connected_codel(CodelTable& table, Coordinates& coords,
                            const Codel& codel, Coord current);
std::vector<ConnectedCodel> extract_connected_codels(const CodelTable& table);

#endif  // PIET_I_CONNECTED_CODEL_HPP
