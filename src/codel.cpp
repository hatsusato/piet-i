#include "codel.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>

namespace /* unnamed */ {
using UnaryPredicate = std::function<bool(const Coord&)>;
using BinaryPredicate = std::function<bool(const Coord&, const Coord&)>;

std::array<int, 4> coordinates_range(const std::vector<Coord>& coords) {
  assert(!coords.empty());
  int right, up, left, down;
  right = left = std::get<0>(coords.front());
  up = down = std::get<1>(coords.front());
  for (auto&& coord : coords) {
    const auto x = std::get<0>(coord);
    const auto y = std::get<1>(coord);
    right = std::max(right, x);
    up = std::min(up, y);
    left = std::min(left, x);
    down = std::max(down, y);
  }
  return {{right, up, left, down}};
}
UnaryPredicate generate_same_predicate(Direction dir, int value) {
  using namespace std::placeholders;
  switch (dir) {
    case Direction::RIGHT:
    case Direction::LEFT:
      return std::bind(same_element<0>, _1, value);
    case Direction::UP:
    case Direction::DOWN:
      return std::bind(same_element<1>, _1, value);
    default:
      assert(false);
      return nullptr;
  }
}
BinaryPredicate generate_compare_predicate(Direction dir) {
  // Choose::LEFT side is less than Choose::RIGHT side
  switch (dir) {
    case Direction::RIGHT:
      return less_element<1>;
    case Direction::UP:
      return less_element<0>;
    case Direction::LEFT:
      return greater_element<1>;
    case Direction::DOWN:
      return greater_element<0>;
    default:
      assert(false);
      return nullptr;
  }
}
std::tuple<Coord, Coord> range_edge(const std::vector<Coord>& coords,
                                    UnaryPredicate same,
                                    BinaryPredicate compare) {
  using std::begin;
  using std::end;
  assert(!coords.empty());
  std::vector<Coord> filtered;
  std::copy_if(begin(coords), end(coords), std::back_inserter(filtered), same);
  assert(!filtered.empty());
  const auto minmax =
      std::minmax_element(begin(filtered), end(filtered), compare);
  return std::make_tuple(*std::get<0>(minmax), *std::get<1>(minmax));
}
}  // namespace /* unnamed */

Codel::Codel()
    : Codel(Color::UNKNOWN, Brightness::UNKNOWN)
{}
Codel::Codel(Color color, Brightness brightness)
    : color_part_(static_cast<unsigned>(color)),
      brightness_part_(static_cast<unsigned>(brightness))
{}
Codel::Codel(const Pixel& pixel)
    : Codel(what_color(pixel), how_bright(pixel))
{}
bool Codel::is_valid() const {
  return (color_part_ < static_cast<unsigned>(Color::UNKNOWN) &&
          brightness_part_ < static_cast<unsigned>(Brightness::UNKNOWN));
}
Color Codel::color() const {
  return static_cast<Color>(color_part_);
}
Brightness Codel::brightness() const {
  return static_cast<Brightness>(brightness_part_);
}
void Codel::set_color(Color color) {
  color_part_ = static_cast<unsigned>(color);
}
void Codel::set_brightness(Brightness brightness) {
  brightness_part_ = static_cast<unsigned>(brightness);
}
bool operator==(const Codel& lhs, const Codel& rhs) {
  return (lhs.is_valid() && rhs.is_valid() &&
          lhs.color() == rhs.color() &&
          lhs.brightness() == rhs.brightness());
}
bool operator!=(const Codel& lhs, const Codel& rhs) {
  return !(lhs == rhs);
}
const auto Codel::unknown = Codel();

CodelTable::CodelTable()
    : width_(0), height_(0), rows_()
{}
CodelTable::CodelTable(size_t width, size_t height)
    : width_(width), height_(height), rows_() {
  resize(width, height);
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

ConnectedCodel::ConnectedCodel(const Codel& codel,
                               const std::vector<Coord>& coords)
    : codel_(codel), coords_(coords), boundary_() {
  calculate_boundary();
}
const Codel& ConnectedCodel::codel() const {
  return codel_;
}
bool ConnectedCodel::includes(const Coord& coord) const {
  using std::begin;
  using std::end;
  return std::find(begin(coords_), end(coords_), coord) != end(coords_);
}
void ConnectedCodel::calculate_boundary() {
  using std::begin;
  using std::end;
  assert(!coords_.empty());
  const auto range = coordinates_range(coords_);
  Coord left, right;
  for (int i = 0; i < 4; ++i) {
    const auto dir = static_cast<Direction>(i);
    const auto same = generate_same_predicate(dir, range[i]);
    const auto compare = generate_compare_predicate(dir);
    std::tie(left, right) = range_edge(coords_, same, compare);
    boundary_[i][0] = next_coordinate(left, dir);
    boundary_[i][1] = next_coordinate(right, dir);
  }
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

CodelTable make_codel_table(const Image& image) {
  const auto step = codel_size(image);
  const auto w = image.get_width() / step;
  const auto h = image.get_height() / step;
  CodelTable table(w, h);
  for (size_t row = 0; row < h; ++row) {
    const auto& src_row = image[row * step];
    auto& dst_row = table[row];
    for (size_t col = 0; col < w; ++col) {
      dst_row[col] = static_cast<Codel>(src_row[col * step]);
    }
  }
  return table;
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
  auto tmp = table;
  std::vector<ConnectedCodel> result;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const auto& codel = tmp[y][x];
      if (codel.is_valid()) {
        std::vector<Coord> coords;
        search_connected_codel(tmp, coords, codel, x, y);
        result.emplace_back(codel, coords);
      }
    }
  }
  return result;
}
