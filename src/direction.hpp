#ifndef PIET_I_DIRECTION_HPP
#define PIET_I_DIRECTION_HPP

enum class DP {
  RIGHT, DOWN, LEFT, UP
};

DP& operator++(DP& dp);
DP& operator+=(DP& dp, int value);

enum class CC {
  LEFT, RIGHT
};

#endif  // PIET_I_DIRECTION_HPP
