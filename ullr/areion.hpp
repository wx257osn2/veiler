#ifndef VEILER_ULLR_AREION_HPP_INCLUDED
#define VEILER_ULLR_AREION_HPP_INCLUDED

#include<array>
#include<utility>

namespace veiler{

namespace ullr{

template<typename T,typename... Args>
  constexpr std::array<T,sizeof...(Args)>&& areion(Args... args){return std::move(std::array<T,sizeof...(Args)>{args...});}

}//End : namespace ullr

}//End : namespace veiler

#endif //VEILER_ULLR_AREION_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
