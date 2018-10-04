//Copyright (C) 2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_TEMPLE_ULLR_HPP_INCLUDED
#define VEILER_TEMPLE_ULLR_HPP_INCLUDED


namespace veiler{

using std::get;
template<std::size_t I, typename T, std::size_t N, std::enable_if_t<I < N, std::nullptr_t> = nullptr>
constexpr T& get(T(&t)[N])noexcept{return t[I];}

namespace detail{

template<typename T, typename Tuple, std::size_t... Ids>
constexpr T construct_from_tuple_impl(Tuple&& tuple, std::index_sequence<Ids...>)noexcept(std::is_nothrow_constructible<T, decltype(veiler::get<Ids>(std::declval<Tuple>()))...>::value){
  return T(veiler::get<Ids>(std::forward<Tuple>(tuple))...);
}

template<typename T, typename Tuple, std::size_t... Ids>
constexpr T initialize_from_tuple_impl(Tuple&& tuple, std::index_sequence<Ids...>)noexcept(std::is_nothrow_constructible<T, decltype(veiler::get<Ids>(std::declval<Tuple>()))...>::value){
  return T{veiler::get<Ids>(std::forward<Tuple>(tuple))...};
}

}

template<typename T, typename Tuple>
constexpr T construct_from_tuple(Tuple&& tuple)
  noexcept(noexcept(detail::construct_from_tuple_impl<T>(std::declval<Tuple>(     ), std::make_index_sequence<veiler::tuple_size<Tuple>::value>{}))){
             return detail::construct_from_tuple_impl<T>(std::forward<Tuple>(tuple), std::make_index_sequence<veiler::tuple_size<Tuple>::value>{});
}

template<typename T, typename Tuple>
constexpr T initialize_from_tuple(Tuple&& tuple)
  noexcept(noexcept(detail::initialize_from_tuple_impl<T>(std::declval<Tuple>(     ), std::make_index_sequence<veiler::tuple_size<Tuple>::value>{}))){
             return detail::initialize_from_tuple_impl<T>(std::forward<Tuple>(tuple), std::make_index_sequence<veiler::tuple_size<Tuple>::value>{});
}

}//End : namespace veiler

#endif//VEILER_TEMPLE_ULLR_HPP_INCLUDED
