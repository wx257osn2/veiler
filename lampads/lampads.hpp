#ifndef VEILER_LAMPADS_HPP_INCLUDED
#define VEILER_LAMPADS_HPP_INCLUDED

#include<cstddef>
#include<utility>
#include<stdexcept>
#include<veiler/pelops/ullr.hpp>
#include<veiler/temple.hpp>
#include<veiler/aux_/forward.hpp>

namespace veiler{

namespace _detail{

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


template<typename T>
constexpr T unwrap_refil_or_copy(T t){return t;}
template<typename T>
constexpr T& unwrap_refil_or_copy(veiler::refil<T> t){return t.get();}
template<typename T>
using unwrap_refil_t = decltype((unwrap_refil_or_copy(std::declval<T>())));


template<typename T>
class Val{
  T t;
 public:
  using ret_type = struct:std::true_type{
    template<typename...>using type = T;
  };
  constexpr Val() = default;
  constexpr Val(T&& t):t(veiler::forward<T>(t)){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename... Args>
  constexpr T run(Args&&...)const{return t;}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename... Args>
  constexpr veiler::tuple<T> bind_run(Args&&...)const{return veiler::make_tuple(t);}
};


struct ret_type_dummy:std::false_type{
  template<typename R, typename...>using type = R;
};


template<typename T, typename R = ret_type_dummy
#ifdef __clang__
,long long RecursionCounter = 0ll
#endif
>
class Lampads{
  using ret_type = typename T::ret_type;
  T t;
 public:
  using result_type = R;
  constexpr Lampads() = default;
  constexpr Lampads(const Lampads&) = default;
  constexpr Lampads(Lampads&&) = default;
  template<typename X, typename... Args>
  constexpr Lampads(X&& x, Args&&... args):t(veiler::forward<X>(x), veiler::forward<Args>(args)...){}
  template<typename... Args>
  constexpr R operator()(Args&&... args)const{
    return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t, unwrap_refil_or_copy(veiler::forward<Args>(args))...);
  }
  static constexpr T&& _get(Lampads&& t){return veiler::forward<T>(t.t);}
  static constexpr const T& _get(const Lampads& t){return t.t;}
};
template<typename T
#ifdef __clang__
,long long RecursionCounter
#endif
>
class Lampads<T, ret_type_dummy
#ifdef __clang__
, RecursionCounter
#endif
>{
  using ret_type = typename T::ret_type;
  T t;
 public:
  constexpr Lampads() = default;
  constexpr Lampads(const Lampads&) = default;
  constexpr Lampads(Lampads&&) = default;
  template<typename X, typename... Args>
  constexpr Lampads(X&& x, Args&&... args):t(veiler::forward<X>(x), veiler::forward<Args>(args)...){}
  template<typename... Args>
  constexpr auto operator()(Args&&... args)const
                        ->typename ret_type::template type<void, unwrap_refil_t<Args>...>{
    return t.template run<typename ret_type::template type<void, unwrap_refil_t<Args>...> VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t, unwrap_refil_or_copy(veiler::forward<Args>(args))...);
  }
  static constexpr T&& _get(Lampads&& t){return veiler::forward<T>(t.t);}
  static constexpr const T& _get(const Lampads& t){return t.t;}
};


template<typename T>
constexpr Val<unwrap_refil_t<T>> unwrap_lampads_or_valize(T&& t){
   return Val<unwrap_refil_t<T>>(unwrap_refil_or_copy(veiler::forward<T>(t)));
}
template<typename T, typename R>
constexpr T unwrap_lampads_or_valize(Lampads<T, R>&& t){
  return Lampads<T, R>::_get(veiler::forward<Lampads<T, R>>(t));
}
template<typename T, typename R>
constexpr T unwrap_lampads_or_valize(const Lampads<T, R>& t){
  return Lampads<T, R>::_get(t);
}
template<typename Ret, typename T, typename R>
constexpr Lampads<T, Ret> ret(Lampads<T, R>&& t){return Lampads<T, Ret>{Lampads<T, R>::_get(veiler::forward<Lampads<T, R>>(t))};}
template<typename Ret, typename T, typename R>
constexpr Lampads<T, Ret> ret(const Lampads<T, R>& t){return Lampads<T, Ret>{Lampads<T, R>::_get(t)};}


template<typename T>
using unwrap_lampads_or_valize_t = decltype((unwrap_lampads_or_valize(std::declval<T>())));


template<typename T>
struct is_lampads : std::false_type{};
template<typename T>
struct is_lampads<is_lampads<T>> : std::false_type{};
template<typename T, typename R>
struct is_lampads<Lampads<T, R>> : std::true_type{};


template<long long N, typename = void>struct Placeholder;
template<long long N>
struct Placeholder<N, typename std::enable_if<(N>0)>::type>{
  using ret_type = struct:std::true_type{
    template<typename, typename... Args>
    using type = veiler::type_at<veiler::type_tuple<Args...>, N-1>;
  };
  constexpr Placeholder() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S&, Args&&... args)const
    ->decltype((veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...)))){
         return veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...));
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S&, Args&&... args)const
    ->decltype((veiler::make_tuple(veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...))))){
         return veiler::make_tuple(veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...)));
  }
};
template<long long N>
struct Placeholder<N, typename std::enable_if<(N<=0)>::type>{
 private:
  template<long long... Indices, typename... Args>
  constexpr auto run_impl(veiler::index_tuple<Indices...>, Args&&... args)const
    ->veiler::tuple<veiler::type_at<veiler::type_tuple<Args...>, Indices>...>{
    return veiler::make_tuple<veiler::type_at<veiler::type_tuple<Args...>, Indices>...>(veiler::get<Indices>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...))...);
  }
 public:
  using ret_type = ret_type_dummy;
  constexpr Placeholder() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S&, Args&&...)const{
    //throw std::logic_error("Veiler.Lampads - can't use variadic placeholder except in self and bind.");
    return R{};
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S&, Args&&... args)const
    ->decltype((this->run_impl(veiler::make_index_range<-N, sizeof...(Args)>{}, veiler::forward<Args>(args)...))){
         return this->run_impl(veiler::make_index_range<-N, sizeof...(Args)>{}, veiler::forward<Args>(args)...);
  }
};


