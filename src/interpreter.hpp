#ifndef PIET_I_INTERPRETER_HPP
#define PIET_I_INTERPRETER_HPP

#include <stack>
#include <vector>
#include "color_block.hpp"
#include "utils.hpp"

class Stack : public std::stack<int, std::vector<int> > {
 public:
  void push_command(size_t number);
  void pop_command();
  template <typename Op, bool zero_check = false>
  void binary_command();
  template <typename Op>
  void unary_command();
  Direction pointer_command(Direction direction);
  Choose switch_command(Choose choose);
  void duplicate_command();
  void roll_command();
  void in_number_command();
  void in_char_command();
  void out_number_command();
  void out_char_command();
 private:
  int pop_get();
};

class Interpreter {
  using Command = void (Interpreter::*)();
 public:
  explicit Interpreter(std::vector<ColorBlockPtr>&& network);
 private:
  void nop_command();
  void push_command();
  void pop_command();
  void add_command();
  void subtract_command();
  void multiply_command();
  void divide_command();
  void mod_command();
  void not_command();
  void greater_command();
  void pointer_command();
  void switch_command();
 private:
  const std::vector<ColorBlockPtr> network_;
  const ColorBlockBase* current_;
  Direction direction_;
  Choose choose_;
  Stack stack_;
  Command commands_[18];
};

#endif  // PIET_I_INTERPRETER_HPP
