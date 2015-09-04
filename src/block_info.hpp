#ifndef PIET_I_BLOCK_INFO_HPP
#define PIET_I_BLOCK_INFO_HPP

#include <memory>
#include <tuple>
#include "block.hpp"
#include "direction.hpp"
#include "utility.hpp"

using Block = std::unique_ptr<BlockBase>;
class CodelTable;
class AdjacentCodel;
class Coord;

class BlockInfo {
  using ColourBlockData =
      std::tuple<std::unique_ptr<ColourBlock>, AdjacentCodel>;
  using MonoBlockData = std::tuple<Block, BlockPointer>;
 public:
  explicit BlockInfo(const CodelTable& table);
  std::vector<Block> extract_blocks();
 private:
  void initialize(const std::vector<AdjacentCodel>& adjacent_codels);
  void connect_all();
  void connect(ColourBlockData& colour_block);
  BlockPointer get_access_point(const Coord& coord,
                                Direction direction);
  BlockPointer make_white_path(
      const AdjacentCodel& adjacent, const Coord& coord, Direction direction);
  BlockPointer black_block() const;
 private:
  std::vector<ColourBlockData> colour_blocks_;
  std::vector<MonoBlockData> mono_blocks_;
};

std::vector<Block> colour_block_network(const CodelTable& table);

#endif  // PIET_I_BLOCK_INFO_HPP
