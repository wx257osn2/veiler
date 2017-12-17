#ifndef VEILER_EIKTHYRNIR_DOVAH_HPP_INCLUDED
#define VEILER_EIKTHYRNIR_DOVAH_HPP_INCLUDED

#include<veiler/eikthyrnir/eikthyrnir.hpp>

namespace veiler{

template<typename Ex, typename F, std::enable_if_t<!std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<decltype(f()), Ex>
try{
  return f();
}catch(...){
  return make_unexpected(error_traits<Ex>::make_error_from_current_exception());
}

template<typename Ex, typename F, std::enable_if_t<std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<void, Ex>
try{
  f();
  return {};
}catch(...){
  return make_unexpected(error_traits<Ex>::make_error_from_current_exception());
}

template<typename F, std::enable_if_t<!std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<decltype(f())>
try{
  return f();
}catch(...){
  return make_unexpected_from_current_exception();
}

template<typename F, std::enable_if_t<std::is_same<std::decay_t<decltype(std::declval<F>()())>, void>::value>* = nullptr>
auto do_(F&& f)->expected<void>
try{
  f();
  return {};
}catch(...){
  return make_unexpected_from_current_exception();
}

}//End : namespace veiler

#endif//VEILER_EIKTHYRNIR_DOVAH_HPP_INCLUDED
