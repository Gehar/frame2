/*
 * tests-gal.cpp
 *
 *  Created on: 27 авг. 2019 г.
 *      Author: user
 */

#include "catch.hpp"

#include <frame2/gal.h>

#include "test_binary_format.h"

TEST_CASE( "Class frame2::Gal serialization", "[Gal]" )
{
  /*
   * Сериализацию/десериализацию не проверяем, так как она
   * вся реализована в шаблоне и была протестирована
   * класса Glo.
   */

  frame2::Gal nka;
  REQUIRE ( nka.ByteSize() == sizeof(BinaryId));
  REQUIRE_FALSE ( nka.has_e1bc_aux() );
  REQUIRE_FALSE ( nka.has_e1bc_obs() );
  REQUIRE_FALSE ( nka.has_e1bc_msg() );

  SECTION( "add e1bc_aux block" )
  {
    nka.add_e1bc_aux();
    REQUIRE       ( nka.has_e1bc_aux() );
    REQUIRE_FALSE ( nka.has_e1bc_obs() );
    REQUIRE_FALSE ( nka.has_e1bc_msg() );

    uint32_t flags = 0x12345678;
    nka.mutable_e1bc_aux()->set_flags(flags);
    REQUIRE ( nka.e1bc_aux().flags() == flags );
  }

  SECTION( "add e1bc_obs block" )
  {
    nka.add_e1bc_obs();
    REQUIRE_FALSE ( nka.has_e1bc_aux() );
    REQUIRE       ( nka.has_e1bc_obs() );
    REQUIRE_FALSE ( nka.has_e1bc_msg() );

    double d = 1.23456778e6;
    nka.mutable_e1bc_obs()->set_code_pr(d);
    REQUIRE ( nka.e1bc_obs().code_pr() == d );
  }

  SECTION( "add e1bc_msg block" )
  {
    nka.add_e1bc_msg();
    REQUIRE_FALSE ( nka.has_e1bc_aux() );
    REQUIRE_FALSE ( nka.has_e1bc_obs() );
    REQUIRE       ( nka.has_e1bc_msg() );

    uint8_t crc_error = 0xbc;
    nka.mutable_e1bc_msg()->set_crc_error(crc_error);
    REQUIRE ( nka.e1bc_msg().crc_error() == crc_error );
  }

}
