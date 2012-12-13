#ifndef _VEILER_ISTASHA_HPP_INCLUDED
#define _VEILER_ISTASHA_HPP_INCLUDED

namespace veiler{

#define VEILER_ISTASHA_CREATE(class_name,derived_name,...) \
template<typename T>\
struct class_name{\
  class_name();\
private:\
  static const T derived_name;\
  constexpr int _veiler_istasha_dummy_func_(){return 0;};\
  __VA_ARGS__\
};\
template<typename T>const T class_name<T>::derived_name;

#define VEILER_ISTASHA_DEFAULT_CONSTRUCTOR(class_name,derived_name) \
template<typename T>class_name<T>::class_name(){static_cast<class_name<T>>(derived_name)._veiler_istasha_dummy_func_();}

VEILER_ISTASHA_CREATE(istasha,_veiler_istasha_derived_,)
VEILER_ISTASHA_DEFAULT_CONSTRUCTOR(istasha,_veiler_istasha_derived_)

}//End : namespace veiler

#endif //_VEILER_ISTASHA_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
