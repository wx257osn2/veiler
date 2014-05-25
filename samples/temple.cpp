#include<iostream>
#include<veiler/temple.hpp>

template<long long... Indices>
constexpr auto make_tuple_from_index_range(veiler::index_tuple<Indices...>)
  ->decltype(veiler::make_tuple(Indices...)){
    return veiler::make_tuple(Indices...);
}

template<long long... Indices>
constexpr auto make_value_btree_from_index_range(veiler::index_tuple<Indices...>)
  ->typename veiler::detail::temple::make_value_btree<sizeof...(Indices), veiler::type_tuple<decltype(Indices)...>>::type{
    return typename veiler::detail::temple::make_value_btree<sizeof...(Indices), veiler::type_tuple<decltype(Indices)...>>::type(Indices...);
}


constexpr int test(int a,int b,int c,int d,int e){
  return a+b+c+d+e;
}



int main(){
  using namespace veiler;
  std::cout << get<0>(make_tuple_from_index_range(make_index_range<0,1>{})) << std::endl;
  {
    static constexpr auto t = make_tuple(1,2,3,4,5);
    static_assert(get<1>(t) == 2,"");
  }
  {
    static constexpr auto t = make_tuple(1,2,3,4,5);
    static_assert((test |apply| t) == 15,"");
    static_assert(apply(test,t) == 15,"");
  }
  {
    static constexpr auto t = tuple_cat(make_tuple(1,2,3),make_tuple(4,5),make_tuple(6),make_tuple(),make_tuple(7,8,9,10));
    static_assert(t == make_tuple(1,2,3,4,5,6,7,8,9,10),"");
  }
  {
    constexpr auto t = make_tuple_from_index_range(make_index_range<0,256>{});
    static_assert(get<255>(t) == 255,"");
  }
  {
    auto t = make_tuple(1,2,3,4,5);
    t = make_tuple(2,3,4,5,6);
    t = std::move(make_tuple(1,2,3,4,5));
    t = make_tuple(1.0,2.0,3.,4,5);
  }
}

