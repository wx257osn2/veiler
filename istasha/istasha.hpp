#ifndef _VEILER_ISTASHA_HPP_INCLUDED
#define _VEILER_ISTASHA_HPP_INCLUDED

namespace veiler{

#define VEILER_ISTASHA_CREATE(class_name,basetype_name,derived_name,...) \
template<typename basetype_name>\
struct class_name{\
  class_name();\
private:\
  static basetype_name derived_name;\
  constexpr int _veiler_istasha_dummy_func_(){return 0;};\
  __VA_ARGS__\
};\
template<typename basetype_name>basetype_name class_name<basetype_name>::derived_name;\
template<typename basetype_name>class_name<basetype_name>::class_name()

VEILER_ISTASHA_CREATE(istasha,T,_veiler_istasha_derived_){static_cast<istasha<T>>(_veiler_istasha_derived_)._veiler_istasha_dummy_func_();}

}//End : namespace veiler

#endif //_VEILER_ISTASHA_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
