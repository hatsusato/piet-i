#include "color_block.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

BlockBase::BlockBase() {}
BlockBase::~BlockBase() {}
size_t BlockBase::codel_size() const {
  return 0;
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
BlockPointer BlockBase::address() const {
  return this;
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
bool ColorBlock::is_colored() const {
  return true;
}
BlockPointer ColorBlock::next(Direction direction, Choose choose) const {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  return next_[d][c];
}
void ColorBlock::set_next(BlockPointer next,
                          Direction direction, Choose choose) {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  next_[d][c] = next;
}

BlackBlock::BlackBlock()
{}
Codel BlackBlock::codel() const {
  return Codel(Color::BLACK, Brightness::NORMAL);
}
bool BlackBlock::is_black() const {
  return true;
}
BlockPointer BlackBlock::next(Direction, Choose) const {
  return this;
}

WhiteBlock::WhiteBlock(BlockPointer next)
    : next_(next)
{}
Codel WhiteBlock::codel() const {
  return Codel(Color::WHITE, Brightness::NORMAL);
}
bool WhiteBlock::is_white() const {
  return true;
}
BlockPointer WhiteBlock::next(Direction, Choose) const {
  return next_;
}
