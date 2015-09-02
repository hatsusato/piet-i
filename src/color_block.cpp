#include "color_block.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

BlockBase::BlockBase() {}
BlockBase::~BlockBase() {}
Colour BlockBase::colour() const {
  return Colour::unknown;
}
size_t BlockBase::codel_size() const {
  return 0;
}
bool BlockBase::is_colour() const {
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

ColourBlock::ColourBlock(Colour colour, size_t size)
    : colour_(colour), codel_size_(size), next_() {
  assert(colour_);
}
Colour ColourBlock::colour() const {
  return colour_;
}
size_t ColourBlock::codel_size() const {
  return codel_size_;
}
bool ColourBlock::is_colour() const {
  return true;
}
BlockPointer ColourBlock::next(Direction direction, Choose choose) const {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  return next_[d][c];
}
void ColourBlock::set_next(BlockPointer next,
                           Direction direction, Choose choose) {
  const auto d = static_cast<int>(direction);
  const auto c = static_cast<int>(choose);
  next_[d][c] = next;
}

BlackBlock::BlackBlock()
{}
bool BlackBlock::is_black() const {
  return true;
}
BlockPointer BlackBlock::next(Direction, Choose) const {
  return this;
}

WhiteBlock::WhiteBlock(BlockPointer next)
    : next_(next)
{}
bool WhiteBlock::is_white() const {
  return true;
}
BlockPointer WhiteBlock::next(Direction, Choose) const {
  return next_;
}
