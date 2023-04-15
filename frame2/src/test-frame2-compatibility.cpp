/*
 * test-frame2-compatibility.cpp
 *
 *  Created on: 8 июл. 2020 г.
 *      Author: user
 */

#include <cstring>
#include <cstdlib>

#include "catch.hpp"

#include <frame2/frame2.h>

#include "test_binary_format.h"

extern const uint32_t frame_version;
extern const uint32_t frame_id;

#pragma pack(push,1)
struct BinaryFrame2_comptibility_t
{
  FrameHdr hdr;
  SubframeHdr glo_subhdr;
  BinaryId glo0_id;
  BinaryAux glo0_aux;
  BinaryObs glo0_obs;
  BinaryMsgOf glo0_msg;
};
#pragma pack(pop)

TEST_CASE( "Class frame2::Frame2 comptibility test", "[comptibility]" )
{
  constexpr int max_ch_cnt = 12;
  frame2::Frame2 frame(max_ch_cnt);

  BinaryFrame2_comptibility_t bin;
  constexpr size_t bin_size = sizeof(BinaryFrame2_comptibility_t);

  /* Данные заполняем случайными числами */
  uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  for (size_t i = 0; i < bin_size; i++)
  {
    u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
  }

  bin.hdr.frame_id = frame_id;
  bin.hdr.version = frame_version;
  bin.hdr.size = bin_size;

  bin.glo_subhdr.subframe_size = bin_size - offsetof(BinaryFrame2_comptibility_t, glo_subhdr);
  bin.glo_subhdr.id = 1; /* идентификатор ГЛОНАСС */
  bin.glo0_id.mSetOfObsFlags = 0x8000;
  bin.glo0_id.mSetOfMsgFlags = 0x8000;
  bin.glo0_id.mSetOfAuxFlags = 0x8000;
  bin.glo0_msg.size = sizeof(bin.glo0_msg);
  bin.glo0_aux.fields_existence = 0;

  /* Разбираем кадр */
  size_t ret_size = frame.ParseFromArray(&bin, bin_size);
  /* Проверяем результаты */
  REQUIRE ( ret_size == bin_size );
  REQUIRE ( frame.glo_size() == 1 );
  REQUIRE ( frame.gps_size() == 0 );
  REQUIRE ( frame.gal_size() == 0 );
  REQUIRE ( frame.version() == frame_version );
  REQUIRE ( frame.SizeOfOmitedData() == 0 );
  REQUIRE ( frame.UnrecognizedSubframes() == 0 );

  /* Проверяем, что в записи нет прочитаных компонент */
  size_t index = 0;
  REQUIRE ( frame.glo(index).has_l1of_aux() == false );
  REQUIRE ( frame.glo(index).has_l1of_obs() == false );
  REQUIRE ( frame.glo(index).has_l1of_msg() == false );

}
