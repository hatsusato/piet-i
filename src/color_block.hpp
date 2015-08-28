#ifndef PIET_I_COLOR_BLOCK_HPP
#define PIET_I_COLOR_BLOCK_HPP

#include "codel.hpp"
#include "utils.hpp"

class ColorBlockBase {
 public:
  virtual ~ColorBlockBase() = 0;
  const ColorBlockBase* address() const;
 protected:
  ColorBlockBase();
 private:
  ColorBlockBase(ColorBlockBase&&) = delete;
};
using ColorBlockPtr = std::unique_ptr<ColorBlockBase>;

class ColorBlock : public ColorBlockBase {
 public:
  ColorBlock(const Codel& codel);
  void set_next(const ColorBlockBase* next,
                Direction direction, Choose choose);
 private:
  Codel codel_;
  const ColorBlockBase* next_[4][2];
};

class BlackBlock : public ColorBlockBase {
 public:
  BlackBlock();
};

class WhiteBlock : public ColorBlockBase {
 public:
  WhiteBlock(const ColorBlockBase* next);
 private:
  const ColorBlockBase* next_;
};

#endif  // PIET_I_COLOR_BLOCK_HPP
