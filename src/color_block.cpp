#include "color_block.hpp"

ColorBlockBase::ColorBlockBase() {}
ColorBlockBase::~ColorBlockBase() {}
const ColorBlockBase* ColorBlockBase::address() const {
  return this;
}
