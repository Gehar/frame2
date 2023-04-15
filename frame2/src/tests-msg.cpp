/*
 * tests-msg.cpp
 *
 *  Created on: 22 нояб. 2019 г.
 *      Author: user
 */

#include "catch.hpp"

#include <frame2/msg.h>
#include <navstr/navstr.h>
#include <cstdio>
#include <cstring>

TEST_CASE( "Class frame2::Msg", "[Msg]" )
{
  constexpr size_t nbits = 100;
  struct BinMsg
  {
    uint8_t size; /* длина структуры в байтах */
    uint8_t crc_error;
    uint8_t str[navstr::NavStr<nbits>::nbytes()];
  };

  frame2::Msg<nbits> msg;
  REQUIRE ( msg.str_size() ==  navstr::NavStr<nbits>::nbytes());
  REQUIRE ( msg.str().size() ==  navstr::NavStr<nbits>::nbytes());
  REQUIRE ( msg.crc_error() ==  0);
  REQUIRE ( msg.ByteSize() ==  (navstr::NavStr<nbits>::nbytes() + offsetof(BinMsg, str)));

  BinMsg bin;
  constexpr size_t bin_size = sizeof(BinMsg);
  constexpr size_t buf_size = sizeof(BinMsg);
  uint8_t buf[buf_size];
  /* Данные заполняем случайными числами */
  uint8_t *u8_ptr = reinterpret_cast<uint8_t*>(&bin);
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  for (size_t i = 0; i < bin_size; i++)
  {
    u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
  }

  for (size_t i = 0; i < navstr::NavStr<nbits>::nbytes(); i++)
  {
    char str[1024];
    sprintf(str, "check frame2::Msg with %d bytes string length (nbytes = %d)", static_cast<int>(i), static_cast<int>(navstr::NavStr<nbits>::nbytes()));
    SECTION ( str )
    {
      /* Проверяем сериализацию/десериализацию */
      bin.size = static_cast<uint8_t>(offsetof(BinMsg, str) + i);
      size_t ret_size = msg.ParseFromArray(&bin);
      REQUIRE ( ret_size == bin.size );
      memset( buf, 0x00, buf_size );
      ret_size = msg.SerializeToArray(buf);
      REQUIRE ( ret_size == bin.size );
      REQUIRE ( memcmp(buf, &bin, bin.size) == 0 );

      /* Проверяем метод Msg::set_str() */
      for (size_t k = 0; k < i; k++) /* для этого заполним строку случайными числами */
      {
        bin.str[k] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
      }
      navstr::NavStr<nbits> nav_str(bin.str, i);
      msg.set_str(nav_str);
      REQUIRE ( msg.str_size() == i );
      ret_size = msg.SerializeToArray(buf);
      REQUIRE ( ret_size == bin.size );
      REQUIRE ( memcmp(buf, &bin, bin.size) == 0 );
    }
  }

  struct BinBigMsg
  {
    uint8_t size; /* длина структуры в байтах */
    uint8_t crc_error;
    uint8_t str[navstr::NavStr<nbits>::nbytes() + 1];
  };

  BinBigMsg bin_big;
  constexpr size_t bin_big_size = sizeof(BinBigMsg);
//  constexpr size_t buf_big_size = sizeof(BinBigMsg);
//  uint8_t buf_big[buf_size];
  /* Данные заполняем случайными числами */
  u8_ptr = reinterpret_cast<uint8_t*>(&bin_big);
  for (size_t i = 0; i < bin_size; i++)
  {
    u8_ptr[i] = static_cast<uint8_t>(std::rand() / (RAND_MAX / 255));
  }
  bin_big.size = static_cast<uint8_t>(bin_big_size);
  size_t ret_size = msg.ParseFromArray(&bin_big);
  REQUIRE ( ret_size == sizeof(bin_big) );
  REQUIRE ( msg.str_size() == navstr::NavStr<nbits>::nbytes() );
  REQUIRE ( msg.ByteSize() == (offsetof(BinMsg, str) + navstr::NavStr<nbits>::nbytes()) );

}