namespace udl{

template<long long N, char c, char... Chars>
struct udl_to_placeholder{
  using type = typename std::enable_if<'0'<=c && c<='9', typename udl_to_placeholder<N*10 + c-'0', Chars...>::type>::type;
};
template<long long N>
struct udl_to_placeholder<N,'\0'>{using type = Lampads<Placeholder<N>>;};
template<char... Chars>
constexpr typename udl_to_placeholder<0, Chars...,'\0'>::type operator""_(){
   return typename udl_to_placeholder<0, Chars...,'\0'>::type{};
}

template<long long N, char c, char... Chars>
struct udl_to_variadic_placeholder{
  using type = typename std::enable_if<'0'<=c && c<='9', typename udl_to_variadic_placeholder<N*10 + c-'0', Chars...>::type>::type;
};
template<long long N>
struct udl_to_variadic_placeholder<N,'\0'>{using type = Lampads<Placeholder<-N>>;};
template<char... Chars>
constexpr typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type operator""_tail(){
   return typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type{};
}

}


#define DECL_BIOPE(ope,id) \
  template<typename T, typename U>\
  class id{\
    T t;\
    U u;\
    struct ret_type_t:std::true_type{\
      template<typename R, typename... Args>\
      using type = decltype((std::declval<typename T::ret_type::template type<R, Args...>>() ope std::declval<typename U::ret_type::template type<R, Args...>>()));\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value && U::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args... args)const\
      ->decltype((t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...) \
              ope u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...))){\
           return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...) \
              ope u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...);\
    }\
    template<typename... LTypes, typename... RTypes, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<LTypes...> lt, veiler::tuple<RTypes...> rt, veiler::index_tuple<Indices...>)const\
      ->decltype((veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) ope veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...))){\
           return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) ope veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args... args)const\
      ->decltype((this->bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::forward<decltype((u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{}))){\
           return this->bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::forward<decltype((u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{});\
    }\
    constexpr id() = default;\
    constexpr id(const T&  t, const U&  u):t(t),                     u(u)                    {}\
    constexpr id(const T&  t,       U&& u):t(t),                     u(veiler::forward<U>(u)){}\
    constexpr id(      T&& t, const U&  u):t(veiler::forward<T>(t)), u(u)                    {}\
    constexpr id(      T&& t,       U&& u):t(veiler::forward<T>(t)), u(veiler::forward<U>(u)){}\
  };\
  template<typename T, typename U, typename std::enable_if<is_lampads<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value || is_lampads<typename std::remove_cv<typename std::remove_reference<U>::type>::type>::value>::type* = nullptr>\
  constexpr Lampads<id<unwrap_lampads_or_valize_t<T>, unwrap_lampads_or_valize_t<U>>> operator ope(T&& t, U&& u){\
     return Lampads<id<unwrap_lampads_or_valize_t<T>, unwrap_lampads_or_valize_t<U>>>(unwrap_lampads_or_valize(veiler::forward<T>(t)), unwrap_lampads_or_valize(veiler::forward<U>(u)));\
  }

#define DECL_UNOPE(ope,id)\
  template<typename T>\
  class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename R, typename... Args>\
      using type = decltype((ope std::declval<typename T::ret_type::template type<R, Args...>>()));\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args&&... args)const\
      ->decltype((ope t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))){\
           return ope t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);\
    }\
    template<typename... Types, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<Types...> tu, veiler::index_tuple<Indices...>)const\
      ->decltype((veiler::make_tuple((ope veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)))...))){\
           return veiler::make_tuple((ope veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)))...);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args... args)const\
    ->decltype((bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{}))){\
         return bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{});\
    }\
    constexpr id() = default;\
    constexpr id(T&& t):t(veiler::forward<T>(t)){}\
    constexpr id(const T& t):t(t){}\
  };\
  template<typename T, typename std::enable_if<is_lampads<typename std::remove_cv<typename std::remove_reference<T>::type>::type>{}>::type* = nullptr>\
  constexpr Lampads<id<unwrap_lampads_or_valize_t<T>>> operator ope(T&& t){\
     return Lampads<id<unwrap_lampads_or_valize_t<T>>>(unwrap_lampads_or_valize(veiler::forward<T>(t)));\
  }

