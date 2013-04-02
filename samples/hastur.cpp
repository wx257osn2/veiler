#include<iostream>
#include<veiler/hastur.hpp>

namespace test{
  typedef VEILER_HASTUR_TAG_CREATE(type) type;
  typedef VEILER_HASTUR_TAG_CREATE(value) value;
  typedef VEILER_HASTUR_TAG_CREATE(func) func;
}

struct Foo{
  typedef int type;
};

struct Bar{
  int value;
};

struct Baz{
  int func(){return 0;}
};

int main(){
  std::cout<<std::boolalpha<<
  "Foo : type  -  "<<veiler::hastur<test::type>::type<Foo>()<<'\n'<<
  "      value -  "<<veiler::hastur<test::value>::value<Foo>()<<'\n'<<
  "      func  -  "<<veiler::hastur<test::func>::func<Foo>()<<'\n'<<
  "Bar : type  -  "<<veiler::hastur<test::type>::type<Bar>()<<'\n'<<
  "      value -  "<<veiler::hastur<test::value>::value<Bar>()<<'\n'<<
  "      func  -  "<<veiler::hastur<test::func>::func<Bar>()<<'\n'<<
  "Baz : type  -  "<<veiler::hastur<test::type>::type<Baz>()<<'\n'<<
  "      value -  "<<veiler::hastur<test::value>::value<Baz>()<<'\n'<<
  "      func  -  "<<veiler::hastur<test::func>::func<Baz>()<<'\n'<<
  std::endl;
}
