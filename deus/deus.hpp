//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include<utility>
#include<tuple>
#include<memory>
#include<veiler/temple.hpp>
#include<veiler/aux_/forward.hpp>
#include<veiler/aux_/move.hpp>

namespace veiler{

namespace deus{

namespace impl{

template<typename T, typename Tuple, std::size_t... Indices>
T create_instance_impl(Tuple&& t, index_tuple<Indices...>){return T(veiler::get<Indices>(t)...);}

template<typename T, typename... Args>
T create_instance(veiler::tuple<Args...>&& t){return create_instance_impl<T>(std::forward<veiler::tuple<Args...>>(t), veiler::make_indexes<Args...>{});}

template<typename T, typename... Args>
T create_instance(const veiler::tuple<Args...>& t){return create_instance_impl<T>(t, veiler::make_indexes<Args...>{});}

struct none{
  template<typename... Args>
  constexpr none(Args&&...)noexcept{}
};
struct default_guard{
  template<typename T>
  bool operator()(const T&)const{return true;}
};
struct default_action{
  template<typename T>
  void operator()(const T&)const{}
};
template<typename>class event;
template<typename>class state;

struct guard_tag{};
struct action_tag{};
struct both_tag{};

template<typename From, typename Event, typename To, typename Guard, typename Action>
struct transition : Guard, Action{
  explicit constexpr transition()noexcept{static_assert(std::is_same<Guard, default_guard>::value && std::is_same<Action, default_action>::value, "");}
  template<typename G>
  constexpr transition(guard_tag&&, G&& g):Guard(std::forward<G>(g)){}
  template<typename A>
  constexpr transition(action_tag&&, A&& a):Action(std::forward<A>(a)){}
  template<typename G, typename A>
  constexpr transition(both_tag&&, G&& g, A&& a):Guard(std::forward<G>(g)), Action(std::forward<A>(a)){}
  template<typename A>
  constexpr transition<From, Event, To, Guard, A> operator/(A&& a)const{
    static_assert(std::is_same<Action, default_action>::value, "Veiler.Deus - cannot add two or more actions to transition.");
    return transition<From, Event, To, Guard, A>(both_tag{}, std::forward<Guard>(*this), std::forward<A>(a));
  }
  template<typename E>
  bool guard(E&& ev){return (*static_cast<Guard*>(this))(std::forward<E>(ev));}
  template<typename E>
  void action(E&& ev){(*static_cast<Action*>(this))(std::forward<E>(ev));}
};
template<typename State, typename Guard>
struct _guarded_state : Guard{
  template<typename G>
  constexpr _guarded_state(G&& g):Guard(std::forward<G>(g)){}
  constexpr transition<State, none, none, Guard, default_action> operator--(int)const{
    return transition<State, none, none, Guard, default_action>{guard_tag{}, std::forward<Guard>(*const_cast<_guarded_state<State, Guard>*>(this))};
  }
};
template<typename State>
class state{
  struct inner{
    using state_type = State;
    constexpr inner()noexcept = default;
    constexpr transition<State, none, none, default_guard, default_action> operator--(int)const{return transition<State, none, none, default_guard, default_action>{};}
    template<typename Guard>
    constexpr _guarded_state<State, std::decay_t<Guard>> operator[](Guard&& g)const{return _guarded_state<State, std::decay_t<Guard>>{std::forward<Guard>(g)};}
  };
 public:
  constexpr state()noexcept{}
  template<typename Action>
  constexpr transition<none, none, State, default_guard, std::decay_t<Action>> operator/(Action&& a)const{
    return transition<none, none, State, default_guard, std::decay_t<Action>>{action_tag{}, veiler::forward<Action>(a)};
  }
  constexpr inner operator--(int)const{return inner{};}
};
template<typename Event, typename... Args>
struct event_args_wrapper{veiler::tuple<Args...> args;};
template<typename Event>
class event{
  struct sysu{
    constexpr sysu()noexcept = default;
    template<typename State, typename From = typename std::decay<State>::type::state_type>
    friend constexpr transition<From, Event, none, default_guard, default_action> operator-(State&& s, sysu&&)noexcept{
      return transition<From, Event, none, default_guard, default_action>{};
    }
  };
  template<typename Guard>
  class _guarded_event : Guard{
    struct sysu : private Guard{
      template<typename G>
      constexpr sysu(G&& g):Guard(std::forward<G>(g)){}
      template<typename State, typename From = typename std::decay<State>::type::state_type>
      friend constexpr transition<From, Event, none, Guard, default_action> operator-(State&& s, sysu&& g){
        return transition<From, Event, none, Guard, default_action>{guard_tag{}, std::forward<Guard>(g)};
      }
    };
   public:
    template<typename G>
    constexpr _guarded_event(G&& g):Guard(std::forward<G>(g)){}
    constexpr sysu operator--(int){return sysu{std::forward<Guard>(*this)};}
  };
 public:
  constexpr event()noexcept{}
  constexpr sysu operator--(int)const noexcept{return sysu{};}
  template<typename Guard>
  constexpr _guarded_event<std::decay_t<Guard>> operator[](Guard&& g)const{return _guarded_event<std::decay_t<Guard>>{std::forward<Guard>(g)};}
  template<typename... Args>
  constexpr event_args_wrapper<Event, Args...> operator()(Args&&... args)const{return event_args_wrapper<Event, Args...>{{std::forward<Args>(args)...}};}
};
template<typename From, typename Event, typename To, typename Guard, typename Action>
constexpr transition<From, Event, To, Guard, Action> operator>(transition<From, Event, none, Guard, default_action>&& s, transition<none, none, To, default_guard, Action>&& e){
  return transition<From, Event, To, Guard, Action>{both_tag{}, std::forward<Guard>(s), std::forward<Action>(e)};
}
template<typename From, typename Event, typename To, typename Action>
constexpr transition<From, Event, To, default_guard, Action> operator>(transition<From, Event, none, default_guard, default_action>&& s, transition<none, none, To, default_guard, Action>&& e){
  return transition<From, Event, To, default_guard, Action>{action_tag{}, std::forward<Action>(e)};
}
template<typename From, typename Event, typename To, typename Guard>
constexpr transition<From, Event, To, Guard, default_action> operator>(transition<From, Event, none, Guard, default_action>&& s, const state<To>& e){
  return transition<From, Event, To, Guard, default_action>{guard_tag{}, std::forward<Guard>(s)};
}
template<typename From, typename Event, typename To>
constexpr transition<From, Event, To, default_guard, default_action> operator>(transition<From, Event, none, default_guard, default_action>&& s, const state<To>& e){
  return transition<From, Event, To, default_guard, default_action>{};
}


template<typename Statemachine>
class state_machine : public state<Statemachine>{
  using TransitionTable = typename Statemachine::transition_table;
  TransitionTable tt;
  using Statuses = typename TransitionTable::Statuses;
  template<typename Status>
  class status_id{
    template<typename>struct impl_;
    template<typename... As>
    struct impl_<type_tuple<As...>>{using type = index_tuple<std::is_same<Status, As>::value...>;};
    template<typename,typename>struct impl;
    template<long long... As, long long... Bs>
    struct impl<index_tuple<As...>, index_tuple<Bs...>>{static constexpr long long value = veiler::make_operate_tuple<long long>(((As+1ll)*Bs)...).add_();};
    using A = make_index_range<0, veiler::tuple_size<Statuses>{}>;
    using B = typename impl_<Statuses>::type;
  public:
    static const long long value = impl<A, B>::value - 1ll;
  };
  class holder{
    struct base{};
    template<typename T>
    struct derived : base{
      template<typename... Args>
      explicit derived(Args&&... args) : t(std::forward<Args>(args)...){}
      T t;
    };
    std::shared_ptr<base> impl;
    long long state;
  public:
    holder(const holder&) = default;
    holder(holder&&) = default;
    template<template<typename>class Wrapper, typename T>
    holder(Wrapper<T>&& t) : impl(new derived<T>()), state(status_id<T>::value){}
    template<typename T, typename... Args>
    void transit(Args&&... args){impl.reset();impl = std::make_shared<derived<T>>(veiler::forward<Args>(args)...);state = status_id<T>::value;}
    friend bool operator==(const holder& lhs, long long rhs){return lhs.state == rhs;}
    friend bool operator!=(const holder& lhs, long long rhs){return !(lhs == rhs);}
  }state;
#if 0
  template<typename Event, std::size_t N, typename F, typename G>
  static void exec_events_impl(holder& state, TransitionTable& tt, F&& f, G&& g){
    using transition = type_at<TransitionTable,N>;
    if(state != status_id<type_at<transition,0>>::value){
      f(state, tt);
      return;
    }
    Event ev;
    if(!veiler::get<N>(tt.table).guard(ev))return;
    veiler::get<N>(tt.table).action(ev);
    if(state != status_id<type_at<transition,2>>::value)
      state.template transit<type_at<transition,2>>();
    g(state, tt);
  }
  template<typename Event, std::size_t N, typename EventArgs, typename F, typename G>
  static void exec_events_impl(holder& state, TransitionTable& tt, const EventArgs& args, F&& f, G&& g){
    using transition = type_at<TransitionTable,N>;
    if(state != status_id<type_at<transition,0>>::value){
      f(state, tt, args);
      return;
    }
    Event ev = (create_instance<Event>)(args);
    if(!veiler::get<N>(tt.table).guard(ev))return;
    veiler::get<N>(tt.table).action(ev);
    if(state != status_id<type_at<transition,2>>::value)
      state.template transit<type_at<transition,2>>();
    g(state, tt, args);
  }
  template<typename Event, std::size_t N = 0, bool = std::is_same<Event, type_at<type_at<TransitionTable,N>,1>>::value, bool = std::is_same<none, type_at<type_at<TransitionTable,N>,1>>::value, bool FinaleFlag = N != TransitionTable::size-1>
  struct exec_events_{
    static void exec(holder& state, TransitionTable& tt){
      exec_events_impl<Event, N>(state, tt, [](holder& s, TransitionTable& t){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t);}, [](holder& s, TransitionTable& t){exec_events_<none, N, false, false, FinaleFlag>::exec(s, t);});
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      exec_events_impl<Event, N>(state, tt, args, [](holder& s, TransitionTable& t, const EventArgs& a){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t, a);}, [](holder& s, TransitionTable& t, auto&&){exec_events_<none, N, false, false, FinaleFlag>::exec(s, t);});
    }
  };
  template<typename Event, std::size_t N, bool IsEvent, bool FinaleFlag>
  struct exec_events_<Event, N, IsEvent, true, FinaleFlag>{
    static void exec(holder& state, TransitionTable& tt){
      exec_events_impl<none, N>(state, tt, [](holder& s, TransitionTable& t){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t);}, [](holder& s, TransitionTable& t){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t);});
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      exec_events_impl<none, N>(state, tt, args, [](holder& s, TransitionTable& t, const EventArgs& a){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t, a);}, [](holder& s, TransitionTable& t, const EventArgs& a){exec_events_<Event, N, false, false, FinaleFlag>::exec(s, t, a);});
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event, N, false, false, true>{
    static void exec(holder& state, TransitionTable& tt){
      exec_events_<Event, N+1>::exec(state, tt);
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      exec_events_<Event, N+1>::exec(state, tt, args);
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event, N, false, false, false>{
    static void exec(holder& state, TransitionTable& tt){}
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){}
  };
#else
  template<typename Event, std::size_t N = 0, bool = std::is_same<Event, type_at<type_at<TransitionTable,N>,1>>::value, bool = std::is_same<none, type_at<type_at<TransitionTable,N>,1>>::value, bool = N != TransitionTable::size-1>
  struct exec_events_{
    static void exec(holder& state, TransitionTable& tt){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value){
        exec_events_<Event, N+1>::exec(state, tt);
        return;
      }
      Event ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>();
      exec_events_<none, N+1>::exec(state, tt);
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value){
        exec_events_<Event, N+1>::exec(state, tt, args);
        return;
      }
      Event ev = (create_instance<Event>)(args);
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>(ev);
      exec_events_<none, N+1>::exec(state, tt);
    }
  };
  template<typename Event, std::size_t N, bool IsEvent>
  struct exec_events_<Event, N, IsEvent, true, true>{
    static void exec(holder& state, TransitionTable& tt){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value){
        exec_events_<Event, N+1>::exec(state, tt);
        return;
      }
      none ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>();
      exec_events_<Event, N+1>::exec(state, tt);
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value){
        exec_events_<Event, N+1>::exec(state, tt, args);
        return;
      }
      none ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>(ev);
      exec_events_<Event, N+1>::exec(state, tt, args);
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event, N, false, false, true>{
    static void exec(holder& state, TransitionTable& tt){
      exec_events_<Event, N+1>::exec(state, tt);
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      exec_events_<Event, N+1>::exec(state, tt, args);
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event, N, true, false, false>{
    static void exec(holder& state, TransitionTable& tt){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value)return;
      Event ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>();
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value)return;
      Event ev = (create_instance<Event>)(args);
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>(ev);
    }
  };
  template<typename Event, std::size_t N, bool IsEvent>
  struct exec_events_<Event, N, IsEvent, true, false>{
    static void exec(holder& state, TransitionTable& tt){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value)return;
      none ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>();
    }
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){
      using transition = type_at<TransitionTable,N>;
      if(state != status_id<type_at<transition,0>>::value)return;
      none ev;
      if(!veiler::get<N>(tt.table).guard(ev))return;
      veiler::get<N>(tt.table).action(ev);
      if(state != status_id<type_at<transition,2>>::value)
        state.template transit<type_at<transition,2>>(ev);
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event, N, false, false, false>{
    static void exec(holder& state, TransitionTable& tt){}
    template<typename EventArgs>
    static void exec(holder& state, TransitionTable& tt, const EventArgs& args){}
  };
#endif
  template<typename Event>
  void exec_events(){
    exec_events_<Event>::exec(state, tt);
  }
  template<typename Event, typename EventArgs>
  void exec_events(const EventArgs& args){
    exec_events_<Event>::exec(state, tt, args);
  }
 public:
  state_machine(const TransitionTable& table) : tt(                table ), state(typename Statemachine::initial_state{}){}
  state_machine(const state_machine&   sm   ) : tt(             sm.tt    ), state(typename Statemachine::initial_state{}){}
  state_machine(      state_machine&&  sm   ) : tt(veiler::move(sm.tt   )), state(typename Statemachine::initial_state{}){}
  template<typename Status>
  bool is(const deus::impl::state<Status>&){return state == status_id<Status>::value;}
  template<typename Event>
  friend state_machine& operator<<=(state_machine& sm, const event<Event>& e){
    sm.exec_events<Event>();
    return sm;
  }
  template<typename Event, typename... Args>
  friend state_machine& operator<<=(state_machine& sm, const event_args_wrapper<Event, Args...>& e){
    sm.exec_events<Event>(e.args);
    return sm;
  }
};

template<typename StateMachine, typename Event>
auto forward_event(StateMachine&& sm, Event&& ev){
  return sm --- ev --> sm / [&sm](auto&& x){sm <<= deus::impl::event<typename std::remove_cv<typename std::remove_reference<decltype(x)>::type>::type>{};};
}

template<typename TransitionTable, typename InitialState>
auto make_state_machine(const TransitionTable& tt, const InitialState&){
  struct sm{
    using transition_table = TransitionTable;
    using initial_state = InitialState;
  };
  return state_machine<sm>(tt);
}

template<typename... Transitions>
struct transition_table{
  tuple<Transitions...> table;
  using Statuses = unique_types<type_tuple<type_at<Transitions,0>...,type_at<Transitions,2>...>>;
  static const std::size_t size = sizeof...(Transitions);
};

template<typename... Transitions>
auto make_transition_table(Transitions&&... transitions)
       ->transition_table<Transitions...>{
  return transition_table<Transitions...>{tuple<Transitions...>{veiler::forward<Transitions>(transitions)...}};
}

}

using impl::state;
using impl::event;
using impl::transition;
using impl::state_machine;
using impl::forward_event;
using impl::make_state_machine;
using impl::transition_table;
using impl::make_transition_table;


}

}
