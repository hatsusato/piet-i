#ifndef PIET_I_INTEGER_HPP
#define PIET_I_INTEGER_HPP

#include <cstdint>
#include <boost/operators.hpp>

struct Integer : private boost::operators<Integer> {
 public:
  Integer() : value_(0) {}
  template <typename T>
  explicit Integer(T value) : value_(value) {}
  operator intmax_t() const;
  bool operator<(const Integer& that) const;
  bool operator==(const Integer& that) const;
 private:
  intmax_t value_;
};

#endif  // PIET_I_INTEGER_HPP
