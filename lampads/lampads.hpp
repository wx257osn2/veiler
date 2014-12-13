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
using unwrap_refil_t = decltype(unwrap_refil_or_copy(std::declval<T>()));


template<typename T>
class Val{
  T t;
 public:
  struct ret_type:std::true_type{
    template<typename...>using type = T;
    static constexpr bool depends_on_args = false;
  };
  template<typename... Args>
  constexpr Val(Args&&... args):t(veiler::forward<Args>(args)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename... Args>
  constexpr T run(Args&&...)const{return t;}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename... Args>
  constexpr veiler::tuple<T> bind_run(Args&&...)const{return veiler::make_tuple(t);}
};


struct ret_type_dummy:std::false_type{
  template<typename...>using type = void;
  static constexpr bool depends_on_args = true;
};


template<typename T, typename U>
class Brac{
  T t;
  U u;
  struct ret_type_t:std::true_type{
    template<typename... Args>
    using type = decltype(std::declval<typename T::ret_type::template type<Args...>>()[std::declval<typename U::ret_type::template type<Args...>>()]);
    static constexpr bool depends_on_args = T::ret_type::depends_on_args || U::ret_type::depends_on_args;
  };
 public:
  using ret_type = typename std::conditional<T::ret_type::value && U::ret_type::value, ret_type_t, ret_type_dummy>::type;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->decltype(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) 
              [u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)]){
        return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) 
              [u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)];
  }
  template<typename... LTypes, typename... RTypes, long long... Indices>
  constexpr auto bind_run_impl(veiler::tuple<LTypes...> lt, veiler::tuple<RTypes...> rt, veiler::index_tuple<Indices...>)const
    ->decltype(veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt))[veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt))])...)){
        return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt))[veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt))])...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype(this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{})){
        return this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{});
  }
  constexpr Brac() = default;
  constexpr Brac(const T&  t, const U&  u):t(t),                     u(u)                    {}
  constexpr Brac(const T&  t,       U&& u):t(t),                     u(veiler::forward<U>(u)){}
  constexpr Brac(      T&& t, const U&  u):t(veiler::forward<T>(t)), u(u)                    {}
  constexpr Brac(      T&& t,       U&& u):t(veiler::forward<T>(t)), u(veiler::forward<U>(u)){}
};


template<typename T, typename U>
class Asgn{
  T t;
  U u;
  struct ret_type_t:std::true_type{
    template<typename... Args>
    using type = decltype(std::declval<typename T::ret_type::template type<Args...>>() = std::declval<typename U::ret_type::template type<Args...>>());
    static constexpr bool depends_on_args = T::ret_type::depends_on_args || U::ret_type::depends_on_args;
  };
 public:
  using ret_type = typename std::conditional<T::ret_type::value && U::ret_type::value, ret_type_t, ret_type_dummy>::type;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->decltype(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) 
             = u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)){
        return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) 
             = u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);
  }
  template<typename... LTypes, typename... RTypes, long long... Indices>
  constexpr auto bind_run_impl(veiler::tuple<LTypes...> lt, veiler::tuple<RTypes...> rt, veiler::index_tuple<Indices...>)const
    ->decltype(veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) = veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...)){
        return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) = veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype(this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{})){
        return this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{});
  }
  constexpr Asgn() = default;
  constexpr Asgn(const T&  t, const U&  u):t(t),                     u(u)                    {}
  constexpr Asgn(const T&  t,       U&& u):t(t),                     u(veiler::forward<U>(u)){}
  constexpr Asgn(      T&& t, const U&  u):t(veiler::forward<T>(t)), u(u)                    {}
  constexpr Asgn(      T&& t,       U&& u):t(veiler::forward<T>(t)), u(veiler::forward<U>(u)){}
};


template<typename T>
struct is_lampads : std::false_type{};


template<typename T>
constexpr Val<unwrap_refil_t<T>> unwrap_lampads_or_valize(T&& t){
   return Val<unwrap_refil_t<T>>(unwrap_refil_or_copy(veiler::forward<T>(t)));
}


template<typename T>
using unwrap_lampads_or_valize_t = decltype(unwrap_lampads_or_valize(std::declval<T>()));


