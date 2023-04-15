/*
 * test-frame2.cpp
 *
 *  Created on: 19 июн. 2019 г.
 *      Author: user
 */

#include <cstring>
#include <cstdlib>

#include "catch.hpp"

#include <frame2/frame2.h>

#include "miscellaneous.h"
#include "test_binary_format.h"

#pragma pack(push,1)
struct BinaryFrame2_3sys
{
  FrameHdr hdr;

  /*
   * Galileo
   */
  SubframeHdr gal_subhdr;
  /* Запись Galileo №0 */
  BinaryId gal0_id;
  BinaryObs gal0_e1bc;
  /* Запись Galileo №1 */
  BinaryId gal1_id;
  /* Запись Galileo №2 */
  BinaryId gal2_id;

  /*
   * GPS
   */
  SubframeHdr gps_subhdr;   // 5
  /* Запись GPS №0 */
  BinaryId gps0_id;     // 20
  BinaryObs gps0_l1ca;  // 32
  /* Запись GPS №1 */
  BinaryId gps1_id;     //20
  /* Запись GPS №2 */
  BinaryId gps2_id;     //20
  BinaryAux gps2_aux_l1ca;  // 7
  BinaryObs gps2_l1ca;      // 32
  BinaryMsgGpsL1ca gps2_msg_l1ca;   // 40

  /*
   * ГЛОНАСС
   */
  SubframeHdr glo_subhdr;
  /* Запись ГЛОНАСС №0 */
  BinaryId glo0_id;
  BinaryObs glo0_l1of;
  /* Запись ГЛОНАСС №1 */
  BinaryId glo1_id;
  BinaryObs glo1_l2of;
  /* Запись ГЛОНАСС №2 */
  BinaryId glo2_id;
  BinaryObs glo2_l1of;
  BinaryObs glo2_l2of;
  BinaryMsgOf glo2_msg_l1of;
  BinaryMsgOf glo2_msg_l2of;
};
#pragma pack(pop)

static const int max_channel_cnt = 3;
extern const uint32_t frame_version = 5;
extern const uint32_t frame_id = 0x326d7266;

static const FrameHdr const_frame_hdr = {frame_id, 0, frame_version, 0x1032547698badcfell, 1, 0x6fcdab897e452301ll};

