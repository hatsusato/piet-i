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
  template <typename OutputIt>
  OutputIt extract(OutputIt dst);
 private:
  void add(const AdjacentCodel& adjacent);
};

class MonoBlockData : public std::vector<MonoDatum> {
 public:
  MonoBlockData();
  BlockPointer black_block() const;
  BlockPointer make_white(BlockPointer pointer);
  template <typename OutputIt>
  OutputIt extract(OutputIt dst);
 private:
  const_iterator which_hold(BlockPointer pointer) const;
};

class BlockInfo {
 public:
  explicit BlockInfo(const CodelTable& table);
  template <typename OutputIt>
  OutputIt extract(OutputIt dst);
 private:
  void connect_all();
  void connect(ColourDatum& colour_block);
  BlockPointer get_target(const Coord& coord, Direction direction);
 private:
  ColourBlockData colour_blocks_;
  MonoBlockData mono_blocks_;
};

std::vector<Block> colour_block_network(const CodelTable& table);

#endif  // PIET_I_BLOCK_INFO_HPP
