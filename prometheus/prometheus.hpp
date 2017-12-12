//Copyright (C) 2012-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VEILER_PROMETHEUS_HPP_INCLUDED
#define VEILER_PROMETHEUS_HPP_INCLUDED

#include<cstddef>
#include<type_traits>

namespace veiler{

namespace detail{

namespace prometheus{

template<std::size_t N>struct prometheus : prometheus<N-1>{};
template<>             struct prometheus<0>{};

}//End : namespace prometheus

}//End : namespace detail

template<std::size_t N>
using prometheus = const detail::prometheus::prometheus<N>*const;

template<std::size_t N>
constexpr typename std::decay<prometheus<N>>::type prometheus_fire(){return static_cast<prometheus<N>>(nullptr);}


}//End : namespace veiler

#endif //VEILER_PROMETHEUS_HPP_INCLUDED
