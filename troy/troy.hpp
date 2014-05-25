#ifndef VEILER_TROY_HPP_INCLUDED
#define VEILER_TROY_HPP_INCLUDED
#include<type_traits>
#include<veiler/hastur.hpp>

namespace veiler{

namespace detail{

namespace temple{

template<typename...>struct type_tuple;

}//End : namespace temple

namespace troy{

template<typename F>
struct member_pointer_traits_impl{
  using type = F;
  using master = void;
};
template<typename R, typename T>
struct member_pointer_traits_impl<R T::*>{
  using type = R T::*;
  using master = T*;
  using result_type = R;
};
template<typename R, typename T, typename... Args>
struct member_pointer_traits_impl<R(T::*)(Args...)>{
  using type = R(T::*)(Args...);
  using master = T*;
  using result_type = R;
};
template<typename R, typename T, typename... Args>
struct member_pointer_traits_impl<R(T::*)(Args...)const>{
  using type = R(T::*)(Args...);
  using master = const T*;
  using result_type = R;
};
template<typename R, typename T, typename... Args>
struct member_pointer_traits_impl<R(T::*)(Args...)volatile>{
  using type = R(T::*)(Args...);
  using master = volatile T*;
  using result_type = R;
};
template<typename R, typename T, typename... Args>
struct member_pointer_traits_impl<R(T::*)(Args...)const volatile>{
  using type = R(T::*)(Args...);
  using master = const volatile T*;
  using result_type = R;
};




template<typename,typename=void,int=0>struct func_traits_impl{};
template<typename R, typename... Args>
struct func_traits_impl<R(Args...)>{
  using result_type = R;
  using arg_types = temple::type_tuple<Args...>;
};
template<typename R, typename... Args>
struct func_traits_impl<R(*)(Args...)>:func_traits_impl<R(Args...)>{};
template<typename R, typename T, typename... Args, typename U>
struct func_traits_impl<R(T::*)(Args...), U>{
  using result_type = R;
  using arg_types = temple::type_tuple<U, Args...>;
};
template<typename F>
struct func_traits_impl<F, void, 1>{
  using result_type = typename F::result_type;
};
template<typename F>
struct func_traits_impl<F, void, 3>{
  using result_type = typename F::result_type;
  using arg_types = temple::type_tuple<typename F::argument_type>;
};
template<typename F>
struct func_traits_impl<F, void, 5>{
  using result_type = typename F::result_type;
  using arg_types = temple::type_tuple<typename F::first_argument_type, typename F::second_argument_type>;
};

using result_          = VEILER_HASTUR_TAG_CREATE(result_type);
using argument_        = VEILER_HASTUR_TAG_CREATE(argument_type);
using first_argument_  = VEILER_HASTUR_TAG_CREATE(first_argument_type);
using second_argument_ = VEILER_HASTUR_TAG_CREATE(second_argument_type);

}//End : namespace troy

}//End : namespace detail


template<typename F>
using member_pointer_troy = detail::troy::member_pointer_traits_impl<typename std::remove_cv<F>::type>;


template <class M, class T>
using is_member_pointer_of = std::is_base_of<
                      typename std::remove_pointer<
                        typename member_pointer_troy<M>::master
                               >::type,
                               T
                             >;


template<typename F>
using func_troy = detail::troy::func_traits_impl<typename member_pointer_troy<F>::type,
                                                 typename member_pointer_troy<F>::master,
                                                 veiler::hastur<detail::troy::result_>::type<F>{}           * 1 +
                                                 veiler::hastur<detail::troy::argument_>::type<F>{}         * 2 +
                                                (veiler::hastur<detail::troy::first_argument_>::type<F>{} &&
                                                 veiler::hastur<detail::troy::second_argument_>::type<F>{}) * 4>;


}//End : namespace veiler

#endif//VEILER_TROY_HPP_INCLUDED

//Copyright (C) 2014 I
//  Distributed under the Veiler Source License 1.0.
