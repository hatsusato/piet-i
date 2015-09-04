#include "block_info.hpp"
#include <algorithm>
#include "adjacent_codel.hpp"
#include "codel_table.hpp"
#include "coord.hpp"

BlockInfo::BlockInfo(const CodelTable& table)
    : colour_blocks_(), mono_blocks_() {
  initialize(extract_connected_codels(table));
  connect_all();
}
std::vector<Block> BlockInfo::extract_blocks() {
  std::vector<Block> result;
  for (auto&& data : std::move(colour_blocks_)) {
    if (auto block = std::move(std::get<0>(data))) {
      result.push_back(std::move(block));
    }
  }
  for (auto&& data : std::move(mono_blocks_)) {
    result.push_back(std::move(std::get<0>(data)));
  }
  colour_blocks_.clear();
  mono_blocks_.clear();
  return result;
}
void BlockInfo::initialize(
    const std::vector<ConnectedCodel>& connected_codels) {
  assert(colour_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.emplace_back(make_unique<BlackBlock>(), nullptr);
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel);
    auto block = codel.is_colour() ?
        make_unique<ColourBlock>(codel.colour(), connected.size()) : nullptr;
    colour_blocks_.emplace_back(std::move(block), connected);
  }
}
void BlockInfo::connect_all() {
  using namespace std::placeholders;
  using std::begin;
  using std::end;
  const auto connect = std::bind(&BlockInfo::connect, this, _1);
  std::for_each(begin(colour_blocks_), end(colour_blocks_), connect);
}
void BlockInfo::connect(ColourBlockData& colour_block) {
  auto& block = std::get<0>(colour_block);
  const auto& connected = std::get<1>(colour_block);
  if (connected.codel().is_colour()) {
    for (Direction direction; direction; ++direction) {
      for (Choose choose; choose; ++choose) {
        const auto& edge = connected.edge(direction, choose);
        const auto next = get_access_point(edge, direction);
        assert(block->colour() && block->colour() != next->colour());
        block->set_next(next, direction, choose);
      }
    }
  }
}
BlockPointer BlockInfo::get_access_point(const Coord& coord,
                                         Direction direction) {
  using std::begin;
  using std::end;
  const auto includes =
      std::find_if(begin(colour_blocks_), end(colour_blocks_),
                   [coord](const ColourBlockData& data) -> bool {
                     return std::get<1>(data).includes(coord);
                   });
  if (includes == end(colour_blocks_)) {
    return black_block();
  } else {
    const auto& block = std::get<0>(*includes);
    const auto& connected = std::get<1>(*includes);
    assert(connected.codel());
    switch (connected.codel().type()) {
      case ColourType::WHITE:
        return make_white_path(connected, coord, direction);
      case ColourType::BLACK:
        return black_block();
      default:
        return block->address();
    }
  }
}
BlockPointer BlockInfo::make_white_path(
    const ConnectedCodel& connected, const Coord& coord, Direction direction) {
  const auto next_coord = connected.find_out_of_range(coord, direction);
  const auto next_pointer = get_access_point(next_coord, direction);
  const auto exist = std::find_if(
      begin(mono_blocks_), end(mono_blocks_),
      [next_pointer](const MonoBlockData& data) -> bool {
        return std::get<1>(data) == next_pointer;
      });
  if (exist == end(mono_blocks_)) {
    mono_blocks_.emplace_back(make_unique<WhiteBlock>(next_pointer),
                              next_pointer);
    return std::get<0>(mono_blocks_.back())->address();
  } else {
    return std::get<0>(*exist)->address();
  }
}
BlockPointer BlockInfo::black_block() const {
  assert(!mono_blocks_.empty());
  return std::get<0>(mono_blocks_.front())->address();
}

std::vector<Block> colour_block_network(const CodelTable& table) {
  return BlockInfo(table).extract_blocks();
}
