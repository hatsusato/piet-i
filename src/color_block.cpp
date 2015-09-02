#include "color_block.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

BlockBase::BlockBase() {}
BlockBase::~BlockBase() {}
size_t BlockBase::codel_size() const {
  return 0;
}
const BlockBase* BlockBase::address() const {
  return this;
}
bool BlockBase::is_colored() const {
  return false;
}
bool BlockBase::is_white() const {
  return false;
}
bool BlockBase::is_black() const {
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
const BlockBase* ColorBlock::next(Direction direction,
                                  Choose choose) const {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  return next_[d][c];
}
void ColorBlock::set_next(const BlockBase* next,
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
const BlockBase* BlackBlock::next(Direction, Choose) const {
  return this;
}
bool BlackBlock::is_black() const {
  return true;
}

WhiteBlock::WhiteBlock(const BlockBase* next)
    : next_(next)
{}
Codel WhiteBlock::codel() const {
  return Codel(Color::WHITE, Brightness::NORMAL);
}
const BlockBase* WhiteBlock::next(Direction, Choose) const {
  return next_;
}
bool WhiteBlock::is_white() const {
  return true;
}