#define VEILER_LAMPADS_DECL_LAMPADS(...) {\
  using ret_type = typename T::ret_type;\
  T t;\
 public:\
  constexpr Lampads() = default;\
  constexpr Lampads(const Lampads&) = default;\
  constexpr Lampads(Lampads&&) = default;\
  template<typename X, typename... Args>\
  explicit constexpr Lampads(X&& x, Args&&... args):t(veiler::forward<X>(x), veiler::forward<Args>(args)...){}\
  static constexpr T&& _get(Lampads&& t){return veiler::forward<T>(t.t);}\
  static constexpr const T& _get(const Lampads& t){return t.t;}\
  template<typename U>\
  Lampads<Brac<T, unwrap_lampads_or_valize_t<U>>> operator [](U&& u)&&{\
     return Lampads<Brac<T, unwrap_lampads_or_valize_t<U>>>(veiler::forward<T>(t), unwrap_lampads_or_valize(veiler::forward<U>(u)));\
  }\
  template<typename U>\
  Lampads<Asgn<T, unwrap_lampads_or_valize_t<U>>> operator =(U&& u)&&{\
     return Lampads<Asgn<T, unwrap_lampads_or_valize_t<U>>>(veiler::forward<T>(t), unwrap_lampads_or_valize(veiler::forward<U>(u)));\
  }\
  template<typename U>\
  constexpr Lampads<Brac<T, unwrap_lampads_or_valize_t<U>>> operator [](U&& u)const &{\
     return Lampads<Brac<T, unwrap_lampads_or_valize_t<U>>>(t, unwrap_lampads_or_valize(veiler::forward<U>(u)));\
  }\
  template<typename U>\
  constexpr Lampads<Asgn<T, unwrap_lampads_or_valize_t<U>>> operator =(U&& u)const &{\
     return Lampads<Asgn<T, unwrap_lampads_or_valize_t<U>>>(t, unwrap_lampads_or_valize(veiler::forward<U>(u)));\
  }\
  __VA_ARGS__\
}

template<typename T
#ifdef __clang__
,long long RecursionCounter = 0ll
#endif
,typename = void>class Lampads;
template<typename T
#ifdef __clang__
,long long RecursionCounter
#endif
>
class Lampads<T
#ifdef __clang__
, RecursionCounter
#endif
, typename std::enable_if<T::ret_type::depends_on_args == false>::type
> VEILER_LAMPADS_DECL_LAMPADS(
  using result_type = typename T::ret_type::template type<>;
  template<typename... Args>
  constexpr result_type operator()(Args&&... args)const{
    return veiler::forward<result_type>(t.template run<result_type VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t, unwrap_refil_or_copy(veiler::forward<Args>(args))...));
  }
);
template<typename T
#ifdef __clang__
,long long RecursionCounter
#endif
>
class Lampads<T
#ifdef __clang__
, RecursionCounter
#endif
, typename std::enable_if<T::ret_type::depends_on_args == true>::type
> VEILER_LAMPADS_DECL_LAMPADS(
  template<typename... Args>
  constexpr auto operator()(Args&&... args)const
    ->decltype(t.template run<typename ret_type::template type<unwrap_refil_t<Args>...> VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t, unwrap_refil_or_copy(veiler::forward<Args>(args))...)){
        return t.template run<typename ret_type::template type<unwrap_refil_t<Args>...> VEILER_LAMPADS_RECURSION_COUNTER(, 1ll)>(t, unwrap_refil_or_copy(veiler::forward<Args>(args))...);
  }
);
#undef VEILER_LAMPADS_DECL_LAMPADS


template<typename T>
constexpr T unwrap_lampads_or_valize(Lampads<T>&& t){
  return Lampads<T>::_get(veiler::forward<Lampads<T>>(t));
}
template<typename T>
constexpr T unwrap_lampads_or_valize(const Lampads<T>& t){
  return Lampads<T>::_get(t);
}


template<typename T>
struct is_lampads<Lampads<T>> : std::true_type{};


template<typename T>
constexpr Lampads<Val<unwrap_refil_t<T>>> val(T&& t){
   return Lampads<Val<unwrap_refil_t<T>>>(unwrap_refil_or_copy(veiler::forward<T>(t)));
}

