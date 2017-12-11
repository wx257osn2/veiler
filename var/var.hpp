//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_VAR_HPP_INCLUDED
#define VEILER_VAR_HPP_INCLUDED

namespace veiler{ 
namespace detail{
    struct varDefaulttag{};
}
template<typename T,typename Tag=detail::varDefaulttag>struct var_helper : public T{
    var_helper(const T& arg) : T(arg){}
};
}
#define VEILER_VAR_FUNC(arg0,...) (veiler::var_helper<decltype(arg0)>(arg0), __VA_ARGS__)
#define VEILER_VAR_FUNC_USE_TAG(tag,arg0,...) (veiler::var_helper<decltype(arg0),tag>(arg0), __VA_ARGS__)
#define VEILER_VAR_IMPL(T,argstype,impl) veiler::var_helper<T>&& operator,(veiler::var_helper<T>&& lhs,const argstype& rhs){impl;return std::move(lhs);}
#define VEILER_VAR_IMPL_USE_TAG(T,tag,argstype,impl) veiler::var_helper<T,tag>&& operator,(veiler::var_helper<T,tag>&& lhs,const argstype& rhs){impl;return std::move(lhs);}
#define VEILER_VAR_IMPL_UNCONST(T,argstype,impl) veiler::var_helper<T>&& operator,(veiler::var_helper<T>&& lhs,argstype& rhs){impl;return std::move(lhs);}
#define VEILER_VAR_IMPL_USE_TAG_UNCONST(T,tag,argstype,impl) veiler::var_helper<T,tag>&& operator,(veiler::var_helper<T,tag>&& lhs,argstype& rhs){impl;return std::move(lhs);}
#define VEILER_VAR_DET_IMPL_START(T,Tag) namespace veiler{template<>struct var_helper<T,Tag>:public T{typedef T type;typedef Tag tag;
#define VEILER_VAR_DET_IMPL_CTOR() var_helper(const type& arg) : type(arg)
#define VEILER_VAR_DET_IMPL_CTOR_UNCONST() var_helper(type& arg) : type(arg)
#define VEILER_VAR_DET_IMPL_IMPL(Type) var_helper<type,tag>& operator,(const Type& arg)
#define VEILER_VAR_DET_IMPL_IMPL_UNCONST(Type) var_helper<type,tag>& operator,(Type& arg)
#define VEILER_VAR_DET_IMPL_END };}

#endif //VEILER_VAR_HPP_INCLUDED
