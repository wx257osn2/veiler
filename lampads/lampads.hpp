#ifndef VEILER_LAMPADS_HPP_INCLUDED
#define VEILER_LAMPADS_HPP_INCLUDED

#include<cstddef>
#include<utility>
#include<veiler/pelops/ullr.hpp>
#include<veiler/temple.hpp>

namespace veiler{

namespace detail{

namespace lampads{


#ifdef __clang__
#define VEILER_LAMPADS_RECURSION_COUNTER_DECL(...) , long long RecursionCounter, long long TemplateDepth __VA_ARGS__
#define VEILER_LAMPADS_RECURSION_COUNTER(...) , RecursionCounter __VA_ARGS__
#define VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(...) , TemplateDepth __VA_ARGS__
#ifndef VEILER_LAMPADS_RECURSION_LIMIT
#define VEILER_LAMPADS_RECURSION_LIMIT 96
#endif
#else
#define VEILER_LAMPADS_RECURSION_COUNTER_DECL(...)
#define VEILER_LAMPADS_RECURSION_COUNTER(...)
#define VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(...)
#endif



struct ret_type_dummy:std::false_type{
  static void eval(...);
};

template<typename T
#ifdef __clang__
,long long RecursionCounter = 0ll
#endif
>
class lampads{
  using ret_type = typename T::ret_type;
 public:
  T t;
  template<typename... Args>
  constexpr lampads(Args&&... args):t(std::forward<Args>(args)...){}
  constexpr lampads(T t):t(std::forward<T>(t)){}
  template<typename... Args>
  constexpr auto operator()(Args&&... args)const
                        ->decltype((ret_type::eval(std::forward<Args>(args)...))){
    return t.template run<decltype((ret_type::eval(std::forward<Args>(args)...)))VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t,std::forward<Args>(args)...);
  }
};

template<long long N, typename = void>struct placeholder;
template<long long N>
struct placeholder<N, typename std::enable_if<(N>0)>::type>{
  using ret_type = struct:std::true_type{
    template<typename... Args>static auto eval(Args&&...)->veiler::type_at<veiler::type_tuple<Args...>,N-1>;
  };
  constexpr placeholder() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S&, Args&&... args)const{
    return veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(std::forward<Args>(args)...));
  }
};



/*
template<long long N>struct placeholder<N, typename std::enable_if<(N<=0)>::type>{
  using ret_type = ret_type_t<ret_type_dummy, false>;
  constexpr placeholder(){}
  template<typename... Args>constexpr auto operator()(Args&&... args)const{return lampads<placeholder<N>>()(std::forward<Args>(args)...);}
  template<typename R, typename S, typename... Args>constexpr R run(S s, Args&&... args)const{return impl(veiler::make_index_range<-N, sizeof...(Args)>{}, std::forward<Args>(args)...);}
  template<typename... Args>auto _ret_type(Args... args)const->type_at<type_tuple<Args...>,N-1>;
 private:
  template<typename R, long long... Indices, typename... Args>constexpr R impl(index_tuple<Indices...>, Args&&... args)const{return veiler::make_tuple(get<Indices>(std::forward<veiler::tuple<Args&&...>>(veiler::forward_as_tuple<Args&&...>(std::forward<Args>(args)...)))...);}
};
*/

namespace udl{
template<std::size_t N, char c, char... Chars>
struct udl_to_placeholder{
  using type = typename std::enable_if<'0'<=c && c<='9', typename udl_to_placeholder<N*10 + c-'0', Chars...>::type>::type;
};
template<std::size_t N>
struct udl_to_placeholder<N,'\0'>{using type = lampads<placeholder<static_cast<long long>(N)>>;};
template<char... Chars>
constexpr typename udl_to_placeholder<0, Chars...,'\0'>::type
operator"" _(){
   return typename udl_to_placeholder<0, Chars...,'\0'>::type();
}

template<std::size_t N, char c, char... Chars>
struct udl_to_variadic_placeholder{
  using type = typename std::enable_if<'0'<=c && c<='9', typename udl_to_variadic_placeholder<N*10 + c-'0', Chars...>::type>::type;
};
template<std::size_t N>
struct udl_to_variadic_placeholder<N,'\0'>{using type = lampads<placeholder<static_cast<long long>(-N)>>;};
template<char... Chars>
constexpr typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type
operator"" _tail(){
   return typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type();
}

}

