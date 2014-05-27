#ifndef VEILER_ULLR_ERIDIAN_HPP_INCLUDED
#define VEILER_ULLR_ERIDIAN_HPP_INCLUDED

namespace veiler{

namespace ullr{

enum class endian{big=0,little=1};

namespace detail{

inline endian endian_check(int x = 1){return endian(*reinterpret_cast<char*>(&x));}

}//End : namespace detail

inline endian eridian(){return detail::endian_check();}

}//End : namespace ullr

}//End : namespace veiler

#endif //VEILER_ULLR_ERIDIAN_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
