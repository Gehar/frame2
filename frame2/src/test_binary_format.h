/*
 * test_binary_format.h
 *
 *  Created on: 19 июн. 2019 г.
 *      Author: user
 */

#ifndef TEST_BINARY_FORMAT_H_
#define TEST_BINARY_FORMAT_H_

#include <cstdint>
#include <cstddef>

#pragma pack(push,1)

const size_t DI_OF_LEN = 11;
const size_t DI_OC_LEN = 32;
const size_t DI_GPS_L1CA_LEN = 38;
const size_t DI_L3OC_LEN = 38;
const size_t DI_CFG_LEN = 38;
const size_t DI_SRL_LEN = 222;
const uint8_t code_num_not_defined = 0xff;
const int8_t  freq_num_not_defined = 0x7f;

/* 28 байт */
struct FrameHdr
{
  uint32_t frame_id;
  uint32_t size;
  uint32_t version;
  int64_t timestamp_hw_ms;
  uint8_t timestamp_id;
  int64_t timestamp_ms;
};

/* 5 байт */
struct SubframeHdr
{
  uint32_t subframe_size;
  uint8_t id;
};

/* 19 байт */
struct BinaryId
{
  uint16_t mSetOfObsFlags;
  uint16_t mSetOfMsgFlags;
  uint16_t mSetOfAuxFlags;
  uint16_t channel;
  uint8_t prn_num;
  int8_t freq_num;
  uint8_t sys_num;
  float azimuth;
  float elevation;
};

/* 24 байт */
struct BinaryObs
{
  double code_pr;
  double phase_pr;
  float dfs;
  float snr;
};

/* 13 байт */
struct BinaryMsgOf
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_OF_LEN];
};

/* 49 байт */
struct BinaryMsgCode
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_OC_LEN];
};

/* 40 байт */
struct BinaryMsgGpsL1ca
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_GPS_L1CA_LEN];
};

/* 52 байт */
struct BinaryMsgL3oc
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_L3OC_LEN];
};

/* 49 байт */
struct BinaryMsgCfg
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_CFG_LEN];
};

/* 224 байт */
struct BinaryMsgSrl
{
  uint8_t size; /* длина структуры в байтах */
  uint8_t crc_error;
  uint8_t str[DI_SRL_LEN];
};

struct BinaryAux
{
  int8_t mode;
  uint32_t flags;
  uint8_t fields_existence;
};

struct BinaryIdObs
{
  BinaryId id;
  BinaryObs obs;
};

struct BinaryIdMsg
{
  BinaryId id;
  BinaryMsgOf msg;
};

struct BinaryIdMsgOc
{
  BinaryId id;
  BinaryMsgCode msg;
};

struct BinaryIdAux
{
  BinaryId id;
  BinaryAux aux;
};

struct BinaryGps
{
  BinaryId id;
  BinaryAux aux_l1ca;
  BinaryObs l1ca;
  BinaryMsgGpsL1ca msg_l1ca;
};

struct BinaryAllObsMsg
{
  BinaryId id;
  BinaryAux aux_l1of;
  BinaryAux aux_l2of;
  BinaryAux aux_l1oc;
  BinaryAux aux_l2q;
  BinaryAux aux_l3oc;
  BinaryAux aux_l1cfg;
  BinaryAux aux_l2cfg;
  BinaryAux aux_srl;
  BinaryAux aux_l1sc;
  BinaryAux aux_l2sc;
  BinaryObs l1of;
  BinaryObs l2of;
  BinaryObs l1oc;
  BinaryObs l2q;
  BinaryObs l3oc;
  BinaryObs l1cfg;
  BinaryObs l2cfg;
  BinaryObs srl;
  BinaryObs l1sc;
  BinaryObs l2sc;
  BinaryMsgOf msg_l1of;
  BinaryMsgOf msg_l2of;
  BinaryMsgCode msg_l1oc;
  BinaryMsgCode msg_l2q;
  BinaryMsgL3oc msg_l3oc;
  BinaryMsgCfg msg_l1cfg;
  BinaryMsgCfg msg_l2cfg;
  BinaryMsgSrl msg_srl;
  BinaryMsgCode msg_l1sc;
  BinaryMsgCode msg_l2sc;
};

#pragma pack(pop)

#endif /* TEST_BINARY_FORMAT_H_ */
