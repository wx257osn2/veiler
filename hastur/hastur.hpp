#ifndef _VEILER_HASTUR_HPP_INCLUDED
#define _VEILER_HASTUR_HPP_INCLUDED

#include<utility>

namespace veiler{

#define VEILER_HASTUR_TAG_CREATE(name)\
  struct{\
    template<typename T, typename = typename T::name>\
     static auto _type(T x,int) -> std::true_type;\
    template<typename T>\
     static auto _type(T x,long) -> std::false_type;\
    template<typename T, decltype(&T::name) = &T::name>\
     static auto _value(T x,int) -> std::true_type;\
    template<typename T>\
     static auto _value(T x,long) -> std::false_type;\
    template<typename T,typename... Args>\
     static auto _func(T x,int,Args... args) -> decltype(x.name(args...),std::true_type());\
    template<typename T>\
     static auto _func(T x,long...) -> std::false_type;\
  }

template<typename has> struct hastur{
  template<typename T>struct       type  : decltype(has::_type(std::declval<T>(),0)){};
  template<typename T>struct       value : decltype(has::_value(std::declval<T>(),0)){};
  template<typename T,
           typename... Args>struct func  : decltype(has::_func(std::declval<T>(),0,std::declval<Args>()...)){};
};

}//End : namespace veiler

#endif //_VEILER_HASTUR_HPP_INCLUDED

//Copyright (C) 2013 I
//  Distributed under the Veiler Source License 1.0.
