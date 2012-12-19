#ifndef _VEILER_FORSETI_OLYMPUS_HPP_INCLUDED
#define _VEILER_FORSETI_OLYMPUS_HPP_INCLUDED

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

}//End : namespace detail

#define VEILER_PELOPS_LUPEGEM_USING_HEADER detail:
#include "../pelops/lupegem/using.hpp"

VEILER_PELOPS_LUPEGEM_USING((:openmode)(:r)(:w)(:a)(:b)(:_)(:rb)(:wb)(:ab))

#include "../pelops/lupegem/undef_using.hpp"


}//End : namespace olympus

}//End : namespace forseti

}//End : namespace veiler

#endif //_VEILER_FORSETI_OLYMPUS_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
