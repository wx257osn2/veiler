#ifndef VEILER_PELOPS_ULLR_HPP_INCLUDED
#define VEILER_PELOPS_ULLR_HPP_INCLUDED

#define VEILER_PELOPS_ULLR_COMMA VEILER_PELOPS_ULLR_COMMA_ VEILER_PELOPS_ULLR_LEFT_ VEILER_PELOPS_ULLR_RIGHT_
#define VEILER_PELOPS_ULLR_CAT(a,b) VEILER_PELOPS_ULLR_CAT_(a,b)
#define VEILER_PELOPS_ULLR_STR(a) VEILER_PELOPS_ULLR_STR_(a)
#define VEILER_PELOPS_ULLR_EXP(...) __VA_ARGS__


#define VEILER_PELOPS_ULLR_COMMA_() ,
#define VEILER_PELOPS_ULLR_LEFT_ (
#define VEILER_PELOPS_ULLR_RIGHT_ )
#define VEILER_PELOPS_ULLR_CAT_(a,b) a##b
#define VEILER_PELOPS_ULLR_STR_(a) #a

#endif //VEILER_PELOPS_ULLR_HPP_INCLUDED

//Copyright (C) 2012 I
//  Distributed under the Veiler Source License 1.0.