template<typename T, typename... Args, typename std::enable_if<!is_lampads<T>::value>::type* = nullptr>
constexpr Lampads<Val<T>> val(Args&&... args){
   return Lampads<Val<T>>(veiler::forward<Args>(args)...);
}


template<typename RetType, typename T>
class Ret{
  T t;
 public:
  struct ret_type:std::true_type{
    template<typename...>using type = RetType;
    static constexpr bool depends_on_args = false;
  };
  constexpr Ret(T&& t):t(veiler::forward<T>(t)){}
  constexpr Ret(const T& t):t(t){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr RetType run(const S& s, Args&&... args)const{return veiler::forward<RetType>(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...));}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr veiler::tuple<RetType> bind_run(const S& s, Args&&... args)const{return veiler::make_tuple(veiler::forward<RetType>(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)));}
};

template<typename R, typename T>
constexpr Lampads<Ret<R, T>> ret(Lampads<T>&& t){return Lampads<Ret<R, T>>{Lampads<T>::_get(veiler::forward<Lampads<T>>(t))};}
template<typename R, typename T>
constexpr Lampads<Ret<R, T>> ret(const Lampads<T>& t){return Lampads<Ret<R, T>>{Lampads<T>::_get(t)};}


template<long long N>
struct Placeholder{
  static_assert(N > 0, "Veiler.Lampads - Placeholder can't be negative and zero.");
  struct ret_type:std::true_type{
    template<typename... Args>
    using type = veiler::type_at<veiler::type_tuple<Args...>, N-1>;
    static constexpr bool depends_on_args = true;
  };
  constexpr Placeholder() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S&, Args&&... args)const
    ->veiler::type_at<veiler::type_tuple<Args...>, N-1>{
    return veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...));
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args, typename std::enable_if<N-1 < sizeof...(Args)>::type* = nullptr>
  constexpr auto bind_run(const S&, Args&&... args)const
    ->veiler::tuple<veiler::type_at<veiler::type_tuple<Args...>, N-1>>{
    return veiler::make_tuple(veiler::get<N-1>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...)));
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args, typename std::enable_if<(N-1 >= sizeof...(Args))>::type* = nullptr>
  constexpr auto bind_run(const S&, Args&&...)const
    ->veiler::tuple<>{
    return veiler::make_tuple();
  }
};
template<long long N>
struct VariadicPlaceholder{
  static_assert(N >= 0, "Veiler.Lampads - VariadicPlaceholder can't be negative.");
 private:
  template<long long... Indices, typename... Args>
  constexpr auto run_impl(veiler::index_tuple<Indices...>, Args&&... args)const
    ->veiler::tuple<veiler::type_at<veiler::type_tuple<Args...>, Indices>...>{
    return veiler::make_tuple<veiler::type_at<veiler::type_tuple<Args...>, Indices>...>(veiler::get<Indices>(veiler::forward_as_tuple<Args&&...>(veiler::forward<Args>(args)...))...);
  }
 public:
  using ret_type = ret_type_dummy;
  constexpr VariadicPlaceholder() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr R run(const S&, Args&&...)const{
    //throw std::logic_error("Veiler.Lampads - can't use variadic placeholder except in self and bind.");
    return R{};
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args, typename std::enable_if<N < sizeof...(Args)>::type* = nullptr>
  constexpr auto bind_run(const S&, Args&&... args)const
    ->decltype(this->run_impl(veiler::make_index_range<N, sizeof...(Args)>{}, veiler::forward<Args>(args)...)){
        return this->run_impl(veiler::make_index_range<N, sizeof...(Args)>{}, veiler::forward<Args>(args)...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args, typename std::enable_if<(N >= sizeof...(Args))>::type* = nullptr>
  constexpr auto bind_run(const S&, Args&&...)const
    ->veiler::tuple<>{
    return veiler::make_tuple();
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
struct udl_to_variadic_placeholder<N,'\0'>{using type = Lampads<VariadicPlaceholder<N>>;};
template<char... Chars>
constexpr typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type operator""_tail(){
   return typename udl_to_variadic_placeholder<0, Chars...,'\0'>::type{};
}

}


struct NumOfArgs{
  struct ret_type:std::true_type{
    template<typename...>
    using type = std::size_t;
    static constexpr bool depends_on_args = false;
  };
  constexpr NumOfArgs() = default;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr std::size_t run(const S&, Args&&...)const{
    return sizeof...(Args);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr veiler::tuple<std::size_t> bind_run(const S&, Args&&...)const{
    return veiler::make_tuple(sizeof...(Args));
  }
};

namespace{
constexpr Lampads<NumOfArgs> num_of_args{};
}

#define VEILER_LAMPADS_DECL_BIOPE(ope,id) \
  template<typename T, typename U>\
  class id{\
    T t;\
    U u;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>\
      using type = decltype(std::declval<typename T::ret_type::template type<Args...>>() ope std::declval<typename U::ret_type::template type<Args...>>());\
      static constexpr bool depends_on_args = T::ret_type::depends_on_args || U::ret_type::depends_on_args;\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value && U::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args&&... args)const\
      ->decltype(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...) \
             ope u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)){\
          return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...) \
             ope u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...);\
    }\
    template<typename... LTypes, typename... RTypes, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<LTypes...> lt, veiler::tuple<RTypes...> rt, veiler::index_tuple<Indices...>)const\
      ->decltype(veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) ope veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...)){\
          return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<LTypes...>>(lt)) ope veiler::get<Indices>(veiler::forward<veiler::tuple<RTypes...>>(rt)))...);\
    }\
    template<typename... RTypes, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<>, veiler::tuple<RTypes...> rt, veiler::index_tuple<Indices...>)const\
      ->veiler::tuple<RTypes...>{\
      return veiler::forward<veiler::tuple<RTypes...>>(rt);\
    }\
    template<typename... LTypes, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<LTypes...> lt, veiler::tuple<>, veiler::index_tuple<Indices...>)const\
      ->veiler::tuple<LTypes...>{\
      return veiler::forward<veiler::tuple<LTypes...>>(lt);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args&&... args)const\
      ->decltype(this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{})){\
          return this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::forward<decltype(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{});\
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

#define VEILER_LAMPADS_DECL_UNOPE(ope,id)\
  template<typename T>\
  class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>\
      using type = decltype(ope std::declval<typename T::ret_type::template type<Args...>>());\
      static constexpr bool depends_on_args = T::ret_type::depends_on_args;\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args&&... args)const\
      ->decltype(ope t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)){\
          return ope t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);\
    }\
    template<typename... Types, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<Types...> tu, veiler::index_tuple<Indices...>)const\
      ->decltype(veiler::make_tuple((ope veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)))...)){\
          return veiler::make_tuple((ope veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)))...);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args&&... args)const\
    ->decltype(this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{})){\
        return this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{});\
    }\
    constexpr id() = default;\
    constexpr id(T&& t):t(veiler::forward<T>(t)){}\
    constexpr id(const T& t):t(t){}\
  };\
  template<typename T, typename std::enable_if<is_lampads<typename std::remove_cv<typename std::remove_reference<T>::type>::type>{}>::type* = nullptr>\
  constexpr Lampads<id<unwrap_lampads_or_valize_t<T>>> operator ope(T&& t){\
     return Lampads<id<unwrap_lampads_or_valize_t<T>>>(unwrap_lampads_or_valize(veiler::forward<T>(t)));\
  }

