#include "color_block.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

namespace /* unnamed */ {
template <size_t N, class T>
struct Extractor {
  ColorBlockPtr operator()(T&& data) {
    return std::move(std::get<N>(data));
  }
};
}  // namespace /* unnamed */

ColorBlockBase::ColorBlockBase() {}
ColorBlockBase::~ColorBlockBase() {}
size_t ColorBlockBase::codel_size() const {
  return 0;
}
const ColorBlockBase* ColorBlockBase::address() const {
  return this;
}
bool ColorBlockBase::is_colored() const {
  return false;
}
bool ColorBlockBase::is_white() const {
  return false;
}
bool ColorBlockBase::is_black() const {
  return false;
}

ColorBlock::ColorBlock(const ConnectedCodel& connected)
    : codel_(connected.codel()), codel_size_(connected.size()), next_() {
  assert(codel_.is_colored());
}
Codel ColorBlock::codel() const {
  return codel_;
}
size_t ColorBlock::codel_size() const {
  return codel_size_;
}
const ColorBlockBase* ColorBlock::next(Direction direction,
                                       Choose choose) const {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  return next_[d][c];
}
void ColorBlock::set_next(const ColorBlockBase* next,
                          Direction direction, Choose choose) {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  next_[d][c] = next;
}
bool ColorBlock::is_colored() const {
  return true;
}

BlackBlock::BlackBlock()
{}
Codel BlackBlock::codel() const {
  return Codel(Color::BLACK, Brightness::NORMAL);
}
const ColorBlockBase* BlackBlock::next(Direction, Choose) const {
  return this;
}
bool BlackBlock::is_black() const {
  return true;
}

WhiteBlock::WhiteBlock(const ColorBlockBase* next)
    : next_(next)
{}
Codel WhiteBlock::codel() const {
  return Codel(Color::WHITE, Brightness::NORMAL);
}
const ColorBlockBase* WhiteBlock::next(Direction, Choose) const {
  return next_;
}
bool WhiteBlock::is_white() const {
  return true;
}

ColorBlockInfo::ColorBlockInfo(const CodelTable& table)
    : table_(table), color_blocks_(), mono_blocks_() {
  initialize();
  connect_all();
}
std::vector<ColorBlockPtr> ColorBlockInfo::extract_color_blocks() {
  using std::begin;
  using std::end;
  struct NullChecker {
    bool operator()(const ColorBlockPtr& ptr) {
      return static_cast<bool>(ptr);
    }
  };
  std::vector<ColorBlockPtr> extracted;
  std::transform(std::make_move_iterator(begin(color_blocks_)),
                 std::make_move_iterator(end(color_blocks_)),
                 std::back_inserter(extracted),
                 Extractor<1, ColorBlockData>());
  std::vector<ColorBlockPtr> result;
  std::copy_if(std::make_move_iterator(begin(extracted)),
               std::make_move_iterator(end(extracted)),
               std::back_inserter(result), NullChecker());
  extracted.clear();
  std::transform(std::make_move_iterator(begin(mono_blocks_)),
                 std::make_move_iterator(end(mono_blocks_)),
                 std::back_inserter(extracted),
                 Extractor<1, MonoBlockData>());
  std::move(begin(extracted), end(extracted), std::back_inserter(result));
  color_blocks_.clear();
  mono_blocks_.clear();
  return result;
}
void ColorBlockInfo::initialize() {
  assert(color_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.emplace_back(nullptr, make_unique<BlackBlock>());
  const auto connected_codels = extract_connected_codels(table_);
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    auto block_pointer = codel.is_colored() ?
        make_unique<ColorBlock>(connected) : nullptr;
    color_blocks_.emplace_back(connected, std::move(block_pointer));
  }
}
void ColorBlockInfo::connect_all() {
  using namespace std::placeholders;
  using std::begin;
  using std::end;
  const auto connect = std::bind(&ColorBlockInfo::connect, this, _1);
  std::for_each(begin(color_blocks_), end(color_blocks_), connect);
}
void ColorBlockInfo::connect(ColorBlockData& color_block) {
  const auto& connected = std::get<0>(color_block);
  auto& pointer = std::get<1>(color_block);
  if (connected.codel().is_colored()) {
    for (int d = 0; d < 4; ++d) {
      const auto direction = static_cast<Direction>(d);
      for (int c = 0; c < 2; ++c) {
        const auto choose = static_cast<Choose>(c);
        const auto edge = connected.edge(direction, choose);
        const auto next = get_access_point(edge, direction);
        assert(pointer->codel() != next->codel());
        pointer->set_next(next, direction, choose);
      }
    }
  }
}
const ColorBlockBase* ColorBlockInfo::get_access_point(const Coord& coord,
                                                       Direction direction) {
  using std::begin;
  using std::end;
  struct IncludesPredicate {
    IncludesPredicate(const Coord& coord) : coord_(coord) {}
    bool operator()(const ColorBlockData& color_block) {
      return std::get<0>(color_block).includes(coord_);
    }
   private:
    Coord coord_;
  };
  const auto includes = std::find_if(begin(color_blocks_), end(color_blocks_),
                                     IncludesPredicate(coord));
  if (includes == end(color_blocks_)) {
    return black_block();
  } else {
    const auto& connected = std::get<0>(*includes);
    const auto& color_block = std::get<1>(*includes);
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    switch (codel.color()) {
      case Color::WHITE:
        return make_white_path(connected, coord, direction);
      case Color::BLACK:
        return black_block();
      default:
        return color_block->address();
    }
  }
}
const ColorBlockBase* ColorBlockInfo::make_white_path(
    const ConnectedCodel& connected, const Coord& coord, Direction direction) {
  struct HavePointer {
    HavePointer(const ColorBlockBase* next) : next_(next) {}
    bool operator()(const MonoBlockData& data) {
      return std::get<0>(data) == next_;
    }
   private:
    const ColorBlockBase* next_;
  };
  const auto next_coord = connected.find_out_of_range(coord, direction);
  const auto next_pointer = get_access_point(next_coord, direction);
  const auto exist = std::find_if(begin(mono_blocks_), end(mono_blocks_),
                                  HavePointer(next_pointer));
  if (exist == end(mono_blocks_)) {
    mono_blocks_.emplace_back(next_pointer,
                              make_unique<WhiteBlock>(next_pointer));
    return std::get<1>(mono_blocks_.back())->address();
  } else {
    return std::get<1>(*exist)->address();
  }
}
const ColorBlockBase* ColorBlockInfo::black_block() const {
  assert(!mono_blocks_.empty());
  return std::get<1>(mono_blocks_.front())->address();
}

std::vector<ColorBlockPtr> color_block_network(const CodelTable& table) {
  return ColorBlockInfo(table).extract_color_blocks();
}
