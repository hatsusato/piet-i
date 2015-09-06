#ifndef PIET_I_INTERPRETER_HPP
#define PIET_I_INTERPRETER_HPP

#include <array>
#include <stack>
#include <vector>
#include "block.hpp"
#include "direction.hpp"

using Integer = int;
class CodelTable;

class Stack : public std::stack<Integer, std::vector<Integer> > {
 public:
  void push_command(size_t number);
  void pop_command();
  template <typename Op, bool zero_check = false>
  void binary_command();
  template <typename Op>
  void unary_command();
  void pointer_command(DP& dp);
  void switch_command(CC& cc);
  void duplicate_command();
  void roll_command();
  template <typename T>
  void in_command();
  template <typename T>
  void out_command();
 private:
  Integer pop_get();
};

class Interpreter {
  using Command = void (Interpreter::*)();
 public:
  explicit Interpreter(const CodelTable& table);
  void run();
  bool stepwise_execute();
 private:
  void do_command(BlockPointer next);
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
  void duplicate_command();
  void roll_command();
  void in_number_command();
  void in_char_command();
  void out_number_command();
  void out_char_command();
 private:
  const std::vector<Block> network_;
  BlockPointer current_;
  DP dp_;
  CC cc_;
  Stack stack_;
  std::array<Command, Colour::count> commands_;
};

#endif  // PIET_I_INTERPRETER_HPP
