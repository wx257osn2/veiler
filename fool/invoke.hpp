//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_FOOL_INVOKE_HPP_INCLUDED
#define VEILER_FOOL_INVOKE_HPP_INCLUDED

#include<utility>

namespace veiler{

namespace detail{

namespace fool{

template<typename F, typename T, typename... Args>
constexpr auto invoke(F&& f, T&& t, Args&&... ts)
  noexcept(noexcept((std::forward<T>(t).*std::forward<F>(f))(std::forward<Args>(ts)...)))
         ->decltype((std::forward<T>(t).*std::forward<F>(f))(std::forward<Args>(ts)...)){
             return (std::forward<T>(t).*std::forward<F>(f))(std::forward<Args>(ts)...);
}
template<typename F, typename T, typename... Args>
constexpr auto invoke(F&& f, T&& t, Args&&... ts)
  noexcept(noexcept(((*std::forward<T>(t)).*std::forward<F>(f))(std::forward<Args>(ts)...)))
         ->decltype(((*std::forward<T>(t)).*std::forward<F>(f))(std::forward<Args>(ts)...)){
             return ((*std::forward<T>(t)).*std::forward<F>(f))(std::forward<Args>(ts)...);
}
template<typename F, typename T>
constexpr auto invoke(F&& f, T&& t)
  noexcept(noexcept(std::forward<T>(t).*std::forward<F>(f)))
         ->decltype(std::forward<T>(t).*std::forward<F>(f)){
             return std::forward<T>(t).*std::forward<F>(f);
}
template<typename F, typename T>
constexpr auto invoke(F&& f, T&& t)
  noexcept(noexcept((*std::forward<T>(t)).*std::forward<F>(f)))
         ->decltype((*std::forward<T>(t)).*std::forward<F>(f)){
             return (*std::forward<T>(t)).*std::forward<F>(f);
}
template<typename F, typename... Args>
constexpr auto invoke(F&& f, Args&&... args)
  noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
         ->decltype(std::forward<F>(f)(std::forward<Args>(args)...)){
             return std::forward<F>(f)(std::forward<Args>(args)...);
}

}

}

using detail::fool::invoke;

}

#endif//VEILER_FOOL_INVOKE_HPP_INCLUDED
