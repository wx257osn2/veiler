#ifndef _VEILER_REFIL_HPP_INCLUDED
#define _VEILER_REFIL_HPP_INCLUDED

namespace veiler{

template<typename T>class refil{
  T& ref;
 public:
  refil(T& t):ref(t){}
  operator T&(){return ref;}
  operator const T&()const{return ref;}
};

}//End : namespace veiler

#endif //_VEILER_REFIL_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
