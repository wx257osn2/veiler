//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_TEMPLE_INDIAN_TEMPLE_HPP_INCLUDED
#define VEILER_TEMPLE_INDIAN_TEMPLE_HPP_INCLUDED

#include<veiler/temple/thai_temple.hpp>

namespace veiler{

namespace detail{

namespace temple{



template<long long...>struct index_tuple;


template<typename>struct index_at_impl;
template<typename... Types>
struct index_at_impl<type_tuple<Types...>>{template<typename T>static constexpr T eval(Types..., T t, ...){return t;}};

template<typename T, std::size_t N>
class index_at{
  template<template<long long...>class IndexTuple, long long... Indices>
  static constexpr long long impl(IndexTuple<Indices...>)noexcept{return index_at_impl<make_type_tuple<N, long long>>::eval(Indices...);}
 public:
  static constexpr long long value = impl(T{});
  constexpr operator long long()const noexcept{return value;}
};

template<typename>struct index_size_impl;
template<template<long long...>class IndexTuple, long long... Indices>
struct index_size_impl<IndexTuple<Indices...>>{
  static constexpr std::size_t value = sizeof...(Indices);
  constexpr operator std::size_t()const{return value;}
  constexpr index_size_impl(){}
};

template<typename T>
using index_size = index_size_impl<typename std::remove_cv<typename std::remove_reference<T>::type>::type>;


template<typename,long long>struct make_index_range_next;
template<long long... Indices, long long Next>
struct make_index_range_next<index_tuple<Indices...>, Next>{using type = index_tuple<Indices..., (Indices+Next)...>;};

template<typename,long long,long long>struct make_index_range_next_;
template<long long... Indices, long long Next, long long Tail>
struct make_index_range_next_<index_tuple<Indices...>, Next, Tail>{using type = index_tuple<Indices..., (Indices+Next)..., Tail>;};

template<long long,long long,long long,typename = void>struct make_index_range_impl;
template<long long Begin, long long Step, long long Next>
struct make_index_range_impl<Begin, Step, Next, typename std::enable_if<(Next==0 || Next==1)>::type>{
  using type = typename std::conditional<Next==0, index_tuple<>, index_tuple<Begin>>::type;
};
template<long long Begin, long long Step, long long Next>
struct make_index_range_impl<Begin, Step, Next, typename std::enable_if<(Next>1)>::type>{
  using type = typename std::conditional<Next % 2,
                 typename make_index_range_next_<
                   typename make_index_range_impl<Begin, Step, Next/2>::type,
                                     Next/2  * Step,
                            Begin + (Next-1) * Step
                          >::type,
                 typename make_index_range_next<
                   typename make_index_range_impl<Begin, Step, Next/2>::type,
                                     Next/2  * Step
                          >::type
                        >::type;
};

template<long long Begin, long long End, long long Step = (Begin<End ? 1 : -1)>
using make_index_range = typename make_index_range_impl<Begin, Step, (End - Begin + (Step>0 ? Step-1 : Step+1)) / Step>::type;


template<typename,typename>struct reverse_index_tuple_impl_impl;
template<long long... Indices, long long... IndicesIndices>
struct reverse_index_tuple_impl_impl<index_tuple<Indices...>, index_tuple<IndicesIndices...>>{
  using type = index_tuple<index_at<index_tuple<Indices...>, sizeof...(IndicesIndices) - 1 - IndicesIndices>::value...>;
};

template<typename>struct reverse_index_tuple_impl;
template<long long... Indices>
struct reverse_index_tuple_impl<index_tuple<Indices...>> : 
       reverse_index_tuple_impl_impl<index_tuple<Indices...>, make_index_range<0,sizeof...(Indices)>>{};

template<typename T>
using reverse_index_tuple = typename reverse_index_tuple_impl<T>::type;


template<typename... Types>
using make_indexes = make_index_range<0, sizeof...(Types)>;

template<typename... Types>
using make_reverse_indexes = make_index_range<sizeof...(Types)-1, -1>;

template<long long... Indices>
struct index_tuple{
  static constexpr std::size_t size()noexcept{return sizeof...(Indices);}
};



}//End : namespace temple

}//End : namespace detail

using detail::temple::index_at;
using detail::temple::index_size;
using detail::temple::reverse_index_tuple;
using detail::temple::make_index_range;
using detail::temple::make_reverse_indexes;
using detail::temple::make_indexes;
using detail::temple::index_tuple;

}//End : namespace veiler

#endif//VEILER_TEMPLE_INDIAN_TEMPLE_HPP_INCLUDED
