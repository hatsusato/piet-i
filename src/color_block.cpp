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
