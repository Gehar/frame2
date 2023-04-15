/*
 * Gps_test.cpp
 *
 *  Created on: 20 авг. 2019 г.
 *      Author: user
 */

#include <cstring>
#include <limits>

#include "catch.hpp"

#include <frame2/gps.h>

#include "test_binary_format.h"

static const BinaryId const_id = {0, 0, 0, 0x0fed, 24, -111, 221, 359.99f, -89.99f};
static const BinaryObs const_l1ca_obs = {12234245.312, -2134.798, 1.1265f, 49.384f};
static const BinaryAux const_aux = {6, 0, 0};
static const BinaryMsgGpsL1ca const_l1ca_msg = {sizeof(BinaryMsgGpsL1ca), 0x21, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};

TEST_CASE( "Class frame2::Gps serialization", "[Gps]" )
{
  frame2::Gps gps;

  REQUIRE ( gps.ByteSize() == sizeof(BinaryId));
  REQUIRE_FALSE ( gps.has_l1ca_obs() );
  REQUIRE_FALSE ( gps.has_l1ca_aux() );
  REQUIRE_FALSE ( gps.has_l1ca_msg() );

  SECTION( "check empty Gps serialization/deserialization" )
  {
    BinaryId id = const_id;
    size_t id_size = sizeof(id);

    /* Проверяем разбор данных по аппарату без РНП и ЦИ */
    size_t ret_size = gps.ParseFromArray(&id, id_size);

    REQUIRE ( ret_size == id_size );
    REQUIRE_FALSE ( gps.has_l1ca_obs() );
    REQUIRE_FALSE ( gps.has_l1ca_aux() );
    REQUIRE_FALSE ( gps.has_l1ca_msg() );
    REQUIRE ( gps.id().channel() == id.channel );
    REQUIRE ( gps.id().prn_num() == id.prn_num );
    REQUIRE ( gps.id().freq_num() == id.freq_num );
    REQUIRE ( gps.id().sys_num() == id.sys_num );
    REQUIRE ( gps.id().azimuth() == id.azimuth );
    REQUIRE ( gps.id().elevation() == id.elevation );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryId)+1;
    uint8_t buf[buf_size];
    ret_size = gps.SerializeToArray(buf, id_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    ret_size = gps.SerializeToArray(buf, id_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == id_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &const_id, id_size) == 0 );

    ret_size = gps.SerializeToArray(buf, id_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == id_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &const_id, id_size) == 0 );
  }

  SECTION( "add L1CA block" )
  {
    gps.add_l1ca_obs();

    REQUIRE ( gps.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE ( gps.has_l1ca_obs() );
    REQUIRE_FALSE ( gps.has_l1ca_aux() );
    REQUIRE_FALSE ( gps.has_l1ca_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OF RNP data" )
  {
    BinaryIdObs bin;
    bin.id = const_id;
    bin.id.mSetOfObsFlags = 0x01;
    bin.obs = const_l1ca_obs;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с РНП по L1OF, но без ЦИ */
    size_t ret_size = gps.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( gps.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE ( gps.has_l1ca_obs() );
    REQUIRE_FALSE ( gps.has_l1ca_aux() );
    REQUIRE_FALSE ( gps.has_l1ca_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( gps.id().channel() == bin.id.channel );
    REQUIRE ( gps.id().prn_num() == bin.id.prn_num );
    REQUIRE ( gps.id().freq_num() == bin.id.freq_num );
    REQUIRE ( gps.id().sys_num() == bin.id.sys_num );
    REQUIRE ( gps.id().azimuth() == bin.id.azimuth );
    REQUIRE ( gps.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных РНП L1CA */
//    REQUIRE ( gps.l1ca_obs().amplitude() == bin.obs.amplitude );
    REQUIRE ( gps.l1ca_obs().code_pr() == bin.obs.code_pr );
    REQUIRE ( gps.l1ca_obs().phase_pr() == bin.obs.phase_pr );
    REQUIRE ( gps.l1ca_obs().dfs() == bin.obs.dfs );
//    REQUIRE ( gps.l1ca_obs().phase_std() == bin.obs.phase_std );
    REQUIRE ( gps.l1ca_obs().snr() == bin.obs.snr );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdObs)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Gps serialization/deserialization for L1CA with RNP, DI and Aux" )
  {
    BinaryGps bin;
    bin.id = const_id;
    bin.id.mSetOfObsFlags = 0x03;
    bin.id.mSetOfMsgFlags = 0x03;
    bin.id.mSetOfAuxFlags = 0x03;
    bin.aux_l1ca = const_aux;
    bin.l1ca = const_l1ca_obs;
    bin.msg_l1ca = const_l1ca_msg;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с ЦИ по L1OF, но без РНП */
    size_t ret_size = gps.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( gps.ByteSize() == sizeof(BinaryGps) );
    REQUIRE ( gps.has_l1ca_obs() );
    REQUIRE ( gps.has_l1ca_aux() );
    REQUIRE ( gps.has_l1ca_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( gps.id().channel() == bin.id.channel );
    REQUIRE ( gps.id().prn_num() == bin.id.prn_num );
    REQUIRE ( gps.id().freq_num() == bin.id.freq_num );
    REQUIRE ( gps.id().sys_num() == bin.id.sys_num );
    REQUIRE ( gps.id().azimuth() == bin.id.azimuth );
    REQUIRE ( gps.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных вспомогательного блока L1OF */
    REQUIRE ( gps.l1ca_aux().state() == bin.aux_l1ca.mode );
    REQUIRE ( gps.l1ca_aux().state() == bin.aux_l1ca.mode );

    /* Проверяем состав данных РНП L1OF */
//    REQUIRE ( gps.l1ca_obs().amplitude() == bin.l1ca.amplitude );
    REQUIRE ( gps.l1ca_obs().code_pr() == bin.l1ca.code_pr );
    REQUIRE ( gps.l1ca_obs().phase_pr() == bin.l1ca.phase_pr );
    REQUIRE ( gps.l1ca_obs().dfs() == bin.l1ca.dfs );
//    REQUIRE ( gps.l1ca_obs().phase_std() == bin.l1ca.phase_std );
    REQUIRE ( gps.l1ca_obs().snr() == bin.l1ca.snr );

    /* Проверяем состав данных ЦИ L1OF */
    REQUIRE ( memcmp(&(gps.l1ca_msg().str()), bin.msg_l1ca.str, gps.l1ca_msg().str_size()) == 0 );
    REQUIRE ( gps.l1ca_msg().crc_error() == bin.msg_l1ca.crc_error );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryAllObsMsg)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = gps.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }
}
