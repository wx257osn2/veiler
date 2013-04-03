#include<iostream>
#include<veiler/hastur.hpp>

typedef VEILER_HASTUR_TAG_CREATE(begin) begin;
typedef VEILER_HASTUR_TAG_CREATE(end) end;
typedef VEILER_HASTUR_TAG_CREATE(iterator) iter;
template<typename T>
using has_begin = veiler::hastur<begin>::func<T>;
template<typename T>
using has_end = veiler::hastur<end>::func<T>;
template<typename T>
using has_iterator = veiler::hastur<iter>::type<T>;

template<typename T>constexpr void check(){
  static_assert(has_begin<T>() && has_end<T>() && has_iterator<T>(),"Your type doesn't fulfill concept.");
}

#include<vector>

int main(){
  check<std::vector<int>>();
  //check<int>();   //Compile error.
}
