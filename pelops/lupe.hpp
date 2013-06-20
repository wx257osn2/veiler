#ifndef VEILER_PELOPS_LUPE_HPP_INCLUDED
#define VEILER_PELOPS_LUPE_HPP_INCLUDED

#include "ullr.hpp"
#include "dibella.hpp"

#define VEILER_PELOPS_LUPE(macro,args) VEILER_PELOPS_ULLR_CAT(VEILER_PELOPS_DIBELLA(macro args),_END)

#endif //VEILER_PELOPS_LUPE_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
