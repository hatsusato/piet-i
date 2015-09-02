#ifndef PIET_I_BLOCK_INFO_HPP
#define PIET_I_BLOCK_INFO_HPP

#include "memory.hpp"
#include "color_block.hpp"

class Block = std::unique_ptr<BlockBase>;

class BlockInfo {
  using ColorBlockData = std::tuple<ConnectedCodel,
                                    std::unique_ptr<ColorBlock> >;
  using MonoBlockData = std::tuple<BlockPointer, Block>;
 public:
  explicit BlockInfo(const CodelTable& table);
  std::vector<Block> extract_color_blocks();
 private:
  void initialize();
  void connect_all();
  void connect(ColorBlockData& color_block);
  BlockPointer get_access_point(const Coord& coord,
                                Direction direction);
  BlockPointer make_white_path(
      const ConnectedCodel& connected, const Coord& coord, Direction direction);
  BlockPointer black_block() const;
 private:
  CodelTable table_;
  std::vector<ColorBlockData> color_blocks_;
  std::vector<MonoBlockData> mono_blocks_;
};

std::vector<Block> color_block_network(const CodelTable& table);

#endif  // PIET_I_BLOCK_INFO_HPP
