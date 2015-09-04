#ifndef PIET_I_UTILITY_HPP
#define PIET_I_UTILITY_HPP

#include <utility>

namespace std {
template <class, class>
class unique_ptr;
template <class>
struct default_delete;
}

template <typename T, typename... Args>
std::unique_ptr<T, std::default_delete<T> > make_unique(Args&&... args) {
  return std::unique_ptr<T, std::default_delete<T> >(
      new T(std::forward<Args>(args)...));
}

#endif  // PIET_I_UTILITY_HPP
