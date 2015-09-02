#include "block_info.hpp"

namespace /* unnamed */ {
template <size_t N, class T>
struct Extractor {
  Block operator()(T&& data) {
    return std::move(std::get<N>(data));
  }
};
}  // namespace /* unnamed */

BlockInfo::BlockInfo(const CodelTable& table)
    : table_(table), colour_blocks_(), mono_blocks_() {
  initialize();
  connect_all();
}
std::vector<Block> BlockInfo::extract_colour_blocks() {
  using std::begin;
  using std::end;
  struct NullChecker {
    bool operator()(const Block& ptr) {
      return static_cast<bool>(ptr);
    }
  };
  std::vector<Block> extracted;
  std::transform(std::make_move_iterator(begin(colour_blocks_)),
                 std::make_move_iterator(end(colour_blocks_)),
                 std::back_inserter(extracted),
                 Extractor<1, ColourBlockData>());
  std::vector<Block> result;
  std::copy_if(std::make_move_iterator(begin(extracted)),
               std::make_move_iterator(end(extracted)),
               std::back_inserter(result), NullChecker());
  extracted.clear();
  std::transform(std::make_move_iterator(begin(mono_blocks_)),
                 std::make_move_iterator(end(mono_blocks_)),
                 std::back_inserter(extracted),
                 Extractor<1, MonoBlockData>());
  std::move(begin(extracted), end(extracted), std::back_inserter(result));
  colour_blocks_.clear();
  mono_blocks_.clear();
  return result;
}
void BlockInfo::initialize() {
  assert(colour_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.emplace_back(nullptr, make_unique<BlackBlock>());
  const auto connected_codels = extract_connected_codels(table_);
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    auto block_pointer = codel.is_colored() ?
        make_unique<ColourBlock>(connected) : nullptr;
    colour_blocks_.emplace_back(connected, std::move(block_pointer));
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
  auto& pointer = std::get<1>(colour_block);
  if (connected.codel().is_colored()) {
    for (int d = 0; d < 4; ++d) {
      const auto direction = static_cast<Direction>(d);
      for (int c = 0; c < 2; ++c) {
        const auto choose = static_cast<Choose>(c);
        const auto edge = connected.edge(direction, choose);
        const auto next = get_access_point(edge, direction);
        assert(pointer->codel() != next->codel());
        pointer->set_next(next, direction, choose);
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
  return BlockInfo(table).extract_colour_blocks();
}