#define DECL_PSOPE(ope,id)\
  template<typename T>\
  class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename R, typename... Args>\
      using type = decltype((std::declval<typename T::ret_type::template type<R, Args...>>() ope));\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args&&... args)const\
      ->decltype((t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) ope)){\
           return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) ope;\
    }\
    template<typename... Types, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<Types...> tu, veiler::index_tuple<Indices...>)const\
      ->decltype((veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)) ope)...))){\
           return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)) ope)...);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args... args)const\
    ->decltype((bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{}))){\
         return bind_run_impl(veiler::forward<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)), veiler::make_index_range<0, veiler::tuple_size<decltype((t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)))>{}>{});\
    }\
    constexpr id() = default;\
    constexpr id(T&& t):t(veiler::forward<T>(t)){}\
    constexpr id(const T& t):t(t){}\
  };\
  template<typename T, typename std::enable_if<is_lampads<typename std::remove_cv<typename std::remove_reference<T>::type>::type>{}>::type* = nullptr>\
  constexpr Lampads<id<unwrap_lampads_or_valize_t<T>>> operator ope(T&& t, int){\
     return Lampads<id<unwrap_lampads_or_valize_t<T>>>(unwrap_lampads_or_valize(veiler::forward<T>(t)));\
  }

  DECL_BIOPE(+,Add)
  DECL_BIOPE(-,Sub)
  DECL_BIOPE(*,Mul)
  DECL_BIOPE(/,Div)
  DECL_BIOPE(%,Rem)
  DECL_BIOPE(==,Eq)
  DECL_BIOPE(!=,Ne)
  DECL_BIOPE(<,Lt)
  DECL_BIOPE(>,Gt)
  DECL_BIOPE(<=,Le)
  DECL_BIOPE(>=,Ge)
  DECL_BIOPE(&&,And)
  DECL_BIOPE(||,Or)
  DECL_BIOPE(<<,Lsh)
  DECL_BIOPE(>>,Rsh)
  DECL_BIOPE(&,Band)
  DECL_BIOPE(|,Bor)
  DECL_BIOPE(^,Bxor)
  DECL_BIOPE(+=,Sadd)
  DECL_BIOPE(-=,Ssub)
  DECL_BIOPE(*=,Smul)
  DECL_BIOPE(/=,Sdiv)
  DECL_BIOPE(%=,Srem)
  DECL_BIOPE(<<=,Slsh)
  DECL_BIOPE(>>=,Srsh)
  DECL_BIOPE(&=,Sand)
  DECL_BIOPE(|=,Sor)
  DECL_BIOPE(VEILER_PELOPS_ULLR_COMMA,Comma)
  DECL_UNOPE(+,Pos)
  DECL_UNOPE(-,Neg)
  DECL_UNOPE(++,PreInc)
  DECL_UNOPE(--,PreDec)
  DECL_PSOPE(++,PostInc)
  DECL_PSOPE(--,PostDec)
  DECL_UNOPE(!,Not)
  DECL_UNOPE(~,Bnot)
  DECL_UNOPE(*,Ref)
  DECL_UNOPE(&,Addr)

