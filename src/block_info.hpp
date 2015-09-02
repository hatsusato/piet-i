#ifndef PIET_I_BLOCK_INFO_HPP
#define PIET_I_BLOCK_INFO_HPP

class BlockInfo {
  using ColorBlockData = std::tuple<ConnectedCodel,
                                    std::unique_ptr<ColorBlock> >;
  using MonoBlockData = std::tuple<const ColorBlockBase*, ColorBlockPtr>;
 public:
  explicit BlockInfo(const CodelTable& table);
  std::vector<ColorBlockPtr> extract_color_blocks();
 private:
  void initialize();
  void connect_all();
  void connect(ColorBlockData& color_block);
  const ColorBlockBase* get_access_point(const Coord& coord,
                                         Direction direction);
  const ColorBlockBase* make_white_path(
      const ConnectedCodel& connected, const Coord& coord, Direction direction);
  const ColorBlockBase* black_block() const;
 private:
  CodelTable table_;
  std::vector<ColorBlockData> color_blocks_;
  std::vector<MonoBlockData> mono_blocks_;
};

std::vector<ColorBlockPtr> color_block_network(const CodelTable& table);

#endif  // PIET_I_BLOCK_INFO_HPP
