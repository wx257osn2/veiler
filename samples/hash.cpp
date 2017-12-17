#include<veiler/fool/hash.hpp>
#include<cassert>

#include<list>
#include<unordered_map>
#include<iterator>
#include<utility>

int main(){
  {
    std::list<int> l = {3, 10, 15};
    std::unordered_map<std::list<int>::iterator, int, veiler::hash<std::list<int>::iterator>> um;
    for(auto it = l.begin(); it != l.end(); ++it)
      um[it] = *it + 1;
    assert(um[          l.begin()    ] == *          l.begin()    +1 &&
           um[std::next(l.begin()   )] == *std::next(l.begin()   )+1 &&
           um[std::next(l.begin(), 2)] == *std::next(l.begin(), 2)+1);
  }
  {
    std::unordered_map<std::pair<int, double>, int, veiler::hash<std::pair<int, double>>> um;
    um[{1, 1.}] = 1;
    um[{2, 2.}] = 4;
    assert((um[{1, 1.}] == 1 && um[{2, 2.}] == 4));
  }
  {
    std::unordered_map<std::tuple<int, double, char>, int, veiler::hash<std::tuple<int, double, char>>> um;
    um[{1, 1., 'a'}] = 1;
    um[{2, 2., 'b'}] = 4;
    assert((um[{1, 1., 'a'}] == 1 && um[{2, 2., 'b'}] == 4));
  }
  {
    std::unordered_map<std::variant<int, double>, int, veiler::hash<std::variant<int, double>>> um;
    um[{1}] = 1;
    um[{2.}] = 4;
    assert(um[{1}] == 1 && um[{2.}] == 4);
  }
}