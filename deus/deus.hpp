#include<utility>
#include<tuple>
#include<memory>
#include<veiler/temple.hpp>

namespace veiler{

namespace deus{

namespace impl{

struct none{};
struct default_guard{
  template<typename T>
  bool operator()(const T&)const{return true;}
};
struct default_action{
  template<typename T>
  void operator()(const T&)const{}
};
template<typename>class event;
template<typename>struct state;

template<typename From, typename Event, typename To, typename Guard, typename Action>
class transition{
  Guard  g;
  Action a;
 public:
  transition(Guard&& g):g(std::forward<Guard>(g)){}
  transition(Action&& a):a(std::forward<Action>(a)){}
  transition(Guard&& g, Action&& a):g(std::forward<Guard>(g), std::forward<Action>(a)){}
  template<typename E>
  bool guard(E&& ev)const{return g(std::forward<E>(ev));}
  template<typename E>
  void action(E&& ev)const{a(std::forward<E>(ev));}
};
template<typename From, typename Event, typename To, typename Guard>
class transition<From, Event, To, Guard, default_action>{
  Guard  g;
 public:
  transition(Guard&& g):g(std::forward<Guard>(g)){}
  template<typename A>
  transition<From, Event, To, Guard, A> operator=(A&& a)const{return transition<From, Event, To, Guard, A>(std::forward<Guard>(g), std::forward<A>(a));}
  template<typename E>
  bool guard(E&& ev)const{return g(std::forward<E>(ev));}
  template<typename E>
  void action(E&& ev)const{}
};
template<typename From, typename Event, typename To>
class transition<From, Event, To, default_guard, default_action>{
public:
  template<typename G>
  transition<From, Event, To, G, default_action> operator[](G&& g)const{return transition<From, Event, To, G, default_action>(std::forward<G>(g));}
  template<typename A>
  transition<From, Event, To, default_guard, A> operator=(A&& a)const{return transition<From, Event, To, default_guard, A>(std::forward<A>(a));}
  template<typename E>
  bool guard(E&& ev)const{return true;}
  template<typename E>
  void action(E&& ev)const{}
};
template<typename State>
struct state{
  constexpr state(){}
  template<typename Guard>
  transition<none, none, State, Guard, default_action> operator[](Guard&& g)const{return transition<none, none, State, Guard, default_action>(std::forward<Guard>(g));}
  template<typename Action>
  transition<none, none, State, default_guard, Action> operator=(Action&& a)const{return transition<none, none, State, default_guard, Action>(std::forward<Action>(a));}
  transition<State, none, none, default_guard, default_action> operator--(int)const{ return transition<State, none, none, default_guard, default_action>(); }
};
template<typename Event>
class event{
  struct sysu{
    template<typename From>
    friend transition<From, Event, none, default_guard, default_action> operator-(transition<From, none, none, default_guard, default_action>&& s, sysu&&){
      return transition<From, Event, none, default_guard, default_action>();
    }
  };
 public:
  constexpr event(){}
  sysu operator--(int)const{return sysu();}
  template<typename... Args>
  struct wrapper{tuple<Args...> args;};
  template<typename... Args>
  wrapper<Args...> operator()(Args&&... args){return wrapper<Args...>{{std::forward<Args>(args)...}};}
};
template<typename From, typename Event, typename To, typename Guard, typename Action>
transition<From, Event, To, Guard, Action> operator>(transition<From, Event, none, default_guard, default_action>&& s, transition<none, none, To, Guard, Action>&& e){
  return transition<From, Event, To, Guard, Action>{};
}
template<typename From, typename Event, typename To>
transition<From, Event, To, default_guard, default_action> operator>(transition<From, Event, none, default_guard, default_action>&& s, const state<To>& e){
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
      derived() : t(){}
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
    void transit(Args&&... args){impl.reset();impl = std::make_shared<derived<T>>(std::forward<Args>(args)...);state = status_id<T>::value;}
    friend bool operator==(const holder& lhs, long long rhs){return lhs.state == rhs;}
    friend bool operator!=(const holder& lhs, long long rhs){return !(lhs == rhs);}
  }state;
  
  template<typename Event, std::size_t N = 0, bool = N != TransitionTable::size>
  struct exec_events_{
    static void exec(holder& state, const TransitionTable& tt){
      using transition = type_at<TransitionTable,N>;
      if(state == status_id<type_at<transition,0>>::value && (std::is_same<Event, type_at<transition,1>>::value || std::is_same<none, type_at<transition,1>>::value)){
        Event ev;
        if(veiler::get<N>(tt.table).guard(ev)){
          veiler::get<N>(tt.table).action(ev);
          if(state != status_id<type_at<transition,2>>::value)
            state.template transit<type_at<transition,2>>();
          exec_events_<none, N+1>::exec(state, tt);
          return;
        }
      }
      exec_events_<Event, N+1>::exec(state, tt);
    }
    template<typename EventArgs>
    static void exec(holder& state, const TransitionTable& tt, const EventArgs& e){
      using transition = type_at<TransitionTable,N>;
      if(state == status_id<type_at<transition,0>>::value && (std::is_same<Event, type_at<transition,1>>::value || std::is_same<none, type_at<transition,1>>::value)){
        Event ev = make<Event>(e.args);
        if(veiler::get<N>(tt.table).guard(ev)){
          veiler::get<N>(tt.table).action(ev);
          if(state != status_id<type_at<transition,2>>::value)
            state.template transit<type_at<transition,2>>(ev);
          exec_events_<none, N+1>::exec(state, tt);
          return;
        }
      }
      exec_events_<Event, N+1>::exec(state, tt, e);
    }
  };
  template<typename Event, std::size_t N>
  struct exec_events_<Event,N,false>{static void exec(holder&, const TransitionTable&){}};
  template<typename Event>
  void exec_events(){
    exec_events_<Event>::exec(state, tt);
  }
  template<typename Event, typename EventArgs>
  void exec_events(const EventArgs& e){
    exec_events_<Event>::exec(state, tt, e);
  }
 public:
  state_machine(const TransitionTable& table) : tt(             table ), state(typename Statemachine::initial_state{}){}
  state_machine(const state_machine&   sm   ) : tt(          sm.tt    ), state(typename Statemachine::initial_state{}){}
  state_machine(      state_machine&&  sm   ) : tt(std::move(sm.tt   )), state(typename Statemachine::initial_state{}){}
  template<typename Status>
  bool is(const deus::impl::state<Status>&){return state == status_id<Status>::value;}
  template<typename Event>
  friend state_machine& operator<<=(state_machine& sm, const event<Event>& e){
    sm.exec_events<Event>();
    return sm;
  }
  template<typename Event, typename... Args>
  friend state_machine& operator<<=(state_machine& sm, const typename event<Event>::wrapper<Args...>& e){
    sm.exec_events<Event>(e);
    return sm;
  }
};

template<typename StateMachine, typename Event>
auto forward_event(StateMachine&& sm, Event&& ev){
  return sm --- ev --> sm = [&sm](auto&& x){sm <<= deus::impl::event<typename std::remove_cv<typename std::remove_reference<decltype(x)>::type>::type>{};};
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
  return transition_table<Transitions...>{tuple<Transitions...>{std::forward<Transitions>(transitions)...}};
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
