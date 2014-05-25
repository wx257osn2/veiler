#ifndef VEILER_ULLR_AEDRA_HPP_INCLUDED
#define VEILER_ULLR_AEDRA_HPP_INCLUDED

#include<veiler/hastur.hpp>

namespace veiler{

namespace ullr{

namespace detail{

template<bool>struct aedra_impl;
template<>
struct aedra_impl<false>{
  template<typename T>
  static constexpr T* impl(T& t){return &t;}
};
template<>
struct aedra_impl<true>{
  template<typename T>
  static T* impl(T& t){return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));}
};


struct not_has_address_operator{
  template<typename T>
  static auto func(T x,veiler::prometheus<1>) -> decltype(&x,std::true_type());
  template<typename T>
  static auto func(T x,veiler::prometheus<0>) -> std::false_type;
};

struct has_address_operator_overload_member{
  template<typename T>
  static auto func(T x,veiler::prometheus<1>) -> decltype(x.operator&(),std::true_type());
  template<typename T>
  static auto func(T x,veiler::prometheus<0>) -> std::false_type;
};

struct has_address_operator_overload_nomember{
  template<typename T>
  static auto func(T x,veiler::prometheus<1>) -> decltype(operator&(x),std::true_type());
  template<typename T>
  static auto func(T x,veiler::prometheus<0>) -> std::false_type;
};


}

template<typename T>
constexpr T* aedra (T& t){
  return detail::aedra_impl<!veiler::hastur<detail::not_has_address_operator>::func<T>{}               ||
                             veiler::hastur<detail::has_address_operator_overload_member>::func<T>{}   ||
                             veiler::hastur<detail::has_address_operator_overload_nomember>::func<T>{}>::template impl<T>(t);
}

}

}

#endif//VEILER_ULLR_AEDRA_HPP_INCLUDED

//Copyright (C) 2014 I
//  Distributed under the Veiler Source License 1.0.