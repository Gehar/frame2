/*
 * tests-glo.cpp
 *
 *  Created on: 17 июн. 2019 г.
 *      Author: user
 */

#include <cstring>
#include <limits>

#include "catch.hpp"

#include <frame2/glo.h>

#include "test_binary_format.h"

static const BinaryId const_id = {0, 0, 0, 0x0fed, 24, -100, 190, 359.99f, -89.99f};
static const BinaryObs const_l1of_obs = {20000000.123, -1000.987, 0.5467f, 50.438f};
static const BinaryAux const_aux = {7, 0, 0};
static const BinaryObs const_l2of_obs = {20019300.321, -999.768, 0.4357f, 49.983f};
static const BinaryMsgOf const_l1of_msg = {sizeof(BinaryMsgOf), 0x21, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
static const BinaryMsgOf const_l2of_msg = {sizeof(BinaryMsgOf), 0x78, {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
static const BinaryMsgCode const_l1oc_msg = {sizeof(BinaryMsgCode), 0x43, {2, 3, 5, 7, 9, 11, 10, 8, 6, 4, 1, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                                                                       21, 22, 23, 24, 25, 26, 27, 28, 29}};

TEST_CASE( "Class frame2::Glo serialization", "[Glo]" )
{
  frame2::Glo glo;

  REQUIRE ( glo.ByteSize() == sizeof(BinaryId));
  REQUIRE ( glo.id().channel() == 0);
  REQUIRE ( glo.id().prn_num() == code_num_not_defined);
  REQUIRE ( glo.id().freq_num() == freq_num_not_defined);
  REQUIRE ( glo.id().sys_num() == code_num_not_defined);
  REQUIRE_FALSE ( glo.has_l1of_aux() );
  REQUIRE_FALSE ( glo.has_l1of_obs() );
  REQUIRE_FALSE ( glo.has_l1of_msg() );
  REQUIRE_FALSE ( glo.has_l2of_aux() );
  REQUIRE_FALSE ( glo.has_l2of_obs() );
  REQUIRE_FALSE ( glo.has_l2of_msg() );
  REQUIRE_FALSE ( glo.has_l1oc_aux() );
  REQUIRE_FALSE ( glo.has_l1oc_obs() );
  REQUIRE_FALSE ( glo.has_l1oc_msg() );
  REQUIRE_FALSE ( glo.has_l2q_aux() );
  REQUIRE_FALSE ( glo.has_l2q_obs() );
  REQUIRE_FALSE ( glo.has_l2q_msg() );
  REQUIRE_FALSE ( glo.has_l3oc_aux() );
  REQUIRE_FALSE ( glo.has_l3oc_obs() );
  REQUIRE_FALSE ( glo.has_l3oc_msg() );
  REQUIRE_FALSE ( glo.has_l1sc_aux() );
  REQUIRE_FALSE ( glo.has_l1sc_obs() );
  REQUIRE_FALSE ( glo.has_l1sc_msg() );
  REQUIRE_FALSE ( glo.has_l2sc_aux() );
  REQUIRE_FALSE ( glo.has_l2sc_obs() );
  REQUIRE_FALSE ( glo.has_l2sc_msg() );
  REQUIRE_FALSE ( glo.has_l1cfg_aux() );
  REQUIRE_FALSE ( glo.has_l1cfg_obs() );
  REQUIRE_FALSE ( glo.has_l1cfg_msg() );
  REQUIRE_FALSE ( glo.has_l2cfg_aux() );
  REQUIRE_FALSE ( glo.has_l2cfg_obs() );
  REQUIRE_FALSE ( glo.has_l2cfg_msg() );
  REQUIRE_FALSE ( glo.has_srl_aux() );
  REQUIRE_FALSE ( glo.has_srl_obs() );
  REQUIRE_FALSE ( glo.has_srl_msg() );

  SECTION( "check empty Glo serialization/deserialization" )
  {
    BinaryId id = const_id;
    size_t id_size = sizeof(id);

    /* Проверяем разбор данных по аппарату без РНП и ЦИ */
    size_t ret_size = glo.ParseFromArray(&id, id_size);

    REQUIRE ( ret_size == id_size );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
    REQUIRE ( glo.id().channel() == id.channel );
    REQUIRE ( glo.id().prn_num() == id.prn_num );
    REQUIRE ( glo.id().freq_num() == id.freq_num );
    REQUIRE ( glo.id().sys_num() == id.sys_num );
    REQUIRE ( glo.id().azimuth() == id.azimuth );
    REQUIRE ( glo.id().elevation() == id.elevation );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryId)+1;
    uint8_t buf[buf_size];
    ret_size = glo.SerializeToArray(buf, id_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    ret_size = glo.SerializeToArray(buf, id_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == id_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &const_id, id_size) == 0 );

    ret_size = glo.SerializeToArray(buf, id_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == id_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &const_id, id_size) == 0 );
  }

  SECTION( "add L1OF block" )
  {
    glo.add_l1of_obs();

    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OF RNP data" )
  {
    BinaryIdObs bin;
    bin.id = const_id;
    bin.id.mSetOfObsFlags = 0x01;
    bin.obs = const_l1of_obs;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с РНП по L1OF, но без ЦИ */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE       ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных РНП L1OF */
    REQUIRE ( glo.l1of_obs().code_pr() == bin.obs.code_pr );
    REQUIRE ( glo.l1of_obs().phase_pr() == bin.obs.phase_pr );
    REQUIRE ( glo.l1of_obs().dfs() == bin.obs.dfs );
    REQUIRE ( glo.l1of_obs().snr() == bin.obs.snr );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdObs)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add L2OF block" )
  {
    glo.add_l2of_obs();

    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OF RNP data" )
  {
    BinaryIdObs bin;
    bin.id = const_id;
    bin.id.mSetOfObsFlags = 0x02;
    bin.obs = const_l2of_obs;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с РНП по L1OF, но без ЦИ */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных РНП L2OF */
//    REQUIRE ( glo.l2of_obs().amplitude() == bin.obs.amplitude );
    REQUIRE ( glo.l2of_obs().code_pr() == bin.obs.code_pr );
    REQUIRE ( glo.l2of_obs().phase_pr() == bin.obs.phase_pr );
    REQUIRE ( glo.l2of_obs().dfs() == bin.obs.dfs );
//    REQUIRE ( glo.l2of_obs().phase_std() == bin.obs.phase_std );
    REQUIRE ( glo.l2of_obs().snr() == bin.obs.snr );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdObs)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add msg L1OF block" )
  {
    glo.add_l1of_msg();

    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdMsg));
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE       ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OF Msg data" )
  {
    BinaryIdMsg bin;
    bin.id = const_id;
    bin.id.mSetOfMsgFlags = 0x01;
    bin.msg = const_l1of_msg;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с ЦИ по L1OF, но без РНП */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdMsg) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE       ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных ЦИ L1OF */
    REQUIRE ( memcmp(&(glo.l1of_msg().str()), bin.msg.str, glo.l1of_msg().str_size()) == 0 );
    REQUIRE ( glo.l1of_msg().crc_error() == bin.msg.crc_error );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdMsg)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add msg L2OF block" )
  {
    glo.add_l2of_msg();

    REQUIRE( glo.ByteSize() == sizeof(BinaryIdMsg) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L2OF Msg data" )
  {
    BinaryIdMsg bin;
    bin.id = const_id;
    bin.id.mSetOfMsgFlags = 0x02;
    bin.msg = const_l2of_msg;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с ЦИ по L1OF, но без РНП */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdMsg) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных ЦИ L2OF */
    REQUIRE ( memcmp(&(glo.l2of_msg().str()), bin.msg.str, glo.l2of_msg().str_size()) == 0 );
    REQUIRE ( glo.l2of_msg().crc_error() == bin.msg.crc_error );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdMsg)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add aux L1OF block" )
  {
    glo.add_l1of_aux();

    REQUIRE( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OF Aux data" )
  {
    BinaryIdAux bin;
    bin.id = const_id;
    bin.id.mSetOfAuxFlags = 0x01;
    bin.aux = const_aux;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с одним вспомогательным блоком */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE       ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных */
    REQUIRE ( glo.l1of_aux().state() == bin.aux.mode );
    REQUIRE ( glo.l2of_aux().state() == -1 ); /* Доступ к отсутствующему блоку должен вернуть значение по умолчанию */
    REQUIRE ( glo.l2of_aux().flags() == 0 );
    REQUIRE ( std::isnan(glo.l2of_aux().ampl_data()));
    REQUIRE ( std::isnan(glo.l2of_aux().phase_std_data()));

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdAux)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add aux L2OF block" )
  {
    glo.add_l2of_aux();

    REQUIRE( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L2OF Aux data" )
  {
    BinaryIdAux bin;
    bin.id = const_id;
    bin.id.mSetOfAuxFlags = 0x02;
    bin.aux = const_aux;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с одним вспомогательным блоком */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE       ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных */
    REQUIRE ( glo.l1of_aux().state() == -1 ); /* Доступ к отсутствующему блоку должен вернуть значение по умолчанию */
    REQUIRE ( glo.l2of_aux().state() == bin.aux.mode );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdAux)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add L1OC block" )
  {
    glo.add_l1oc_obs();

    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE       ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OC RNP data" )
  {
    BinaryIdObs bin;
    bin.id = const_id;
    bin.id.mSetOfObsFlags = 0x04;
    bin.obs = const_l1of_obs;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с РНП по L1OF, но без ЦИ */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdObs) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE       ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных РНП L1OF */
    REQUIRE ( glo.l1oc_obs().code_pr() == bin.obs.code_pr );
    REQUIRE ( glo.l1oc_obs().phase_pr() == bin.obs.phase_pr );
    REQUIRE ( glo.l1oc_obs().dfs() == bin.obs.dfs );
    REQUIRE ( glo.l1oc_obs().snr() == bin.obs.snr );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdObs)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add msg L1OC block" )
  {
    glo.add_l1oc_msg();

    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdMsgOc));
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE       ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OC Msg data" )
  {
    BinaryIdMsgOc bin;
    bin.id = const_id;
    bin.id.mSetOfMsgFlags = 0x04;
    bin.msg = const_l1oc_msg;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с ЦИ по L1OF, но без РНП */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdMsgOc) );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE_FALSE ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE       ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных ЦИ L1OC */
    REQUIRE ( memcmp(&(glo.l1oc_msg().str()), bin.msg.str, glo.l1of_msg().str_size()) == 0 );
    REQUIRE ( glo.l1oc_msg().crc_error() == bin.msg.crc_error );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdMsg)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION( "add aux L1OC block" )
  {
    glo.add_l1oc_aux();

    REQUIRE( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE       ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );
  }

  SECTION( "check Glo serialization/deserialization L1OC Aux data" )
  {
    BinaryIdAux bin;
    bin.id = const_id;
    bin.id.mSetOfAuxFlags = 0x04;
    bin.aux = const_aux;
    size_t bin_size = sizeof(bin);

    /* Проверяем разбор данных по аппарату с одним вспомогательным блоком */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryIdAux) );
    REQUIRE_FALSE ( glo.has_l1of_aux() );
    REQUIRE_FALSE ( glo.has_l1of_obs() );
    REQUIRE_FALSE ( glo.has_l1of_msg() );
    REQUIRE_FALSE ( glo.has_l2of_aux() );
    REQUIRE_FALSE ( glo.has_l2of_obs() );
    REQUIRE_FALSE ( glo.has_l2of_msg() );
    REQUIRE       ( glo.has_l1oc_aux() );
    REQUIRE_FALSE ( glo.has_l1oc_obs() );
    REQUIRE_FALSE ( glo.has_l1oc_msg() );
    REQUIRE_FALSE ( glo.has_l2q_aux() );
    REQUIRE_FALSE ( glo.has_l2q_obs() );
    REQUIRE_FALSE ( glo.has_l2q_msg() );
    REQUIRE_FALSE ( glo.has_l3oc_aux() );
    REQUIRE_FALSE ( glo.has_l3oc_obs() );
    REQUIRE_FALSE ( glo.has_l3oc_msg() );
    REQUIRE_FALSE ( glo.has_l1sc_aux() );
    REQUIRE_FALSE ( glo.has_l1sc_obs() );
    REQUIRE_FALSE ( glo.has_l1sc_msg() );
    REQUIRE_FALSE ( glo.has_l2sc_aux() );
    REQUIRE_FALSE ( glo.has_l2sc_obs() );
    REQUIRE_FALSE ( glo.has_l2sc_msg() );
    REQUIRE_FALSE ( glo.has_l1cfg_aux() );
    REQUIRE_FALSE ( glo.has_l1cfg_obs() );
    REQUIRE_FALSE ( glo.has_l1cfg_msg() );
    REQUIRE_FALSE ( glo.has_l2cfg_aux() );
    REQUIRE_FALSE ( glo.has_l2cfg_obs() );
    REQUIRE_FALSE ( glo.has_l2cfg_msg() );
    REQUIRE_FALSE ( glo.has_srl_aux() );
    REQUIRE_FALSE ( glo.has_srl_obs() );
    REQUIRE_FALSE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных */
    REQUIRE ( glo.l1oc_aux().state() == bin.aux.mode );
    REQUIRE ( glo.l1oc_aux().flags() == bin.aux.flags );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryIdAux)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }

  SECTION ( "check Glo serialization/deserialization for all blocks" )
  {
    BinaryAllObsMsg bin;
    constexpr size_t bin_size = sizeof(BinaryAllObsMsg);

    /* Данные заполняем случайными числами */
    uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < bin_size; i++)
    {
      u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
    }

    /*
     * Некоторые поля должны содержать определенные значения
     */
    bin.id.mSetOfAuxFlags = 0x03ff;
    bin.id.mSetOfObsFlags = 0x03ff;
    bin.id.mSetOfMsgFlags = 0x03ff;
    bin.aux_l1of.fields_existence = 0;
    bin.aux_l2of.fields_existence = 0;
    bin.aux_l1oc.fields_existence = 0;
    bin.aux_l2q.fields_existence = 0;
    bin.aux_l3oc.fields_existence = 0;
    bin.aux_l1cfg.fields_existence = 0;
    bin.aux_l2cfg.fields_existence = 0;
    bin.aux_srl.fields_existence = 0;
    bin.aux_l1sc.fields_existence = 0;
    bin.aux_l2sc.fields_existence = 0;
    bin.msg_l1of.size = sizeof(BinaryMsgOf);
    bin.msg_l2of.size = sizeof(BinaryMsgOf);
    bin.msg_l1oc.size = sizeof(BinaryMsgCode);
    bin.msg_l2q.size = sizeof(BinaryMsgCode);
    bin.msg_l3oc.size = sizeof(BinaryMsgL3oc);
    bin.msg_l1cfg.size = sizeof(BinaryMsgCfg);
    bin.msg_l2cfg.size = sizeof(BinaryMsgCfg);
    bin.msg_srl.size = sizeof(BinaryMsgSrl);
    bin.msg_l1sc.size = sizeof(BinaryMsgCode);
    bin.msg_l2sc.size = sizeof(BinaryMsgCode);

    /* Проверяем разбор данных по аппарату с ЦИ по L1OF, но без РНП */
    size_t ret_size = glo.ParseFromArray(&bin, bin_size);

    REQUIRE ( ret_size == bin_size );
    REQUIRE ( glo.ByteSize() == sizeof(BinaryAllObsMsg) );
    REQUIRE ( glo.has_l1of_aux() );
    REQUIRE ( glo.has_l1of_obs() );
    REQUIRE ( glo.has_l1of_msg() );
    REQUIRE ( glo.has_l2of_aux() );
    REQUIRE ( glo.has_l2of_obs() );
    REQUIRE ( glo.has_l2of_msg() );
    REQUIRE ( glo.has_l1oc_aux() );
    REQUIRE ( glo.has_l1oc_obs() );
    REQUIRE ( glo.has_l1oc_msg() );
    REQUIRE ( glo.has_l2q_aux() );
    REQUIRE ( glo.has_l2q_obs() );
    REQUIRE ( glo.has_l3oc_aux() );
    REQUIRE ( glo.has_l3oc_obs() );
    REQUIRE ( glo.has_l3oc_msg() );
    REQUIRE ( glo.has_l2q_msg() );
    REQUIRE ( glo.has_l1sc_aux() );
    REQUIRE ( glo.has_l1sc_obs() );
    REQUIRE ( glo.has_l1sc_msg() );
    REQUIRE ( glo.has_l2sc_aux() );
    REQUIRE ( glo.has_l2sc_obs() );
    REQUIRE ( glo.has_l2sc_msg() );
    REQUIRE ( glo.has_l1cfg_aux() );
    REQUIRE ( glo.has_l1cfg_obs() );
    REQUIRE ( glo.has_l1cfg_msg() );
    REQUIRE ( glo.has_l2cfg_aux() );
    REQUIRE ( glo.has_l2cfg_obs() );
    REQUIRE ( glo.has_l2cfg_msg() );
    REQUIRE ( glo.has_srl_aux() );
    REQUIRE ( glo.has_srl_obs() );
    REQUIRE ( glo.has_srl_msg() );

    /* Проверяем состав данных заголовка */
    REQUIRE ( glo.id().channel() == bin.id.channel );
    REQUIRE ( glo.id().prn_num() == bin.id.prn_num );
    REQUIRE ( glo.id().freq_num() == bin.id.freq_num );
    REQUIRE ( glo.id().sys_num() == bin.id.sys_num );
    REQUIRE ( glo.id().azimuth() == bin.id.azimuth );
    REQUIRE ( glo.id().elevation() == bin.id.elevation );

    /* Проверяем состав данных вспомогательных блоков*/
    REQUIRE ( glo.l1of_aux().state() == bin.aux_l1of.mode );
    REQUIRE ( glo.l2of_aux().state() == bin.aux_l2of.mode );
    REQUIRE ( glo.l1oc_aux().state() == bin.aux_l1oc.mode );
    REQUIRE ( glo.l2q_aux().state() == bin.aux_l2q.mode );
    REQUIRE ( glo.l3oc_aux().state() == bin.aux_l3oc.mode );
    REQUIRE ( glo.l1cfg_aux().state() == bin.aux_l1cfg.mode );
    REQUIRE ( glo.l2cfg_aux().state() == bin.aux_l2cfg.mode );
    REQUIRE ( glo.srl_aux().state() == bin.aux_srl.mode );
    REQUIRE ( glo.l1sc_aux().state() == bin.aux_l1sc.mode );
    REQUIRE ( glo.l2sc_aux().state() == bin.aux_l2sc.mode );

    /* Проверяем состав данных РНП L1OF */
    REQUIRE ( glo.l1of_obs().code_pr() == bin.l1of.code_pr );
    REQUIRE ( glo.l1of_obs().phase_pr() == bin.l1of.phase_pr );
    REQUIRE ( glo.l1of_obs().dfs() == bin.l1of.dfs );
    REQUIRE ( glo.l1of_obs().snr() == bin.l1of.snr );

    /* Проверяем состав данных РНП L2OF */
    REQUIRE ( glo.l2of_obs().code_pr() == bin.l2of.code_pr );
    REQUIRE ( glo.l2of_obs().phase_pr() == bin.l2of.phase_pr );
    REQUIRE ( glo.l2of_obs().dfs() == bin.l2of.dfs );
    REQUIRE ( glo.l2of_obs().snr() == bin.l2of.snr );

    /* Проверяем состав данных ЦИ L1OF */
    REQUIRE ( memcmp(&(glo.l1of_msg().str()), bin.msg_l1of.str, glo.l1of_msg().str_size()) == 0 );
    REQUIRE ( glo.l1of_msg().crc_error() == bin.msg_l1of.crc_error );

    /* Проверяем состав данных ЦИ L2OF */
    REQUIRE ( memcmp(&(glo.l2of_msg().str()), bin.msg_l2of.str, glo.l2of_msg().str_size()) == 0 );
    REQUIRE ( glo.l2of_msg().crc_error() == bin.msg_l2of.crc_error );

    /* Теперь проверяем сериализацию */
    const size_t buf_size = sizeof(BinaryAllObsMsg)+1;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size-1); /* Передаем буфер не достаточной длины */
    REQUIRE ( ret_size == 0 ); /* ничего не должно сериализироваться */

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size); /* Передаем буфер минимально необходимой длины */
    REQUIRE ( ret_size == bin_size ); /* Данные должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );

    memset( buf, 0x00, buf_size );
    ret_size = glo.SerializeToArray(buf, bin_size+1); /* Передаем буфер большей длины, чем нужно */
    REQUIRE ( ret_size == bin_size ); /* Данные так же должны быть сериализированы */
    REQUIRE ( memcmp(buf, &bin, bin_size) == 0 );
  }
}
