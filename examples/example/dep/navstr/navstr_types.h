/*
 * nav_str_types.h
 *
 *  Created on: 22 окт. 2019 г.
 *      Author: user
 */

#ifndef NAVSTR_TYPES_H_
#define NAVSTR_TYPES_H_

#include <navstr/navstr.h>

namespace navstr
{
  constexpr size_t GloOfBitsNum = 85;
  constexpr size_t GloL1ofBitsNum = 85;
  constexpr size_t GloL2ofBitsNum = 85;
  constexpr size_t GloL1cfgBitsNum = 300;
  constexpr size_t GloL2cfgBitsNum = 300;
  constexpr size_t GloL1ocBitsNum = 250;
  constexpr size_t GloL3ocBitsNum = 300;
  constexpr size_t GloSrlBitsNum = 1776;
  constexpr size_t GalE1BitsNum = 240;     /* Длина строки Galileo без метки времени (4.3.2.1. Synchronisation Pattern) */
  constexpr size_t GpsLnavBitsNum = 300;

  typedef navstr::NavStr<GloOfBitsNum> GloOf;
  typedef navstr::NavStr<GloL1ofBitsNum> GloL1of;
  typedef navstr::NavStr<GloL1cfgBitsNum> GloL1cfg;
  typedef navstr::NavStr<GloL2ofBitsNum> GloL2of;
  typedef navstr::NavStr<GloL2cfgBitsNum> GloL2cfg;
  typedef navstr::NavStr<GloL1ocBitsNum> GloL1oc;
  typedef navstr::NavStr<GloL3ocBitsNum> GloL3oc;
  typedef navstr::NavStr<GloSrlBitsNum> GloSrl;
  typedef navstr::NavStr<GalE1BitsNum> GalE1;
  typedef navstr::NavStr<GpsLnavBitsNum> GpsLnav;
}

#endif /* NAVSTR_TYPES_H_ */
