#ifndef PIET_I_CODEL_HPP
#define PIET_I_CODEL_HPP

#include <vector>
#include "utils.hpp"

struct Codel {
 public:
  Codel();
  Codel(Color color, Brightness brightness);
  explicit Codel(const Pixel& pixel);
  bool is_valid() const;
  Color color() const;
  Brightness brightness() const;
  void set_color(Color color);
  void set_brightness(Brightness brightness);
 private:
  struct {
    unsigned color_part_ : 4;
    unsigned brightness_part_ : 4;
  };
 public:
  static const Codel unknown;
};
bool operator==(const Codel& lhs, const Codel& rhs);
bool operator!=(const Codel& lhs, const Codel& rhs);

class CodelTable {
 public:
  using RowType = std::vector<Codel>;
  CodelTable();
  CodelTable(size_t width, size_t heigth);
  size_t width() const;
  size_t height() const;
  void resize(size_t width, size_t height);
  RowType& operator[](size_t row);
  const RowType& operator[](size_t row) const;
 private:
  size_t width_, height_;
  std::vector<RowType> rows_;
};

class ConnectedCodel {
 public:
  explicit ConnectedCodel(const Codel& codel);
  const Codel& codel() const;
  const std::vector<Coord>& coords() const;
  void push(int x, int y);
  bool includes(const Coord& coord) const;
 private:
  Codel codel_;
  std::vector<Coord> coords_;
};

class ConnectedCodelBoundary {
 public:
  explicit ConnectedCodelBoundary(const ConnectedCodel& connected);
 private:
  // Direction count is 4, Choose count is 2
  Coord boundary_[4][2];
};

size_t codel_size(const Image& image);
CodelTable make_codel_table(const Image& image);

void search_connected_codel(CodelTable& image, ConnectedCodel& connected,
                            int x, int y);
std::vector<ConnectedCodel> extract_connected_codels(const CodelTable& table);
std::vector<ConnectedCodelBoundary> make_connected_codel_boundaries(
    const std::vector<ConnectedCodel>& connected_codels);

#endif  // PIET_I_CODEL_HPP
