#include<iostream>
#include<veiler/ullr/areion.hpp>

int main(){
  auto t=veiler::ullr::areion<int>(3,2,1);
  for(auto n:t){std::cout<<n<<std::endl;}
}

