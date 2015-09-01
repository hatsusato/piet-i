#ifndef PIET_I_CONNECTED_CODEL_HPP
#define PIET_I_CONNECTED_CODEL_HPP

class ConnectedCodel {
 public:
  explicit ConnectedCodel(const Codel& codel,
                          const std::vector<Coord>& coords);
  const Codel& codel() const;
  size_t size() const;
  const Coord& edge(Direction direction, Choose choose) const;
  bool includes(const Coord& coord) const;
  Coord find_out_of_range(const Coord& coord, Direction direction) const;
 private:
  void calculate_boundary();
 private:
  Codel codel_;
  std::vector<Coord> coords_;
  // Direction count is 4, Choose count is 2
  Coord boundary_[4][2];
};

void search_connected_codel(CodelTable& table, std::vector<Coord>& coords,
                            const Codel& codel, int x, int y);
std::vector<ConnectedCodel> extract_connected_codels(const CodelTable& table);

#endif  // PIET_I_CONNECTED_CODEL_HPP
