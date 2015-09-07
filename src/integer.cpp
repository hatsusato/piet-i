#include "integer.hpp"
#include <cinttypes>
#include <limits>
#include <stdexcept>

#define OVERFLOW()\
  throw std::overflow_error("error: overflow")

namespace /* unnamed */ {
constexpr auto max_limit = std::numeric_limits<intmax_t>::max();
constexpr auto min_limit = std::numeric_limits<intmax_t>::min();
}  // namespace /* unnamed */ {

Integer::operator intmax_t() const {
  return value_;
}
bool Integer::operator<(const Integer& that) const {
  return value_ < that.value_;
}
bool Integer::operator==(const Integer& that) const {
  return value_ == that.value_;
}
Integer& Integer::operator+=(const Integer& that) {
  if (0 < that.value_ && (max_limit - that.value_) < value_) {
    OVERFLOW();
  } else if (that.value_ < 0 && value_ < (min_limit - that.value_)) {
    OVERFLOW();
  } else {
    value_ += that.value_;
  }
  return *this;
}
Integer& Integer::operator-=(const Integer& that) {
  if (0 < that.value_ && value_ < (min_limit + that.value_)) {
    OVERFLOW();
  } else if (that.value_ < 0 && (max_limit + that.value_) < value_) {
    OVERFLOW();
  } else {
    value_ -= that.value_;
  }
  return *this;
}
Integer& Integer::operator*=(const Integer& that) {
  if (value_ == 0 || that.value_ == 0) {
    value_ = 0;
  } else if (that.value_ == -1) {
    if (value_ < -max_limit) {
      OVERFLOW();
    }
  } else {
    if (0 < value_) {
      const auto limit = 0 < that.value_ ? max_limit : min_limit;
      if ((limit / that.value_) < value_) {
        OVERFLOW();
      }
    } else {
      const auto limit = 0 < that.value_ ? min_limit : max_limit;
      if (value_ < (limit / that.value_)) {
        OVERFLOW();
      }
    }
  }
  value_ *= that.value_;
  return *this;
}
Integer& Integer::operator/=(const Integer& that) {
  if (that.value_ == -1 && value_ < -max_limit) {
    OVERFLOW();
  }
  value_ /= that.value_;
  return *this;
}
Integer& Integer::operator%=(const Integer& that) {
  if (that.value_ < -max_limit) {
    if (0 < value_) {
      value_ += that.value_;
    } else if (value_ <= that.value_) {
      value_ -= that.value_;
    }
  } else {
    const bool this_sign = 0 < value_;
    const bool that_sign = 0 < that.value_;
    const auto that_abs = std::imaxabs(that.value_);
    if (value_ < -max_limit) {
      value_ += that_abs;
    }
    value_ = (this_sign ? 1 : -1) * (std::imaxabs(value_) % that_abs);
    if (value_ != 0 && this_sign != that_sign) {
      value_ += that.value_;
    }
  }
  return *this;
}
Integer& Integer::operator|=(const Integer& that) {
  value_ |= that.value_;
  return *this;
}
Integer& Integer::operator&=(const Integer& that) {
  value_ &= that.value_;
  return *this;
}
Integer& Integer::operator^=(const Integer& that) {
  value_ ^= that.value_;
  return *this;
}
Integer& Integer::operator++() {
  if (value_ == max_limit) {
    OVERFLOW();
  }
  ++value_;
  return *this;
}
Integer& Integer::operator--() {
  if (value_ == min_limit) {
    OVERFLOW();
  }
  --value_;
  return *this;
}
Integer Integer::operator+() const {
  return *this;
}
Integer Integer::operator-() const {
  if (value_ < -max_limit) {
    OVERFLOW();
  }
  return Integer(-value_);
}
