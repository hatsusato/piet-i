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
 private:
  Codel codel_;
  const ColorBlockBase* next_[4][2];
};

class BlackBlock : public ColorBlockBase {
};

class WhiteBlock : public ColorBlockBase {
};

#endif  // PIET_I_COLOR_BLOCK_HPP
