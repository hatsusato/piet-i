#include "color_block.hpp"

ColorBlockBase::ColorBlockBase() {}
ColorBlockBase::~ColorBlockBase() {}

ColorBlockInfo::ColorBlockInfo(const CodelTable& table)
    : table_(table), color_blocks_(), mono_blocks_() {
  initialize();
}
void ColorBlockInfo::initialize() {
  assert(color_blocks_.empty() && mono_blocks_.empty());
  mono_blocks_.push_back(make_unique<BlackBlock>());
  const auto connected_codels = extract_connected_codels(table_);
  for (auto&& connected : connected_codels) {
    const auto& codel = connected.codel();
    assert(codel.is_valid());
    auto block_pointer = codel.is_colored() ?
        make_unique<ColorBlock>(codel) : nullptr;
    color_blocks_.emplace_back(connected, std::move(block_pointer));
  }
}
void ColorBlockInfo::connect(ColorBlockData& color_block) {
  const auto& connected = std::get<0>(color_block);
  auto& pointer = std::get<1>(color_block);
  if (connected.codel().is_colored()) {
    for (int d = 0; d < 4; ++d) {
      const auto direction = static_cast<Direction>(d);
      for (int c = 0; c < 2; ++c) {
        const auto choose = static_cast<Choose>(c);
        const auto edge = connected.edge(direction, choose);
        const auto next = get_access_point(edge, direction);
        pointer->set_next(next, direction, choose);
      }
    }
  }
}
