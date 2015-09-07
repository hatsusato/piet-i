#include "integer.hpp"

Integer::operator intmax_t() const {
  return value_;
}
bool Integer::operator<(const Integer& that) const {
  return value_ < that.value_;
}
bool Integer::operator==(const Integer& that) const {
  return value_ == that.value_;
}
