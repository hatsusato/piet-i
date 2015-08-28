#ifndef PIET_I_COLOR_BLOCK_HPP
#define PIET_I_COLOR_BLOCK_HPP

#include "codel.hpp"
#include "utils.hpp"

class ColorBlockBase {
 public:
  virtual ~ColorBlockBase() = 0;
 protected:
  ColorBlockBase();
 private:
  ColorBlockBase(ColorBlockBase&&) = delete;
};
using ColorBlockPtr = std::unique_ptr<ColorBlockBase>;

class ColorBlock : public ColorBlockBase {
};

class BlackBlock : public ColorBlockBase {
};

class WhiteBlock : public ColorBlockBase {
};

class ColorBlockInfo {
  using ColorBlockData = std::tuple<ConnectedCodel,
                                    std::unique_ptr<ColorBlock> >;
 public:
  ColorBlockInfo(const CodelTable& table);
 private:
  void initialize();
  void connect(ColorBlockData& color_block);
 private:
  CodelTable table_;
  std::vector<ColorBlockData> color_blocks_;
  std::vector<ColorBlockPtr> mono_blocks_;
};

#endif  // PIET_I_COLOR_BLOCK_HPP
