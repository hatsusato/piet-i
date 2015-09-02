#ifndef PIET_I_COLOR_BLOCK_HPP
#define PIET_I_COLOR_BLOCK_HPP

#include "codel.hpp"
#include "codel_table.hpp"
#include "connected_codel.hpp"
#include "memory.hpp"

class BlockBase {
 public:
  virtual ~BlockBase() = 0;
  virtual Codel codel() const = 0;
  virtual size_t codel_size() const;
  virtual const BlockBase* next(Direction direction,
                                Choose choose) const = 0;
  const BlockBase* address() const;
  virtual bool is_colored() const;
  virtual bool is_white() const;
  virtual bool is_black() const;
 protected:
  BlockBase();
 private:
  BlockBase(BlockBase&&) = delete;
};
using ColorBlockPtr = std::unique_ptr<BlockBase>;

class ColorBlock : public BlockBase {
 public:
  explicit ColorBlock(const ConnectedCodel& connected);
  Codel codel() const override;
  size_t codel_size() const override;
  const BlockBase* next(Direction direction,
                        Choose choose) const override;
  void set_next(const BlockBase* next,
                Direction direction, Choose choose);
  bool is_colored() const override;
 private:
  Codel codel_;
  size_t codel_size_;
  const BlockBase* next_[4][2];
};

class BlackBlock : public BlockBase {
 public:
  BlackBlock();
  Codel codel() const override;
  const BlockBase* next(Direction direction,
                        Choose choose) const override;
  bool is_black() const override;
};

class WhiteBlock : public BlockBase {
 public:
  explicit WhiteBlock(const BlockBase* next);
  Codel codel() const override;
  const BlockBase* next(Direction direction,
                        Choose choose) const override;
  bool is_white() const override;
 private:
  const BlockBase* next_;
};

#endif  // PIET_I_COLOR_BLOCK_HPP
