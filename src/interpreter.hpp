#ifndef PIET_I_INTERPRETER_HPP
#define PIET_I_INTERPRETER_HPP

#include <stack>
#include <vector>

class Stack : public std::stack<int, std::vector<int> > {
 public:
  void push_command(size_t number);
  void pop_command();
  template <typename Op, bool zero_check = false>
  void binary_command();
 private:
  int pop_get();
};

#endif  // PIET_I_INTERPRETER_HPP
