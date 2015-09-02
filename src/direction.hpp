#ifndef PIET_I_DIRECTION_HPP
#define PIET_I_DIRECTION_HPP

enum class DP {
  RIGHT, DOWN, LEFT, UP
};

DP& operator++(DP& dp);
DP& operator+=(DP& dp, int value);

enum class CC {
  LEFT, RIGHT
};

CC& operator++(CC& cc);
CC& operator+=(CC& cc, int value);

struct Direction {
 public:
  Direction();
  Direction(DP dp);
  explicit operator bool() const;
  operator DP() const;
  Direction& operator++();
  int value() const;
 private:
  bool valid_;
  DP dp_;
};

struct Choose {
 public:
  Choose();
  Choose(CC cc);
 private:
  bool valid_;
  CC cc_;
};

#endif  // PIET_I_DIRECTION_HPP
