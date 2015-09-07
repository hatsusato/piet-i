#include "integer.hpp"

Integer::operator intmax_t() const {
  return value_;
}
