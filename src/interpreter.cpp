#include "interpreter.hpp"
#include <algorithm>
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
template <typename T>
void Stack::in_command() {
  T value;
  std::wcin >> value;
  push(static_cast<int>(value));
}
template <typename T>
void Stack::out_command() {
  std::wcout << static_cast<T>(pop_get());
}
int Stack::pop_get() {
  const auto value = top();
  pop();
  return value;
}
