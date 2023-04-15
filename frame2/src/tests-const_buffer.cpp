/*
 * tests-const_buffer.cpp
 *
 *  Created on: 18 июн. 2019 г.
 *      Author: user
 */

#include <cstring>

#include "catch.hpp"

#include <frame2/const_buffer.h>

TEST_CASE( "class frame2::ConstBuffer testing", "[ConstBuffer]" )
{
  const size_t ext_buf_size = 1024;
  uint8_t ext_buf[ext_buf_size];
  frame2::ConstBuffer buf(ext_buf, ext_buf_size);

  REQUIRE ( buf.IsSpaceAvailable(ext_buf_size) == true);
  REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf));
  REQUIRE ( buf.Position() == 0 );

  uint8_t   var_u8  =  161;                     // 0xA1
  int8_t    var_i8  = -95;                      // 0xA1
  uint16_t  var_u16 =  41524;                   // 0xA234
  int16_t   var_i16 = -24012;                   // 0xA234
  uint32_t  var_u32 =  2739234697ul;            // 0xA3456789
  int32_t   var_i32 = -1555732599ul;            // 0xA3456789
  uint64_t  var_u64 =  11841784880787614872ull; // 0xA456789bcdefec98
  int64_t   var_i64 = -6604959192921936744ll;   // 0xA456789bcdefec98

  SECTION ( "add uint8_t" )
  {
    /* Подготавливаем буфер */
    uint8_t &var = var_u8, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add int8_t" )
  {
    /* Подготавливаем буфер */
    int8_t &var = var_i8, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add uint16_t" )
  {
    /* Подготавливаем буфер */
    uint16_t &var = var_u16, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add int16_t" )
  {
    /* Подготавливаем буфер */
    int16_t &var = var_i16, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add uint32_t" )
  {
    /* Подготавливаем буфер */
    uint32_t &var = var_u32, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add int32_t" )
  {
    /* Подготавливаем буфер */
    int32_t &var = var_i32, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add uint64_t" )
  {
    /* Подготавливаем буфер */
    uint64_t &var = var_u64, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add int64_t" )
  {
    /* Подготавливаем буфер */
    int64_t &var = var_i64, tmp = 0;
    size_t var_size = sizeof(var);
    std::memcpy(ext_buf, &var, var_size);

    /* Получаем данные из буфера */
    buf.Get(tmp);

    /* Проверки */
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true);
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size));
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( tmp == var );
  }

  SECTION ( "add all types from int8_t/uint8_t to int64_t/uint64_t" )
  {
    /*
     * Специально перемешаем так, чтобы выравнивания не были кратны границам 2/4/8 байт.
     * А так же чередуем беззнаковые/знаковые.
     */

#pragma pack(push,1)
    struct Binary
    {
      uint8_t var_u8;
      int16_t var_i16;
      uint32_t var_u32;
      int64_t var_i64;
      int8_t var_i8;
      uint16_t var_u16;
      int32_t var_i32;
      uint64_t var_u64;
    };
#pragma pack(pop)

    Binary bin_buf;
    bin_buf.var_u8  = var_u8;
    bin_buf.var_i16 = var_i16;
    bin_buf.var_u32 = var_u32;
    bin_buf.var_i64 = var_i64;
    bin_buf.var_i8  = var_i8;
    bin_buf.var_u16 = var_u16;
    bin_buf.var_i32 = var_i32;
    bin_buf.var_u64 = var_u64;
    size_t var_size = sizeof(bin_buf);
    std::memcpy(ext_buf, &bin_buf, var_size);

    uint8_t   tmp_u8  = 0;
    int8_t    tmp_i8  = 0;
    uint16_t  tmp_u16 = 0;
    int16_t   tmp_i16 = 0;
    uint32_t  tmp_u32 = 0;
    int32_t   tmp_i32 = 0;
    uint64_t  tmp_u64 = 0;
    int64_t   tmp_i64 = 0;

    buf.Get(tmp_u8);
    buf.Get(tmp_i16);
    buf.Get(tmp_u32);
    buf.Get(tmp_i64);
    buf.Get(tmp_i8);
    buf.Get(tmp_u16);
    buf.Get(tmp_i32);
    buf.Get(tmp_u64);

    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );

    REQUIRE( tmp_u8 == var_u8 );
    REQUIRE( tmp_u16 == var_u16 );
    REQUIRE( tmp_u32 == var_u32 );
    REQUIRE( tmp_u64 == var_u64 );
    REQUIRE( tmp_i8 == var_i8 );
    REQUIRE( tmp_i16 == var_i16 );
    REQUIRE( tmp_i32 == var_i32 );
    REQUIRE( tmp_i64 == var_i64 );
  }
}


