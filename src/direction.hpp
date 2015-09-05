#ifndef PIET_I_DIRECTION_HPP
#define PIET_I_DIRECTION_HPP

#include "utility.hpp"

enum class Direction : int {
  RIGHT, DOWN, LEFT, UP, COUNT
};
ENUM_INDEXING(Direction);

enum class Choose : int {
  LEFT, RIGHT, COUNT
};
ENUM_INDEXING(Choose);

struct DP {
 public:
  constexpr DP() : direction_(Direction::RIGHT) {}
  explicit constexpr DP(Direction direction) : direction_(direction) {}
  explicit constexpr operator bool() const {
    return direction_ != Direction::COUNT;
  }
 private:
  Direction direction_;
};

struct CC {
 public:
  constexpr CC() : choose_(Choose::LEFT) {}
  explicit constexpr CC(Choose choose) : choose_(choose) {}
  explicit constexpr operator bool() const {
    return choose_ != Choose::COUNT;
  }
 private:
  Choose choose_;
};

#endif  // PIET_I_DIRECTION_HPP