void fill_f2_3sys_struct(BinaryFrame2_3sys &bin)
{
  constexpr size_t bin_size = sizeof(BinaryFrame2_3sys);

  /* Данные заполняем случайными числами */
  uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  for (size_t i = 0; i < bin_size; i++)
  {
    u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
  }

  /* Обязательные поля заоплняем данными */
  bin.hdr = const_frame_hdr;
  bin.hdr.size = bin_size;
  /* Обязательные поля Galileo */
  bin.gal_subhdr.subframe_size = offsetof(BinaryFrame2_3sys, gps_subhdr) - offsetof(BinaryFrame2_3sys, gal_subhdr);
  bin.gal_subhdr.id = 3; /* идентификатор Galileo */
  bin.gal0_id.mSetOfObsFlags = 0x01;
  bin.gal0_id.mSetOfMsgFlags = 0;
  bin.gal0_id.mSetOfAuxFlags = 0;
  bin.gal1_id.mSetOfObsFlags = 0;
  bin.gal1_id.mSetOfMsgFlags = 0;
  bin.gal1_id.mSetOfAuxFlags = 0;
  bin.gal2_id.mSetOfObsFlags = 0;
  bin.gal2_id.mSetOfMsgFlags = 0;
  bin.gal2_id.mSetOfAuxFlags = 0;
  /* Обязательные поля GPS */
  bin.gps_subhdr.subframe_size = offsetof(BinaryFrame2_3sys, glo_subhdr) - offsetof(BinaryFrame2_3sys, gps_subhdr);
  bin.gps_subhdr.id = 2; /* идентификатор GPS */
  bin.gps0_id.mSetOfObsFlags = 0x01;
  bin.gps0_id.mSetOfMsgFlags = 0;
  bin.gps0_id.mSetOfAuxFlags = 0;
  bin.gps1_id.mSetOfObsFlags = 0x00;
  bin.gps1_id.mSetOfMsgFlags = 0;
  bin.gps1_id.mSetOfAuxFlags = 0;
  bin.gps2_id.mSetOfObsFlags = 0x01;
  bin.gps2_id.mSetOfMsgFlags = 0x01;
  bin.gps2_id.mSetOfAuxFlags = 0x01;
  bin.gps2_aux_l1ca.fields_existence = 0;
  bin.gps2_msg_l1ca.size = sizeof(bin.gps2_msg_l1ca);
  /* Обязательные поля ГЛОНАСС */
  bin.glo_subhdr.subframe_size = bin_size - offsetof(BinaryFrame2_3sys, glo_subhdr);
  bin.glo_subhdr.id = 1; /* идентификатор ГЛОНАСС */
  bin.glo0_id.mSetOfObsFlags = 0x01;
  bin.glo0_id.mSetOfMsgFlags = 0;
  bin.glo0_id.mSetOfAuxFlags = 0;
  bin.glo1_id.mSetOfObsFlags = 0x02;
  bin.glo1_id.mSetOfMsgFlags = 0;
  bin.glo1_id.mSetOfAuxFlags = 0;
  bin.glo2_id.mSetOfObsFlags = 0x03;
  bin.glo2_id.mSetOfMsgFlags = 0x03;
  bin.glo2_id.mSetOfAuxFlags = 0;
  bin.glo2_msg_l1of.size = sizeof(bin.glo2_msg_l1of);
  bin.glo2_msg_l2of.size = sizeof(bin.glo2_msg_l2of);
}

void
test_copy_constructor(frame2::Frame2 frm)
{
  REQUIRE ( frm.version() == frame_version );
}