template<typename T>
class val{
  T t;
 public:
  using ret_type = struct:std::true_type{
    static T eval(...);
  };
  constexpr val() = default;
  constexpr val(T t):t(std::forward<T>(t)){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL()>
  constexpr R run(...)const{return t;}
  constexpr T operator()(...)const{return t;}
};


#define DECL_BIOPE(ope,id) \
  template<typename T, typename U>class id{\
    T t;\
    U u;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>static auto eval(Args&&... args)->decltype(T::ret_type::eval(std::forward<Args>(args)...) ope U::ret_type::eval(std::forward<Args>(args)...));\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value&&U::ret_type::value,\
                       ret_type_t,\
                       ret_type_dummy\
                     >::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr R run(const S& s, Args... args)const{\
      return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...) ope u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...);\
    }\
    constexpr id() = default;\
    constexpr id(T t,U u):t(std::forward<T>(t)),u(std::forward<U>(u)){}\
  };\
  template<typename T, typename U> constexpr lampads<id<T,U>> operator ope(lampads<T> t, lampads<U> u){\
    return lampads<id<T,U>>(std::forward<T>(t.t),std::forward<U>(u.t));\
  }\
  template<typename T, typename U> constexpr lampads<id<T,val<U>>> operator ope(lampads<T> t, U&& u){\
    return lampads<id<T,val<U>>>(std::forward<T>(t.t),std::forward<U>(u));\
  }\
  template<typename T, typename U> constexpr lampads<id<val<T>,U>> operator ope(T&& t, lampads<U> u){\
    return lampads<id<val<T>,U>>(std::forward<T>(t),std::forward<U>(u.t));\
  }

#define DECL_UNOPE(ope,id)\
  template<typename T>class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>static auto eval(Args&&... args)->decltype(ope T::ret_type::eval(std::forward<Args>(args)...));\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value,ret_type_t,ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr R run(const S& s, Args&&... args)const{\
      return ope t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,std::forward<Args>(args)...);\
    }\
    constexpr id() = default;\
    constexpr id(T t):t(std::forward<T>(t)){}\
  };\
  template<typename T> constexpr lampads<id<T>> operator ope(lampads<T> t){\
    return lampads<id<T>>(std::forward<T>(t.t));\
  }\
  template<typename T> constexpr lampads<id<val<T>>> operator ope(T&& t){\
    return lampads<id<val<T>>>(std::forward<T>(t));\
  }

#define DECL_PSOPE(ope,id)\
  template<typename T>class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>static auto eval(Args&&... args)->decltype(T::ret_type::eval(std::forward<Args>(args)...) ope);\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value,ret_type_t,ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr R run(const S& s, Args&&... args)const{\
      return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,std::forward<Args>(args)...) ope;\
    }\
    constexpr id() = default;\
    constexpr id(T t):t(std::forward<T>(t)){}\
  };\
  template<typename T>constexpr lampads<id<T>> operator ope(lampads<T> t,int){\
    return lampads<id<T>>(t.t);\
  }\
  template<typename T>constexpr lampads<id<val<T>>> operator ope(T&& t,int){\
    return lampads<id<val<T>>>(t);\
  }

  DECL_BIOPE(+,Add);
  DECL_BIOPE(-,Sub);
  DECL_BIOPE(*,Mul);
  DECL_BIOPE(/,Div);
  DECL_BIOPE(%,Rem);
  DECL_BIOPE(==,Eq);
  DECL_BIOPE(!=,Ne);
  DECL_BIOPE(<,Lt);
  DECL_BIOPE(>,Gt);
  DECL_BIOPE(<=,Le);
  DECL_BIOPE(>=,Ge);
  DECL_BIOPE(&&,And);
  DECL_BIOPE(||,Or);
  DECL_BIOPE(<<,Lsh);
  DECL_BIOPE(>>,Rsh);
  DECL_BIOPE(&,Band);
  DECL_BIOPE(|,Bor);
  DECL_BIOPE(^,Bxor);
  DECL_BIOPE(+=,Sadd);
  DECL_BIOPE(-=,Ssub);
  DECL_BIOPE(*=,Smul);
  DECL_BIOPE(/=,Sdiv);
  DECL_BIOPE(%=,Srem);
  DECL_BIOPE(<<=,Slsh);
  DECL_BIOPE(>>=,Srsh);
  DECL_BIOPE(&=,Sand);
  DECL_BIOPE(|=,Sor);
  DECL_BIOPE(VEILER_PELOPS_ULLR_COMMA,Comma);
  DECL_UNOPE(+,Pos);
  DECL_UNOPE(-,Neg);
  DECL_UNOPE(++,PreInc);
  DECL_UNOPE(--,PreDec);
  DECL_PSOPE(++,PostInc);
  DECL_PSOPE(--,PostDec);
  DECL_UNOPE(!,Not);
  DECL_UNOPE(~,Bnot);
  DECL_UNOPE(*,Ref);
  DECL_UNOPE(&,Addr);

