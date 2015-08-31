#include "interpreter.hpp"

void Stack::push_command(size_t number) {
  push(static_cast<int>(number));
}
void Stack::pop_command() {
  if (!empty()) {
    pop();
  }
}
int Stack::pop_get() {
  const auto value = top();
  pop();
  return value;
}
