#include "block_info.hpp"
#include <cassert>
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
BlockPointer MonoBlockData::black_block() const {
  assert(!empty());
  return std::get<0>(front())->address();
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
auto MonoBlockData::which_hold(BlockPointer pointer) const -> const_iterator {
  const auto predicate = [pointer](const MonoDatum& datum) -> bool {
    return std::get<1>(datum) == pointer;
  };
  return std::find_if(begin(), end(), predicate);
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
void BlockInfo::connect(ColourDatum& datum) {
  auto& block = std::get<0>(datum);
  const auto& adjacent = std::get<1>(datum);
  if (adjacent.codel().is_colour()) {
    for (DP dp; dp; dp.next()) {
      for (CC cc; cc; cc.next()) {
        const auto& edge = adjacent.edge(dp, cc);
        const auto target = get_target(edge, dp);
        assert(block->colour() && block->colour() != target->colour());
        block->set_next(target, dp, cc);
      }
    }
  }
}
BlockPointer BlockInfo::get_target(const Coord& coord, DP dp) {
  using std::end;
  const auto which = colour_blocks_.which_include(coord);
  if (which != end(colour_blocks_)) {
    const auto& block = std::get<0>(*which);
    const auto& adjacent = std::get<1>(*which);
    const auto& codel = adjacent.codel();
    assert(codel);
    if (codel.is_colour()) {
      return block->address();
    } else if (codel.is_white()) {
      const auto next_coord = adjacent.find_out_of_range(coord, dp);
      const auto next_pointer = get_target(next_coord, dp);
      return mono_blocks_.make_white(next_pointer);
    }
  }
  return mono_blocks_.black_block();
}

std::vector<Block> colour_block_network(const CodelTable& table) {
  std::vector<Block> result;
  BlockInfo(table).extract(std::back_inserter(result));
  return result;
}