#undef DECL_BIOPE
#undef DECL_UNOPE
#undef DECL_PSOPE


template<typename, typename, typename = void>
struct common_type;
template<typename T>
struct common_type<T, T>{using type = T;};
template<typename T>
struct common_type<T&&, T&&>{using type = T&&;};
template<typename T>
struct common_type<T, T&&, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type>{using type = T;};
template<typename T>
struct common_type<T*, T*>{using type = T*;};
template<typename T>
struct common_type<T*const, T*const>{using type = T*const;};
template<typename T>
struct common_type<T*volatile,T*volatile>{using type = T*volatile;};
template<typename T>
struct common_type<T*const volatile, T*const volatile>{using type = T*const volatile;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<T, U>::value && !std::is_convertible<U, T>::value>::type>{using type = T;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_convertible<T, U>::value>::type>{using type = U;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<T, U>::value &&  std::is_convertible<U, T>::value && !std::is_same<T, U>::value>::type>{using type = typename std::common_type<T, U>::type;};


template<typename, typename, typename, typename, typename = void>struct _IfImpl_ret_type;
template<typename T, typename U, typename R, typename... Args>
struct _IfImpl_ret_type<T, U, R, type_tuple<Args...>, typename std::enable_if< T::ret_type::value &&  U::ret_type::value>::type>{
  using type = typename common_type<typename T::ret_type::template type<R, Args...>, typename U::ret_type::template type<R, Args...>>::type;
};
template<typename T, typename U, typename R, typename... Args>
struct _IfImpl_ret_type<T, U, R, type_tuple<Args...>, typename std::enable_if< T::ret_type::value && !U::ret_type::value>::type>{
  using type = typename T::ret_type::template type<R, Args...>;
};
template<typename T, typename U, typename R, typename... Args>
struct _IfImpl_ret_type<T, U, R, type_tuple<Args...>, typename std::enable_if<!T::ret_type::value &&  U::ret_type::value>::type>{
  using type = typename U::ret_type::template type<R, Args...>;
};
template<typename T, typename U, typename R, typename... Args>
struct _IfImpl_ret_type<T, U, R, type_tuple<Args...>, typename std::enable_if<!T::ret_type::value && !U::ret_type::value>::type>{
  using type = R;
};

template<typename C, typename T, typename U>class IfImpl{
  C c; T t; U u;
  using ret_type_t = struct:std::true_type{
    template<typename R, typename... Args>
    using type = typename _IfImpl_ret_type<T, U, R, type_tuple<Args...>>::type;
  };
 public:
  using ret_type = typename std::conditional<T::ret_type::value || U::ret_type::value, ret_type_t, ret_type_dummy>::type;
  constexpr IfImpl(C c, T t, U u):c(veiler::forward<C>(c)),t(veiler::forward<T>(t)),u(veiler::forward<U>(u)){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->typename _IfImpl_ret_type<T, U, R, type_tuple<Args...>>::type{
    return c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
          ?t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
          :u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype((c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)
               ?t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
               :u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))){
         return c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)
               ?t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
               :u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);
  }
};
template<typename C, typename T>class Else{
  C&& c; T&& t;
 public:
  constexpr Else(C&& c, T&& t):c(veiler::forward<C>(c)),t(veiler::forward<T>(t)){}
  template<typename U>
  constexpr Lampads<IfImpl<unwrap_lampads_or_valize_t<C>, unwrap_lampads_or_valize_t<T>, unwrap_lampads_or_valize_t<U>>> operator[](U&& u)const{
     return Lampads<IfImpl<unwrap_lampads_or_valize_t<C>, unwrap_lampads_or_valize_t<T>, unwrap_lampads_or_valize_t<U>>>(unwrap_lampads_or_valize(veiler::forward<C>(c)),unwrap_lampads_or_valize(veiler::forward<T>(t)),unwrap_lampads_or_valize(veiler::forward<U>(u)));
  }
};
template<typename C, typename T>struct IfElseBridge{constexpr IfElseBridge(Else<C,T>&& e):else_(veiler::forward<Else<C,T>>(e)){} Else<C,T> else_;};
template<typename C>class If{
  C&& c;
 public:
  constexpr If(C&& cond):c(veiler::forward<C>(cond)){}
  template<typename T>constexpr IfElseBridge<C, T> operator[](T&& t)const{return IfElseBridge<C, T>(Else<C, T>(veiler::forward<C>(c), veiler::forward<T>(t)));}
};
template<typename C>constexpr If<C> if_(C&& c){return If<C>(veiler::forward<C>(c));}


template<typename F, typename... Params>
class Bind{
  F f;
  veiler::tuple<Params...> params;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr auto run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const
    ->decltype((veiler::apply(veiler::forward<F>(f), veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...)))){
         return veiler::apply(veiler::forward<F>(f), veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...));
  }
