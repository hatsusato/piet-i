#ifndef PIET_I_INTEGER_HPP
#define PIET_I_INTEGER_HPP

#include <cstdint>
#include <ostream>
#include <boost/operators.hpp>

struct Integer : private boost::operators<Integer> {
 public:
  Integer() : value_(0) {}
  template <typename T>
  explicit Integer(T value) : value_(value) {}
  operator intmax_t() const;
  bool operator<(const Integer& that) const;
  bool operator==(const Integer& that) const;
  Integer& operator+=(const Integer& that);
  Integer& operator-=(const Integer& that);
  Integer& operator*=(const Integer& that);
  Integer& operator/=(const Integer& that);
  Integer& operator%=(const Integer& that);
  Integer& operator|=(const Integer& that);
  Integer& operator&=(const Integer& that);
  Integer& operator^=(const Integer& that);
  Integer& operator++();
  Integer& operator--();
  Integer operator+() const;
  Integer operator-() const;
 private:
  intmax_t value_;
};

std::wostream& operator<<(std::wostream& os, const Integer& integer);

#endif  // PIET_I_INTEGER_HPP
