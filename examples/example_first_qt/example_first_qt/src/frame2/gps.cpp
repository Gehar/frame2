/*
 * Gps.cpp
 *
 *  Created on: 20 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/gps.h>

namespace frame2
{

  Gps::Gps () : mSetObsFlags(0), mSetMsgFlags(0), mSetAuxFlags(0)
  {
//    Clear();
  }

  Gps::~Gps ()
  {
  }

  Gps& Gps::operator=(const Gps& gps)
  {
    //проверка на самоприсваивание
    if (this == &gps)
      return *this;
    mSetObsFlags = gps.mSetObsFlags;
    mSetMsgFlags = gps.mSetMsgFlags;
    mSetAuxFlags = gps.mSetAuxFlags;
    mId = gps.mId;
    for (size_t i = 0; i < Component::MAX; i++)
      mAux[i] = gps.mAux[i];
    for (size_t i = 0; i < Component::MAX; i++)
      mObs[i] = gps.mObs[i];
    mMsgL1ca = gps.mMsgL1ca;
    return *this;
  }

} /* namespace frame2 */
