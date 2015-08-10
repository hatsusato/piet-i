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
    unsigned color_part : 4;
    unsigned brightness_part : 4;
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
  size_t w, h;
  std::vector<RowType> rows;
};

size_t codel_size(const Image& image);
CodelTable make_codel_table(const Image& image);

#endif  // PIET_I_CODEL_HPP
