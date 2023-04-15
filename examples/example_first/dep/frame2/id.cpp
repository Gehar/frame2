/*
 * ID.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/id.h>
#include <limits>

namespace frame2
{

  const float Id::fNaN = std::numeric_limits<float>::quiet_NaN();
  const Id::Binary Id::scDefaultBinary =
    { 0, Id::scValueUndefined, Id::scFreqNumUndefined, Id::scValueUndefined, Id::fNaN, Id::fNaN };

  Id::Id () : mBinary(Id::scDefaultBinary)
  {
//    Clear();
  }

  Id::~Id ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
