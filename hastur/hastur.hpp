//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_HASTUR_HPP_INCLUDED
#define VEILER_HASTUR_HPP_INCLUDED

#include<utility>
#include<veiler/prometheus.hpp>

namespace veiler{

#define VEILER_HASTUR_TAG_CREATE(name)\
  struct{\
    template<typename T, typename = typename T::name>\
     static auto type(T x,veiler::prometheus<1>) -> std::true_type;\
    template<typename T>\
     static auto type(T x,veiler::prometheus<0>) -> std::false_type;\
    template<typename T, decltype(&T::name) = &T::name>\
     static auto value(T x,veiler::prometheus<1>) -> std::true_type;\
    template<typename T>\
     static auto value(T x,veiler::prometheus<0>) -> std::false_type;\
    template<typename T,typename... Args>\
     static auto func(T x,veiler::prometheus<1>,Args... args) -> decltype(x.name(args...),std::true_type());\
    template<typename T>\
     static auto func(T x,veiler::prometheus<0>...) -> std::false_type;\
  }

template<typename has> struct hastur{
  template<typename T>struct       type  : decltype(has::type(std::declval<T>(),veiler::prometheus_fire<1>())){};
  template<typename T>struct       value : decltype(has::value(std::declval<T>(),veiler::prometheus_fire<1>())){};
  template<typename T,
           typename... Args>struct func  : decltype(has::func(std::declval<T>(),veiler::prometheus_fire<1>(),std::declval<Args>()...)){};
};

}//End : namespace veiler

#endif //VEILER_HASTUR_HPP_INCLUDED
