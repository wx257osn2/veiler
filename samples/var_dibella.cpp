#include <string> 
#define vastrcat(str,...) VEILER_VAR_FUNC(std::string(str),__VA_ARGS__)
VEILER_VAR_IMPL(std::string,std::string,{lhs.append(rhs);})

#include <sstream>
struct printftag{};
#define print_f(format,...) VEILER_VAR_FUNC_USE_TAG(printftag,std::string(format),__VA_ARGS__)
VEILER_VAR_DET_IMPL_START(std::string,printftag)
    unsigned char count;
    VEILER_VAR_DET_IMPL_CTOR(,count(0)){}
    template<typename T>VEILER_VAR_DET_IMPL_IMPL(T){
        std::string target(1,'%');
        {
            std::stringstream s;
            s<<count+1;
            target+=s.str();
        }
        target+='%';
        std::string::size_type temp=0;
        while((temp=this->find(target,temp))!=std::string::npos){
            std::stringstream s;
            s<<arg;
            replace(temp,target.size(),s.str().c_str());
        }
        ++count;
        return *this;
    }
VEILER_VAR_DET_IMPL_END

#include<utility>
#include<veiler/pelops/dibella.hpp>
#include<veiler/var.hpp>
VEILER_VAR_IMPL(VEILER_PELOPS_DIBEllA(std::pair<int,int>),VEILER_PELOPS_DIBELLA(std::pair<int,int>),{lhs.first+=rhs.first;lhs.second+=rhs.second;})
#define plus(args0,...) VEILER_VAR_FUNC(VEILER_PELOPS_DIBELLA(std::pair<int,int>)(args0),__VA_ARGS__)


#include <iostream> 
int main(){
    std::string test("ÇƒÇ∑Ç∆ ÅF ");
    std::cout<<vastrcat("ÇƒÇ∑Ç∆     ","ï∂éöóÒÇ","Ç¢Ç≠Ç¬Ç≈Ç‡","åãçá","Ç≈Ç´ÇÈ\n")
             <<vastrcat(test,"Ç±ÇÍÇ‡","ÇøÇ·ÇÒÇ∆","ñ‚ëËÇ»Ç≠","ìÆÇ≠\n")
             <<print_f("%2%.%1%ÇÃÇÊÇ§Ç»Ç»Ç…Ç©  %3%    %4%\n","Format","Boost",30,5.7)
             <<std::endl;
    
    std::pair<int,int> a[10];
    for(int i=0;i<10;++i)a[i].first=a[i].second=i;
    std::cout<<plus(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]).first<<std::endl;
    return 0;
}
