#include "color_block.hpp"
#include <algorithm>

ColorBlockBase::ColorBlockBase() {}
ColorBlockBase::~ColorBlockBase() {}
const ColorBlockBase* ColorBlockBase::address() const {
  return this;
}

ColorBlock::ColorBlock(const Codel& codel)
    : codel_(codel), next_() {
  assert(codel.is_colored());
}
void ColorBlock::set_next(const ColorBlockBase* next,
                          Direction direction, Choose choose) {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  next_[d][c] = next;
}

BlackBlock::BlackBlock()
{}

WhiteBlock::WhiteBlock(const ColorBlockBase* next)
    : next_(next)
{}

ColorBlockInfo::ColorBlockInfo(const CodelTable& table)
    : table_(table), color_blocks_(), mono_blocks_() {
  initialize();
}
void ColorBlockInfo::initialize() {
  assert(color_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.push_back(make_unique<BlackBlock>());
  const auto connected_codels = extract_connected_codels(table_);
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    auto block_pointer = codel.is_colored() ?
        make_unique<ColorBlock>(codel) : nullptr;
    color_blocks_.emplace_back(connected, std::move(block_pointer));
  }
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
      case Color::WHITE: {
        const auto next_coord = connected.find_out_of_range(coord, direction);
        const auto next_pointer = get_access_point(next_coord, direction);
        mono_blocks_.push_back(make_unique<WhiteBlock>(next_pointer));
      }
        return mono_blocks_.back()->address();
      case Color::BLACK:
        return black_block();
      default:
        return color_block->address();
    }
  }
}
const ColorBlockBase* ColorBlockInfo::black_block() const {
  assert(!mono_blocks_.empty());
  return mono_blocks_.front()->address();
}
