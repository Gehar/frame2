/*
 * glo.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/glo.h>

namespace frame2
{

  Glo::Glo () :
      NavSystemBase(Component::MAX)
  {
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[Component::L1OF] = &mMsgL1of;
    pMsg[Component::L2OF] = &mMsgL2of;
    pMsg[Component::L1OC] = &mMsgL1oc;
    pMsg[Component::L3OC] = &mMsgL3oc;
    pMsg[Component::L1CFG] = &mMsgL1cfg;
    pMsg[Component::L2CFG] = &mMsgL2cfg;
    pMsg[Component::SRL] = &mMsgSrl;
    pMsg[Component::L2Q] = &mMsgL2q;
    pMsg[Component::L1SC] = &mMsgL1sc;
    pMsg[Component::L2SC] = &mMsgL2sc;
  }

  Glo::Glo (const Glo &glo) :
      NavSystemBase(glo.cNumSignals)
  {
    /* Копируем содержимое */
    mSetOfObsFlags = glo.mSetOfObsFlags;
    mSetOfMsgFlags = glo.mSetOfMsgFlags;
    mSetOfAuxFlags = glo.mSetOfAuxFlags;
    mId = glo.mId;
    for (int i = 0; i < cNumSignals; i++)
    {
      mAux[i] = glo.mAux[i];
      mObs[i] = glo.mObs[i];
    }
    mMsgL1of = glo.mMsgL1of;
    mMsgL2of = glo.mMsgL2of;
    mMsgL1oc = glo.mMsgL1oc;
    mMsgL3oc = glo.mMsgL3oc;
    mMsgL1cfg = glo.mMsgL1cfg;
    mMsgL2cfg = glo.mMsgL2cfg;
    mMsgSrl = glo.mMsgSrl;
    mMsgL2q = glo.mMsgL2q;
    mMsgL1sc = glo.mMsgL1sc;
    mMsgL2sc = glo.mMsgL2sc;
    /* Настраиваем указатели */
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[Component::L1OF] = &mMsgL1of;
    pMsg[Component::L2OF] = &mMsgL2of;
    pMsg[Component::L1OC] = &mMsgL1oc;
    pMsg[Component::L3OC] = &mMsgL3oc;
    pMsg[Component::L1CFG] = &mMsgL1cfg;
    pMsg[Component::L2CFG] = &mMsgL2cfg;
    pMsg[Component::SRL] = &mMsgSrl;
    pMsg[Component::L2Q] = &mMsgL2q;
    pMsg[Component::L1SC] = &mMsgL1sc;
    pMsg[Component::L2SC] = &mMsgL2sc;
  }

  Glo& Glo::operator=(const Glo& glo)
  {
    //проверка на самоприсваивание
    if (this == &glo)
      return *this;

    /* Копируем содержимое */
    mSetOfObsFlags = glo.mSetOfObsFlags;
    mSetOfMsgFlags = glo.mSetOfMsgFlags;
    mSetOfAuxFlags = glo.mSetOfAuxFlags;
    mId = glo.mId;
    for (int i = 0; i < cNumSignals; i++)
    {
      mAux[i] = glo.mAux[i];
      mObs[i] = glo.mObs[i];
    }
    mMsgL1of = glo.mMsgL1of;
    mMsgL2of = glo.mMsgL2of;
    mMsgL1oc = glo.mMsgL1oc;
    mMsgL3oc = glo.mMsgL3oc;
    mMsgL1cfg = glo.mMsgL1cfg;
    mMsgL2cfg = glo.mMsgL2cfg;
    mMsgSrl = glo.mMsgSrl;
    mMsgL2q = glo.mMsgL2q;
    mMsgL1sc = glo.mMsgL1sc;
    mMsgL2sc = glo.mMsgL2sc;
    /* Настраиваем указатели */
    pId = &mId;
    pAux = mAux;
    pObs = mObs;
    pMsg[Component::L1OF] = &mMsgL1of;
    pMsg[Component::L2OF] = &mMsgL2of;
    pMsg[Component::L1OC] = &mMsgL1oc;
    pMsg[Component::L3OC] = &mMsgL3oc;
    pMsg[Component::L1CFG] = &mMsgL1cfg;
    pMsg[Component::L2CFG] = &mMsgL2cfg;
    pMsg[Component::SRL] = &mMsgSrl;
    pMsg[Component::L2Q] = &mMsgL2q;
    pMsg[Component::L1SC] = &mMsgL1sc;
    pMsg[Component::L2SC] = &mMsgL2sc;
    return *this;
  }

  Glo::~Glo ()
  {
    return;
  }

} /* namespace frame2 */
