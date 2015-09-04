#ifndef PIET_I_BLOCK_INFO_HPP
#define PIET_I_BLOCK_INFO_HPP

#include <memory>
#include <tuple>
#include <vector>
#include "block.hpp"
#include "direction.hpp"

class AdjacentCodel;
class CodelTable;
class Coord;
using ColourDatum = std::tuple<std::unique_ptr<ColourBlock>, AdjacentCodel>;
using MonoDatum = std::tuple<Block, BlockPointer>;

class ColourBlockData : public std::vector<ColourDatum> {
 public:
  ColourBlockData(const CodelTable& table);
  const_iterator which_include(const Coord& coord) const;
 private:
  void add(const AdjacentCodel& adjacent);
};

class MonoBlockData : public std::vector<MonoDatum> {
 public:
  MonoBlockData();
};

class BlockInfo {
 public:
  explicit BlockInfo(const CodelTable& table);
  std::vector<Block> extract_blocks();
 private:
  void connect_all();
  void connect(ColourBlockData& colour_block);
  BlockPointer get_access_point(const Coord& coord,
                                Direction direction);
  BlockPointer make_white_path(
      const AdjacentCodel& adjacent, const Coord& coord, Direction direction);
  BlockPointer black_block() const;
 private:
  ColourBlockData colour_blocks_;
  MonoBlockData mono_blocks_;
};

std::vector<Block> colour_block_network(const CodelTable& table);

#endif  // PIET_I_BLOCK_INFO_HPP