#define VEILER_LAMPADS_DECL_PSOPE(ope,id)\
  template<typename T>\
  class id{\
    T t;\
    struct ret_type_t:std::true_type{\
      template<typename... Args>\
      using type = decltype(std::declval<typename T::ret_type::template type<Args...>>() ope);\
      static constexpr bool depends_on_args = T::ret_type::depends_on_args;\
    };\
   public:\
    using ret_type = typename std::conditional<T::ret_type::value, ret_type_t, ret_type_dummy>::type;\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto run(const S& s, Args&&... args)const\
      ->decltype(t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) ope){\
          return t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...) ope;\
    }\
    template<typename... Types, long long... Indices>\
    constexpr auto bind_run_impl(veiler::tuple<Types...> tu, veiler::index_tuple<Indices...>)const\
      ->decltype(veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)) ope)...)){\
          return veiler::make_tuple((veiler::get<Indices>(veiler::forward<veiler::tuple<Types...>>(tu)) ope)...);\
    }\
    template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
    constexpr auto bind_run(const S& s, Args&&... args)const\
    ->decltype(this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{})){\
        return this->bind_run_impl(veiler::forward<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)), veiler::make_index_range<0, veiler::tuple_size<decltype(t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...))>{}>{});\
    }\
    constexpr id() = default;\
    constexpr id(T&& t):t(veiler::forward<T>(t)){}\
    constexpr id(const T& t):t(t){}\
  };\
  template<typename T, typename std::enable_if<is_lampads<typename std::remove_cv<typename std::remove_reference<T>::type>::type>{}>::type* = nullptr>\
  constexpr Lampads<id<unwrap_lampads_or_valize_t<T>>> operator ope(T&& t, int){\
     return Lampads<id<unwrap_lampads_or_valize_t<T>>>(unwrap_lampads_or_valize(veiler::forward<T>(t)));\
  }

  VEILER_LAMPADS_DECL_BIOPE(+,Add)
  VEILER_LAMPADS_DECL_BIOPE(-,Sub)
  VEILER_LAMPADS_DECL_BIOPE(*,Mul)
  VEILER_LAMPADS_DECL_BIOPE(/,Div)
  VEILER_LAMPADS_DECL_BIOPE(%,Rem)
  VEILER_LAMPADS_DECL_BIOPE(==,Eq)
  VEILER_LAMPADS_DECL_BIOPE(!=,Ne)
  VEILER_LAMPADS_DECL_BIOPE(<,Lt)
  VEILER_LAMPADS_DECL_BIOPE(>,Gt)
  VEILER_LAMPADS_DECL_BIOPE(<=,Le)
  VEILER_LAMPADS_DECL_BIOPE(>=,Ge)
  VEILER_LAMPADS_DECL_BIOPE(&&,And)
  VEILER_LAMPADS_DECL_BIOPE(||,Or)
  VEILER_LAMPADS_DECL_BIOPE(<<,Lsh)
  VEILER_LAMPADS_DECL_BIOPE(>>,Rsh)
  VEILER_LAMPADS_DECL_BIOPE(&,Band)
  VEILER_LAMPADS_DECL_BIOPE(|,Bor)
  VEILER_LAMPADS_DECL_BIOPE(^,Bxor)
  VEILER_LAMPADS_DECL_BIOPE(+=,Sadd)
  VEILER_LAMPADS_DECL_BIOPE(-=,Ssub)
  VEILER_LAMPADS_DECL_BIOPE(*=,Smul)
  VEILER_LAMPADS_DECL_BIOPE(/=,Sdiv)
  VEILER_LAMPADS_DECL_BIOPE(%=,Srem)
  VEILER_LAMPADS_DECL_BIOPE(<<=,Slsh)
  VEILER_LAMPADS_DECL_BIOPE(>>=,Srsh)
  VEILER_LAMPADS_DECL_BIOPE(&=,Sand)
  VEILER_LAMPADS_DECL_BIOPE(|=,Sor)
  VEILER_LAMPADS_DECL_BIOPE(^=,Sxor)
  VEILER_LAMPADS_DECL_BIOPE(->*,ArwP)
  VEILER_LAMPADS_DECL_BIOPE(VEILER_PELOPS_ULLR_COMMA,Comma)
  VEILER_LAMPADS_DECL_UNOPE(+,Pos)
  VEILER_LAMPADS_DECL_UNOPE(-,Neg)
  VEILER_LAMPADS_DECL_UNOPE(++,PreInc)
  VEILER_LAMPADS_DECL_UNOPE(--,PreDec)
  VEILER_LAMPADS_DECL_PSOPE(++,PostInc)
  VEILER_LAMPADS_DECL_PSOPE(--,PostDec)
  VEILER_LAMPADS_DECL_UNOPE(!,Not)
  VEILER_LAMPADS_DECL_UNOPE(~,Bnot)
  VEILER_LAMPADS_DECL_UNOPE(*,Ref)
  VEILER_LAMPADS_DECL_UNOPE(&,Addr)

