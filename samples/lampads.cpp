#include<veiler/lampads.hpp>
using veiler::if_;
using veiler::udl::operator"" _;
using veiler::self;

int main(){
  constexpr auto a = if_(1_ == 3)[3].else_[2_](1,2,3,4);
  constexpr auto b = (3_ * 4)(1,2,3,4);
  static_assert(a * 6 == b,"");
  constexpr auto c = if_(1_ == 0)[1].else_[1_ * self(1_-1)](4);
  static_assert(c == 24,"");
}
