#ifndef VEILER_PROMETHEUS_HPP_INCLUDED
#define VEILER_PROMETHEUS_HPP_INCLUDED

#include<utility>

namespace veiler{

namespace detail{

namespace prometheus{

template<std::size_t N>struct prometheus : prometheus<N-1>{};
template<>             struct prometheus<0>{};

}

}

template<std::size_t N>
using prometheus = const detail::prometheus::prometheus<N>*const;

template<std::size_t N>
constexpr prometheus<N> prometheus_fire(){return static_cast<prometheus<N>>(nullptr);}


}//End : namespace veiler

#endif //VEILER_PROMETHEUS_HPP_INCLUDED

//Copyright (C) 2013 I
//  Distributed under the Veiler Source License 1.0.