#undef DECL_BIOPE
#undef DECL_UNOPE
#undef DECL_PSOPE

template<typename C, typename T, typename U>class IfImpl{
  C c; T t; U u;
 public:
  using ret_type = typename std::conditional<T::ret_type::value,typename T::ret_type,typename U::ret_type>::type;
  constexpr IfImpl(C c, T t, U u):c(std::forward<C>(c)),t(std::forward<T>(t)),u(std::forward<U>(u)){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S& s, Args&&... args)const{return c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)?t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,std::forward<Args>(args)...)
                                                                                                                                                                       :u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,std::forward<Args>(args)...);}
};
template<typename C, typename T>class Else{
  C c; T t;
 public:
  constexpr Else(C c, T t):c(std::forward<C>(c)),t(std::forward<T>(t)){}
  template<typename U>constexpr auto operator[](U&& u)const->lampads<IfImpl<C,T,val<U>>>{return lampads<IfImpl<C,T,val<U>>>(c,t,std::forward<U>(u));}
  template<typename U>constexpr auto operator[](lampads<U> u)const->lampads<IfImpl<C,T,U>>{return lampads<IfImpl<C,T,U>>(c,t,std::forward<U>(u.t));}
};
template<typename C, typename T>struct IfElseBridge{constexpr IfElseBridge(Else<C,T>&& e):else_(std::forward<Else<C,T>>(e)){} Else<C,T> else_;};
template<typename C>class If{
  C c;
 public:
  constexpr If(C cond):c(std::forward<C>(cond)){}
  template<typename T>constexpr auto operator[](T&& t)const->IfElseBridge<C,val<T>>{return IfElseBridge<C,val<T>>(Else<C,val<T>>(c,val<T>(std::forward<T>(t))));}
  template<typename T>constexpr auto operator[](lampads<T> t)const->IfElseBridge<C,T>{return IfElseBridge<C,T>(Else<C,T>(c,std::forward<T>(t.t)));}
};
template<typename C>constexpr auto if_(C&& c)->If<val<C>>{return If<val<C>>(std::forward<C>(c));}
template<typename C>constexpr auto if_(lampads<C> c)->If<C>{return If<C>(std::forward<C>(c.t));}

template<typename... Params>
class Self{
  veiler::tuple<Params...> params;
  using indexes = veiler::make_indexes<Params...>;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= RecursionCounter * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const{return s.template run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH( * 0)>(s, veiler::get<Indices>(params)(std::forward<Args>(args)...)...);}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT < RecursionCounter * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const{return R{};}
 public:
  using ret_type = ret_type_dummy;
  constexpr Self(Params... ps):params(std::forward<Params>(ps)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S& s, Args&&... args)const{return run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(indexes{}, s, std::forward<Args>(args)...);}
};
template<typename P>constexpr lampads<val<P>> self_impl(P&& p){return lampads<val<P>>(std::forward<P>(p));}
template<typename P>constexpr lampads<P> self_impl(lampads<P>&& p){return std::forward<lampads<P>>(p);}
template<typename... Params>constexpr auto self(Params&&... ps)->lampads<Self<decltype(self_impl(std::forward<Params>(ps)))...>>{return Self<decltype(self_impl(std::forward<Params>(ps)))...>(self_impl(std::forward<Params>(ps))...);}

}//End : namespace lampads

}//End : namespace detail

using detail::lampads::if_;
namespace udl{
using detail::lampads::udl::operator"" _;
}
using detail::lampads::self;

}//End : namespace veiler

#undef VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH
#undef VEILER_LAMPADS_RECURSION_COUNTER
#undef VEILER_LAMPADS_RECURSION_COUNTER_DECL

#endif//VEILER_LAMPADS_HPP_INCLUDED

//Copyright (C) 2014 I
//  Distributed under the Veiler Source License 1.0.
