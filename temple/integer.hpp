//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_TEMPLE_INTEGER_HPP_INCLUDED
#define VEILER_TEMPLE_INTEGER_HPP_INCLUDED

#include<utility>
#include<veiler/temple/type/type1.hpp>

namespace veiler{

namespace detail{

namespace temple{


template<typename>struct integer_at_impl;
template<typename... Types>
struct integer_at_impl<type_tuple<Types...>>{template<typename T>static constexpr T eval(Types..., T t, ...){return t;}};

template<typename T, std::size_t N>
class integer_at{
  template<typename I, template<typename, I...>class IntegerSequence, I... Indices>
  static constexpr I impl(IntegerSequence<I, Indices...>)noexcept{return integer_at_impl<make_type_tuple<N, I>>::eval(Indices...);}
  template<typename I, template<I...>class IntegerSequence, I... Indices>
  static constexpr I impl(IntegerSequence<Indices...>)noexcept{return integer_at_impl<make_type_tuple<N, I>>::eval(Indices...);}
 public:
  static constexpr typename T::value_type value = impl<typename T::value_type>(T{});
  constexpr operator typename T::value_type()const noexcept{return value;}
};

template<typename T,std::make_signed_t<typename T::value_type>>struct make_integer_range_next;
template<typename I, I... Indices, std::make_signed_t<I> Next>
struct make_integer_range_next<std::integer_sequence<I, Indices...>, Next>{using type = std::integer_sequence<I, Indices..., (Indices+Next)...>;};

template<typename T,std::make_signed_t<typename T::value_type>,typename T::value_type>struct make_integer_range_next_;
template<typename I, I... Indices, std::make_signed_t<I> Next, I Tail>
struct make_integer_range_next_<std::integer_sequence<I, Indices...>, Next, Tail>{using type = std::integer_sequence<I, Indices..., (Indices+Next)..., Tail>;};

template<typename I,I,std::make_signed_t<I>,std::make_signed_t<I>,typename = void>struct make_integer_range_impl;
template<typename I, I Begin, std::make_signed_t<I> Step, std::make_signed_t<I> Next>
struct make_integer_range_impl<I, Begin, Step, Next, typename std::enable_if<(Next==0 || Next==1)>::type>{
  using type = typename std::conditional<Next==0, std::integer_sequence<I>, std::integer_sequence<I, Begin>>::type;
};
template<typename I, I Begin, std::make_signed_t<I> Step, std::make_signed_t<I> Next>
struct make_integer_range_impl<I, Begin, Step, Next, typename std::enable_if<(Next>1)>::type>{
  using type = typename std::conditional<Next % 2 == 1,
                 typename make_integer_range_next_<
                   typename make_integer_range_impl<I, Begin, Step, Next/2>::type,
                                     Next/2  * Step,
                            Begin + (Next-1) * Step
                          >::type,
                 typename make_integer_range_next<
                   typename make_integer_range_impl<I, Begin, Step, Next/2>::type,
                                     Next/2  * Step
                          >::type
                        >::type;
};

template<typename I, I Begin, I End, std::make_signed_t<I> Step = (Begin<End ? 1 : -1)>
using make_integer_range = typename make_integer_range_impl<I, Begin, Step, (End - Begin + (Step>0 ? Step-1 : Step+1)) / Step>::type;


template<typename,typename>struct reverse_integer_sequence_impl_impl;
template<typename I, I... Indices, std::size_t... IndicesIndices>
struct reverse_integer_sequence_impl_impl<std::integer_sequence<I, Indices...>, std::index_sequence<IndicesIndices...>>{
  using type = std::integer_sequence<I, integer_at<std::integer_sequence<I, Indices...>, sizeof...(IndicesIndices) - 1 - IndicesIndices>::value...>;
};

template<typename>struct reverse_integer_sequence_impl;
template<typename I, I... Indices>
struct reverse_integer_sequence_impl<std::integer_sequence<I, Indices...>> : 
       reverse_integer_sequence_impl_impl<std::integer_sequence<I, Indices...>, make_integer_range<std::size_t, 0, sizeof...(Indices)>>{};

template<typename T>
using reverse_integer_sequence = typename reverse_integer_sequence_impl<T>::type;


template<typename... Types>
using make_reverse_indexes_for = make_integer_range<std::make_signed_t<std::size_t>, sizeof...(Types)-1, -1>;



}//End : namespace temple

}//End : namespace detail

using detail::temple::integer_at;
using detail::temple::reverse_integer_sequence;
using detail::temple::make_integer_range;
using detail::temple::make_reverse_indexes_for;

}//End : namespace veiler

#endif//VEILER_TEMPLE_INTEGER_HPP_INCLUDED
