/*
 * obs.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/obs.h>

#include <limits>

namespace frame2
{

  const float Obs::fNaN = std::numeric_limits<float>::quiet_NaN();
  const double Obs::NaN = std::numeric_limits<double>::quiet_NaN();
  const Obs::Binary Obs::scDefaultBinary = {Obs::NaN, Obs::NaN, Obs::fNaN, Obs::fNaN};

  Obs::Obs () : mBinary(Obs::scDefaultBinary)
  {
  }

  Obs::~Obs ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
