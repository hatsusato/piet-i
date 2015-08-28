#include "color_block.hpp"

ColorBlockBase::ColorBlockBase() {}
ColorBlockBase::~ColorBlockBase() {}

ColorBlockInfo::ColorBlockInfo(const CodelTable& table)
    : table_(table), color_blocks_(), mono_blocks_() {
}
