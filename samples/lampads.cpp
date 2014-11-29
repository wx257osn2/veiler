#include<veiler/lampads.hpp>
using veiler::lampads::if_;
using veiler::lampads::udl::operator"" _;
using veiler::lampads::udl::operator"" _tail;
using veiler::lampads::self;
using veiler::lampads::bind;
using veiler::lampads::ret;
using veiler::lampads::num_of_args;

constexpr auto func(int a, int b, int c)->int{
  return a + b + c;
}

int main(){
  constexpr auto a = if_(1_ == 3)[3].else_[2_](1,2,3,4);
  constexpr auto b = (3_ * 4)(1,2,3,4);
  static_assert(a * 6 == b,"");
  constexpr auto c = if_(1_ == 0)[1].else_[1_ * self(1_-1)](4);
  static_assert(c == 24,"");
  constexpr auto d = bind(func, 0_tail)(1,2,3);
  static_assert(d == 6, "");
  constexpr auto e = if_(1_ == 3)[bind(func, 1_tail)].else_[self(1_ + 1, 1_tail)](1, 1, 2, 3);
  static_assert(e == 6, "");
  constexpr auto f = 1_ % 2 == 0;
  static_assert(f(3) == false, "");
  constexpr auto g = if_(f)[1_].else_[1_ + 1](3);
  static_assert(g == 4, "");
  constexpr auto h = if_(num_of_args == 1)[1_].else_[self(1_ + 2_, 2_tail)](1, 2, 3);
  static_assert(h == 6, "");
}
