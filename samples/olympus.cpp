#include<veiler/forseti/olympus.hpp>
#include<string>
#include<iostream>

int main(){
  using namespace veiler::forseti::olympus;
  std::fstream f("test.txt",w);
  f<<"test";
  f.close();
  f.open("test2",w+b);
  f<<"test";
  f.close();
  f.open("test3.txt",wb);
  f<<"test";
  f.close();
  f.open("test.txt",a+_);
  std::string str;
  f>>str;
  std::cout<<str<<std::endl;
}
