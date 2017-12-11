//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_AUX_MOVE_HPP_INCLUDED
#define VEILER_AUX_MOVE_HPP_INCLUDED

#include<type_traits>

namespace veiler{

template<typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& t)noexcept{
  return static_cast<typename std::remove_reference<T>::type&&>(t);
}

}//End : namespace veiler

#endif//VEILER_AUX_MOVE_HPP_INCLUDED
