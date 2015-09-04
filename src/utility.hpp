#ifndef PIET_I_MEMORY_HPP
#define PIET_I_MEMORY_HPP

#include <memory>

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif  // PIET_I_MEMORY_HPP
