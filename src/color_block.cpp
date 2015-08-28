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
