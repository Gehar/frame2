/*
 * test-frame2_interface.cpp
 *
 *  Created on: 10 июн. 2021 г.
 *      Author: user
 */

#include "catch.hpp"

#include <frame2_interface/frame2_interface.h>


TEST_CASE( "Class frame2::Frame2Interface", "[Frame2Interface]" )
{
  /*
   * Просто проверяем, что класс Frame2Interface<> компилируется и работает
   */

  printf("===============================================================================\n");
  printf("Check Frame2Interface class\n");
  frame2::Frame2 frm2(12);
  frame2::Frame2Interface<frame2::Frame2InterfaceGloL1of> frm2i(frm2);
  printf("Id.channel: %d\n", static_cast<int>(frm2i.id(0).channel()));
  printf("Obs.dfs: %f\n", static_cast<double>(frm2i.obs(0).dfs()));
  printf("Aux.state: %d\n", static_cast<int>(frm2i.aux(0).state()));
#if __cplusplus >= 201402L
  printf("Msg.crc_error: %d\n", static_cast<int>(frm2i.msg(0).crc_error()));
#endif
}