#ifdef __clang__
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT <  (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S&, Args&&...)const{
         return R{};
  }
#endif
 public:
  using ret_type = struct:std::true_type{
    template<typename, typename... Args>
    using type = typename veiler::func_troy<F>::result_type;
  };
  constexpr Bind(F f, Params... params):f(veiler::forward<F>(f)), params(veiler::forward<Params>(params)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->decltype((this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...))){
         return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype((this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...))){
         return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
};

template<typename... Params>
class Self{
  veiler::tuple<Params...> params;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename Tuple>
  constexpr R run_impl_impl(veiler::index_tuple<Indices...>, const S& s, Tuple&& tpl)const{return s.template run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH( * 0ll)>(s, veiler::get<Indices>(veiler::forward<Tuple>(tpl))...);}
#ifdef __clang__
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT < (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename Tuple>
  constexpr R run_impl_impl(veiler::index_tuple<Indices...>, const S&, Tuple&&)const{return R{};}
#endif
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const{
    return run_impl_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_index_range<0, tuple_size<decltype(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...))>{}>{}, s, veiler::forward<decltype((veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...)))>(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...)));}
 public:
  using ret_type = ret_type_dummy;
  constexpr Self(Params&&... ps):params(veiler::forward<Params>(ps)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S& s, Args&&... args)const{return run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr veiler::tuple<R> bind_run(const S& s, Args&&... args)const{return veiler::make_tuple(veiler::forward<R>(run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)));}
};
template<typename... Params>
constexpr Lampads<Self<unwrap_lampads_or_valize_t<Params>...>> self(Params&&... ps){
  return Self<unwrap_lampads_or_valize_t<Params>...>(unwrap_lampads_or_valize(veiler::forward<Params>(ps))...);
}
template<typename F, typename... Params>
constexpr Lampads<Bind<F, unwrap_lampads_or_valize_t<Params>...>> bind(F&& f, Params&&... ps){
  return Bind<F, unwrap_lampads_or_valize_t<Params>...>(veiler::forward<F>(f), unwrap_lampads_or_valize(veiler::forward<Params>(ps))...);
}



}//End : namespace lampads

}//End : namespace _detail

namespace lampads{

using _detail::lampads::if_;
namespace udl{
using _detail::lampads::udl::operator"" _;
using _detail::lampads::udl::operator"" _tail;
}//End : namespace udl
using _detail::lampads::self;
using _detail::lampads::bind;
using _detail::lampads::ret;

}//End : namespace lampads

}//End : namespace veiler

#undef VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH
#undef VEILER_LAMPADS_RECURSION_COUNTER
#undef VEILER_LAMPADS_RECURSION_COUNTER_DECL

#endif//VEILER_LAMPADS_HPP_INCLUDED

//Copyright (C) 2014 I
//  Distributed under the Veiler Source License 1.0.
