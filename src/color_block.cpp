#include "color_block.hpp"

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