#undef VEILER_LAMPADS_DECL_BIOPE
#undef VEILER_LAMPADS_DECL_UNOPE
#undef VEILER_LAMPADS_DECL_PSOPE


template<typename, typename, typename = void>
struct common_type;
template<typename T>
struct common_type<T, T>{using type = T;};
template<typename T>
struct common_type<T&&, T&&>{using type = T&&;};
template<typename T>
struct common_type<T, T&&, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type>{using type = T;};
template<typename T>
struct common_type<T&&, T, typename std::enable_if<!std::is_lvalue_reference<T>::value>::type>{using type = T;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<T, U>::value && !std::is_convertible<U, T>::value>::type>{using type = T;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_convertible<T, U>::value>::type>{using type = U;};
template<typename T, typename U>
struct common_type<T, U, typename std::enable_if<std::is_convertible<T, U>::value &&  std::is_convertible<U, T>::value && !std::is_same<T, U>::value>::type>{using type = typename std::common_type<T, U>::type;};


template<typename, typename, typename = void>struct _IfImpl_ret_type;
template<typename T, typename U>
struct _IfImpl_ret_type<T, U, typename std::enable_if< T::ret_type::value &&  U::ret_type::value>::type>{
  template<typename... Args>
  using type = typename common_type<typename T::ret_type::template type<Args...>, typename U::ret_type::template type<Args...>>::type;
  static constexpr bool depends_on_args = T::ret_type::depends_on_args || U::ret_type::depends_on_args;
};
template<typename T, typename U>
struct _IfImpl_ret_type<T, U, typename std::enable_if< T::ret_type::value && !U::ret_type::value>::type>{
  template<typename... Args>
  using type = typename T::ret_type::template type<Args...>;
  static constexpr bool depends_on_args = T::ret_type::depends_on_args;
};
template<typename T, typename U>
struct _IfImpl_ret_type<T, U, typename std::enable_if<!T::ret_type::value &&  U::ret_type::value>::type>{
  template<typename... Args>
  using type = typename U::ret_type::template type<Args...>;
  static constexpr bool depends_on_args = U::ret_type::depends_on_args;
};
template<typename T, typename U>
struct _IfImpl_ret_type<T, U, typename std::enable_if<!T::ret_type::value && !U::ret_type::value>::type>:ret_type_dummy{};

template<typename C, typename T, typename U>class IfImpl{
  C c; T t; U u;
  struct ret_type_t:std::true_type{
    template<typename... Args>
    using type = typename _IfImpl_ret_type<T, U>::template type<Args...>;
    static constexpr bool depends_on_args = _IfImpl_ret_type<T, U>::depends_on_args;
  };
 public:
  using ret_type = typename std::conditional<T::ret_type::value || U::ret_type::value, ret_type_t, ret_type_dummy>::type;
  constexpr IfImpl(C c, T t, U u):c(veiler::forward<C>(c)),t(veiler::forward<T>(t)),u(veiler::forward<U>(u)){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->typename _IfImpl_ret_type<T, U>::template type<Args...>{
    return c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
          ?t.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
          :u.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...);}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype(c.template run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,args...)
              ?t.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)
              :u.template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s,veiler::forward<Args>(args)...)){
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


using has_result_type = VEILER_HASTUR_TAG_CREATE(result_type);
template<bool, typename, typename...>class Bind;
template<typename F, typename... Params>
class Bind<true, F, Params...>{
  F f;
  veiler::tuple<Params...> params;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr auto run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const
    ->decltype(veiler::apply(f, veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...))){
        return veiler::apply(f, veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...));
  }
