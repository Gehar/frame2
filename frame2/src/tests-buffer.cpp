/*
 * tests-buffer.cpp
 *
 *  Created on: 18 июн. 2019 г.
 *      Author: user
 */

#include <cstring>

#include "catch.hpp"

#include <frame2/buffer.h>

TEST_CASE( "class frame2::Buffer testing", "[Buffer]" )
{
  /*
   * Здесь бы добавить проверку, что класс не пишет за границы
   */

  const size_t ext_buf_size = 1024;
  uint8_t ext_buf[ext_buf_size];
  frame2::Buffer buf(ext_buf, ext_buf_size);

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
    buf.Add(var_u8);

    size_t var_size = sizeof(var_u8);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_u8, var_size) == 0 );
  }

  SECTION ( "add int8_t" )
  {
    buf.Add(var_i8);

    size_t var_size = sizeof(var_i8);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_i8, var_size) == 0 );
  }

  SECTION ( "add uint16_t" )
  {
    buf.Add(var_u16);

    size_t var_size = sizeof(var_u16);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_u16, var_size) == 0 );
  }

  SECTION ( "add int16_t" )
  {
    buf.Add(var_i16);

    size_t var_size = sizeof(var_i16);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_i16, var_size) == 0 );
  }

  SECTION ( "add uint32_t" )
  {
    buf.Add(var_u32);

    size_t var_size = sizeof(var_u32);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_u32, var_size) == 0 );
  }

  SECTION ( "add int32_t" )
  {
    buf.Add(var_i32);

    size_t var_size = sizeof(var_i32);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_i32, var_size) == 0 );
  }

  SECTION ( "add uint64_t" )
  {
    buf.Add(var_u64);

    size_t var_size = sizeof(var_u64);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_u64, var_size) == 0 );
  }

  SECTION ( "add int64_t" )
  {
    buf.Add(var_i64);

    size_t var_size = sizeof(var_i64);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );
    REQUIRE ( std::memcmp(ext_buf, &var_i64, var_size) == 0 );
  }

  SECTION ( "add all types from int8_t/uint8_t to int64_t/uint64_t" )
  {
    /*
     * Специально перемешаем так, чтобы выравнивания не были кратны границам 2/4/8 байт.
     * А так же чередуем беззнаковые/знаковые.
     */
    buf.Add(var_u8);
    buf.Add(var_i16);
    buf.Add(var_u32);
    buf.Add(var_i64);
    buf.Add(var_i8);
    buf.Add(var_u16);
    buf.Add(var_i32);
    buf.Add(var_u64);

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

    size_t var_size = sizeof(Binary);
    REQUIRE ( buf.IsSpaceAvailable(ext_buf_size - var_size) == true );
    REQUIRE ( buf.CurrentPointer() == reinterpret_cast<void*>(ext_buf + var_size) );
    REQUIRE ( buf.Position() == var_size );


    Binary bin_buf;
    bin_buf.var_u8  = var_u8;
    bin_buf.var_i16 = var_i16;
    bin_buf.var_u32 = var_u32;
    bin_buf.var_i64 = var_i64;
    bin_buf.var_i8  = var_i8;
    bin_buf.var_u16 = var_u16;
    bin_buf.var_i32 = var_i32;
    bin_buf.var_u64 = var_u64;
    REQUIRE ( std::memcmp(ext_buf, &bin_buf, var_size) == 0 );
  }
}
