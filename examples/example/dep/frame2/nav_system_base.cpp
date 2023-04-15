/*
 * nav_system_base.cpp
 *
 *  Created on: 22 авг. 2019 г.
 *      Author: user
 */

#include <frame2/nav_system_base.h>

namespace frame2
{

  NavSystemBase::NavSystemBase (const int NumSignals):
      cNumSignals(NumSignals), mSetOfObsFlags(0), mSetOfMsgFlags(0), mSetOfAuxFlags(0), pId(nullptr), pAux(nullptr), pObs(nullptr), pMsg({nullptr})
  {
//    /* Блок Id всегда есть, остальные блоки могут отсутствовать */
//    pId = nullptr;
//    pAux = nullptr;
//    pObs = nullptr;
//    pMsg.fill(nullptr);
  }

  NavSystemBase::~NavSystemBase ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
