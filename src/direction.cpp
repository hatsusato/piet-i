#include "direction.hpp"

DP& operator++(DP& dp) {
  dp = static_cast<DP>((static_cast<int>(dp) + 1) % 4);
  return dp;
}
DP& operator+=(DP& dp, int value) {
  dp = static_cast<DP>((static_cast<int>(dp) + 4 + value % 4) % 4);
  return dp;
}
