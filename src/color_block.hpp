#ifndef PIET_I_COLOR_BLOCK_HPP
#define PIET_I_COLOR_BLOCK_HPP

#include "utils.hpp"

class ColorBlockBase {
 public:
  virtual ~ColorBlockBase() = 0;
};
using ColorBlockPtr = std::unique_ptr<ColorBlockBase>;

#endif  // PIET_I_COLOR_BLOCK_HPP
