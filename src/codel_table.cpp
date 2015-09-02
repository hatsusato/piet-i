#include "codel_table.hpp"
#include "codel.hpp"
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
  for (auto&& row : rows_) {
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

size_t codel_size(const Image& image) {
  const size_t width = image.get_width();
  const size_t height = image.get_height();
  Pixel prev;
  size_t minimum = gcd(width, height);
  size_t count = 0;
  // vertical iterate
  for (size_t row = 0; row < height; ++row) {
    for (size_t col = 0; col < width; ++col) {
      const auto pixel = image[row][col];
      if (0 < count && prev != pixel) {
        minimum = gcd(minimum, count);
        count = 0;
      }
      ++count;
      prev = pixel;
    }
  }
  count = 0;
  // horizontal iterate
  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < height; ++row) {
      const auto pixel = image[row][col];
      if (0 < count && prev != pixel) {
        minimum = gcd(minimum, count);
        count = 0;
      }
      ++count;
      prev = pixel;
    }
  }
  return minimum;
}

CodelTable make_codel_table(const Image& image, RegardUnknownAs as) {
  const auto step = codel_size(image);
  const auto w = image.get_width() / step;
  const auto h = image.get_height() / step;
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
  CodelTable table(w, h);
  for (size_t row = 0; row < h; ++row) {
    const auto& src_row = image[row * step];
    auto& dst_row = table[row];
    for (size_t col = 0; col < w; ++col) {
      const auto& pixel = src_row[col * step];
      const auto codel = make_codel(pixel);
      dst_row[col] = codel ? codel : alternative;
    }
  }
  return table;
}
