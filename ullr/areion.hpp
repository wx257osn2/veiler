//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_ULLR_AREION_HPP_INCLUDED
#define VEILER_ULLR_AREION_HPP_INCLUDED

#include<array>
#include<utility>

namespace veiler{

namespace ullr{

template<typename T,typename... Args>
  constexpr std::array<T,sizeof...(Args)> areion(Args... args)noexcept{return std::array<T,sizeof...(Args)>{args...};}

}//End : namespace ullr

}//End : namespace veiler

#endif //VEILER_ULLR_AREION_HPP_INCLUDED
