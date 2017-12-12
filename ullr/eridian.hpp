//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_ULLR_ERIDIAN_HPP_INCLUDED
#define VEILER_ULLR_ERIDIAN_HPP_INCLUDED

namespace veiler{

namespace ullr{

enum class endian : unsigned char{big=0,little=1};

namespace detail{

inline endian endian_check(int x = 1){return endian(*reinterpret_cast<char*>(&x));}

}//End : namespace detail

inline endian eridian(){return detail::endian_check();}

}//End : namespace ullr

}//End : namespace veiler

#endif //VEILER_ULLR_ERIDIAN_HPP_INCLUDED
