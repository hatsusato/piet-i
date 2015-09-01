#ifndef PIET_I_CODEL_TABLE_HPP
#define PIET_I_CODEL_TABLE_HPP

#include <vector>
#include "codel.hpp"
#include "png.hpp"

class CodelTable {
 public:
  using RowType = std::vector<Codel>;
  CodelTable();
  CodelTable(size_t width, size_t heigth);
  CodelTable clone() const;
  size_t width() const;
  size_t height() const;
  void resize(size_t width, size_t height);
  RowType& operator[](size_t row);
  const RowType& operator[](size_t row) const;
 private:
  size_t width_, height_;
  std::vector<RowType> rows_;
};

size_t codel_size(const Image& image);
CodelTable make_codel_table(const Image& image);

#endif  // PIET_I_CODEL_TABLE_HPP
