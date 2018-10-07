//Copyright (C) 2016-2018 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_PEGASUS_PEGASUS_HPP_INCLUDED
#define VEILER_PEGASUS_PEGASUS_HPP_INCLUDED

#include<utility>
#include<type_traits>
#include<variant>
#include<stdexcept>
#include<tuple>
#include<cstddef>
#include<vector>
#include<optional>
#include<any>
#include<unordered_map>
#include<veiler/eikthyrnir/eikthyrnir.hpp>
#include<veiler/temple.hpp>
#include<veiler/prometheus.hpp>
#include<veiler/fool/hash.hpp>
#include<veiler/pelops/ullr.hpp>

namespace veiler{

namespace pegasus{

namespace detail{
template<typename>struct is_variant : std::false_type{};
template<typename... Ts>struct is_variant<std::variant<Ts...>> : std::true_type{};
}

struct unit{
  constexpr unit() = default;
  friend constexpr bool operator==(const unit&, const unit&)noexcept{return true;}
  friend constexpr bool operator!=(const unit&, const unit&)noexcept{return false;}
  friend constexpr bool operator< (const unit&, const unit&)noexcept{return false;}
  friend constexpr bool operator<=(const unit&, const unit&)noexcept{return true;}
  friend constexpr bool operator> (const unit&, const unit&)noexcept{return false;}
  friend constexpr bool operator>=(const unit&, const unit&)noexcept{return true;}
};

template<typename Iterator>
class iterator_range{
  Iterator b;
  Iterator e;
 public:
  using iterator = Iterator;
  template<typename B, typename E>
  constexpr iterator_range(B&& beg, E&& end):b{std::forward<B>(beg)}, e{std::forward<E>(end)}{}
  Iterator begin()const{return b;}
  Iterator end()const{return e;}
};
template<typename B, typename E, typename T = std::common_type_t<std::decay_t<B>, std::decay_t<E>>>
iterator_range<T> make_range(B&& b, E&& e){return iterator_range<T>{std::forward<B>(b), std::forward<E>(e)};}

namespace error_type{
struct unknown{};
template<typename Input>
struct not_match{Input failed_point;};
struct reach_end{};
struct semantic_check_failed{std::any description;};
template<typename Input>
struct cut_failed{std::variant<unknown, not_match<Input>, reach_end, semantic_check_failed> inner_error;};
}

template<typename T>
using parse_error = std::variant<error_type::unknown, error_type::not_match<T>, error_type::reach_end, error_type::semantic_check_failed, error_type::cut_failed<T>>;

template<typename T>
struct parser_not_match_exception : std::invalid_argument{
  parser_not_match_exception(const T& t):invalid_argument("Veiler.Pegasus parse failed : not match"), data{t}{}
  const error_type::not_match<T>& get()const noexcept{return data;}
 private:
  error_type::not_match<T> data;
};
struct parser_reach_end_exception : std::out_of_range{parser_reach_end_exception():out_of_range("Veiler.Pegasus parse failed : reach the end of input"){}};
struct parser_semantic_check_exception : std::runtime_error{
  template<typename T>
  parser_semantic_check_exception(T&& t):runtime_error{"Veiler.Pegasus parse failed : semantic check failed"}, data{std::forward<T>(t)}{}
  const error_type::semantic_check_failed& get()const noexcept{return data;}
 private:
  error_type::semantic_check_failed data;
};
template<typename T>
struct parser_cut_exception : std::invalid_argument{
  parser_cut_exception(const error_type::cut_failed<T>& t):invalid_argument("Veiler.Pegasus parse failed : cut failure"), data{t}{}
  const error_type::cut_failed<T>& get()const noexcept{return data;}
 private:
  error_type::cut_failed<T> data;
};

}

template<typename T>
struct error_traits<pegasus::parse_error<T>>{
  static constexpr bool can_make_error_from_current_exception = true;
  static pegasus::parse_error<T> make_error(const pegasus::error_type::not_match<T>& t)noexcept{return {t};}
  static pegasus::parse_error<T> make_error(const pegasus::error_type::reach_end& t)noexcept{return {t};}
  static pegasus::parse_error<T> make_error(const pegasus::error_type::semantic_check_failed& t)noexcept{return {t};}
  static pegasus::parse_error<T> make_error(const pegasus::error_type::cut_failed<T>& t)noexcept{return {t};}
  static pegasus::parse_error<T> make_error(const pegasus::parse_error<T>& t)noexcept{return t;}
  static pegasus::parse_error<T> make_error_from_current_exception()noexcept try{
    throw;
  }catch(pegasus::parser_not_match_exception<T>& e){
    return e.get();
  }catch(pegasus::parser_reach_end_exception&){
    return pegasus::error_type::reach_end{};
  }catch(pegasus::parser_semantic_check_exception& e){
    return e.get();
  }catch(pegasus::parser_cut_exception<T>& e){
    return e.get();
  }catch(...){
    return pegasus::error_type::unknown{};
  }
  static void rethrow(const pegasus::parse_error<T>& e){
    struct visitor{
      [[noreturn]] void operator()(const pegasus::error_type::not_match<T>& t){throw pegasus::parser_not_match_exception<T>(t.failed_point);}
      [[noreturn]] void operator()(const pegasus::error_type::semantic_check_failed& e){throw pegasus::parser_semantic_check_exception(e.description);}
      [[noreturn]] void operator()(const pegasus::error_type::reach_end&){throw pegasus::parser_reach_end_exception();}
      [[noreturn]] void operator()(const pegasus::error_type::cut_failed<T>& c){throw pegasus::parser_cut_exception<T>(c);}
      [[noreturn]] void operator()(const pegasus::error_type::unknown&){throw std::logic_error("Veiler.Pegasus exception : unknown error");}
    };
    std::visit(visitor{}, e);
  }
};

namespace pegasus{

template<typename T>struct getea_;
template<typename T>
constexpr auto getea(T&& t){return getea_<std::decay_t<T>>::getea(std::forward<T>(t));}

template<typename T>class matcha;

namespace detail{

template<typename T>
struct is_matcha_ : std::false_type{};
template<typename T>
struct is_matcha_<matcha<T>> : std::true_type{};

}

template<typename T>
using is_matcha = detail::is_matcha_<std::decay_t<T>>;

namespace detail{

template<typename T, typename U, typename = std::nullptr_t>struct cache_combine;
template<typename T, typename U>
struct cache_combine<T, U, std::enable_if_t<!std::is_same<T, U>::value && !std::is_same<T, void>::value && !std::is_same<U, void>::value && !is_variant<T>::value && !is_variant<U>::value, std::nullptr_t>>{
  using type = std::variant<T, U>;
};
template<typename T>
struct cache_combine<T, T, std::nullptr_t>{
  using type = T;
};
template<typename T>
struct cache_combine<T, void, std::enable_if_t<!std::is_same<T, void>::value, std::nullptr_t>>{
  using type = T;
};
template<typename T>
struct cache_combine<void, T, std::enable_if_t<!std::is_same<T, void>::value, std::nullptr_t>> : cache_combine<T, void, std::nullptr_t>{};
template<typename T, typename... Us>
struct cache_combine<T, std::variant<Us...>, std::enable_if_t<!is_variant<T>::value && !std::is_same<T, void>::value, std::nullptr_t>>{
  using type = veiler::rebind<veiler::unique_types<veiler::type_tuple<T, Us...>>, std::variant>;
};
template<typename... Ts, typename U>
struct cache_combine<std::variant<Ts...>, U, std::enable_if_t<!is_variant<U>::value && !std::is_same<U, void>::value, std::nullptr_t>> : cache_combine<U, std::variant<Ts...>, std::nullptr_t>{};
template<typename... Ts, typename... Us>
struct cache_combine<std::variant<Ts...>, std::variant<Us...>, std::enable_if_t<!std::is_same<std::variant<Ts...>, std::variant<Us...>>::value, std::nullptr_t>>{
  using type = veiler::rebind<veiler::unique_types<veiler::type_tuple<Ts..., Us...>>, std::variant>;
};

template<typename>struct is_type_tuple : std::false_type{};
template<typename... Ts>struct is_type_tuple<type_tuple<Ts...>> : std::true_type{};
template<typename T, typename U, typename = std::nullptr_t>struct combine_list;
template<typename T>
struct combine_list<T, T, std::nullptr_t>{
  using type = T;
};
template<typename T>
struct combine_list<T, void, std::enable_if_t<!std::is_same<T, void>::value, std::nullptr_t>>{
  using type = T;
};
template<typename T>
struct combine_list<void, T, std::enable_if_t<!std::is_same<T, void>::value, std::nullptr_t>> : combine_list<T, void, std::nullptr_t>{};
template<typename... Ts, typename... Us>
struct combine_list<type_tuple<Ts...>, type_tuple<Us...>, std::enable_if_t<!std::is_same<type_tuple<Ts...>, type_tuple<Us...>>::value, std::nullptr_t>>{
  using type = veiler::unique_types<veiler::type_tuple<Ts..., Us...>>;
};

template<typename, typename>struct packrat_cache_type_sentinel;
template<typename... Ts, typename T>
struct packrat_cache_type_sentinel<veiler::type_tuple<Ts...>, T> : std::disjunction<std::is_same<Ts, T>...>{};

template<bool,typename,typename...>struct cache_id_impl;
template<typename T, typename P, typename Q>
struct cache_id_impl<true, T, P, Q>{using type = void;};
template<typename T, typename P, typename Q>
struct cache_id_impl<false, T, P, Q> : T::template cache_id<P, Q>{};
template<typename T, typename P, typename Q>
using cache_id = typename cache_id_impl<packrat_cache_type_sentinel<Q, T>::value, T, P, Q>::type;

}

namespace detail{
template<typename T, typename U>
struct alter_result{
  using type = std::variant<T, U>;
  template<typename T_>
  static constexpr type make(T_&& t){return std::forward<T_>(t);}
};
template<typename T>
struct alter_result<T, T>{
  using type = T;
  template<typename T_>
  static constexpr type make(T_&& t){return std::forward<T_>(t);}
};
template<typename T>
struct alter_result<T, unit>{
  using type = std::optional<T>;
  template<typename T_>
  static type make(T_&& t){return std::forward<T_>(t);}
  static type make(unit){return std::nullopt;}
};
template<typename T>
struct alter_result<unit, T>:alter_result<T, unit>{};
template<>
struct alter_result<unit, unit>{
  using type = unit;
  static type make(unit){return {};}
};
template<typename... Ts, typename U>
struct alter_result<std::variant<Ts...>, U>{
  using type = veiler::rebind<veiler::unique_types<veiler::type_tuple<Ts..., U>>, std::variant>;
  static constexpr type make(const std::variant<Ts...>& ts){return std::visit([](const auto& v){return type{v};}, ts);}
  static constexpr type make(std::variant<Ts...>&& ts){return std::visit([](auto&& v){return type{std::move(v)};}, ts);}
  template<typename U_>
  static constexpr type make(U_&& u){return std::forward<U_>(u);}
};
template<typename T, typename... Us>
struct alter_result<T, std::variant<Us...>>{
  using type = veiler::rebind<veiler::unique_types<veiler::type_tuple<T, Us...>>, std::variant>;
  template<typename T_>
  static constexpr type make(T_&& t){return std::forward<T_>(t);}
  static constexpr type make(const std::variant<Us...>& us){return std::visit([](const auto& v){return type{v};}, us);}
  static constexpr type make(std::variant<Us...>&& us){return std::visit([](auto&& v){return type{std::move(v)};}, us);}
};
template<typename... Ts, typename... Us>
struct alter_result<std::variant<Ts...>, std::variant<Us...>>{
  using type = veiler::rebind<veiler::unique_types<veiler::type_tuple<Ts..., Us...>>, std::variant>;
  static constexpr type make(const std::variant<Ts...>& ts){return std::visit([](const auto& v){return type{v};}, ts);}
  static constexpr type make(std::variant<Ts...>&& ts){return std::visit([](auto&& v){return type{std::move(v)};}, ts);}
  static constexpr type make(const std::variant<Us...>& us){return std::visit([](const auto& v){return type{v};}, us);}
  static constexpr type make(std::variant<Us...>&& us){return std::visit([](auto&& v){return type{std::move(v)};}, us);}
};
template<typename... Ts>
struct alter_result<std::variant<Ts...>, unit>{
  using type = std::optional<std::variant<Ts...>>;
  static type make(const std::variant<Ts...>& ts){return ts;}
  static type make(std::variant<Ts...>&& ts){return std::move(ts);}
  static type make(unit){return std::nullopt;}
};
template<typename... Us>
struct alter_result<unit, std::variant<Us...>>{
  using type = std::optional<std::variant<Us...>>;
  static type make(unit){return std::nullopt;}
  static type make(const std::variant<Us...>& us){return us;}
  static type make(std::variant<Us...>&& us){return std::move(us);}
};
template<typename T, typename U>
struct alter_result<std::optional<T>, U>{
  using type = std::optional<typename alter_result<T, U>::type>;
  static type make(const std::optional<T>& t){
    if(t)
      return type{*t};
    return std::nullopt;
  }
  static type make(std::optional<T>&& t){
    if(t)
      return type{std::move(*t)};
    return std::nullopt;
  }
  template<typename T_>
  static constexpr type make(T_&& t){return std::forward<T_>(t);}
};
template<typename T, typename U>
struct alter_result<T, std::optional<U>>{
  using type = std::optional<typename alter_result<T, U>::type>;
  template<typename T_>
  static constexpr type make(T_&& t){return std::forward<T_>(t);}
  static type make(const std::optional<U>& u){
    if(u)
      return type{*u};
    return std::nullopt;
  }
  static type make(std::optional<U>&& u){
    if(u)
      return type{std::move(*u)};
    return std::nullopt;
  }
};
template<typename T>
struct alter_result<std::optional<T>, unit>{
  using type = std::optional<T>;
  static type make(const std::optional<T>& t){return t;}
  static type make(std::optional<T>&& t){return std::move(t);}
  static type make(unit){return std::nullopt;}
};
template<typename T>
struct alter_result<unit, std::optional<T>> : alter_result<std::optional<T>, unit>{};
template<typename T, typename... Us>
struct alter_result<std::optional<T>, std::variant<Us...>>{
  using type = std::optional<typename alter_result<T, std::variant<Us...>>::type>;
  static type make(const std::optional<T>& t){
    if(t)
      return type{*t};
    return std::nullopt;
  }
  static type make(std::optional<T>&& t){
    if(t)
      return type{std::move(*t)};
    return std::nullopt;
  }
  static type make(const std::variant<Us...>& us){return std::visit([](const auto& v){return type{v};}, us);}
  static type make(std::variant<Us...>&& us){return std::visit([](auto&& v){return type{std::move(v)};}, us);}
};
template<typename... Ts, typename U>
struct alter_result<std::variant<Ts...>, std::optional<U>>{
  using type = std::optional<typename alter_result<std::variant<Ts...>, U>::type>;
  static type make(const std::variant<Ts...>& ts){return std::visit([](const auto& v){return type{v};}, ts);}
  static type make(std::variant<Ts...>&& ts){return std::visit([](auto&& v){return type{std::move(v)};}, ts);}
  static type make(const std::optional<U>& u){
    if(u)
      return type{*u};
    return std::nullopt;
  }
  static type make(std::optional<U>&& u){
    if(u)
      return type{std::move(*u)};
    return std::nullopt;
  }
};

template<typename Packrats, typename T>
struct packrat_hash_impl{
  template<typename U>struct is_same : std::is_same<T, U>{};
  static constexpr std::size_t packrat_hash = find_unique_type_index<Packrats, is_same>::value;
};

template<typename Backup, typename Omittable, typename Skipper, typename Packrats>
struct adhoc_optimize_flag{
  using has_backup = Backup;
  using is_omittable = Omittable;
  using has_skipper = Skipper;
  template<bool C>using backup = adhoc_optimize_flag<std::integral_constant<bool, C>, Omittable, Skipper, Packrats>;
  template<bool C>using omittable = adhoc_optimize_flag<Backup, std::integral_constant<bool, C>, Skipper, Packrats>;
  template<bool C>using skipper = adhoc_optimize_flag<Backup, Omittable, std::integral_constant<bool, C>, Packrats>;
  template<typename T>static constexpr std::size_t packrat_hash = packrat_hash_impl<Packrats, T>::packrat_hash;
};

}
template<typename T, typename U>
class alter{
  T t;
  U u;
 public:
  template<typename... Ts>
  struct cache{using type = typename detail::cache_combine<typename T::template cache<Ts...>::type, typename U::template cache<Ts...>::type>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = typename detail::combine_list<detail::cache_id<T, P, Q>, detail::cache_id<U, P, Q>>::type;};
  template<typename T_, typename U_>
  constexpr alter(T_&& t, U_&& u):t{std::forward<T_>(t)}, u{std::forward<U_>(u)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::alter_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    if constexpr(!HasBackup){
      const auto backup = v;
      return (*this)(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
    }
    else{
      const auto& backup = std::forward<C>(c).backup();
      using alter_result = typename detail::alter_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>;
      {
        auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
        if(ret){
          if constexpr(IsOmittable){
            if constexpr(std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
              return ret;
            else
              return {};
          }
          else
            if constexpr(std::is_same<std::decay_t<decltype(*ret)>, typename alter_result::type>::value)
              return ret;
            else
              return alter_result::make(std::move(*ret));
        }
        if(std::visit([](auto&& t){return std::is_same<std::decay_t<decltype(t)>, error_type::cut_failed<std::decay_t<V>>>::value;}, ret.error())){
          if constexpr((IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value) || std::is_same<std::decay_t<decltype(*ret)>, typename alter_result::type>::value)
            return ret;
          else
            return veiler::make_unexpected(std::move(ret.error()));
        }
      }
      v = backup;
      auto ret = u(std::forward<S>(s), f, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
      if(ret){
        if constexpr(IsOmittable){
          if constexpr(std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
            return ret;
          else
            return {};
        }
        else
          if constexpr(std::is_same<std::decay_t<decltype(*ret)>, typename alter_result::type>::value)
            return ret;
          else
            return alter_result::make(std::move(*ret));
      }
      else{
        if constexpr((IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value) || std::is_same<std::decay_t<decltype(*ret)>, typename alter_result::type>::value)
          return ret;
        else
          return veiler::make_unexpected(std::move(ret.error()));
      }
    }
  }
};
template<typename T, typename U, std::enable_if_t<is_matcha<T>::value && is_matcha<U>::value, std::nullptr_t> = nullptr>
constexpr auto operator|(T&& t, U&& u){
  return matcha<alter<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>>{alter<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>{veiler::pegasus::getea(std::forward<T>(t)), veiler::pegasus::getea(std::forward<U>(u))}};
}

namespace detail{
template<typename T, typename U>
struct seq_result{
  using type = std::tuple<T, U>;
  template<typename T_, typename U_>
  static constexpr type make(T_&& t, U_&& u){return std::make_tuple(std::forward<T_>(t), std::forward<U_>(u));}
};
template<typename T>
struct seq_result<T, T>{
  using type = std::vector<T>;
  template<typename T_, typename U_>
  static constexpr type make(T_&& t, U_&& u){return std::vector<T>{std::forward<T_>(t), std::forward<U_>(u)};}
};
template<typename T>
struct seq_result<T, std::vector<T>>{
  using type = std::vector<T>;
  template<typename T_, typename U_>
  static constexpr type make(T_&& t, U_&& u){u.insert(u.begin(), std::forward<T_>(t));return std::forward<U_>(u);}
};
template<typename T>
struct seq_result<std::vector<T>, T>{
  using type = std::vector<T>;
  template<typename T_, typename U_>
  static constexpr type make(T_&& t, U_&& u){t.emplace_back(std::forward<U_>(u));return std::forward<T_>(t);}
};
template<typename T>
struct seq_result<std::vector<T>, std::vector<T>>{
  using type = std::vector<T>;
  template<typename T_, typename U_>
  static constexpr type make(T_&& t, U_&& u){t.reserve(t.size()+u.size());t.insert(t.end(), std::make_move_iterator(u.begin()), std::make_move_iterator(u.end()));return std::forward<T_>(t);}
};
template<typename T>
struct seq_result<T, unit>{
  using type = T;
  template<typename T_>
  static constexpr type make(T_&& t, unit){return std::forward<T_>(t);}
};
template<typename U>
struct seq_result<unit, U>{
  using type = U;
  template<typename U_>
  static constexpr type make(unit, U_&& u){return std::forward<U_>(u);}
};
template<>
struct seq_result<unit, unit>{
  using type = unit;
  static constexpr type make(unit, unit){return {};}
};
template<typename... Ts, typename U>
struct seq_result<std::tuple<Ts...>, U>{
  using type = std::tuple<Ts..., U>;
  template<typename U_>
  static constexpr type make(const std::tuple<Ts...>& ts, U_&& u){return std::tuple_cat(ts, std::make_tuple(std::forward<U_>(u)));}
  template<typename U_>
  static constexpr type make(std::tuple<Ts...>&& ts, U_&& u){return std::tuple_cat(std::move(ts), std::make_tuple(std::forward<U_>(u)));}
};
template<typename T, typename... Us>
struct seq_result<T, std::tuple<Us...>>{
  using type = std::tuple<T, Us...>;
  template<typename T_>
  static constexpr type make(T_&& t, const std::tuple<Us...>& us){return std::tuple_cat(std::make_tuple(std::forward<T_>(t)), us);}
  template<typename T_>
  static constexpr type make(T_&& t, std::tuple<Us...>&& us){return std::tuple_cat(std::make_tuple(std::forward<T_>(t)), std::move(us));}
};
template<typename... Ts>
struct seq_result<std::tuple<Ts...>, unit>{
  using type = std::tuple<Ts...>;
  static constexpr type make(const std::tuple<Ts...>& ts, unit){return ts;}
  static constexpr type make(std::tuple<Ts...>&& ts, unit){return std::move(ts);}
};
template<typename... Us>
struct seq_result<unit, std::tuple<Us...>>{
  using type = std::tuple<Us...>;
  static constexpr type make(unit, const std::tuple<Us...>& us){return us;}
  static constexpr type make(unit, std::tuple<Us...>&& us){return std::move(us);}
};
template<typename... Ts, typename... Us>
struct seq_result<std::tuple<Ts...>, std::tuple<Us...>>{
  using type = std::tuple<Ts..., Us...>;
  static constexpr type make(const std::tuple<Ts...>& ts, const std::tuple<Us...>& us){return std::tuple_cat(ts, us);}
  static constexpr type make(std::tuple<Ts...>&& ts, const std::tuple<Us...>& us){return std::tuple_cat(std::move(ts), us);}
  static constexpr type make(const std::tuple<Ts...>& ts, std::tuple<Us...>&& us){return std::tuple_cat(ts, std::move(us));}
  static constexpr type make(std::tuple<Ts...>&& ts, std::tuple<Us...>&& us){return std::tuple_cat(std::move(ts), std::move(us));}
};
}
template<typename T, typename U>
class seq{
  T t;
  U u;
 public:
  template<typename... Ts>
  struct cache{using type = typename detail::cache_combine<typename T::template cache<Ts...>::type, typename U::template cache<Ts...>::type>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = typename detail::combine_list<detail::cache_id<T, P, Q>, detail::cache_id<U, P, Q>>::type;};
  template<typename T_, typename U_>
  constexpr seq(T_&& t, U_&& u):t{std::forward<T_>(t)}, u{std::forward<U_>(u)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::seq_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    if constexpr(!HasBackup){
      const auto backup = v;
      return (*this)(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
    }
    else{
      using seq_result = detail::seq_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>;
      const auto& backup = c.backup();
      auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret){
        if constexpr((IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value) || std::is_same<std::decay_t<decltype(*ret)>, typename seq_result::type>::value)
          return ret;
        else
          return veiler::make_unexpected(std::move(ret.error()));
      }
      auto ret2 = u(std::forward<S>(s), typename optim::template backup<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret2){
        v = backup;
        if constexpr((IsOmittable && std::is_same<std::decay_t<decltype(*ret2)>, unit>::value) || std::is_same<std::decay_t<decltype(*ret2)>, typename seq_result::type>::value)
          return ret2;
        else
          return veiler::make_unexpected(std::move(ret2.error()));
      }
      if constexpr(std::is_same<std::decay_t<decltype(*ret)>, typename seq_result::type>::value && std::is_same<std::decay_t<decltype(*ret2)>, unit>::value)
        return ret;
      else if constexpr(std::is_same<std::decay_t<decltype(*ret2)>, typename seq_result::type>::value && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
        return ret2;
      else if constexpr(not IsOmittable)
        return seq_result::make(std::move(*ret), std::move(*ret2));
      else
        return {};
    }
  }
};
template<typename T, typename U, std::enable_if_t<is_matcha<T>::value && is_matcha<U>::value, std::nullptr_t> = nullptr>
constexpr auto operator>>(T&& t, U&& u){
  return matcha<seq<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>>{seq<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>{veiler::pegasus::getea(std::forward<T>(t)), veiler::pegasus::getea(std::forward<U>(u))}};
}

namespace detail{
template<typename T>
struct list_result{
  using type = std::vector<T>;
  template<typename V, typename T_>
  static constexpr type make(V&& v, T_&& t){v.emplace_back(std::forward<T_>(t));return std::forward<V>(v);}
};
template<>
struct list_result<unit>{
  using type = unit;
  template<typename V, typename T_>
  static constexpr type make(V&&, T_&&){return {};}
};
}
template<typename T, typename U>
class list{
  T t;
  U u;
 public:
  template<typename... Ts>
  struct cache{using type = typename detail::cache_combine<typename T::template cache<Ts...>::type, typename U::template cache<Ts...>::type>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = typename detail::combine_list<detail::cache_id<T, P, Q>, detail::cache_id<U, P, Q>>::type;};
  template<typename T_, typename U_>
  constexpr list(T_&& t, U_&& u):t{std::forward<T_>(t)}, u{std::forward<U_>(u)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    using lr = std::conditional_t<IsOmittable, unit, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>;
    typename lr::type vec [[maybe_unused]];
    {
      auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret){
        if constexpr(IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
          return ret;
        else
          return veiler::make_unexpected(ret.error());
      }
      if constexpr(not IsOmittable)
        vec = std::move(lr::make(std::move(vec), std::move(*ret)));
    }
    auto backup = v;
    while(true){
      if(!u(std::forward<S>(s), typename optim::template omittable<true>::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...)){
        v = std::move(backup);
        if constexpr(IsOmittable)
          return {};
        else
          return vec;
      }
      auto ret = t(std::forward<S>(s), typename optim::template backup<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret){
        v = std::move(backup);
        if constexpr(IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
          return {};
        else
          return vec;
      }
      if constexpr(not IsOmittable)
        vec = std::move(lr::make(std::move(vec), std::move(*ret)));
      backup = v;
    }
  }
};
template<typename T, typename U, std::enable_if_t<is_matcha<T>::value && is_matcha<U>::value>* = nullptr>
constexpr auto operator%(T&& t, U&& u){
  return matcha<list<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>>{list<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>{veiler::pegasus::getea(std::forward<T>(t)), veiler::pegasus::getea(std::forward<U>(u))}};
}

template<typename T, typename U>
class minus{
  T t;
  U u;
 public:
  template<typename... Ts>
  struct cache{using type = typename detail::cache_combine<typename T::template cache<Ts...>::type, typename U::template cache<Ts...>::type>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = typename detail::combine_list<detail::cache_id<T, P, Q>, detail::cache_id<U, P, Q>>::type;};
  template<typename T_, typename U_>
  constexpr minus(T_&& t, U_&& u):t{std::forward<T_>(t)}, u{std::forward<U_>(u)}{}
  template<typename S, typename F, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, F&& f, C&& c, V&& v, Args&&... args)const->decltype(t(s, f, c, v, args...)){
    auto copied_v = v;
    bool u_ret = u(std::forward<S>(s), typename std::decay_t<F>::template omittable<true>{}, std::forward<C>(c), copied_v, std::forward<Args>(args)...);
    if(u_ret)
      return veiler::make_unexpected(error_type::not_match<std::decay_t<V>>{v});
    return t(std::forward<S>(s), typename std::decay_t<F>::template backup<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
  }
};
template<typename T, typename U, std::enable_if_t<is_matcha<T>::value && is_matcha<U>::value, std::nullptr_t> = nullptr>
constexpr auto operator-(T&& t, U&& u){
  return matcha<minus<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>>{minus<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>{veiler::pegasus::getea(std::forward<T>(t)), veiler::pegasus::getea(std::forward<U>(u))}};
}

template<typename T>
class kleene{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_>
  constexpr kleene(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    using lr = std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>;
    typename lr::type vec [[maybe_unused]];
    auto backup = v;
    while(true){
      auto ret = t(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
      if(ret){
        if constexpr(not IsOmittable)
          vec = lr::make(std::move(vec), std::move(*ret));
        backup = v;
        continue;
      }
      if(std::visit([](auto&& t){return std::is_same<std::decay_t<decltype(t)>, error_type::cut_failed<std::decay_t<V>>>::value;}, ret.error())){
        if constexpr(IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
          return ret;
        else
          return veiler::make_unexpected(std::move(ret.error()));
      }
      v = std::move(backup);
      if constexpr(IsOmittable)
        return {};
      else
        return vec;
    }
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value>* = nullptr>
constexpr auto operator*(T&& t){
  return matcha<kleene<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{kleene<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

template<typename T>
class plus_{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_>
  constexpr plus_(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    using lr = std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::list_result<std::decay_t<decltype(*t(s, f, c, v, args...))>>>;
    typename lr::type vec [[maybe_unused]];
    if(auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...)){
      if constexpr(not IsOmittable)
        vec = lr::make(std::move(vec), std::move(*ret));
    }
    else
      if constexpr(IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
        return ret;
      else
        return veiler::make_unexpected(std::move(ret.error()));
    auto backup = v;
    while(true){
      auto ret = t(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
      if(ret){
        if constexpr(not IsOmittable)
          vec = lr::make(std::move(vec), std::move(*ret));
        backup = v;
        continue;
      }
      if(std::visit([](auto&& t){return std::is_same<std::decay_t<decltype(t)>, error_type::cut_failed<std::decay_t<V>>>::value;}, ret.error())){
        if constexpr(IsOmittable && std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
          return ret;
        else
          return veiler::make_unexpected(std::move(ret.error()));
      }
      v = std::move(backup);
      if constexpr(IsOmittable)
          return {};
      else
        return vec;
    }
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value>* = nullptr>
constexpr auto operator+(T&& t){
  return matcha<plus_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{plus_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

template<typename T>
class may{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_>
  constexpr may(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, typename Backup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<Backup, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<IsOmittable, unit, std::optional<std::decay_t<decltype(*t(s, f, c, v, args...))>>>, parse_error<std::decay_t<V>>>{
    if constexpr(IsOmittable){
      t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      return {};
    }
    else{
      auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(ret)
        return {*ret};
      else
        return {std::nullopt};
    }
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
constexpr auto operator-(T&& t){
  return matcha<may<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{may<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

template<typename T>
class expect{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_>
  constexpr expect(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, typename F, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, F&& f, C&& c, V&& v, Args&&... args)const->decltype(t(s, f, c, v, args...)){
    auto copied_v = v;
    return t(std::forward<S>(s), std::forward<F>(f), std::forward<C>(c), copied_v, std::forward<Args>(args)...);
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
constexpr auto operator&(T&& t){
  return matcha<expect<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{expect<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

template<typename T>
class not_{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_>
  constexpr not_(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, typename F, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, F&&, C&& c, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<std::decay_t<F>::is_omittable::value, unit, parse_error<std::decay_t<V>>>, parse_error<std::decay_t<V>>>{
    auto copied_v = v;
    auto ret = t(std::forward<S>(s), typename std::decay_t<F>::template omittable<true>{}, std::forward<C>(c), copied_v, std::forward<Args>(args)...);
    if(ret)
      return veiler::make_unexpected(error_type::not_match<std::decay_t<V>>{std::forward<V>(v)});
    if constexpr(std::decay_t<F>::is_omittable::value)
      return {};
    else
      return ret.error();
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
constexpr auto operator!(T&& t){
  return matcha<not_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{not_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

template<typename T, typename U>
class cut{
  T t;
  U u;
  template<typename V>
  struct visitor{
    error_type::cut_failed<V> operator()(const error_type::unknown& e)const{return error_type::cut_failed<V>{e};}
    error_type::cut_failed<V> operator()(const error_type::not_match<V>& e)const{return error_type::cut_failed<V>{e};}
    error_type::cut_failed<V> operator()(const error_type::reach_end& e)const{return error_type::cut_failed<V>{e};}
    error_type::cut_failed<V> operator()(const error_type::semantic_check_failed& e)const{return error_type::cut_failed<V>{e};}
    error_type::cut_failed<V> operator()(const error_type::cut_failed<V>& e)const{return e;}
  };
 public:
  template<typename... Ts>
  struct cache{using type = typename detail::cache_combine<typename T::template cache<Ts...>::type, typename U::template cache<Ts...>::type>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = typename detail::combine_list<detail::cache_id<T, P, Q>, detail::cache_id<U, P, Q>>::type;};
  template<typename T_, typename U_>
  constexpr cut(T_&& t, U_&& u):t{std::forward<T_>(t)}, u{std::forward<U_>(u)}{}
  template<typename S, bool HasBackup, bool IsOmittable, typename Skip, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<typename std::conditional_t<IsOmittable, veiler::detail::temple::type_wrapper<unit>, detail::seq_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>>::type, parse_error<std::decay_t<V>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, Skip, Packrats>;
    if constexpr(not HasBackup){
      auto backup = v;
      return (*this)(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<Args>(args)...);
    }
    else if constexpr(not IsOmittable){
      const auto& backup = c.backup();
      using seq_result = detail::seq_result<std::decay_t<decltype(*t(s, f, c, v, args...))>, std::decay_t<decltype(*u(s, f, c, v, args...))>>;
      auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret){
        if constexpr(std::is_same<std::decay_t<decltype(*ret)>, typename seq_result::type>::value)
          return ret;
        else
          return veiler::make_unexpected(ret.error());
      }
      c.cache().clear();
      auto ret2 = u(std::forward<S>(s), typename optim::template backup<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret2){
        v = backup;
        return veiler::make_unexpected(std::visit(visitor<std::decay_t<V>>{}, ret2.error()));
      }
      if constexpr(std::is_same<std::decay_t<decltype(*ret)>, typename seq_result::type>::value)
        return ret;
      else if constexpr(std::is_same<std::decay_t<decltype(*ret2)>, typename seq_result::type>::value)
        return ret2;
      else
        return seq_result::make(std::move(*ret), std::move(*ret2));
    }
    else{
      const auto& backup = c.backup();
      if(auto ret = t(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
         !ret){
        if constexpr(std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
          return ret;
        else
          return veiler::make_unexpected(ret.error());
      }
      c.cache().clear();
      auto ret2 = u(std::forward<S>(s), typename optim::template backup<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if(!ret2){
        v = backup;
        return veiler::make_unexpected(std::visit(visitor<std::decay_t<V>>{}, ret2.error()));
      }
      if constexpr(std::is_same<std::decay_t<decltype(*ret2)>, unit>::value)
        return ret2;
      else
        return {};
    }
  }
};
template<typename T, typename U, std::enable_if_t<is_matcha<T>::value && is_matcha<U>::value>* = nullptr>
constexpr auto operator^(T&& t, U&& u){
  return matcha<cut<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>>{cut<decltype(veiler::pegasus::getea(std::forward<T>(t))), decltype(veiler::pegasus::getea(std::forward<U>(u)))>{veiler::pegasus::getea(std::forward<T>(t)), veiler::pegasus::getea(std::forward<U>(u))}};
}

namespace detail{
template<typename T, typename U, std::enable_if_t<std::is_same<T, std::decay_t<U>>::value, std::nullptr_t> = nullptr>
constexpr T packrat_get(U&& u){
  return std::forward<U>(u);
}

template<typename T, typename... Ts>
static constexpr bool contains = std::conjunction<std::is_same<T, Ts>...>::value;

template<typename T, typename... Ts, std::enable_if_t<!std::is_same<T, std::variant<Ts...>>::value && !contains<std::any, Ts...>, std::nullptr_t> = nullptr>
constexpr T packrat_get(const std::variant<Ts...>& u){
  return *std::get_if<T>(&u);
}
template<typename T>T packrat_get(std::any);

template<bool, typename T, typename PackratList, typename Q, typename...>
struct packrat_cache_type_impl;
template<typename T, typename PackratList, typename Q, typename... Ts>
struct packrat_cache_type_impl<true, T, PackratList, Q, Ts...>{
  using type = void;
};
template<typename T, typename PackratList, typename Q, typename... Ts>
struct packrat_cache_type_impl<false, T, PackratList, Q, Ts...>{
  using type = typename detail::cache_combine<std::decay_t<decltype(std::declval<T>()(std::declval<Ts>()...))>, typename T::template cache<add_type<PackratList, T>, Q, Ts...>::type>::type;
};
template<typename T, typename PackratList, typename Q, typename... Ts>
using packrat_cache_type = typename packrat_cache_type_impl<packrat_cache_type_sentinel<PackratList, T>::value, T, PackratList, Q, Ts...>::type;

template<bool,typename,typename,typename,typename>
struct packrat_cache_id_impl;
template<typename T, typename PackratList, typename Q, typename U>
struct packrat_cache_id_impl<true, T, PackratList, Q, U>{
  using type = void;
};
template<typename T, typename PackratList, typename Q, typename U>
struct packrat_cache_id_impl<false, T, PackratList, Q, U>{
  using type = std::conditional_t<std::is_void<detail::cache_id<T, add_type<PackratList, U>, Q>>::value, add_type<PackratList, U>, detail::cache_id<T, add_type<PackratList, U>, Q>>;
};
template<typename T, typename PackratList, typename Q, typename U>
using packrat_cache_id = typename packrat_cache_id_impl<packrat_cache_type_sentinel<PackratList, U>::value, T, PackratList, Q, U>::type;
struct cache;
}
template<typename U, typename T = U>
class packrat_{
  T t;
 public:
  template<typename PackratList, typename Q, typename... Ts>
  struct cache{using type = detail::packrat_cache_type<T, PackratList, Q, Ts...>;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::packrat_cache_id<T, P, Q, U>;};
  template<typename T_>
  packrat_(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, typename F, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, F&& f, C&& c, V&& v, Args&&... args)const->decltype(t(s, f, c, v, args...)){
    constexpr auto type_id = static_cast<std::make_signed_t<std::size_t>>(std::decay_t<F>::template packrat_hash<U>) * (std::decay_t<F>::is_omittable::value || std::is_same<std::decay_t<decltype(*t(s, std::declval<typename std::decay_t<F>::template omittable<false>>(), c, v, args...))>, unit>::value ? -1 : 1);
    const auto i = std::make_pair(v, type_id);
    auto&& cache = c.cache();
    auto fi = cache.find(i);
    if(fi == cache.end()){
      auto ret = t(std::forward<S>(s), typename std::decay_t<F>::template omittable<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      cache.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::make_tuple(v, typename std::decay_t<decltype(cache)>::value_type::second_type::second_type(ret)));
      if constexpr(std::decay_t<F>::is_omittable::value && !std::is_same<unit, std::decay_t<decltype(t(s, f, c, v, args...))>>::value){
        if(ret)
          return unit{};
        else
          return veiler::make_unexpected(ret.error());
      }
      else
        return ret;
    }
    else{
      auto ret = detail::packrat_get<std::decay_t<decltype(t(s, typename std::decay_t<F>::template omittable<false>{}, c, v, args...))>>(fi->second.second);
      if(ret)
        v = fi->second.first;
      if constexpr(std::decay_t<F>::is_omittable::value && !std::is_same<unit, std::decay_t<decltype(t(s, f, c, v, args...))>>::value){
        if(ret)
          return unit{};
        else
          return veiler::make_unexpected(ret.error());
      }
      else
        return ret;
    }
  }
};

template<typename T>
class transient_{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  T get()const{return t;}
  template<typename U>
  constexpr transient_(U&& u):t{std::forward<U>(u)}{}
  template<typename... Args>
  constexpr auto operator()(Args&&... args)const->decltype(t(std::forward<Args>(args)...)){
    return t(std::forward<Args>(args)...);
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
constexpr auto transient(T&& t){
  return matcha<transient_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{transient_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}
namespace detail{
template<typename>struct is_transient : std::false_type{};
template<typename T>struct is_transient<transient_<T>> : std::true_type{};
}
template<typename T, std::enable_if_t<is_matcha<T>::value && !detail::is_transient<std::decay_t<decltype(veiler::pegasus::getea(std::declval<T>()))>>::value, std::nullptr_t> = nullptr>
constexpr auto packrat(T&& t){
  return matcha<packrat_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{packrat_<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}
template<typename T, std::enable_if_t<is_matcha<T>::value && detail::is_transient<std::decay_t<decltype(veiler::pegasus::getea(std::declval<T>()))>>::value, std::nullptr_t> = nullptr>
constexpr auto packrat(T&& t){
  return matcha<decltype(veiler::pegasus::getea(std::forward<T>(t)).get())>{veiler::pegasus::getea(std::forward<T>(t)).get()};
}
template<typename U, typename T, std::enable_if_t<is_matcha<T>::value && !detail::is_transient<std::decay_t<decltype(veiler::pegasus::getea(std::declval<T>()))>>::value, std::nullptr_t> = nullptr>
constexpr auto packrat_with_tag(T&& t){
  return matcha<packrat_<U, decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{packrat_<U, decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}
template<typename, typename T, std::enable_if_t<is_matcha<T>::value && detail::is_transient<std::decay_t<decltype(veiler::pegasus::getea(std::declval<T>()))>>::value, std::nullptr_t> = nullptr>
constexpr auto packrat_with_tag(T&& t){
  return matcha<decltype(veiler::pegasus::getea(std::forward<T>(t)).get())>{veiler::pegasus::getea(std::forward<T>(t)).get()};
}


namespace detail{
template<typename V, typename F, typename R, typename... Args, typename Result = std::decay_t<decltype(std::declval<F>()(std::declval<R>(), std::declval<iterator_range<std::decay_t<V>>>(), std::declval<Args>()...))>, std::enable_if_t<!veiler::is_expected<Result>::value && veiler::is_unexpected<Result>::value, std::nullptr_t> = nullptr>
constexpr auto action_impl(veiler::prometheus<1>, F&& f, R&& r, const iterator_range<std::decay_t<V>>& rng, Args&&... args)->veiler::expected<unit, parse_error<std::decay_t<V>>>{
  return {f(std::forward<R>(r), rng, std::forward<Args>(args)...)};
}
template<typename V, typename F, typename R, typename... Args, typename Result = std::decay_t<decltype(std::declval<F>()(std::declval<R>(), std::declval<iterator_range<std::decay_t<V>>>(), std::declval<Args>()...))>, std::enable_if_t<!veiler::is_expected<Result>::value && !veiler::is_unexpected<Result>::value, std::nullptr_t> = nullptr>
constexpr auto action_impl(veiler::prometheus<1>, F&& f, R&& r, const iterator_range<std::decay_t<V>>& rng, Args&&... args)->veiler::expected<std::decay_t<decltype(std::declval<F>()(std::declval<R>(), std::declval<iterator_range<std::decay_t<V>>>(), std::declval<Args>()...))>, parse_error<std::decay_t<V>>>{
  return {f(std::forward<R>(r), rng, std::forward<Args>(args)...)};
}
}
template<typename T, typename F>
class action{
  T t;
  F f;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_, typename F_>
  constexpr action(T_&& t, F_&& f):t{std::forward<T_>(t)}, f{std::forward<F_>(f)}{}
  template<typename S, bool HasBackup, typename Omittable, typename Skip, typename Packrats, typename C, typename V, typename VE, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, Omittable, Skip, Packrats>, C&& c, V&& v, VE&& e, Args&&... args)const->std::conditional_t<Omittable::value, veiler::expected<unit, parse_error<std::decay_t<V>>>, decltype(detail::action_impl<std::decay_t<V>>(veiler::prometheus_fire<1>(), f, *t(s, detail::adhoc_optimize_flag<std::true_type, std::false_type, Skip, Packrats>{}, c, v, e, args...), iterator_range<std::decay_t<V>>{v, v}, args...))>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, Omittable, Skip, Packrats>;
    if constexpr(not HasBackup){
      auto backup = v;
      return (*this)(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<V>(v), std::forward<VE>(e), std::forward<Args>(args)...);
    }
    else{
      const auto& backup = c.backup();
      auto ret = t(std::forward<S>(s), typename optim::template omittable<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<VE>(e), std::forward<Args>(args)...);
      if(ret){
        [[maybe_unused]] auto ret2 = detail::action_impl<std::decay_t<V>>(veiler::prometheus_fire<1>(), f, std::move(*ret), iterator_range<std::decay_t<V>>{backup, v}, std::forward<Args>(args)...);
        if constexpr(Omittable::value)
          return unit{};
        else
          return ret2;
      }
      return veiler::make_unexpected(std::move(ret.error()));
    }
  }
};

namespace detail{
template<typename T, typename V, typename F, typename E, typename... Args, std::enable_if_t<!veiler::is_expected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value && veiler::is_unexpected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value, std::nullptr_t> = nullptr>
constexpr auto on_error_impl(F&& f, E&& e, Args&&... args)->veiler::expected<T, parse_error<std::decay_t<V>>>{
  return {f(std::forward<E>(e), std::forward<Args>(args)...)};
}
template<typename T, typename V, typename F, typename E, typename... Args, std::enable_if_t<!veiler::is_expected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value && !veiler::is_unexpected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value && std::is_same<T, decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value, std::nullptr_t> = nullptr>
constexpr auto on_error_impl(F&& f, E&& e, Args&&... args)->veiler::expected<std::decay_t<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>, parse_error<std::decay_t<V>>>{
  return {f(std::forward<E>(e), std::forward<Args>(args)...)};
}
template<typename T, typename V, typename F, typename E, typename... Args, std::enable_if_t<!veiler::is_expected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value && !veiler::is_unexpected<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value && !std::is_same<T, decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>::value, std::nullptr_t> = nullptr>
constexpr auto on_error_impl(F&& f, E&& e, Args&&... args)->veiler::expected<std::variant<T, std::decay_t<decltype(std::declval<F>()(std::declval<E>(), std::declval<Args>()...))>>, parse_error<std::decay_t<V>>>{
  return {f(std::forward<E>(e), std::forward<Args>(args)...)};
}
}
template<typename T, typename F>
class on_error_{
  T t;
  F f;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename T_, typename F_>
  constexpr on_error_(T_&& t, F_&& f):t{std::forward<T_>(t)}, f{std::forward<F_>(f)}{}
  template<typename S, typename OF, typename C, typename V, typename VE, typename... Args>
  constexpr auto operator()(S&& s, OF&&, C&& c, V&& v, VE&& e, Args&&... args)const->decltype(detail::on_error_impl<decltype(*t(s, typename std::decay_t<OF>::template omittable<false>{}, c, v, e, args...)), std::decay_t<V>>(f, std::declval<parse_error<std::decay_t<V>>>(), args...)){
    auto ret = t(std::forward<S>(s), typename std::decay_t<OF>::template omittable<false>{}, c, std::forward<V>(v), std::forward<VE>(e), std::forward<Args>(args)...);
    if(ret)
      return std::move(*ret);
    return detail::on_error_impl<decltype(*ret), std::decay_t<V>>(f, std::move(ret.error()), std::forward<Args>(args)...);
  }
};

namespace detail{
template<typename S, typename Se, typename F, typename... Args>
constexpr void skip(S&& skipper, Se&& s, F&&, Args&&... args){
  skipper(std::forward<Se>(s), typename std::decay_t<F>::template omittable<true>::template skipper<false>{}, std::forward<Args>(args)...);
}
}

inline namespace semantic_actions{

struct omit_t{
  constexpr omit_t() = default;
  template<typename... Args>
  constexpr unit operator()(Args&&...)const noexcept{return {};}
  template<typename T>
  class omit{
    T t;
   public:
    template<typename... Ts>
    struct cache{using type = typename T::template cache<Ts...>::type;};
    template<typename P, typename Q>
    struct cache_id{using type = detail::cache_id<T, P, Q>;};
    template<typename T_>
    constexpr omit(T_&& t):t{std::forward<T_>(t)}{}
    template<typename S, typename F, typename C, typename V, typename... Args>
    constexpr auto operator()(S&& s, F&&, C&& c, V&& v, Args&&... args)const->veiler::expected<unit, parse_error<std::decay_t<V>>>{
      auto ret = t(std::forward<S>(s), typename std::decay_t<F>::template omittable<true>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
      if constexpr(std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
        return ret;
      else
        return ret.map([]([[maybe_unused]] auto&&... unused)->unit{return {};});
    }
  };
  template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
  constexpr auto operator[](T&& t)const{
    return matcha<omit<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{omit<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
  }
}static constexpr omit = {};

struct location_t{
  constexpr location_t() = default;
  template<typename I, typename L, typename... Args>
  constexpr L&& operator()(I&&, L&& l, Args&&...)const{return std::forward<L>(l);}
  template<typename T>
  class location{
    T t;
   public:
    template<typename... Ts>
    struct cache{using type = typename T::template cache<Ts...>::type;};
    template<typename P, typename Q>
    struct cache_id{using type = detail::cache_id<T, P, Q>;};
    template<typename T_>
    constexpr location(T_&& t):t{std::forward<T_>(t)}{}
    template<typename S, typename F, typename C, typename V, typename... Args>
    constexpr auto operator()(S&& s, F&&, C&& c, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<std::decay_t<F>::is_omittable::value, unit, iterator_range<std::decay_t<V>>>, parse_error<std::decay_t<V>>>{
      if constexpr(std::decay_t<F>::has_backup::value){
        const auto& backup = std::forward<C>(c).backup();
        auto ret = t(std::forward<S>(s), typename std::decay_t<F>::template omittable<true>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
        if constexpr(std::decay_t<F>::is_omittable::value){
          if constexpr(std::is_same<std::decay_t<decltype(*ret)>, unit>::value)
            return ret;
          else
            return ret.map([]([[maybe_unused]] auto&&... unused)->unit{return {};});
        }
        else
          return ret.map([&]([[maybe_unused]] auto&&... unused)->iterator_range<std::decay_t<V>>{return {backup, v};});
      }
      else{
        const auto copied = v;
        return (*this)(std::forward<S>(s), typename std::decay_t<F>::template backup<true>{}, std::forward<C>(c).backup(copied), std::forward<V>(v), std::forward<Args>(args)...);
      }
    }
  };
}static constexpr location = {};

}

namespace detail{

template<typename, typename>struct skipper_;

class matcha_base{
 public:
  template<typename V>
  struct dummy:std::any{using any::any;explicit operator bool();std::any operator*();parse_error<V> error();};
 public:
  template<typename T, typename V, typename VE, typename... Args>
  static constexpr auto operate(T&& t, V&& v, VE&& ve, Args&&... args){
    struct dummy_magni{
      const std::decay_t<V>& backup()const;
      dummy_magni& backup(const std::decay_t<V>&);
      std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, dummy<std::decay_t<V>>>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>>& cache()const;
    };
    using cache_id = typename std::decay_t<T>::template cache_id<veiler::type_tuple<>, veiler::type_tuple<>>::type;
    using cache_type = typename detail::cache_combine<veiler::expected<unit, parse_error<std::decay_t<V>>>, typename std::decay_t<T>::template cache<veiler::type_tuple<>, veiler::type_tuple<>, std::decay_t<T>, detail::adhoc_optimize_flag<std::false_type, std::false_type, std::false_type, veiler::type_tuple<>>, dummy_magni, std::decay_t<V>, std::decay_t<VE>, std::decay_t<Args>...>::type>::type;
    std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, cache_type>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>> cache;
    struct conf{
      const std::decay_t<V>* b = nullptr;
      constexpr const std::decay_t<V>& backup()const{return *b;}
      constexpr conf& backup(const std::decay_t<V>& bu){b = &bu;return *this;}
      std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, cache_type>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>>* cp;
      auto& cache()const{return *cp;}
    }c{nullptr, &cache};
    return t(t, detail::adhoc_optimize_flag<std::false_type, std::false_type, std::false_type, cache_id>{}, c, std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
  }
  template<typename T, typename S, typename V, typename VE, typename... Args>
  static constexpr auto operate_with_skipper(T&& t, S&& skip, V&& v, VE&& ve, Args&&... args){
    struct dummy_magni{
      const std::decay_t<V>& backup()const{return *static_cast<std::decay_t<V>*>(nullptr);}
      dummy_magni& backup(const std::decay_t<V>&){return *static_cast<dummy_magni*>(nullptr);}
      std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, dummy<std::decay_t<V>>>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>>& cache()const;
    };
    using cache_id = typename std::decay_t<T>::template cache_id<veiler::type_tuple<>, veiler::type_tuple<>>::type;
    using cache_type = typename detail::cache_combine<veiler::expected<unit, parse_error<std::decay_t<V>>>, typename std::decay_t<T>::template cache<veiler::type_tuple<>, veiler::type_tuple<>, std::decay_t<T>, detail::adhoc_optimize_flag<std::false_type, std::false_type, std::true_type, type_tuple<>>, dummy_magni, std::decay_t<V>, std::decay_t<VE>, std::decay_t<Args>...>::type>::type;
    std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, cache_type>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>> cache;
    auto skip_ = veiler::pegasus::getea(skip);
    struct conf{
      const std::decay_t<V>* b = nullptr;
      constexpr const std::decay_t<V>& backup()const{return *b;}
      constexpr conf& backup(const std::decay_t<V>& bu){b = &bu;return *this;}
      std::unordered_map<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>, std::pair<std::decay_t<V>, cache_type>, veiler::hash<std::pair<std::decay_t<V>, std::make_signed_t<std::size_t>>>>* cp;
      auto& cache()const{return *cp;}
      decltype(skip_)* sk;
      auto& skipper()const{return *sk;}
    }c{nullptr, &cache, &skip_};
    detail::skip(skip_, t, detail::adhoc_optimize_flag<std::false_type, std::false_type, std::true_type, cache_id>{}, c, std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
    return t(t, detail::adhoc_optimize_flag<std::false_type, std::false_type, std::true_type, cache_id>{}, c, std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
  }
};

}

template<typename T>
class matcha{
  T t;
 public:
  template<typename U>
  constexpr matcha(U&& u):t{std::forward<U>(u)}{}
  template<typename U>
  constexpr matcha<action<T, std::decay_t<U>>> operator[](U&& u)const{
    return matcha<action<T, std::decay_t<U>>>{action<T, std::decay_t<U>>{t, std::forward<U>(u)}};
  }
  constexpr matcha<semantic_actions::omit_t::omit<T>> operator[](const semantic_actions::omit_t&)const{
    return matcha<semantic_actions::omit_t::omit<T>>{semantic_actions::omit_t::omit<T>{t}};
  }
  constexpr matcha<semantic_actions::location_t::location<T>> operator[](const semantic_actions::location_t&)const{
    return matcha<semantic_actions::location_t::location<T>>{semantic_actions::location_t::location<T>{t}};
  }
  template<typename U>
  constexpr matcha<on_error_<T, std::decay_t<U>>> on_error(U&& u)const{
    return matcha<on_error_<T, std::decay_t<U>>>{on_error_<T, std::decay_t<U>>{t, std::forward<U>(u)}};
  }
  template<typename V, typename VE, typename... Args, std::enable_if_t<static_cast<decltype(static_cast<void>(std::declval<V>() != std::declval<VE>()), static_cast<void>(std::next(std::declval<V>())), static_cast<void>(*std::declval<V>()), true)>(true), std::nullptr_t> = nullptr>
  constexpr auto operator()(V&& v, VE&& ve, Args&&... args)const{
    return detail::matcha_base::operate(t, std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
  }
  template<typename R, typename... Args, std::enable_if_t<std::is_same<decltype(std::begin(std::declval<R>())), decltype(std::end(std::declval<R>()))>::value, std::nullptr_t> = nullptr>
  constexpr auto operator()(R&& r, Args&&... args)const{
    return (*this)(std::begin(r), std::end(r), std::forward<Args>(args)...);
  }
  template<typename S>
  constexpr matcha<detail::skipper_<T, std::decay_t<S>>> with_skipper(S&& skip)const{
    return {t, std::forward<S>(skip)};
  }
  friend getea_<matcha<T>>;
};
template<typename T_>
class matcha<transient_<T_>>{
  using T = transient_<T_>;
  T t;
 public:
  template<typename U>
  constexpr matcha(U&& u):t{std::forward<U>(u)}{}
  template<typename U>
  constexpr matcha<transient_<action<T_, std::decay_t<U>>>> operator[](U&& u)const{
    return matcha<transient_<action<T_, std::decay_t<U>>>>{transient_<action<T_, std::decay_t<U>>>{action<T_, std::decay_t<U>>{t.get(), std::forward<U>(u)}}};
  }
  constexpr matcha<transient_<semantic_actions::omit_t::omit<T_>>> operator[](const semantic_actions::omit_t&)const{
    return matcha<transient_<semantic_actions::omit_t::omit<T_>>>{transient_<semantic_actions::omit_t::omit<T_>>{semantic_actions::omit_t::omit<T_>{t.get()}}};
  }
  constexpr matcha<transient_<semantic_actions::location_t::location<T_>>> operator[](const semantic_actions::location_t&)const{
    return matcha<transient_<semantic_actions::location_t::location<T_>>>{transient_<semantic_actions::location_t::location<T_>>{semantic_actions::location_t::location<T_>{t.get()}}};
  }
  template<typename U>
  constexpr matcha<transient_<on_error_<T_, std::decay_t<U>>>> on_error(U&& u)const{
    return matcha<transient_<on_error_<T_, std::decay_t<U>>>>{transient_<on_error_<T_, std::decay_t<U>>>{on_error_<T_, std::decay_t<U>>{t.get(), std::forward<U>(u)}}};
  }
  template<typename V, typename VE, typename... Args, std::enable_if_t<static_cast<decltype(static_cast<void>(std::declval<V>() != std::declval<VE>()), static_cast<void>(std::next(std::declval<V>())), static_cast<void>(*std::declval<V>()), true)>(true), std::nullptr_t> = nullptr>
  constexpr auto operator()(V&& v, VE&& ve, Args&&... args)const{
    return detail::matcha_base::operate(t.get(), std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
  }
  template<typename R, typename... Args, std::enable_if_t<std::is_same<decltype(std::begin(std::declval<R>())), decltype(std::end(std::declval<R>()))>::value, std::nullptr_t> = nullptr>
  constexpr auto operator()(R&& r, Args&&... args)const{
    return (*this)(std::begin(r), std::end(r), std::forward<Args>(args)...);
  }
  template<typename S>
  constexpr matcha<detail::skipper_<T, S>> with_skipper(S&& skip)const{
    return {t, std::forward<S>(skip)};
  }
  friend getea_<matcha<T>>;
};
template<typename T, typename S>
class matcha<detail::skipper_<T, S>>{
  T t;
  S s;
 public:
  template<typename U, typename P>
  constexpr matcha(U&& u, P&& p):t{std::forward<U>(u)}, s{std::forward<P>(p)}{}
  template<typename U>
  constexpr matcha<detail::skipper_<action<T, std::decay_t<U>>, S>> operator[](U&& u)const{
    return matcha<detail::skipper_<action<T, std::decay_t<U>>, S>>{action<T, std::decay_t<U>>{t, std::forward<U>(u)}, s};
  }
  constexpr matcha<detail::skipper_<semantic_actions::omit_t::omit<T>, S>> operator[](const semantic_actions::omit_t&)const{
    return matcha<detail::skipper_<semantic_actions::omit_t::omit<T>, S>>{semantic_actions::omit_t::omit<T>{t}, s};
  }
  constexpr matcha<detail::skipper_<semantic_actions::location_t::location<T>, S>> operator[](const semantic_actions::location_t&)const{
    return matcha<detail::skipper_<semantic_actions::location_t::location<T>, S>>{semantic_actions::location_t::location<T>{t}, s};
  }
  template<typename U>
  constexpr matcha<detail::skipper_<on_error_<T, std::decay_t<U>>, S>> on_error(U&& u)const{
    return matcha<detail::skipper_<on_error_<T, std::decay_t<U>>, S>>{on_error_<T, std::decay_t<U>>{t, std::forward<U>(u)}, s};
  }
  template<typename V, typename VE, typename... Args, std::enable_if_t<static_cast<decltype(static_cast<void>(std::declval<V>() != std::declval<VE>()), static_cast<void>(std::next(std::declval<V>())), static_cast<void>(*std::declval<V>()), true)>(true), std::nullptr_t> = nullptr>
  constexpr auto operator()(V&& v, VE&& ve, Args&&... args)const{
    return detail::matcha_base::operate_with_skipper(t, s, std::forward<V>(v), std::forward<VE>(ve), std::forward<Args>(args)...);
  }
  template<typename R, typename... Args, std::enable_if_t<std::is_same<decltype(std::begin(std::declval<R>())), decltype(std::end(std::declval<R>()))>::value, std::nullptr_t> = nullptr>
  constexpr auto operator()(R&& r, Args&&... args)const{
    return (*this)(std::begin(r), std::end(r), std::forward<Args>(args)...);
  }
  friend getea_<detail::skipper_<T, S>>;
};

template<typename T>
struct getea_<matcha<T>>{
  static constexpr const T& getea(const matcha<T>& m){return m.t;}
  static constexpr T&& getea(matcha<T>&& m){return std::move(m.t);}
};
template<typename T, typename S>
struct getea_<matcha<detail::skipper_<T, S>>>{
  static constexpr const T& getea(const matcha<detail::skipper_<T, S>>& m){return m.t;}
  static constexpr T&& getea(matcha<detail::skipper_<T, S>>&& m){return std::move(m.t);}
};

template<typename T, typename U>struct member_accessor;
template<typename T>
struct member_accessor<T, T>{
  static constexpr const T&  access(const T& t){return                 t ;}
};
template<typename T, typename U, std::enable_if_t<std::is_reference<decltype(member_accessor<std::decay_t<T>, std::decay_t<U>>::access(std::declval<U>()))>::value>* = nullptr>
constexpr auto&& member_access(U&& u){
  return member_accessor<std::decay_t<T>, std::decay_t<U>>::access(std::forward<U>(u));
}
template<typename T, typename U, std::enable_if_t<!std::is_reference<decltype(member_accessor<std::decay_t<T>, std::decay_t<U>>::access(std::declval<U>()))>::value>* = nullptr>
constexpr auto member_access(U&& u){
  return member_accessor<std::decay_t<T>, std::decay_t<U>>::access(std::forward<U>(u));
}

namespace detail{
template<typename T, typename U, std::enable_if_t<static_cast<decltype(member_access<T>(*std::declval<U>()) != std::declval<T>())>(true), std::nullptr_t> = nullptr>
constexpr bool lit_impl_is_range()noexcept{return false;}
template<typename T, typename U, std::enable_if_t<static_cast<decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()), member_access<std::decay_t<decltype(*std::begin(std::declval<T>()))>>(*std::declval<U>()) != *std::begin(std::declval<T>()))>(true), std::nullptr_t> = nullptr>
constexpr bool lit_impl_is_range()noexcept{return true;}
}
template<typename T>
class lit_{
  T t;
 public:
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  template<typename T_>
  constexpr lit_(T_&& t):t{std::forward<T_>(t)}{}
  template<typename S, bool HasBackup, bool IsOmittable, bool HasSkipper, typename Packrats, typename C, typename U, typename V, typename... Args>
  constexpr auto operator()(S&& s, [[maybe_unused]] detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, std::integral_constant<bool, HasSkipper>, Packrats> f, C&& c, U&& u, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<IsOmittable, unit, std::conditional_t<detail::lit_impl_is_range<T, U>(), iterator_range<std::decay_t<U>>, std::decay_t<U>>>, parse_error<std::decay_t<U>>>{
    using optim = detail::adhoc_optimize_flag<std::integral_constant<bool, HasBackup>, std::integral_constant<bool, IsOmittable>, std::integral_constant<bool, HasSkipper>, Packrats>;
    constexpr bool t_is_range = detail::lit_impl_is_range<T, U>();
    if constexpr(t_is_range){
      if constexpr(HasBackup){
        const auto& backup = c.backup();
        using value_type = std::decay_t<decltype(*std::begin(std::declval<T>()))>;
        for(auto&& x : t){
          if(u == v){
            u = backup;
            return veiler::make_unexpected(error_type::reach_end{});
          }
          if(member_access<value_type>(*std::forward<U>(u)) != x){
            auto copied_u = u;
            u = backup;
            return veiler::make_unexpected(error_type::not_match<std::decay_t<U>>{std::move(copied_u)});
          }
          ++u;
        }
        if constexpr(HasSkipper)
          detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
        if constexpr(IsOmittable)
          return {};
        else
          return make_range(backup, u);
      }
      else{
        const auto backup = u;
        return (*this)(std::forward<S>(s), typename optim::template backup<true>{}, std::forward<C>(c).backup(backup), std::forward<U>(u), std::forward<V>(v));
      }
    }
    else{
      if(u == v)
        return veiler::make_unexpected(error_type::reach_end{});
      if(member_access<T>(*std::forward<U>(u)) != t)
        return veiler::make_unexpected(error_type::not_match<std::decay_t<U>>{std::forward<U>(u)});
      if constexpr(IsOmittable){
        ++u;
        if constexpr(HasSkipper)
          detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
        return {};
      }
      else{
        auto cu = u;
        ++u;
        if constexpr(HasSkipper)
          detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
        return cu;
      }
    }
  }
};
template<typename T, std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value || !std::is_same<typename std::char_traits<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>::char_type, std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>::value, std::nullptr_t> = nullptr>
constexpr matcha<lit_<std::decay_t<T>>> lit(T&& t){return matcha<lit_<std::decay_t<T>>>{lit_<std::decay_t<T>>{std::forward<T>(t)}};}
template<typename T, std::size_t N, std::enable_if_t<!std::is_same<typename std::char_traits<std::decay_t<T>>::char_type, std::decay_t<T>>::value, std::nullptr_t> = nullptr>
constexpr matcha<lit_<std::array<T, N>>> lit(T(&t)[N]){return matcha<lit_<std::array<T, N>>>{lit_<std::array<T, N>>{veiler::initialize_from_tuple<std::array<T, N>>(t)}};}
template<typename T, std::size_t N, std::enable_if_t<std::is_same<typename std::char_traits<std::decay_t<T>>::char_type, std::decay_t<T>>::value, std::nullptr_t> = nullptr>
constexpr matcha<lit_<std::array<T, N-1>>> lit(T(&t)[N]){return matcha<lit_<std::array<T, N-1>>>{lit_<std::array<T, N-1>>{veiler::detail::initialize_from_tuple_impl<std::array<T, N-1>>(t, std::make_index_sequence<N-1>{})}};}

template<typename T>
class range_{
  T f, l;
 public:
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  template<typename T_, typename U_>
  constexpr range_(T_&& f, U_&& l):f{std::forward<T_>(f)}, l{std::forward<U_>(l)}{}
  template<typename S, typename Backup, bool IsOmittable, bool HasSkipper, typename Packrats, typename C, typename U, typename V, typename... Args>
  constexpr auto operator()(S&& s, [[maybe_unused]] detail::adhoc_optimize_flag<Backup, std::integral_constant<bool, IsOmittable>, std::integral_constant<bool, HasSkipper>, Packrats> of, C&& c, U&& u, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<IsOmittable, unit, std::decay_t<U>>, parse_error<std::decay_t<U>>>{
    if(u == v)
      return veiler::make_unexpected(error_type::reach_end{});
    if(!(f <= *u && *u <= l))
      return veiler::make_unexpected(error_type::not_match<std::decay_t<U>>{std::forward<U>(u)});
    if constexpr(IsOmittable){
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), of, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return {};
    }
    else{
      const auto copied = u;
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), of, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return copied;
    }
  }
};
template<typename T, typename U>
constexpr matcha<range_<std::common_type_t<std::decay_t<T>, std::decay_t<U>>>> range(T&& t, U&& u){return matcha<range_<std::common_type_t<std::decay_t<T>, std::decay_t<U>>>>{range_<std::common_type_t<std::decay_t<T>, std::decay_t<U>>>{std::forward<T>(t), std::forward<U>(u)}};}

template<typename F>
class filter_{
  F f;
 public:
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  template<typename F_>
  constexpr filter_(F_&& f):f{std::forward<F_>(f)}{}
  template<typename S, typename Backup, bool IsOmittable, bool HasSkipper, typename Packrats, typename C, typename U, typename V, typename... Args>
  constexpr auto operator()(S&& s, [[maybe_unused]] detail::adhoc_optimize_flag<Backup, std::integral_constant<bool, IsOmittable>, std::integral_constant<bool, HasSkipper>, Packrats> of, C&& c, U&& u, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<IsOmittable, unit, std::decay_t<U>>, parse_error<std::decay_t<U>>>{
    if(u == v)
      return veiler::make_unexpected(error_type::reach_end{});
    auto copied = u;
    if(!f(copied, std::forward<V>(v), std::forward<Args>(args)...))
      return veiler::make_unexpected(error_type::not_match<std::decay_t<U>>{std::forward<U>(u)});
    if constexpr(IsOmittable){
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), of, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return {};
    }
    else{
      copied = u;
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), of, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return copied;
    }
  }
};
template<typename T>
constexpr matcha<filter_<std::decay_t<T>>> filter(T&& t){return matcha<filter_<std::decay_t<T>>>{filter_<std::decay_t<T>>{std::forward<T>(t)}};}

struct read_{
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  constexpr read_() = default;
  template<typename S, typename Backup, bool IsOmittable, bool HasSkipper, typename Packrats, typename C, typename U, typename V, typename... Args>
  constexpr veiler::expected<std::conditional_t<IsOmittable, unit, std::decay_t<U>>, parse_error<std::decay_t<U>>> operator()(S&& s, [[maybe_unused]] detail::adhoc_optimize_flag<Backup, std::integral_constant<bool, IsOmittable>, std::integral_constant<bool, HasSkipper>, Packrats> f, C&& c, U&& u, V&& v, Args&&... args)const{
    if(u == v)
      return veiler::make_unexpected(error_type::reach_end{});
    if constexpr(IsOmittable){
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return {};
    }
    else{
      auto cu = u;
      ++u;
      if constexpr(HasSkipper)
        detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<V>(v), std::forward<Args>(args)...);
      return cu;
    }
  }
};
static constexpr matcha<read_> read = matcha<read_>{read_{}};

struct eps_{
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  constexpr eps_() = default;
  template<typename S, typename Backup, typename Omittable, bool HasSkipper, typename Packrats, typename C, typename U, typename... Args>
  constexpr veiler::expected<unit, parse_error<std::decay_t<U>>> operator()(S&& s, detail::adhoc_optimize_flag<Backup, Omittable, std::integral_constant<bool, HasSkipper>, Packrats> f, C&& c, U&& u, Args&&... args)const noexcept{
    if constexpr(HasSkipper)
      detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<U>(u), std::forward<Args>(args)...);
    return unit{};
  }
};
static constexpr matcha<eps_> eps = matcha<eps_>{eps_{}};

template<typename R>
struct self_{
  template<typename...>
  struct cache{using type = void;};
  template<typename,typename>
  struct cache_id{using type = void;};
  constexpr self_() = default;
  template<typename S, typename Backup, bool IsOmittable, typename Skipper, typename Packrats, typename C, typename V, typename... Args>
  constexpr auto operator()(S&& s, detail::adhoc_optimize_flag<Backup, std::integral_constant<bool, IsOmittable>, Skipper, Packrats> f, C&& c, V&& v, Args&&... args)const->veiler::expected<std::conditional_t<IsOmittable, unit, R>, parse_error<std::decay_t<V>>>{
    return std::forward<S>(s)(std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
  }
};
template<typename T>
static constexpr matcha<self_<T>> self = matcha<self_<T>>{self_<T>{}};

template<typename T>
class frozen_matcha{
  T t;
 public:
  template<typename... Ts>
  struct cache{using type = typename T::template cache<Ts...>::type;};
  template<typename P, typename Q>
  struct cache_id{using type = detail::cache_id<T, P, Q>;};
  template<typename U>
  constexpr frozen_matcha(U&& u):t{std::forward<U>(u)}{}
  template<typename S, typename... Args>
  constexpr auto operator()(S&&, Args&&... args)const{
    return t(t, std::forward<Args>(args)...);
  }
};
template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
constexpr auto freeze(T&& t){
  return matcha<frozen_matcha<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{frozen_matcha<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
}

struct lexeme_t{
  constexpr lexeme_t() = default;
  template<typename T>
  class lexeme{
    T t;
   public:
    template<typename... Ts>
    struct cache{using type = typename T::template cache<Ts...>::type;};
    template<typename P, typename Q>
    struct cache_id{using type = detail::cache_id<T, P, Q>;};
    template<typename T_>
    constexpr lexeme(T_&& t):t{std::forward<T_>(t)}{}
    template<typename S, typename F, typename C, typename V, typename... Args>
    constexpr auto operator()(S&& s, F&& f, C&& c, V&& v, Args&&... args)const->decltype(t(std::forward<S>(s), typename std::decay_t<F>::template skipper<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...)){
      if constexpr(std::decay_t<F>::has_skipper::value){
        auto ret = t(std::forward<S>(s), typename std::decay_t<F>::template skipper<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
        if(ret)
          detail::skip(c.skipper(), std::forward<S>(s), f, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
        return ret;
      }
      else
        return t(std::forward<S>(s), typename std::decay_t<F>::template skipper<false>{}, std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);
    }
  };
  template<typename T, std::enable_if_t<is_matcha<T>::value, std::nullptr_t> = nullptr>
  constexpr auto operator[](T&& t)const{
    return matcha<lexeme<decltype(veiler::pegasus::getea(std::forward<T>(t)))>>{lexeme<decltype(veiler::pegasus::getea(std::forward<T>(t)))>{veiler::pegasus::getea(std::forward<T>(t))}};
  }
}static constexpr lexeme = {};

inline namespace semantic_actions{

struct value_t{
  constexpr value_t() = default;
  template<typename I, typename... Args>
  auto operator()(I&& i, Args&&...)const{return *i;}
};
static constexpr value_t value = {};

template<typename T>
struct change_container_t{
  constexpr change_container_t() = default;
  template<typename C, typename... Args, std::enable_if_t<std::is_same<typename T::value_type, std::decay_t<C>>::value, std::nullptr_t> = nullptr>
  T operator()(C&& c, Args&&...)const{
    return T{std::forward<C>(c)};
  }
  template<typename C, typename... Args, std::enable_if_t<static_cast<decltype(std::begin(std::declval<C>()) == std::end(std::declval<C>()))>(true) && std::is_lvalue_reference<C&&>::value, std::nullptr_t> = nullptr>
  T operator()(C&& c, Args&&...)const{
    return T(std::begin(c), std::end(c));
  }
  template<typename C, typename... Args, std::enable_if_t<static_cast<decltype(std::begin(std::declval<C>()) == std::end(std::declval<C>()))>(true) && std::is_rvalue_reference<C&&>::value, std::nullptr_t> = nullptr>
  T operator()(C&& c, Args&&...)const{
    return T(std::make_move_iterator(std::begin(c)), std::make_move_iterator(std::end(c)));
  }
};
template<typename T>
static constexpr change_container_t<T> change_container = {};

}

template<typename T>
class parsers{
 protected:
  constexpr parsers() = default;
 public:
  using self_type = T;
  static const T& instance(){
    static constexpr T self = {};
    return self;
  }
};
#define VEILER_DETAIL_MATCHA_RULE_IMPL(name, type_, cache_type_, cache_id_, f_, ...) \
  private:\
  struct impl##name{\
    using _implname = impl##name;\
    constexpr impl##name() = default;\
    static auto&& def([[maybe_unused]] const self_type& rules = instance()){\
      static auto p = f_(__VA_ARGS__);\
      return p;\
    }\
    template<typename TopType, typename Q, typename... Ts>\
    struct cache{using _deftype = decltype(veiler::pegasus::getea(_implname::def()));using type = VEILER_PELOPS_ULLR_EXP cache_type_;};\
    template<typename P, typename Q, typename _deftype = decltype(veiler::pegasus::getea(_implname::def()))>\
    struct cache_id{using type = VEILER_PELOPS_ULLR_EXP cache_id_;};\
    template<typename S, typename F, typename C, typename V, typename... Args>\
    constexpr VEILER_PELOPS_ULLR_EXP type_ operator()(S&& s, F&& f, C&& c, V&& v, Args&&... args)const{\
      return veiler::pegasus::getea(def())(std::forward<S>(s), std::forward<F>(f), std::forward<C>(c), std::forward<V>(v), std::forward<Args>(args)...);\
    }\
  };\
  veiler::pegasus::matcha<impl##name> name = veiler::pegasus::matcha<impl##name>{impl##name{}};
#define VEILER_PEGASUS_RULE(name, result_type, ...) VEILER_DETAIL_MATCHA_RULE_IMPL(name, (veiler::expected<std::conditional_t<std::decay_t<F>::is_omittable::value, veiler::pegasus::unit, result_type>, veiler::pegasus::parse_error<std::decay_t<V>>>), (typename std::conditional<veiler::pegasus::detail::packrat_cache_type_sentinel<Q, _implname>::value, veiler::detail::temple::type_wrapper<void>, typename _deftype::template cache<TopType, veiler::add_type<Q, _implname>, Ts...>>::type::type), (veiler::pegasus::detail::cache_id<_deftype, P, veiler::add_type<Q, _implname>>), veiler::pegasus::packrat_with_tag<_implname>, __VA_ARGS__)
#define VEILER_PEGASUS_AUTO_RULE(name, ...) VEILER_DETAIL_MATCHA_RULE_IMPL(name, (auto), (std::conditional_t<veiler::pegasus::detail::packrat_cache_type_sentinel<Q, _implname>::value, void, typename _deftype::template cache<TopType, veiler::add_type<Q, _implname>, Ts...>::type>), (veiler::pegasus::detail::cache_id<_deftype, P, veiler::add_type<Q, _implname>>), veiler::pegasus::packrat_with_tag<_implname>, __VA_ARGS__)
#define VEILER_PEGASUS_INLINE_RULE(name, ...)\
 private:\
  static constexpr auto impl##name = [](){\
    return __VA_ARGS__;\
  }();\
  decltype(impl##name) name = impl##name;

}//End : namespace pegasus

}//End : namespace veiler

#endif//VEILER_PEGASUS_PEGASUS_HPP_INCLUDED
