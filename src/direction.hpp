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

#endif  // PIET_I_DIRECTION_HPP
