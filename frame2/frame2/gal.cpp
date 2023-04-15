/*
 * gal.cpp
 *
 *  Created on: 27 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/gal.h>

namespace frame2
{

  Gal::Gal () :
      NavSystemBase(Component::MAX)
  {
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[0] = &mMsgE1bc;
  }

  Gal::Gal (const Gal &gal) :
          NavSystemBase(gal.cNumSignals)
  {
    /* Копируем содержимое */
    mSetOfObsFlags = gal.mSetOfObsFlags;
    mSetOfMsgFlags = gal.mSetOfMsgFlags;
    mSetOfAuxFlags = gal.mSetOfAuxFlags;
    mId = gal.mId;
    for (int i = 0; i < cNumSignals; i++)
    {
      mAux[i] = gal.mAux[i];
      mObs[i] = gal.mObs[i];
    }
    mMsgE1bc = gal.mMsgE1bc;
    /* Настраиваем указатели */
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[0] = &mMsgE1bc;
  }

  Gal& Gal::operator=(const Gal& gal)
  {
    //проверка на самоприсваивание
    if (this == &gal)
      return *this;

    /* Копируем содержимое */
    mSetOfObsFlags = gal.mSetOfObsFlags;
    mSetOfMsgFlags = gal.mSetOfMsgFlags;
    mSetOfAuxFlags = gal.mSetOfAuxFlags;
    mId = gal.mId;
    for (int i = 0; i < cNumSignals; i++)
    {
      mAux[i] = gal.mAux[i];
      mObs[i] = gal.mObs[i];
    }
    mMsgE1bc = gal.mMsgE1bc;
    /* Настраиваем указатели */
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[0] = &mMsgE1bc;
    return *this;
  }

  Gal::~Gal ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
