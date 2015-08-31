#include "interpreter.hpp"
#include <algorithm>
#include <functional>
#include <iterator>

void Stack::push_command(size_t number) {
  push(static_cast<int>(number));
}
void Stack::pop_command() {
  if (!empty()) {
    pop();
  }
}
template <typename Op, bool zero_check>
void Stack::binary_command() {
  if (2 <= size()) {
    if (!zero_check || top()) {
      const auto rhs = pop_get();
      const auto lhs = pop_get();
      push(static_cast<int>(Op()(lhs, rhs)));
    }
  }
}
template <typename Op>
void Stack::unary_command() {
  if (!empty()) {
    push(static_cast<int>(Op()(pop_get())));
  }
}
Direction Stack::pointer_command(Direction direction) {
  if (!empty()) {
    const auto d = static_cast<int>(direction) + pop_get();
    return static_cast<Direction>(d % 4);
  } else {
    return direction;
  }
}
Choose Stack::switch_command(Choose choose) {
  if (!empty()) {
    const auto c = static_cast<int>(choose) + pop_get();
    return static_cast<Choose>(c % 2);
  } else {
    return choose;
  }
}
void Stack::duplicate_command() {
  if (!empty()) {
    push(top());
  }
}
void Stack::roll_command() {
  if (2 <= size()) {
    const auto depth = *std::next(c.rbegin());
    if (0 <= depth && static_cast<size_t>(depth) <= size() - 2) {
      const auto step = pop_get();
      pop();
      if (2 <= depth) {
        const auto rolls = (step < 0) ? (step % depth + depth) : (step % depth);
        const auto top = c.rbegin();
        std::rotate(top, std::next(top, rolls), std::next(top, depth));
      }
    }
  }
}
void Stack::in_number_command() {
  int number;
  std::wcin >> number;
  push(number);
}
void Stack::in_char_command() {
  wchar_t character;
  std::wcin >> character;
  push(static_cast<int>(character));
}
void Stack::out_number_command() {
  std::wcout << pop_get();
}
void Stack::out_char_command() {
  std::wcout << static_cast<wchar_t>(pop_get());
}
int Stack::pop_get() {
  const auto value = top();
  pop();
  return value;
}

Interpreter::Interpreter(std::vector<ColorBlockPtr>&& network)
    : network_(std::move(network)),
      current_(network_.front()->address()),
      direction_(Direction::RIGHT),
      choose_(Choose::LEFT),
      stack_(),
      commands_()
{}
void Interpreter::nop_command() {}
void Interpreter::push_command() {
  stack_.push_command(current_->codel_size());
}
void Interpreter::pop_command() {
  stack_.pop_command();
}
void Interpreter::add_command() {
  stack_.binary_command<std::plus<int> >();
}
void Interpreter::subtract_command() {
  stack_.binary_command<std::minus<int> >();
}
void Interpreter::multiply_command() {
  stack_.binary_command<std::multiplies<int> >();
}
void Interpreter::divide_command() {
  stack_.binary_command<std::divides<int>, true>();
}
void Interpreter::mod_command() {
  stack_.binary_command<std::modulus<int>, true>();
}
void Interpreter::greater_command() {
  stack_.binary_command<std::greater<int> >();
}
