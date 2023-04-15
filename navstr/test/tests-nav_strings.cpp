/*
 * tests-nav_strings.cpp
 *
 *  Created on: 30 окт. 2019 г.
 *      Author: user
 */

#include <cstdio>
#include "../navstr/navstr.h"
#include "../navstr/navstr_types.h"
#include "catch.hpp"

constexpr uint8_t array_for_test[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xfa, 0xeb};
constexpr size_t array_for_test_size = sizeof(array_for_test);
constexpr uint8_t array_for_check[] =
  { 0, 1, 0, 0, 1, 0, 0, 0, /* 65 */
    0, 0, 1, 0, 1, 1, 0, 0, /* 57 */
    0, 1, 1, 0, 1, 0, 1, 0, /* 49 */
    0, 0, 0, 1, 1, 1, 1, 0, /* 41 */
    0, 1, 0, 1, 1, 0, 0, 1, /* 33 */
    0, 0, 1, 1, 1, 1, 0, 1, /* 25 */
    0, 1, 1, 1, 1, 0, 1, 1, /* 17 */
    0, 1, 0, 1, 1, 1, 1, 1, /*  9 */
    1, 1, 0, 1, 0, 1, 1, 1  /*  1 */ };

uint64_t get_num(int start, int length)
{
  uint64_t tmp = 0;
  for (unsigned int i = 0; i < static_cast<unsigned int>(length); i++)
  {
    tmp |= static_cast<uint64_t> (array_for_check[sizeof(array_for_check) - (start + i)]) << (length - i - 1u);
  }
  return tmp;
}

TEST_CASE( "Template navstr::NavString.field_u64(start, length)", "[NavString]" )
{
  /* Проверяем функцию field_u64 */
  navstr::NavStr<72> str;
  std::copy(array_for_test, array_for_test + sizeof(array_for_test), str.begin());

  int cnt = 0;
  for(int length = 1; length <= 57; length++) // цикл по length, работа гарантируется, только если длина не более 57 бит
  {
    for (int start = 1; start <= static_cast<int>(sizeof(array_for_check) - length + 1); start++) // цикл по start
    {
      char s[100];
      sprintf(s, "Looped section start = %d, length = %d (cnt = %d)", start, length, cnt);
      cnt++;
      DYNAMIC_SECTION(s)
      {
        REQUIRE ( str.field_u64(start, length)  ==  get_num(start, length));
      }
    }
  }
}

TEST_CASE( "Template navstr::NavString size", "[NavString]" )
{
  /* Проверяем размер */
  navstr::NavStr<7> str7;
  REQUIRE ( str7.size() == 1 );
  REQUIRE ( str7.size_bits() == 7 );
  navstr::NavStr<8> str8;
  REQUIRE ( str8.size() == 1 );
  REQUIRE ( str8.size_bits() == 8 );
  navstr::NavStr<9> str9;
  REQUIRE ( str9.size() == 2 );
  REQUIRE ( str9.size_bits() == 9 );
}

TEST_CASE( "Template navstr::NavString.field_iXX(int start_lsb, int length_lsb, int start_msb, int length_msb)", "[NavString]" )
{
  /* Проверяем функцию field_i32 */
  navstr::NavStr<72> str;
  std::copy(array_for_test, array_for_test + sizeof(array_for_test), str.begin());

  uint32_t field_u32_test = array_for_test[array_for_test_size - 2] | (array_for_test[array_for_test_size - 4] << 8);
  uint32_t field_u32 = str.field_u32(25, 8, 9, 8);
  REQUIRE ( field_u32 == field_u32_test );

  int32_t field_i32_test = (static_cast<int32_t>(field_u32_test) << 16) >> 16;
  int32_t field_i32 = str.field_i32(25, 8, 9, 8);
  REQUIRE ( field_i32 == field_i32_test );

  uint64_t field_u64_test = static_cast<uint64_t>(field_u32_test);
  uint64_t field_u64 = str.field_u64(25, 8, 9, 8);
  REQUIRE ( field_u64 == field_u64_test );

  int64_t field_i64_test = static_cast<int64_t>(field_i32_test);
  int64_t field_i64 = str.field_i64(25, 8, 9, 8);
  REQUIRE ( field_i64 == field_i64_test );
}

TEST_CASE( "Type of nav_str_types.h", "[NavString]" )
{
  navstr::GloOf str_glo_of;
  REQUIRE ( str_glo_of.size_bits() == navstr::GloOfBitsNum );
  navstr::GloL1of str_glo_l1of;
  REQUIRE ( str_glo_l1of.size_bits() == navstr::GloL1ofBitsNum );
  navstr::GloL2of str_glo_l2of;
  REQUIRE ( str_glo_l2of.size_bits() == navstr::GloL2ofBitsNum );
  navstr::GloL1oc str_glo_l1oc;
  REQUIRE ( str_glo_l1oc.size_bits() == navstr::GloL1ocBitsNum );
  navstr::GalE1 str_gal_e1;
  REQUIRE ( str_gal_e1.size_bits() == navstr::GalE1BitsNum );
  navstr::GpsLnav str_gps_lnav;
  REQUIRE ( str_gps_lnav.size_bits() == navstr::GpsLnavBitsNum );
}

TEST_CASE( "set_field()", "[NavString]" )
{
  navstr::GloOf str_glo_of;
  constexpr int bit_offset = 85 + 1;
  str_glo_of.set_field(-1, bit_offset - 84, 4);
  std::array<uint8_t, str_glo_of.nbytes()> str_ref;
  str_ref.fill(0);
  str_ref[str_ref.size() - 1] = 0x0f;
  REQUIRE ( str_glo_of == str_ref );
}

TEST_CASE( "test SMR methods", "[NavString]" )
{
  /* Проверяем методы для извлечения полей */
  uint64_t str_buf = 0b000000001111000011110000ll << (5*8);
  navstr::NavStr<64> str;
  str.set(reinterpret_cast<uint8_t*>(&str_buf), sizeof(str_buf));
  REQUIRE (str.field_i32_smr(8, 5) == 0x0f);
  REQUIRE (str.field_i32_smr(9, 4) == (~0x07 + 1));
  REQUIRE (str.field_i64_smr(8, 5) == 0x0f);
  REQUIRE (str.field_i64_smr(9, 4) == (~0x07 + 1));
  REQUIRE (str.field_i64_smr(8, 5, 17, 4) == 0x0ff);
  REQUIRE (str.field_i64_smr(9, 4, 17, 4) == (~0x07f + 1));

  /* Проверяем методы для установки полей */
  uint64_t &str64 = *reinterpret_cast<uint64_t*>(str.data());
  str.fill(0);
  str.set_field_smr(0xff, 8, 9);
  REQUIRE (str64 == (0xffull << (6 * 8)));

  str.fill(0);
  str.set_field_smr(0xff, 9, 8);
  REQUIRE (str64 == (0x81ull << (6 * 8)));
}
