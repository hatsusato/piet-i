#ifndef PIET_I_INTEGER_HPP
#define PIET_I_INTEGER_HPP

#include <cstdint>

struct Integer {
 public:
  Integer() : value_(0) {}
  template <typename T>
  explicit Integer(T value) : value_(value) {}
 private:
  intmax_t value_;
};

#endif  // PIET_I_INTEGER_HPP
