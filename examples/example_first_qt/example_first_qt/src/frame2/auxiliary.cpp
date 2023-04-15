/*
 * Auxiliary.cpp
 *
 *  Created on: 13 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/auxiliary.h>
#include <limits>

namespace frame2
{
  const Auxiliary::Binary Auxiliary::scDefaultBinary = {-1, 0, 0};
  const float Auxiliary::NaN = std::numeric_limits<float>::quiet_NaN();

  Auxiliary::Auxiliary () :
      mBinary(Auxiliary::scDefaultBinary), mAmplData(NaN), mAmplPilot(NaN), mPhaseStdData(NaN), mPhaseStdPilot(NaN), mDataNum(scValueUndefined), mPilotNum(scValueUndefined)
  {
  }

  Auxiliary::~Auxiliary ()
  {
  }

} /* namespace frame2 */
