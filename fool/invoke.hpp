//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_FOOL_INVOKE_HPP_INCLUDED
#define VEILER_FOOL_INVOKE_HPP_INCLUDED

#include<veiler/aux_/forward.hpp>

namespace veiler{

namespace detail{

namespace fool{

template<typename F, typename T, typename... Args>
constexpr auto invoke(F&& f, T&& t, Args&&... ts)
  noexcept(noexcept((veiler::forward<T>(t).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...)))
         ->decltype((veiler::forward<T>(t).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...)){
             return (veiler::forward<T>(t).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...);
}
template<typename F, typename T, typename... Args>
constexpr auto invoke(F&& f, T&& t, Args&&... ts)
  noexcept(noexcept(((*veiler::forward<T>(t)).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...)))
         ->decltype(((*veiler::forward<T>(t)).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...)){
             return ((*veiler::forward<T>(t)).*veiler::forward<F>(f))(veiler::forward<Args>(ts)...);
}
template<typename F, typename T>
constexpr auto invoke(F&& f, T&& t)
  noexcept(noexcept(veiler::forward<T>(t).*veiler::forward<F>(f)))
         ->decltype(veiler::forward<T>(t).*veiler::forward<F>(f)){
             return veiler::forward<T>(t).*veiler::forward<F>(f);
}
template<typename F, typename T>
constexpr auto invoke(F&& f, T&& t)
  noexcept(noexcept((*veiler::forward<T>(t)).*veiler::forward<F>(f)))
         ->decltype((*veiler::forward<T>(t)).*veiler::forward<F>(f)){
             return (*veiler::forward<T>(t)).*veiler::forward<F>(f);
}
template<typename F, typename... Args>
constexpr auto invoke(F&& f, Args&&... args)
  noexcept(noexcept(veiler::forward<F>(f)(veiler::forward<Args>(args)...)))
         ->decltype(veiler::forward<F>(f)(veiler::forward<Args>(args)...)){
             return veiler::forward<F>(f)(veiler::forward<Args>(args)...);
}

}

}

using detail::fool::invoke;

}

#endif//VEILER_FOOL_INVOKE_HPP_INCLUDED
