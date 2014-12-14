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
