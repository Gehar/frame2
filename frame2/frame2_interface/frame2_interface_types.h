/*
 * frame2_interface_types.h
 *
 *  Created on: 10 июн. 2021 г.
 *      Author: user
 */

#ifndef FRAME2_INTERFACE_TYPES_H_
#define FRAME2_INTERFACE_TYPES_H_

#include <frame2/frame2.h>

namespace frame2
{

#define FRAME2_GENERATE_INTERFACE_CLASS(class_name, system, System, component, Component)\
  class class_name\
  {\
  public:\
    inline class_name (const frame2::Frame2 &Frm2);\
    inline size_t size() const;\
    inline const frame2::Id& id (size_t index) const;\
\
    inline bool has_obs(size_t index) const;\
    inline const frame2::Obs& obs (size_t index) const;\
\
    inline bool has_aux(size_t index) const;\
    inline const frame2::Auxiliary& aux(size_t index) const;\
\
    inline bool has_msg(size_t index) const;\
    inline const frame2::System::Msg##Component & msg(size_t index) const;\
\
  protected:\
    const frame2::Frame2 &mFrm2;\
  };\
\
  inline class_name::class_name (const frame2::Frame2 &Frm2) : mFrm2(Frm2) {}\
\
  inline size_t class_name::size () const {return mFrm2.system##_size();}\
\
  inline const frame2::Id& class_name::id (size_t index) const { return mFrm2.system(index).id(); }\
\
  inline bool class_name::has_obs(size_t index) const { return mFrm2.system(index).has_##component##_obs(); }\
\
  inline const frame2::Obs& class_name::obs (size_t index) const { return mFrm2.system(index).component##_obs(); }\
\
  inline bool class_name::has_aux(size_t index) const { return mFrm2.system(index).has_##component##_aux(); }\
\
  inline const frame2::Auxiliary& class_name::aux(size_t index) const { return mFrm2.system(index).component##_aux(); }\
\
  inline bool class_name::has_msg(size_t index) const { return mFrm2.system(index).has_##component##_msg(); }\
\
  inline const frame2::System::Msg##Component& class_name::msg(size_t index) const { return mFrm2.system(index).component##_msg(); }

  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL1of, glo, Glo, l1of, L1of);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL2of, glo, Glo, l2of, L2of);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL1cfg, glo, Glo, l1cfg, L1cfg);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL2cfg, glo, Glo, l2cfg, L2cfg);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL1oc, glo, Glo, l1oc, L1oc);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL2q, glo, Glo, l2q, L2q);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL3oc, glo, Glo, l3oc, L3oc);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL1sc, glo, Glo, l1sc, L1sc);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloL2sc, glo, Glo, l2sc, L2sc);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGloSrl, glo, Glo, srl, Srl);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGpsL1ca, gps, Gps, l1ca, L1ca);
  FRAME2_GENERATE_INTERFACE_CLASS(Frame2InterfaceGalE1bc, gal, Gal, e1bc, E1bc);

}

#endif /* FRAME2_INTERFACE_TYPES_H_ */
