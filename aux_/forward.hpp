#ifndef VEILER_AUX_FORWARD_HPP_INCLUDED
#define VEILER_AUX_FORWARD_HPP_INCLUDED

#include<type_traits>

namespace veiler{

template<typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& t)noexcept{
  return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t)noexcept{
  static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
  return static_cast<T&&>(t);
}

}//End : namespace veiler

#endif//VEILER_AUX_FORWARD_HPP_INCLUDED
