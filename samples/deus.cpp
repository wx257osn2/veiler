#include<iostream>
#include<veiler/deus.hpp>

namespace state{
  namespace detail{
    struct main{main(){std::cout << "main()" << std::endl;}~main(){std::cout << "~main()" << std::endl;}};
    struct setting{setting(){std::cout << "setting()" << std::endl;}~setting(){std::cout << "~setting()" << std::endl;}};
  }
  namespace{
    const veiler::deus::state<detail::main> main;
    const veiler::deus::state<detail::setting> setting;
  }
};
namespace event{
  namespace detail{
    struct toggle{};
  }
  namespace{
    const veiler::deus::event<detail::toggle> toggle;
  }
};

int main(){
  auto sm = veiler::deus::make_state_machine(veiler::deus::make_transition_table(
    state::main    --- event::toggle --> state::setting = [&](const event::detail::toggle&){std::cout << "Toggle" << std::endl;},
    state::setting --- event::toggle --> state::main    = [&](const auto&){std::cout << "Toggle" << std::endl;}
  ), state::main);
  std::cout << sm.is(state::main) << std::endl;
  sm <<= event::toggle;
  std::cout << sm.is(state::setting) << std::endl;
  sm <<= event::toggle;
  std::cout << sm.is(state::main) << std::endl;
}
