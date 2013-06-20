#ifndef VEILER_ULLR_ERIDIAN_HPP_INCLUDED
#define VEILER_ULLR_ERIDIAN_HPP_INCLUDED

namespace veiler{

namespace ullr{

enum class endian{big=0,little=1};

struct{
  endian operator()()const{return check();}
 private:
  endian check(int x=1)const{return endian(*reinterpret_cast<char *>(&x));}
}eridian;

}//End : namespace ullr

}//End : namespace veiler

#endif //VEILER_ULLR_ERIDIAN_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
