#ifndef PIET_I_UTILITY_HPP
#define PIET_I_UTILITY_HPP

#include <type_traits>
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

template <bool B>
using enabler = typename std::enable_if<B, std::nullptr_t>::type;

#define ENABLER(B)\
  enabler<B> = nullptr

template <typename E>
struct enumeration_traits {};

struct enumeration_trait_indexing {
  static constexpr bool does_index = true;
};

template <typename E,
          typename U = typename std::enable_if<
            enumeration_traits<E>::does_index,
            typename std::underlying_type<E>::type>::type>
U operator+(E e) {
  return static_cast<U>(e);
}

#endif  // PIET_I_UTILITY_HPP
