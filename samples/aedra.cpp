#include<veiler/ullr/aedra.hpp>

struct test{
  std::nullptr_t operator&(){return nullptr;}
};

int main(){
  {
    test p;
    test* t = veiler::ullr::aedra(p);
  }
  {
    static constexpr int p = 0;
    constexpr const int* t = veiler::ullr::aedra(p);
  }
}
