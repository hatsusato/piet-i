#ifndef PIET_I_BLOCK_HPP
#define PIET_I_BLOCK_HPP

#include <memory>
#include "colour.hpp"
#include "direction.hpp"
#include "edges.hpp"

class BlockBase;
using BlockPointer = const BlockBase*;
using Block = std::unique_ptr<BlockBase>;

class BlockBase {
 protected:
  BlockBase();
 public:
  virtual ~BlockBase() = 0;
  virtual Colour colour() const;
  virtual size_t codel_size() const;
  virtual bool is_colour() const;
  virtual bool is_white() const;
  virtual bool is_black() const;
  virtual BlockPointer next(DP dp, CC cc) const = 0;
  BlockPointer address() const;
 private:
  BlockBase(BlockBase&&) = delete;
};

class ColourBlock : public BlockBase {
 public:
  ColourBlock(Colour colour, size_t size);
  Colour colour() const override;
  size_t codel_size() const override;
  bool is_colour() const override;
  BlockPointer next(DP dp, CC cc) const override;
  void set_next(BlockPointer next, DP dp, CC cc);
 private:
  Colour colour_;
  size_t codel_size_;
  Edges<BlockPointer> next_;
};

class BlackBlock : public BlockBase {
 public:
  BlackBlock();
  bool is_black() const override;
  BlockPointer next(DP dp, CC cc) const override;
};

class WhiteBlock : public BlockBase {
 public:
  explicit WhiteBlock(BlockPointer next);
  bool is_white() const override;
  BlockPointer next(DP dp, CC cc) const override;
 private:
  BlockPointer next_;
};

#endif  // PIET_I_BLOCK_HPP