TEST_CASE( "Class frame2::Frame2 serialization", "[Glo, Gps, Gal]" )
{
  frame2::Frame2 frame(max_channel_cnt);

  frame2::Frame2 f2(max_channel_cnt);
  f2 = frame;

  test_copy_constructor(frame);

  REQUIRE ( frame.version() == frame_version );
  REQUIRE ( frame.glo_size() == 0 );
  REQUIRE ( frame.gps_size() == 0 );
  REQUIRE ( frame.gal_size() == 0 );
  REQUIRE ( frame.SizeOfOmitedData() == 0 );
  REQUIRE ( frame.UnrecognizedSubframes() == 0 );

  /* Тест с кадром, заполненным нулями */
  SECTION ( "check Frame2 deserialization with zero data" )
  {
    const size_t buf_size = 100;
    uint8_t buf[buf_size];
    std::memset(buf, 0x00, buf_size);
    size_t ret_size = frame.ParseFromArray(buf, buf_size);
    REQUIRE ( ret_size == 0 );
    REQUIRE ( frame.glo_size() == 0 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );
  }

  SECTION ( "check add_glo()" )
  {
    for (int i = 0; i < max_channel_cnt; i++)
    {
      frame2::Glo *pglo = frame.add_glo();
      REQUIRE ( pglo != nullptr );
      REQUIRE ( frame.glo_size() == (i + 1) );
    }
    frame2::Glo *pglo = frame.add_glo();
    REQUIRE ( pglo == nullptr );
    REQUIRE ( frame.glo_size() == max_channel_cnt );
  }

  SECTION ( "check add_gps()" )
  {
    for (int i = 0; i < max_channel_cnt; i++)
    {
      frame2::Gps *pgps = frame.add_gps();
      REQUIRE ( pgps != nullptr );
      REQUIRE ( frame.gps_size() == (i + 1) );
    }
    frame2::Gps *pgps = frame.add_gps();
    REQUIRE ( pgps == nullptr );
    REQUIRE ( frame.gps_size() == max_channel_cnt );
  }

  SECTION ( "check add_gal()" )
  {
    for (int i = 0; i < max_channel_cnt; i++)
    {
      frame2::Gal *pgal = frame.add_gal();
      REQUIRE ( pgal != nullptr );
      REQUIRE ( frame.gal_size() == (i + 1) );
    }
    frame2::Gal *pgal = frame.add_gal();
    REQUIRE ( pgal == nullptr );
    REQUIRE ( frame.gal_size() == max_channel_cnt );
  }

  SECTION ( "check empty Frame2 serialization/deserialization" )
  {
    FrameHdr bin;
    bin = const_frame_hdr;
    const size_t bin_size = sizeof(bin);

    /* В бинарных данных ошибка: длина кадра FrameHdr::size == 0 */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == 0 );
    REQUIRE ( frame.glo_size() == 0 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );

    /* Теперь устраним ошибку и опять разберем кадр */
    bin.size = bin_size;
    ret_size = frame.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 0 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    REQUIRE ( frame.header().timestamp_hw_ms() == bin.timestamp_hw_ms );
    REQUIRE ( frame.header().timestamp_ms() == bin.timestamp_ms );

    /* Выполним сериализацию кадра */
    uint8_t buf[bin_size + 1];
    ret_size = frame.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with one Glo" )
  {
#pragma pack(push,1)
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr subhdr;
      BinaryId glo_id;
      BinaryObs glo_l1of;
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const size_t bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr.subframe_size = sizeof(SubframeHdr) + sizeof(BinaryId) + sizeof(BinaryObs);
    bin.subhdr.id = 1; /* идентификатор ГЛОНАСС */
    bin.glo_id.mSetOfObsFlags = 0x01;
    bin.glo_id.mSetOfMsgFlags = 0;
    bin.glo_id.mSetOfAuxFlags = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 1 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    REQUIRE ( frame.glo(0).has_l1of_obs() );
    REQUIRE_FALSE ( frame.glo(0).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(0).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(0).has_l2of_msg() );
    REQUIRE_FALSE ( frame.glo(0).has_l1of_aux() );
    REQUIRE_FALSE ( frame.glo(0).has_l2of_aux() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    ret_size = frame.SerializeToArray(buf, bin_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with three Glo" )
  {
#pragma pack(push,1)
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr subhdr;

      /* Запись ГЛОНАСС №0 */
      BinaryId glo0_id;
      BinaryObs glo0_l1of;

      /* Запись ГЛОНАСС №1 */
      BinaryId glo1_id;
      BinaryObs glo1_l2of;

      /* Запись ГЛОНАСС №2 */
      BinaryId glo2_id;
      BinaryObs glo2_l1of;
      BinaryObs glo2_l2of;
      BinaryMsgOf glo2_msg_l1of;
      BinaryMsgOf glo2_msg_l2of;
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const size_t bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr.subframe_size = bin_size - sizeof(FrameHdr);
    bin.subhdr.id = 1; /* идентификатор ГЛОНАСС */
    bin.glo0_id.mSetOfObsFlags = 0x01;
    bin.glo0_id.mSetOfMsgFlags = 0;
    bin.glo0_id.mSetOfAuxFlags = 0;
    bin.glo1_id.mSetOfObsFlags = 0x02;
    bin.glo1_id.mSetOfMsgFlags = 0;
    bin.glo1_id.mSetOfAuxFlags = 0;
    bin.glo2_id.mSetOfObsFlags = 0x03;
    bin.glo2_id.mSetOfMsgFlags = 0x03;
    bin.glo2_id.mSetOfAuxFlags = 0;
    bin.glo2_msg_l1of.size = sizeof(bin.glo2_msg_l1of);
    bin.glo2_msg_l2of.size = sizeof(bin.glo2_msg_l2of);

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 3 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    int index = 0;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 1;
    REQUIRE_FALSE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 2;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE ( frame.glo(index).has_l1of_msg() );
    REQUIRE ( frame.glo(index).has_l2of_msg() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    ret_size = frame.SerializeToArray(buf, bin_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with too much Glo" )
  {
    /* Здесь проверяем десериализацию/сериализацию, когда в бинарных данных содежится
     * больше аппаратов, чем было выделено памяти в конструкторе Frame2 */

#pragma pack(push,1)
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr subhdr;

      /* Запись ГЛОНАСС №0 */
      BinaryId glo0_id;
      BinaryObs glo0_l1of;

      /* Запись ГЛОНАСС №1 */
      BinaryId glo1_id;
      BinaryObs glo1_l2of;

      /* Запись ГЛОНАСС №2 */
      BinaryId glo2_id;
      BinaryObs glo2_l1of;
      BinaryObs glo2_l2of;
      BinaryMsgOf glo2_msg_l1of;
      BinaryMsgOf glo2_msg_l2of;

      /*
       * При создании объекта frame в конструкторе было указано максимальное
       * число каналов max_channel_cnt = 3. Следовательно, при десериализации
       * следующие записи не должны быть прочитаны
       */

      /* Запись ГЛОНАСС №3 */
      BinaryId glo3_id;
      BinaryObs glo3_l1of;

      /* Запись ГЛОНАСС №4 */
      BinaryId glo4_id;
      BinaryObs glo4_l1of;
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const int bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr.subframe_size = bin_size - sizeof(FrameHdr);
    bin.subhdr.id = 1; /* идентификатор ГЛОНАСС */
    bin.glo0_id.mSetOfObsFlags = 0x01;
    bin.glo0_id.mSetOfMsgFlags = 0;
    bin.glo0_id.mSetOfAuxFlags = 0;
    bin.glo1_id.mSetOfObsFlags = 0x02;
    bin.glo1_id.mSetOfMsgFlags = 0;
    bin.glo1_id.mSetOfAuxFlags = 0;
    bin.glo2_id.mSetOfObsFlags = 0x03;
    bin.glo2_id.mSetOfMsgFlags = 0x03;
    bin.glo2_id.mSetOfAuxFlags = 0;
    bin.glo2_msg_l1of.size = sizeof(bin.glo2_msg_l1of);
    bin.glo2_msg_l2of.size = sizeof(bin.glo2_msg_l2of);
    bin.glo3_id.mSetOfObsFlags = 0x01;
    bin.glo3_id.mSetOfMsgFlags = 0;
    bin.glo3_id.mSetOfAuxFlags = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 3 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    int expected_omited_data_size = 2 * (sizeof (BinaryId) + sizeof (BinaryObs));
    REQUIRE ( frame.SizeOfOmitedData() == expected_omited_data_size );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    int index = 0;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 1;
    REQUIRE_FALSE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 2;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE ( frame.glo(index).has_l1of_msg() );
    REQUIRE ( frame.glo(index).has_l2of_msg() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    std::memset (&buf, 0x00, sizeof (buf));
    /* Так как не все данные были десериализированы, не и не все будут сереализированы */
    int serialized_size = (bin_size - expected_omited_data_size);
    BinaryFrame2 bin_cutted = bin;
    bin_cutted.hdr.size = serialized_size;
    bin_cutted.subhdr.subframe_size = static_cast<uint32_t>(serialized_size - sizeof(FrameHdr));
    ret_size = frame.SerializeToArray(&buf, serialized_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(&buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == serialized_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(&buf, &bin_cutted, serialized_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == serialized_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin_cutted, serialized_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with one Gps" )
  {
#pragma pack(push,1)
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr subhdr;
      BinaryId gps_id;
      BinaryObs gps_l1of;
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const size_t bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr.subframe_size = sizeof(SubframeHdr) + sizeof(BinaryId) + sizeof(BinaryObs);
    bin.subhdr.id = 2; /* идентификатор ГЛОНАСС */
    bin.gps_id.mSetOfObsFlags = 0x01;
    bin.gps_id.mSetOfMsgFlags = 0;
    bin.gps_id.mSetOfAuxFlags = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 0 );
    REQUIRE ( frame.gps_size() == 1 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    REQUIRE ( frame.gps(0).has_l1ca_obs() );
    REQUIRE_FALSE ( frame.gps(0).has_l1ca_aux() );
    REQUIRE_FALSE ( frame.gps(0).has_l1ca_msg() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    ret_size = frame.SerializeToArray(buf, bin_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with too much Gps" )
  {
    /* Здесь проверяем десериализацию/сериализацию, когда в бинарных данных содежится
     * больше аппаратов, чем было выделено памяти в конструкторе Frame2 */

#pragma pack(push,1)
    struct BinaryFrame2
    {
      FrameHdr hdr;         // 28
      SubframeHdr subhdr;   // 5

      /* Запись GPS №0 */
      BinaryId gps0_id;     // 20
      BinaryObs gps0_l1ca;  // 32

      /* Запись GPS №1 */
      BinaryId gps1_id;     //20

      /* Запись GPS №2 */
      BinaryId gps2_id;     //20
      BinaryAux gps2_aux_l1ca;  // 7
      BinaryObs gps2_l1ca;      // 32
      BinaryMsgGpsL1ca gps2_msg_l1ca;   // 40

      /*
       * При создании объекта frame в конструкторе было указано максимальное
       * число каналов max_channel_cnt = 3. Следовательно, при десериализации
       * следующие записи не должны быть прочитаны
       */

      /* Запись GPS №3 */
      BinaryId gps3_id;     // 20
      BinaryObs gps3_l1ca;  // 32

      /* Запись GPS №4 */
      BinaryId gps4_id;     // 20
      BinaryObs gps4_l1ca;  // 32
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const int bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr.subframe_size = bin_size - sizeof(FrameHdr);
    bin.subhdr.id = 2; /* идентификатор GPS */
    bin.gps0_id.mSetOfObsFlags = 0x01;
    bin.gps0_id.mSetOfMsgFlags = 0;
    bin.gps0_id.mSetOfAuxFlags = 0;
    bin.gps1_id.mSetOfObsFlags = 0x00;
    bin.gps1_id.mSetOfMsgFlags = 0;
    bin.gps1_id.mSetOfAuxFlags = 0;
    bin.gps2_id.mSetOfObsFlags = 0x01;
    bin.gps2_id.mSetOfMsgFlags = 0x01;
    bin.gps2_id.mSetOfAuxFlags = 0x01;
    bin.gps2_msg_l1ca.size = sizeof(BinaryMsgGpsL1ca);
    bin.gps3_id.mSetOfObsFlags = 0x01;
    bin.gps3_id.mSetOfMsgFlags = 0;
    bin.gps3_id.mSetOfAuxFlags = 0;
    bin.gps4_id.mSetOfObsFlags = 0x01;
    bin.gps4_id.mSetOfMsgFlags = 0;
    bin.gps4_id.mSetOfAuxFlags = 0;
    bin.gps2_aux_l1ca.fields_existence = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 0 );
    REQUIRE ( frame.gps_size() == 3 );
    REQUIRE ( frame.gal_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    int expected_omited_data_size = 2 * (sizeof (BinaryId) + sizeof (BinaryObs));
    REQUIRE ( frame.SizeOfOmitedData() == expected_omited_data_size );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    int index = 0;
    REQUIRE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_aux() );

    index = 1;
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_aux() );

    index = 2;
    REQUIRE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE ( frame.gps(index).has_l1ca_aux() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    std::memset (&buf, 0x00, sizeof (buf));
    /* Так как не все данные были десериализированы, не и не все будут сереализированы */
    int serialized_size = (bin_size - expected_omited_data_size);
    BinaryFrame2 bin_cutted = bin;
    bin_cutted.hdr.size = serialized_size;
    bin_cutted.subhdr.subframe_size = static_cast<uint32_t>(serialized_size - sizeof(FrameHdr));
    ret_size = frame.SerializeToArray(&buf, serialized_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    ret_size = frame.SerializeToArray(&buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == serialized_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(&buf, &bin_cutted, serialized_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == serialized_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin_cutted, serialized_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with three Glo, three Gps and three Gal" )
  {
#pragma pack(push,1)
    struct BinaryFrame2_seqv
    {
      FrameHdr hdr;

      /*
       * ГЛОНАСС
       */
      SubframeHdr glo_subhdr;
      /* Запись ГЛОНАСС №0 */
      BinaryId glo0_id;
      BinaryObs glo0_l1of;
      /* Запись ГЛОНАСС №1 */
      BinaryId glo1_id;
      BinaryObs glo1_l2of;
      /* Запись ГЛОНАСС №2 */
      BinaryId glo2_id;
      BinaryObs glo2_l1of;
      BinaryObs glo2_l2of;
      BinaryMsgOf glo2_msg_l1of;
      BinaryMsgOf glo2_msg_l2of;

      /*
       * GPS
       */
      SubframeHdr gps_subhdr;   // 5
      /* Запись GPS №0 */
      BinaryId gps0_id;     // 20
      BinaryObs gps0_l1ca;  // 32
      /* Запись GPS №1 */
      BinaryId gps1_id;     //20
      /* Запись GPS №2 */
      BinaryId gps2_id;     //20
      BinaryAux gps2_aux_l1ca;  // 7
      BinaryObs gps2_l1ca;      // 32
      BinaryMsgGpsL1ca gps2_msg_l1ca;   // 40

      /*
       * Galileo
       */
      SubframeHdr gal_subhdr;
      /* Запись Galileo №0 */
      BinaryId gal0_id;
      BinaryObs gal0_l1ca;
      /* Запись Galileo №1 */
      BinaryId gal1_id;
      /* Запись Galileo №2 */
      BinaryId gal2_id;
    };
#pragma pack(pop)

    BinaryFrame2_3sys bin;
    constexpr size_t bin_size = sizeof(bin);

    fill_f2_3sys_struct(bin);

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 3 );
    REQUIRE ( frame.gps_size() == 3 );
    REQUIRE ( frame.gal_size() == 3 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    /* ГЛОНАСС */
    int index = 0;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 1;
    REQUIRE_FALSE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE_FALSE ( frame.glo(index).has_l1of_msg() );
    REQUIRE_FALSE ( frame.glo(index).has_l2of_msg() );

    index = 2;
    REQUIRE ( frame.glo(index).has_l1of_obs() );
    REQUIRE ( frame.glo(index).has_l2of_obs() );
    REQUIRE ( frame.glo(index).has_l1of_msg() );
    REQUIRE ( frame.glo(index).has_l2of_msg() );

    /* GPS */
    index = 0;
    REQUIRE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_aux() );

    index = 1;
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE_FALSE ( frame.gps(index).has_l1ca_aux() );

    index = 2;
    REQUIRE ( frame.gps(index).has_l1ca_obs() );
    REQUIRE ( frame.gps(index).has_l1ca_msg() );
    REQUIRE ( frame.gps(index).has_l1ca_aux() );

    /* Galileo */
    index = 0;
    REQUIRE ( frame.gal(index).has_e1bc_obs() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_msg() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_aux() );

    index = 1;
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_obs() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_msg() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_aux() );

    index = 2;
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_obs() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_msg() );
    REQUIRE_FALSE ( frame.gal(index).has_e1bc_aux() );

    /* сериализируем в буфер */
    uint8_t buf[bin_size + 1];
    ret_size = frame.SerializeToArray(buf, bin_size-1); /* Маленький буфер */
    REQUIRE ( ret_size == 0 ); /* сериализация не должна выполняться */

    /*
     * Так как при сериализации сначала будет ГЛОНАСС, а затем GPS, то создадим еще одну бинарную
     * структуру с теми же данными, но в соответствующей последовательности
     */
    BinaryFrame2_seqv sqv = {bin.hdr,
        /* ГЛОНАСС */
        bin.glo_subhdr, bin.glo0_id, bin.glo0_l1of,
        bin.glo1_id, bin.glo1_l2of,
        bin.glo2_id, bin.glo2_l1of, bin.glo2_l2of, bin.glo2_msg_l1of, bin.glo2_msg_l2of,
        /* GPS */
        bin.gps_subhdr, bin.gps0_id, bin.gps0_l1ca,
        bin.gps1_id,
        bin.gps2_id, bin.gps2_aux_l1ca, bin.gps2_l1ca, bin.gps2_msg_l1ca,
        /* Galileo */
        bin.gal_subhdr, bin.gal0_id, bin.gal0_e1bc, bin.gal1_id, bin.gal2_id
    };

//    std::cout << "sqv: ";
//    hexdump(&sqv, bin_size);
    ret_size = frame.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
//    std::cout << "buf: ";
//    hexdump(buf, bin_size);
//    std::cout << "dif: ";
//    for (size_t i = 0; i < bin_size; i++)
//    {
//      uint8_t *sqvt = (uint8_t*)(&sqv);
//      if (sqvt[i] == buf[i])
//        std::cout << " 0 ";
//      else
//        std::cout << " 1 ";
//    }
//    std::cout << std::endl;
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &sqv, bin_size) == 0 );

    ret_size = frame.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &sqv, bin_size) == 0 );
  }

  SECTION ( "check Frame2 serialization/deserialization with unrecognized subframe" )
  {
#pragma pack(push,1)
    const size_t subframe_unrecognized_size = 100;
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr subhdr_unrecognized;
      uint8_t subframe_unrecognized[subframe_unrecognized_size];
      SubframeHdr glo_subhdr;
      BinaryId glo0_id;
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const size_t bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.subhdr_unrecognized.subframe_size = sizeof(SubframeHdr) + subframe_unrecognized_size;
    bin.subhdr_unrecognized.id = 0xfd;
    bin.glo_subhdr.subframe_size = static_cast<uint32_t>(bin_size - sizeof(FrameHdr) - bin.subhdr_unrecognized.subframe_size);
    bin.glo_subhdr.id = 1; /* идентификатор ГЛОНАСС */
    bin.glo0_id.mSetOfObsFlags = 0;
    bin.glo0_id.mSetOfMsgFlags = 0;
    bin.glo0_id.mSetOfAuxFlags = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == bin_size );
    REQUIRE ( frame.glo_size() == 1 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == 0 );
    REQUIRE ( frame.UnrecognizedSubframes() == 1 );

    /* Выполняем сериализацию. Так как не распознанный подкадр не будет добавлен,
     * то структура данных после сериализации будет другой */

#pragma pack(push,1)
    struct BinaryFrame2Correct
    {
      FrameHdr hdr;
      SubframeHdr glo_subhdr;
      BinaryId glo0_id;
    };
#pragma pack(pop)

    BinaryFrame2Correct bin_correct;
    const size_t bin_correct_size = sizeof(bin_correct);

    /* Копируем данные */
    bin_correct.hdr = bin.hdr;
    bin_correct.glo_subhdr = bin.glo_subhdr;
    bin_correct.glo0_id = bin.glo0_id;

    /* Исправляем размеры */
    bin_correct.hdr.size = bin_correct_size;
    bin_correct.glo_subhdr.subframe_size = bin_correct_size - sizeof(FrameHdr);

    /* сериализируем в буфер */
    uint8_t buf[bin_correct_size];

    ret_size = frame.SerializeToArray(buf, bin_correct_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_correct_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin_correct, bin_correct_size) == 0 );

  }

  /* Тест с кадром, в котором часть кадра заполнена нулями */
  SECTION ( "check Frame2 serialization/deserialization with zero subframe" )
  {
#pragma pack(push,1)
    const size_t subframe_zero_size = 100;
    struct BinaryFrame2
    {
      FrameHdr hdr;
      SubframeHdr glo_subhdr;
      BinaryId glo0_id;
      uint8_t subframe_sero[subframe_zero_size];
    };
#pragma pack(pop)

    BinaryFrame2 bin;
    const size_t bin_size = sizeof(bin);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }
    std::memset(bin.subframe_sero, 0x00, subframe_zero_size);

    /* Обязательные поля заоплняем данными */
    bin.hdr = const_frame_hdr;
    bin.hdr.size = bin_size;
    bin.glo_subhdr.subframe_size = bin_size - sizeof(FrameHdr) - subframe_zero_size;
    bin.glo_subhdr.id = 1; /* идентификатор ГЛОНАСС */
    bin.glo0_id.mSetOfObsFlags = 0;
    bin.glo0_id.mSetOfMsgFlags = 0;
    bin.glo0_id.mSetOfAuxFlags = 0;

    /* Разбираем кадр */
    size_t ret_size = frame.ParseFromArray(&bin, bin_size);
    REQUIRE ( ret_size == (bin_size - subframe_zero_size) );
    REQUIRE ( frame.glo_size() == 1 );
    REQUIRE ( frame.gps_size() == 0 );
    REQUIRE ( frame.version() == frame_version );
    REQUIRE ( frame.SizeOfOmitedData() == subframe_zero_size );
    REQUIRE ( frame.UnrecognizedSubframes() == 0 );

    /* Выполняем сериализацию. Так как не распознанный подкадр не будет добавлен,
     * то структура данных после сериализации будет другой */

#pragma pack(push,1)
    struct BinaryFrame2Correct
    {
      FrameHdr hdr;
      SubframeHdr glo_subhdr;
      BinaryId glo0_id;
    };
#pragma pack(pop)

    BinaryFrame2Correct bin_correct;
    const size_t bin_correct_size = sizeof(bin_correct);

    /* Копируем данные */
    bin_correct.hdr = bin.hdr;
    bin_correct.glo_subhdr = bin.glo_subhdr;
    bin_correct.glo0_id = bin.glo0_id;

    /* Исправляем размеры */
    bin_correct.hdr.size = bin_correct_size;
    bin_correct.glo_subhdr.subframe_size = bin_correct_size - sizeof(FrameHdr);

    /* сериализируем в буфер */
    uint8_t buf[bin_correct_size];

    ret_size = frame.SerializeToArray(buf, bin_correct_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_correct_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin_correct, bin_correct_size) == 0 );

  }

  SECTION ( "check Frame2 assignment" )
  {
    /* Создаем и заполняем бинарную структуру для frame2 */
    BinaryFrame2_3sys bin;
    constexpr size_t bin_size = sizeof(bin);
    fill_f2_3sys_struct(bin);

    /* Теперь frame2 десериализируем в класс */
    frame.ParseFromArray(&bin, bin_size);
    /* После сериализации порядок блоков в frame2 может отличаться от исходного */
    char buf[bin_size];
    size_t ret_size = frame.SerializeToArray(buf, bin_size);
    REQUIRE ( ret_size == bin_size ); /* Проверили, что все сериализировалось */

    /* Создаем новый объект frame2 с меньшим объемом зарезервированой памяти под каналы */
    frame2::Frame2 frame_second(max_channel_cnt - 1);

    /* Присваиваем */
    frame_second = frame;
    REQUIRE ( frame.glo_size() == 3 );
    REQUIRE ( frame.gps_size() == 3 );
    REQUIRE ( frame.gal_size() == 3 );
    REQUIRE ( frame.glo_size() == frame_second.glo_size() );
    REQUIRE ( frame.gps_size() == frame_second.gps_size() );
    REQUIRE ( frame.gal_size() == frame_second.gal_size() );

    /* Сериализируем новый объект и сравниваем бинарные данные */
    char buf_second[bin_size];
    ret_size = frame_second.SerializeToArray(buf_second, bin_size);
    REQUIRE ( ret_size == bin_size ); /* Проверили, что все сериализировалось */
    REQUIRE ( memcmp(buf, buf_second, bin_size) == 0 );
  }
}
