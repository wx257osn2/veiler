//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_FOOL_HASH_HPP_INCLUDED
#define VEILER_FOOL_HASH_HPP_INCLUDED

#include<functional>
#include<tuple>
#include<variant>
#include<iterator>
#include<type_traits>
#include<veiler/hastur.hpp>

namespace veiler{

namespace detail{

namespace fool{

constexpr std::size_t hash_combine(std::size_t t)noexcept{return t;}

constexpr std::size_t hash_combine(std::size_t lhs, std::size_t rhs)noexcept{
  return lhs ^ (rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
}

template<typename... Args>
constexpr std::size_t hash_combine(std::size_t lhs, std::size_t a, std::size_t b, Args&&... cs)noexcept{
  return hash_combine(hash_combine(lhs, a), b, std::forward<Args>(cs)...);
}

namespace hastur_tag{

using difference = VEILER_HASTUR_TAG_CREATE(difference_type);
using value = VEILER_HASTUR_TAG_CREATE(value_type);
using pointer = VEILER_HASTUR_TAG_CREATE(pointer);
using reference = VEILER_HASTUR_TAG_CREATE(reference);
using iterator_category = VEILER_HASTUR_TAG_CREATE(iterator_category);

}//End : namespace hastur_tag

template<typename T>
using is_iterator = std::conjunction<typename veiler::hastur<hastur_tag::difference>::type<std::iterator_traits<T>>,
                                     typename veiler::hastur<hastur_tag::value>::type<std::iterator_traits<T>>,
                                     typename veiler::hastur<hastur_tag::pointer>::type<std::iterator_traits<T>>,
                                     typename veiler::hastur<hastur_tag::reference>::type<std::iterator_traits<T>>,
                                     typename veiler::hastur<hastur_tag::iterator_category>::type<std::iterator_traits<T>>>;

template<typename T, typename = std::nullptr_t>
struct hash_impl;

template<typename T>
using iterator_condition = std::conjunction<std::negation<std::is_pointer<T>>, is_iterator<T>>;
template<typename T>
struct hash_impl<T, std::enable_if_t<iterator_condition<T>::value, std::nullptr_t>> : hash_impl<typename T::pointer>{
  constexpr hash_impl() = default;
  constexpr hash_impl(const hash_impl&) = default;
  constexpr hash_impl(hash_impl&&) = default;
  ~hash_impl() = default;
  hash_impl& operator=(const hash_impl&) = default;
  hash_impl& operator=(hash_impl&&) = default;
  using result_type = std::size_t;
  using argument_type = T;
  std::size_t operator()(const T& key)const noexcept{
    return hash_impl<typename T::pointer>::operator()(&*key);
  }
};

template<typename T>
struct hash_impl<T, std::enable_if_t<std::negation<iterator_condition<T>>::value, std::nullptr_t>> : std::hash<T>{};

}//End : namespace fool

}//End : namespace detail

template<typename T>
struct hash : detail::fool::hash_impl<T>{};

template<typename T, typename U>
struct hash<std::pair<T, U>> : hash<T>, hash<U>{
  constexpr hash() = default;
  constexpr hash(const hash&) = default;
  constexpr hash(hash&&) = default;
  ~hash() = default;
  hash& operator=(const hash&) = default;
  hash& operator=(hash&&) = default;
  using result_type = std::size_t;
  using argument_type = std::pair<T, U>;
  std::size_t operator()(const std::pair<T, U>& key)const noexcept{
    return detail::fool::hash_combine(hash<T>::operator()(key.first), hash<U>::operator()(key.second));
  }
};

template<typename... Ts>
struct hash<std::tuple<Ts...>> : hash<Ts>...{
 private:
  template<std::size_t... Indices>
  std::size_t impl(const std::tuple<Ts...>& key, std::index_sequence<Indices...>)const noexcept{
    return detail::fool::hash_combine(hash<Ts>::operator()(std::get<Indices>(key))...);
  }
 public:
  constexpr hash() = default;
  constexpr hash(const hash&) = default;
  constexpr hash(hash&&) = default;
  ~hash() = default;
  hash& operator=(const hash&) = default;
  hash& operator=(hash&&) = default;
  using result_type = std::size_t;
  using argument_type = std::tuple<Ts...>;
  std::size_t operator()(const std::tuple<Ts...>& key)const noexcept{
    return impl(key, std::index_sequence_for<Ts...>{});
  }
};

template<typename... Ts>
struct hash<std::variant<Ts...>> : hash<Ts>...{
  constexpr hash() = default;
  constexpr hash(const hash&) = default;
  constexpr hash(hash&&) = default;
  ~hash() = default;
  hash& operator=(const hash&) = default;
  hash& operator=(hash&&) = default;
  using result_type = std::size_t;
  using argument_type = std::variant<Ts...>;
  std::size_t operator()(const std::variant<Ts...>& key)const noexcept{
    return std::visit(*this, key);
  }
};

}//End : namespace veiler

#endif//VEILER_FOOL_HASH_HPP_INCLUDED
