//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_FORSETI_OLYMPUS_HPP_INCLUDED
#define VEILER_FORSETI_OLYMPUS_HPP_INCLUDED

#include<fstream>

namespace veiler{

namespace forseti{

namespace olympus{

namespace detail{

struct openmode{
  friend constexpr openmode operator+(const openmode lhs,const openmode rhs){
    return lhs | rhs | openmode{std::ios::in | std::ios::out};
  }
  friend constexpr openmode operator|(const openmode lhs,const openmode rhs){
    return openmode{lhs.value | rhs.value};
  }
  constexpr operator std::ios::openmode(){return value;}
  std::ios::openmode value;
};
constexpr openmode 
  r ={ std::ios::in                                                               },
  w ={              std::ios::out | std::ios::trunc                               },
  a ={              std::ios::out |                 std::ios::app                 },
  b ={                                                            std::ios::binary},
  _ ={                                                                            },

  rb= r | b ,
  wb= w | b ,
  ab= a | b ;

constexpr openmode olympus_impl(const char* p,openmode o){
  return *p ? *p == 'r' ? olympus_impl(p+1,o|r) : 
              *p == 'w' ? olympus_impl(p+1,o|w) : 
              *p == 'a' ? olympus_impl(p+1,o|a) : 
              *p == 'b' ? olympus_impl(p+1,o|b) : 
              *p == '+' ? olympus_impl(p+1,o+_) : 
                olympus_impl(p+1,o)
            : o;
}

}//End : namespace detail

using detail::openmode;
using detail::r;
using detail::w;
using detail::a;
using detail::b;
using detail::_;
using detail::rb;
using detail::wb;
using detail::ab;

constexpr openmode olympus(const char* mode){return detail::olympus_impl(mode,_);}

}//End : namespace olympus

}//End : namespace forseti

}//End : namespace veiler

#endif //VEILER_FORSETI_OLYMPUS_HPP_INCLUDED
