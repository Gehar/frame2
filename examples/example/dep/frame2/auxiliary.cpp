/*
 * Auxiliary.cpp
 *
 *  Created on: 13 авг. 2019 г.
 *      Author: user
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
//    Clear();
  }

  Auxiliary::~Auxiliary ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
