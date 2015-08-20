#include "codel.hpp"
#include <algorithm>

Codel::Codel()
    : Codel(Color::UNKNOWN, Brightness::UNKNOWN)
{}
Codel::Codel(Color color, Brightness brightness)
    : color_part(static_cast<Byte>(color)),
      brightness_part(static_cast<Byte>(brightness))
{}
Codel::Codel(const Pixel& pixel)
    : Codel(what_color(pixel), how_bright(pixel))
{}
bool Codel::is_valid() const {
  return (color_part < static_cast<Byte>(Color::UNKNOWN) &&
          brightness_part < static_cast<Byte>(Brightness::UNKNOWN));
}
Color Codel::color() const {
  return static_cast<Color>(color_part);
}
Brightness Codel::brightness() const {
  return static_cast<Brightness>(brightness_part);
}
void Codel::set_color(Color color) {
  color_part = static_cast<Byte>(color);
}
void Codel::set_brightness(Brightness brightness) {
  brightness_part = static_cast<Byte>(brightness);
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
    : w(0), h(0), rows()
{}
CodelTable::CodelTable(size_t width, size_t height)
    : w(width), h(height), rows() {
  resize(width, height);
}
size_t CodelTable::width() const {
  return w;
}
size_t CodelTable::height() const {
  return h;
}
void CodelTable::resize(size_t width, size_t height) {
  w = width;
  h = height;
  rows.resize(h);
  for (auto&& row : rows) {
    row.resize(w);
  }
}
auto CodelTable::operator[](size_t row) -> RowType& {
  return rows[row];
}
auto CodelTable::operator[](size_t row) const -> const RowType& {
  return rows[row];
}

ConnectedCodel::ConnectedCodel(const Codel& color)
    : codel(color), coords()
{}
const Codel& ConnectedCodel::color() const {
  return codel;
}
const std::vector<Coord>& ConnectedCodel::coordinates() const {
  return coords;
}
void ConnectedCodel::push(int x, int y) {
  coords.emplace_back(x, y);
}
bool ConnectedCodel::includes(const Coord& coord) const {
  using std::begin;
  using std::end;
  return std::find(begin(coords), end(coords), coord) != end(coords);
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

void search_connected_codel(CodelTable& table, ConnectedCodel& connected,
                            int x, int y) {
  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, -1, 0, 1};
  const auto w = static_cast<int>(table.width());
  const auto h = static_cast<int>(table.height());
  connected.push(x, y);
  table[y][x] = Codel::unknown;
  for (int i = 0; i < 4; ++i) {
    const auto next_x = x + dx[i];
    const auto next_y = y + dy[i];
    if (0 <= next_x && next_x < w && 0 <= next_y && next_y < h) {
      const auto& next_codel = table[next_y][next_x];
      if (next_codel.is_valid() && connected.color() == next_codel) {
        search_connected_codel(table, connected, next_x, next_y);
      }
    }
  }
}