#ifdef __clang__
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT <  (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S&, Args&&...)const{
         return R{};
  }
#endif
 public:
  struct ret_type:std::true_type{
    template<typename...>
    using type = typename veiler::func_troy<F>::result_type;
    static constexpr bool depends_on_args = false;
  };
  constexpr Bind(F&& f, Params&&... params):f(veiler::forward<F>(f)), params(veiler::forward<Params>(params)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)){
        return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)){
        return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
};
template<typename F, typename... Params>
class Bind<false, F, Params...>{
  F f;
  veiler::tuple<Params...> params;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr auto run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const
    ->decltype(veiler::apply(f, veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...))){
        return veiler::apply(f, veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(s, veiler::forward<Args>(args)...)...));
  }
#ifdef __clang__
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT <  (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S&, Args&&...)const{
         return R{};
  }
#endif
 public:
  using ret_type = ret_type_dummy;
  constexpr Bind(F&& f, Params&&... params):f(veiler::forward<F>(f)), params(veiler::forward<Params>(params)...){}
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto run(const S& s, Args&&... args)const
    ->decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)){
        return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>
  constexpr auto bind_run(const S& s, Args&&... args)const
    ->decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)){
        return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);
  }
};

template<typename F, typename... Params>
constexpr Lampads<Bind<veiler::hastur<has_result_type>::type<veiler::func_troy<F>>::value, F, unwrap_lampads_or_valize_t<Params>...>> bind(F&& f, Params&&... ps){
   return Lampads<Bind<veiler::hastur<has_result_type>::type<veiler::func_troy<F>>::value, F, unwrap_lampads_or_valize_t<Params>...>>(veiler::forward<F>(f), unwrap_lampads_or_valize(veiler::forward<Params>(ps))...);
}


