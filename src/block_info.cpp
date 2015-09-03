#include "block_info.hpp"

BlockInfo::BlockInfo(const CodelTable& table)
    : colour_blocks_(), mono_blocks_() {
  initialize(extract_connected_codels(table));
  connect_all();
}
std::vector<Block> BlockInfo::extract_blocks() {
  std::vector<Block> result;
  for (auto&& data : std::move(colour_blocks_)) {
    if (auto block = std::move(std::get<1>(data))) {
      result.push_back(std::move(block));
    }
  }
  for (auto&& data : std::move(mono_blocks_)) {
    result.push_back(std::move(std::get<1>(data)));
  }
  colour_blocks_.clear();
  mono_blocks_.clear();
  return result;
}
void BlockInfo::initialize(
    const std::vector<ConnectedCodel>& connected_codels) {
  assert(colour_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.emplace_back(nullptr, make_unique<BlackBlock>());
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel);
    auto block = codel.is_colour() ?
        make_unique<ColourBlock>(codel.colour(), connected.size()) : nullptr;
    colour_blocks_.emplace_back(connected, std::move(block));
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
  const auto& connected = std::get<0>(colour_block);
  auto& block = std::get<1>(colour_block);
  if (connected.codel().is_colour()) {
    for (Direction direction; direction; ++direction) {
      for (Choose choose; choose; ++choose) {
        const auto& edge = connected.edge(direction, choose);
        const auto next = get_access_point(edge, direction);
        assert(block->colour() && next->colour());
        assert(block->colour() != next->colour());
        block->set_next(next, direction, choose);
      }
    }
  }
}
BlockPointer BlockInfo::get_access_point(const Coord& coord,
                                         Direction direction) {
  using std::begin;
  using std::end;
  struct IncludesPredicate {
    IncludesPredicate(const Coord& coord) : coord_(coord) {}
    bool operator()(const ColourBlockData& colour_block) {
      return std::get<0>(colour_block).includes(coord_);
    }
   private:
    Coord coord_;
  };
  const auto includes = std::find_if(begin(colour_blocks_), end(colour_blocks_),
                                     IncludesPredicate(coord));
  if (includes == end(colour_blocks_)) {
    return black_block();
  } else {
    const auto& connected = std::get<0>(*includes);
    const auto& colour_block = std::get<1>(*includes);
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    switch (codel.colour()) {
      case Colour::WHITE:
        return make_white_path(connected, coord, direction);
      case Colour::BLACK:
        return black_block();
      default:
        return colour_block->address();
    }
  }
}
BlockPointer BlockInfo::make_white_path(
    const ConnectedCodel& connected, const Coord& coord, Direction direction) {
  struct HavePointer {
    HavePointer(BlockPointer next) : next_(next) {}
    bool operator()(const MonoBlockData& data) {
      return std::get<0>(data) == next_;
    }
   private:
    BlockPointer next_;
  };
  const auto next_coord = connected.find_out_of_range(coord, direction);
  const auto next_pointer = get_access_point(next_coord, direction);
  const auto exist = std::find_if(begin(mono_blocks_), end(mono_blocks_),
                                  HavePointer(next_pointer));
  if (exist == end(mono_blocks_)) {
    mono_blocks_.emplace_back(next_pointer,
                              make_unique<WhiteBlock>(next_pointer));
    return std::get<1>(mono_blocks_.back())->address();
  } else {
    return std::get<1>(*exist)->address();
  }
}
BlockPointer BlockInfo::black_block() const {
  assert(!mono_blocks_.empty());
  return std::get<1>(mono_blocks_.front())->address();
}

std::vector<Block> colour_block_network(const CodelTable& table) {
  return BlockInfo(table).extract_blocks();
}
