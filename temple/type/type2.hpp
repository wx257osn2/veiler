//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_TEMPLE_TYPE_TYPE2_HPP_INCLUDED
#define VEILER_TEMPLE_TYPE_TYPE2_HPP_INCLUDED

#include<type_traits>
#include<veiler/temple/type/type1.hpp>
#include<veiler/temple/integer.hpp>

namespace veiler{

namespace detail{

namespace temple{

template<typename Types, template<typename>class Cond>
class find_unique_type_index{
  template<typename>struct impl_;
  template<typename... As>
  struct impl_<type_tuple<As...>>{using type = std::integer_sequence<bool, Cond<As>::value...>;};
  template<typename,typename>struct impl;
  template<std::make_signed_t<std::size_t>... As, bool... Bs>
  struct impl<std::integer_sequence<std::make_signed_t<std::size_t>, As...>, std::integer_sequence<bool, Bs...>>{static constexpr std::make_signed_t<std::size_t> value = ((Bs ? As+1 : 0) + ... + 0);};
  using A = make_integer_range<std::make_signed_t<std::size_t>, 0, veiler::tuple_size<Types>{}>;
  using B = typename impl_<Types>::type;
 public:
  static const std::make_signed_t<std::size_t> value = impl<A, B>::value - 1;
};

}//End : namespace temple

}//End : namespace detail

using detail::temple::find_unique_type_index;

}//End : namespace veiler

#endif//VEILER_TEMPLE_TYPE_TYPE2_HPP_INCLUDED