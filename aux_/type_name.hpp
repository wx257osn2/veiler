//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_AUX_TYPE_NAME_HPP_INCLUDED
#define VEILER_AUX_TYPE_NAME_HPP_INCLUDED

#include<string>
#include<type_traits>

namespace veiler{

namespace detail{

namespace type_name{

template<typename T, std::enable_if_t<!std::is_const<T>::value, std::nullptr_t> = nullptr>
inline std::string c(std::string&& str)noexcept{return std::move(str);}

template<typename T, std::enable_if_t<std::is_const<T>::value, std::nullptr_t> = nullptr>
inline std::string c(std::string&& str){return std::move(str) + " const";}

template<typename T, std::enable_if_t<!std::is_volatile<T>::value, std::nullptr_t> = nullptr>
inline std::string v(std::string&& str){return c<T>(std::move(str));}

template<typename T, std::enable_if_t<std::is_volatile<T>::value, std::nullptr_t> = nullptr>
inline std::string v(std::string&& str){return c<std::remove_volatile_t<T>>(std::move(str)) + " volatile";}

template<typename T, std::enable_if_t<!std::is_reference<T>::value, std::nullptr_t> = nullptr>
inline std::string ref(std::string&& str){return v<T>(std::move(str));}

template<typename T, std::enable_if_t<std::is_lvalue_reference<T>::value, std::nullptr_t> = nullptr>
inline std::string ref(std::string&& str){return v<std::remove_reference_t<T>>(std::move(str)) + " &";}

template<typename T, std::enable_if_t<std::is_rvalue_reference<T>::value, std::nullptr_t> = nullptr>
inline std::string ref(std::string&& str){return v<std::remove_reference_t<T>>(std::move(str)) + " &&";}

}//End : namespace type_name

}//End : namespace detail

}//End : namespace veiler

#ifdef _MSC_VER

namespace veiler{

template<typename T>
inline std::string type_name(){
  return detail::type_name::ref<T>(typeid(T).name());
}

}//End : namespace veiler

#else

#include<cxxabi.h>
#include<memory>
#include<cstdlib>
#include<stdexcept>
#include<new>

namespace veiler{

template<typename T>
inline std::string type_name(){
  int status;
  std::unique_ptr<char, decltype(&std::free)> ptr{abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status), &std::free};
  if(!ptr)
    switch(status){
      case -1:
        throw std::bad_alloc();
      case -2:
        throw std::invalid_argument("veiler::type_name : invalid mangled name");
      case -3:
        throw std::invalid_argument("veiler::type_name : invalid argument");
    }
  return detail::type_name::ref<T>(ptr.get());
}

}//End : namespace veiler

#endif//_MSC_VER

#endif//VEILER_AUX_TYPE_NAME_HPP_INCLUDED
