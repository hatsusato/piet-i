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
struct enumeration_traits;

struct enumeration_trait_indexing {
  static constexpr bool does_index = true;
};

template <typename E,
          typename U = typename std::enable_if<
            enumeration_traits<E>::does_index,
            typename std::underlying_type<E>::type>::type>
constexpr U operator+(E e) {
  return static_cast<U>(e);
}
template <typename E,
          ENABLER(enumeration_traits<E>::does_index)>
constexpr E& operator++(E& e) {
  return (e = static_cast<E>((+e + 1) % enumeration_traits<E>::count));
}
template <typename E, typename T,
          ENABLER(enumeration_traits<E>::does_index)>
constexpr E& operator+=(E& e, T value) {
  using type = typename std::underlying_type<E>::type;
  return (e = static_cast<E>(
      (+e + enumeration_traits<E>::count + static_cast<type>(value))
      % enumeration_traits<E>::count));
}
template <typename E,
          typename U = typename std::enable_if<
            enumeration_traits<E>::does_index,
            typename std::underlying_type<E>::type>::type>
constexpr U operator-(E lhs, E rhs) {
  return ((enumeration_traits<E>::count + +lhs - +rhs)
          % enumeration_traits<E>::count);
}

template <typename E,
          ENABLER(enumeration_traits<E>::does_index)>
constexpr auto index(E e) -> decltype(+e) {
  return +e;
}

#endif  // PIET_I_UTILITY_HPP
