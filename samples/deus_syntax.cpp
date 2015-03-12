#include<iostream>
#include<veiler/deus.hpp>

namespace detail{
  struct state{};
  struct event{};
}
namespace{
  constexpr veiler::deus::state<detail::state> state;
  constexpr veiler::deus::event<detail::event> event;
}

struct{
  template<typename T>
  bool operator()(T&&)const{return true;}
}static const guard = {};

struct{
  template<typename T>
  void operator()(T&&)const{}
}static const action = {};

int main(){
  auto another_statemachine = veiler::deus::make_state_machine(veiler::deus::make_transition_table(state---->state), state);
  auto sm = veiler::deus::make_state_machine(veiler::deus::make_transition_table(
    state --             --> state         ,
    state --             --> state / action,
    state --      [guard]--> state         ,
    state --      [guard]--> state / action,
    state ---event       --> state         ,
    state ---event       --> state / action,
    state ---event[guard]--> state         ,
    state ---event[guard]--> state / action,
    veiler::deus::forward_event(another_statemachine, event)
  ), state);
  sm <<= event;
}
