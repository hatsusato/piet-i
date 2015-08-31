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
  explicit ColorBlock(const ConnectedCodel& connected);
  void set_next(const ColorBlockBase* next,
                Direction direction, Choose choose);
 private:
  Codel codel_;
  size_t codel_size_;
  const ColorBlockBase* next_[4][2];
};

class BlackBlock : public ColorBlockBase {
 public:
  BlackBlock();
};

class WhiteBlock : public ColorBlockBase {
 public:
  explicit WhiteBlock(const ColorBlockBase* next);
 private:
  const ColorBlockBase* next_;
};

class ColorBlockInfo {
  using ColorBlockData = std::tuple<ConnectedCodel,
                                    std::unique_ptr<ColorBlock> >;
  using MonoBlockData = std::tuple<const ColorBlockBase*, ColorBlockPtr>;
 public:
  explicit ColorBlockInfo(const CodelTable& table);
  std::vector<ColorBlockPtr> extract_color_blocks();
 private:
  void initialize();
  void connect_all();
  void connect(ColorBlockData& color_block);
  const ColorBlockBase* get_access_point(const Coord& coord,
                                         Direction direction);
  const ColorBlockBase* make_white_path(
      const ConnectedCodel& connected, const Coord& coord, Direction direction);
  const ColorBlockBase* black_block() const;
 private:
  CodelTable table_;
  std::vector<ColorBlockData> color_blocks_;
  std::vector<MonoBlockData> mono_blocks_;
};

std::vector<ColorBlockPtr> color_block_network(const CodelTable& table);

#endif  // PIET_I_COLOR_BLOCK_HPP
