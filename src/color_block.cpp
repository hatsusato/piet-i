#include "color_block.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

BlockBase::BlockBase() {}
BlockBase::~BlockBase() {}
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

ColorBlock::ColorBlock(const ConnectedCodel& connected)
    : colour_(connected.colour()), codel_size_(connected.size()), next_() {
  assert(colour_.is_colour());
}
Colour ColorBlock::colour() const {
  return colour_;
}
size_t ColorBlock::codel_size() const {
  return codel_size_;
}
bool ColorBlock::is_colour() const {
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
Colour BlackBlock::colour() const {
  return Colour(Color::BLACK, Brightness::NORMAL);
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
Colour WhiteBlock::colour() const {
  return Colour(Color::WHITE, Brightness::NORMAL);
}
bool WhiteBlock::is_white() const {
  return true;
}
BlockPointer WhiteBlock::next(Direction, Choose) const {
  return next_;
}
