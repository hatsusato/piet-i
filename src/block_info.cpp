#include "block_info.hpp"
#include <algorithm>
#include <iterator>
#include <functional>
#include "adjacent_codel.hpp"
#include "codel_table.hpp"
#include "coord.hpp"
#include "utility.hpp"

ColourBlockData::ColourBlockData(const CodelTable& table) {
  using namespace std::placeholders;
  using std::begin;
  using std::end;
  const auto adjacent_codels = make_adjacent_codels(table);
  const auto add = std::bind(&ColourBlockData::add, this, _1);
  std::for_each(begin(adjacent_codels), end(adjacent_codels), add);
}
auto ColourBlockData::which_include(const Coord& coord) const
    -> const_iterator {
  const auto predicate = [coord](const ColourDatum& datum) -> bool {
    return std::get<1>(datum).includes(coord);
  };
  return std::find_if(begin(), end(), predicate);
}
template <typename OutputIt>
OutputIt ColourBlockData::extract(OutputIt dst) {
  for (auto&& datum : std::move(*this)) {
    if (auto block = std::move(std::get<0>(datum))) {
      *dst++ = std::move(block);
    }
  }
  clear();
  return dst;
}
void ColourBlockData::add(const AdjacentCodel& adjacent) {
  const auto& codel = adjacent.codel();
  assert(codel);
  auto block = codel.is_colour() ?
      make_unique<ColourBlock>(codel.colour(), adjacent.size()) : nullptr;
  emplace_back(std::move(block), adjacent);
}

MonoBlockData::MonoBlockData() {
  emplace_back(make_unique<BlackBlock>(), nullptr);
}
auto MonoBlockData::which_hold(BlockPointer pointer) const -> const_iterator {
  const auto predicate = [pointer](const MonoDatum& datum) -> bool {
    return std::get<1>(datum) == pointer;
  };
  return std::find_if(begin(), end(), predicate);
}
BlockPointer MonoBlockData::make_white(BlockPointer pointer) {
  const auto which = which_hold(pointer);
  if (which != end()) {
    return std::get<0>(*which)->address();
  } else {
    emplace_back(make_unique<WhiteBlock>(pointer), pointer);
    return std::get<0>(back())->address();
  }
}
template <typename OutputIt>
OutputIt MonoBlockData::extract(OutputIt dst) {
  for (auto&& datum : std::move(*this)) {
    *dst++ = std::move(std::get<0>(datum));
  }
  clear();
  return dst;
}

BlockInfo::BlockInfo(const CodelTable& table)
    : colour_blocks_(table), mono_blocks_() {
  connect_all();
}
template <typename OutputIt>
OutputIt BlockInfo::extract(OutputIt dst) {
  dst = colour_blocks_.extract(dst);
  dst = mono_blocks_.extract(dst);
  return dst;
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
  const auto& adjacent = std::get<1>(colour_block);
  if (adjacent.codel().is_colour()) {
    for (Direction direction; direction; ++direction) {
      for (Choose choose; choose; ++choose) {
        const auto& edge = adjacent.edge(direction, choose);
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
    const auto& adjacent = std::get<1>(*includes);
    assert(adjacent.codel());
    switch (adjacent.codel().type()) {
      case ColourType::WHITE:
        return make_white_path(adjacent, coord, direction);
      case ColourType::BLACK:
        return black_block();
      default:
        return block->address();
    }
  }
}
BlockPointer BlockInfo::make_white_path(
    const AdjacentCodel& adjacent, const Coord& coord, Direction direction) {
  const auto next_coord = adjacent.find_out_of_range(coord, direction);
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
  std::vector<Block> result;
  BlockInfo(table).extract(std::back_inserter(result));
  return result;
}
