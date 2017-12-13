//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_TEMPLE_THAI_TEMPLE_HPP_INCLUDED
#define VEILER_TEMPLE_THAI_TEMPLE_HPP_INCLUDED

#include<type_traits>

namespace veiler{

namespace detail{

namespace temple{



template<typename...>struct type_tuple;


template<typename T>
struct type_wrapper{using type = T;};


template<typename,typename>struct make_type_tuple_impl_impl;
template<typename... Lefts, typename... Rights>
struct make_type_tuple_impl_impl<type_tuple<Lefts...>, type_tuple<Rights...>>{
  using type = type_tuple<Lefts..., Rights...>;
};

template<std::size_t N, typename T>
struct make_type_tuple_impl :
       make_type_tuple_impl_impl<
typename make_type_tuple_impl<    N/2, T>::type,
typename make_type_tuple_impl<N - N/2, T>::type
       >{};
template<typename T>
struct make_type_tuple_impl<1, T>{using type = type_tuple<T>;};
template<typename T>
struct make_type_tuple_impl<0, T>{using type = type_tuple< >;};

template<std::size_t N, typename T = void>
using make_type_tuple = typename make_type_tuple_impl<N, T>::type;


template<typename>struct tuple_size_impl;
template<template<typename...>class Tuple, typename... Types>
struct tuple_size_impl<Tuple<Types...>>{
  static constexpr std::size_t value = sizeof...(Types);
  constexpr operator std::size_t()const{return value;}
  constexpr tuple_size_impl(){}
};

template<typename T>
using tuple_size = tuple_size_impl<typename std::remove_cv<typename std::remove_reference<T>::type>::type>;


template<typename>struct type_at_impl_impl_impl;
template<typename... Types>
struct type_at_impl_impl_impl<type_tuple<Types...>>{template<typename T>static T eval(Types*..., T*, ...);};

template<std::size_t N, typename... Types>
struct type_at_impl_impl{
  using type = decltype(type_at_impl_impl_impl<make_type_tuple<N>>::eval(static_cast<Types*>(nullptr)...));
};

template<typename, std::size_t>struct type_at_impl_;
template<template<typename...>class Type, typename... Args, std::size_t N>
struct type_at_impl_<type_wrapper<Type<Args...>>, N>{
  using type = typename type_at_impl_impl<N, type_wrapper<Args>...>::type;
};

template<typename T, std::size_t N>
class type_at_impl{
  static_assert(tuple_size<T>::value > N, "Veiler.Temple - type_at can't access there, out of range.");
 public:
  using type = typename type_at_impl_<type_wrapper<T>, N>::type::type;
};

template<typename T, std::size_t N>
using type_at = typename type_at_impl<typename std::remove_cv<typename std::remove_reference<T>::type>::type, N>::type;


struct duplicate_type;
template<long long A, long long B>struct add{static const long long value = A+B;static const long long default_value = 0;};
template<typename...>struct unique_types_impl;
template<typename SourceType, typename Type>struct filter{using type = typename std::conditional<std::is_same<SourceType, Type>::value, duplicate_type, Type>::type;};
template<typename T, typename... Remaining, typename... Uniques>
struct unique_types_impl<type_tuple<T, Remaining...>, Uniques...>{
  using type = typename unique_types_impl<type_tuple<typename filter<T, Remaining>::type...>, Uniques..., T>::type;
};
template<typename... Remaining, typename... Uniques>
struct unique_types_impl<type_tuple<duplicate_type, Remaining...>, Uniques...>:unique_types_impl<type_tuple<Remaining...>, Uniques...>{};
template<typename... Uniques>
struct unique_types_impl<type_tuple<>, Uniques...>{using type = type_tuple<Uniques...>;};

template<typename T>
using unique_types = typename unique_types_impl<T>::type;


template<typename... Types>
struct type_tuple{
  //template<std::size_t N>
  //using at = type_at<type_tuple<Types...>, N>;
  static constexpr std::size_t size(){return sizeof...(Types);}
};



}//End : namespace temple

}//End : namespace detail

using detail::temple::type_at;
using detail::temple::tuple_size;
using detail::temple::unique_types;
using detail::temple::type_tuple;

}//End : namespace veiler

#endif//VEILER_TEMPLE_THAI_TEMPLE_HPP_INCLUDED
