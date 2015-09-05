#include "direction.hpp"

DP& DP::operator++() {
  ++direction_;
  return *this;
}
DP& DP::operator+=(int value) {
  direction_ += value;
  return *this;
}
DP& DP::next() {
  if (*this) {
    direction_ = static_cast<Direction>(index(direction_) + 1);
  }
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
CC& CC::next() {
  if (*this) {
    choose_ = static_cast<Choose>(index(choose_) + 1);
  }
  return *this;
}