template<typename... Params>
class Self{
  veiler::tuple<Params...> params;
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT >= (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename Tuple>
  constexpr R run_impl_impl(veiler::index_tuple<Indices...>, const S& s, Tuple&& tpl)const{return veiler::forward<R>(s.template run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH( * 0ll)>(s, veiler::get<Indices>(veiler::forward<Tuple>(tpl))...));}
#ifdef __clang__
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(, typename std::enable_if<(VEILER_LAMPADS_RECURSION_LIMIT < (RecursionCounter + 2ll) * TemplateDepth)>::type* = nullptr), long long... Indices, typename S, typename Tuple>
  constexpr R run_impl_impl(veiler::index_tuple<Indices...>, const S&, Tuple&&)const{return R{};}
#endif
  template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), long long... Indices, typename S, typename... Args>
  constexpr R run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const{
    return run_impl_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH()>(veiler::make_index_range<0, tuple_size<decltype(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...))>{}>{}, s, veiler::forward<decltype(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...))>(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...)));}
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
   return Lampads<Self<unwrap_lampads_or_valize_t<Params>...>>(unwrap_lampads_or_valize(veiler::forward<Params>(ps))...);
}


#define VEILER_LAMPADS_POLYMORPHIC_MEMBER_ACCESSOR(member) \
  class VEILER_PELOPS_ULLR_CAT(memberAccessor, __LINE__){\
    template<typename... Args>\
    class FunctionAccessor{\
      veiler::tuple<Args...> args;\
      template<typename T, long long... Indices, typename std::enable_if<std::is_rvalue_reference<T&&>::value>::type* = nullptr>\
      static constexpr auto impl(T&& lhs, const FunctionAccessor& rhs, veiler::index_tuple<Indices...>)->decltype(veiler::forward<T>(lhs).member(veiler::get<Indices>(rhs.args)...)){return veiler::forward<T>(lhs).member(veiler::get<Indices>(rhs.args)...);}\
      template<typename T, long long... Indices, typename std::enable_if<std::is_lvalue_reference<typename std::remove_const<T&&>::type>::value>::type* = nullptr>\
      static constexpr auto impl(T&& lhs, const FunctionAccessor& rhs, veiler::index_tuple<Indices...>)->decltype(lhs.member(veiler::get<Indices>(rhs.args)...)){return lhs.member(veiler::get<Indices>(rhs.args)...);}\
     public:\
      template<typename Tuple>\
      constexpr FunctionAccessor(Tuple&& tpl):args(veiler::forward<Tuple>(tpl)){}\
      template<typename T>\
      friend constexpr auto operator->*(T&& lhs, const FunctionAccessor& rhs)->decltype(FunctionAccessor::impl(veiler::forward<T>(lhs), rhs, veiler::make_index_range<0, sizeof...(Args)>{})){return FunctionAccessor::impl(veiler::forward<T>(lhs), rhs, veiler::make_index_range<0, sizeof...(Args)>{});}\
    };\
    template<typename... Args>\
    static FunctionAccessor<Args...> make_func_accessor(veiler::tuple<Args...> args){return FunctionAccessor<Args...>(veiler::forward<veiler::tuple<Args...>>(args));}\
    template<typename... Params>\
    class FunctionBinder{\
      veiler::tuple<Params...> params;\
      template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), long long... Indices, typename S, typename... Args>\
      constexpr auto run_impl(veiler::index_tuple<Indices...>, const S& s, Args&&... args)const\
        ->decltype(make_func_accessor(veiler::forward<decltype(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...))>\
                                                              (veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...)))){\
            return make_func_accessor(veiler::forward<decltype(veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...))>\
                                                              (veiler::tuple_cat(veiler::get<Indices>(params).template bind_run<R VEILER_LAMPADS_RECURSION_COUNTER(+1ll) VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(*0ll)>(s, veiler::forward<Args>(args)...)...)));}\
     public:\
      using ret_type = veiler::_detail::lampads::ret_type_dummy;\
      constexpr FunctionBinder(Params&&... ps):params(veiler::forward<Params>(ps)...){}\
      template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
      constexpr auto run(const S& s, Args&&... args)const\
        ->decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...)){\
            return this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...);\
      }\
      template<typename R VEILER_LAMPADS_RECURSION_COUNTER_DECL(), typename S, typename... Args>\
      constexpr auto bind_run(const S& s, Args&&... args)const\
                  ->veiler::tuple<decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...))>{\
        return veiler::make_tuple<decltype(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...))>(this->run_impl<R VEILER_LAMPADS_RECURSION_COUNTER() VEILER_LAMPADS_RECURSION_TEMPLATE_DEPTH(+1ll)>(veiler::make_indexes<Params...>{}, s, veiler::forward<Args>(args)...));}\
    };\
   public:\
    constexpr VEILER_PELOPS_ULLR_CAT(memberAccessor, __LINE__)() = default;\
    template<typename T, typename std::enable_if<std::is_rvalue_reference<T&&>::value>::type* = nullptr>\
    friend constexpr auto operator->*(T&& lhs, const VEILER_PELOPS_ULLR_CAT(memberAccessor, __LINE__)&)->decltype(veiler::forward<T>(lhs).member){return veiler::forward<T>(lhs).member;}\
    template<typename T, typename std::enable_if<std::is_lvalue_reference<typename std::remove_const<T&&>::type>::value>::type* = nullptr>\
    friend constexpr auto operator->*(T&& lhs, const VEILER_PELOPS_ULLR_CAT(memberAccessor, __LINE__)&)->decltype((lhs.member)){return lhs.member;}\
    template<typename... Params>\
    constexpr veiler::_detail::lampads::Lampads<FunctionBinder<veiler::_detail::lampads::unwrap_lampads_or_valize_t<Params>...>> operator()(Params&&... ps)const{\
       return veiler::_detail::lampads::Lampads<FunctionBinder<veiler::_detail::lampads::unwrap_lampads_or_valize_t<Params>...>>(veiler::_detail::lampads::unwrap_lampads_or_valize(veiler::forward<Params>(ps))...);\
    }\
  }

#define VEILER_LAMPADS_POLYMORPHIC_FUNCTION_ACCESSOR(function) \
  struct VEILER_PELOPS_ULLR_CAT(funcAccessor, __LINE__){\
    constexpr VEILER_PELOPS_ULLR_CAT(funcAccessor, __LINE__)() = default;\
    template<typename... Args>\
    constexpr auto operator()(Args&&... args)const->decltype(function(std::declval<Args>()...)){return function(veiler::forward<Args>(args)...);}\
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
using _detail::lampads::val;
using _detail::lampads::num_of_args;
template<long long N>
using arg_t = _detail::lampads::Lampads<_detail::lampads::Placeholder<N>>;
template<long long N>
using arg_tail_t = _detail::lampads::Lampads<_detail::lampads::VariadicPlaceholder<N>>;

}//End : namespace lampads

}//End : namespace veiler

#endif//VEILER_LAMPADS_HPP_INCLUDED

//Copyright (C) 2014 I
//  Distributed under the Veiler Source License 1.0.
