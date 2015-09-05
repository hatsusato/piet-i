#ifndef PIET_I_ADJACENT_CODEL_HPP
#define PIET_I_ADJACENT_CODEL_HPP

#include <vector>
#include "codel.hpp"
#include "coord.hpp"
#include "direction.hpp"
#include "edges.hpp"

class CodelTable;

class AdjacentCodel {
 public:
  AdjacentCodel(const Codel& codel, const Coordinates& coords);
  const Codel& codel() const;
  size_t size() const;
  const Coord& edge(DP dp, CC cc) const;
  bool includes(const Coord& coord) const;
  Coord find_out_of_range(const Coord& coord, DP dp) const;
 private:
  Codel codel_;
  Coordinates coords_;
  Edges<Coord> edges_;
};

void search_adjacent_codel(CodelTable& table, Coordinates& coords,
                           const Codel& codel, const Coord& current);
std::vector<AdjacentCodel> make_adjacent_codels(const CodelTable& table);

#endif  // PIET_I_ADJACENT_CODEL_HPP
