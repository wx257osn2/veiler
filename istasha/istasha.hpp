//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_ISTASHA_HPP_INCLUDED
#define VEILER_ISTASHA_HPP_INCLUDED

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

#endif //VEILER_ISTASHA_HPP_INCLUDED
