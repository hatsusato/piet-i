#include "direction.hpp"

DP& operator++(DP& dp) {
  dp = static_cast<DP>((static_cast<int>(dp) + 1) % 4);
  return dp;
}
DP& operator+=(DP& dp, int value) {
  dp = static_cast<DP>((static_cast<int>(dp) + 4 + value % 4) % 4);
  return dp;
}

CC& operator++(CC& cc) {
  cc = static_cast<CC>(1 - static_cast<int>(cc));
  return cc;
}
CC& operator+=(CC& cc, int value) {
  return value % 2 ? operator++(cc) : cc;
}

Direction::Direction()
    : valid_(true), dp_(DP::RIGHT)
{}
Direction::Direction(DP dp)
    : valid_(true), dp_(dp)
{}
