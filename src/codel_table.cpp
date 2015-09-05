#include "codel_table.hpp"
#include <algorithm>
#include <iterator>
#include "coord.hpp"

namespace /* unnamed */ {
size_t gcd(size_t a, size_t b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}
}  // namespace /* unnamed */

CodelTable::CodelTable()
    : width_(0), height_(0), rows_()
{}
CodelTable::CodelTable(size_t width, size_t height)
    : width_(width), height_(height), rows_() {
  resize(width, height);
}
CodelTable CodelTable::clone() const {
  using std::begin;
  using std::end;
  CodelTable result(width_, height_);
  auto dst = begin(result.rows_);
  for (auto src = begin(rows_); src != end(rows_); ++src, ++dst) {
    std::copy(begin(*src), end(*src), begin(*dst));
  }
  return result;
}
size_t CodelTable::width() const {
  return width_;
}
size_t CodelTable::height() const {
  return height_;
}
void CodelTable::resize(size_t width, size_t height) {
  width_ = width;
  height_ = height;
  rows_.resize(height);
  for (auto& row : rows_) {
    row.resize(width);
  }
}
auto CodelTable::operator[](size_t row) -> RowType& {
  return rows_[row];
}
auto CodelTable::operator[](size_t row) const -> const RowType& {
  return rows_[row];
}
Codel& CodelTable::at(const Coord& position) {
  return rows_[position.y()][position.x()];
}
const Codel& CodelTable::at(const Coord& position) const {
  return rows_[position.y()][position.x()];
}

size_t codel_magnification(const Image& image) {
  const size_t width = image.get_width();
  const size_t height = image.get_height();
  size_t minimum = gcd(width, height);
  for (size_t row = 0; row < height; ++row) {
    // horizontal traverse
    Pixel prev_horz;
    size_t count_horz = 0;
    for (size_t col = 0; col < width; ++col) {
      const auto& pixel = image[row][col];
      if (0 < count_horz && prev_horz != pixel) {
        minimum = gcd(minimum, count_horz);
        count_horz = 0;
      }
      ++count_horz;
      prev_horz = pixel;
    }
  }
  for (size_t col = 0; col < width; ++col) {
    // vertical traverse
    Pixel prev_vert;
    size_t count_vert = 0;
    for (size_t row = 0; row < height; ++row) {
      const auto& pixel = image[row][col];
      if (0 < count_vert && prev_vert != pixel) {
        minimum = gcd(minimum, count_vert);
        count_vert = 0;
      }
      ++count_vert;
      prev_vert = pixel;
    }
  }
  return minimum;
}

CodelTable make_codel_table(const Image& image, RegardUnknownAs as) {
  const auto magnification = codel_magnification(image);
  const auto width = image.get_width() / magnification;
  const auto height = image.get_height() / magnification;
  const auto alternative = [as]() -> Codel {
    switch (as) {
      case RegardUnknownAs::BLACK:
      return Codel::black;
      case RegardUnknownAs::WHITE:
      return Codel::white;
      default:
      return Codel::unknown;
    }
  }();
  CodelTable table(width, height);
  for (size_t row = 0; row < height; ++row) {
    const auto& src_row = image[row * magnification];
    auto& dst_row = table[row];
    for (size_t col = 0; col < width; ++col) {
      const auto& pixel = src_row[col * magnification];
      const auto codel = make_codel(pixel);
      dst_row[col] = codel ? codel : alternative;
    }
  }
  return table;
}
