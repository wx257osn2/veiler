//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_REFIL_HPP_INCLUDED
#define VEILER_REFIL_HPP_INCLUDED

#include<utility>
#include<veiler/ullr/aedra.hpp>
#include<veiler/troy.hpp>
#include<veiler/fool/invoke.hpp>

namespace veiler{

template<typename T>
class refil:public func_troy<T>{
  T* t;
 public:
  constexpr refil(T& t)noexcept:t(veiler::ullr::aedra(t)){}
  refil(T&&) = delete;
  constexpr refil(const refil& x)noexcept:t(x.t){}
  refil& operator=(const refil& x)noexcept{t = x.t;return *this;}
  constexpr T& get()const noexcept{return *t;}
  constexpr operator T&()const noexcept{return *t;}
  template<typename... Args>
  constexpr auto operator()(Args&&... args)const
    ->decltype(invoke(t,veiler::forward<Args>(args)...)){
        return invoke(t,veiler::forward<Args>(args)...);
  }
};

template<typename T>
constexpr refil<T> ref(T& t)noexcept{return t;}

template<typename T>
constexpr refil<const T> cref(const T& t)noexcept{return t;}

}//End : namespace veiler

#endif//VEILER_REFIL_HPP_INCLUDED
