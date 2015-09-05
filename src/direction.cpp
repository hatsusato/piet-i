#include "direction.hpp"

DP& DP::operator++() {
  ++direction_;
  return *this;
}
DP& DP::operator+=(int value) {
  direction_ += value;
  return *this;
}

CC& CC::operator++() {
  ++choose_;
  return *this;
}
CC& CC::operator+=(int value) {
  choose_ += value;
  return *this;
}
