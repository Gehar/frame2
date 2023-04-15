/*
 * Auxiliary_test.cpp
 *
 *  Created on: 13 авг. 2019 г.
 *      Author: user
 */


#include "catch.hpp"

#include <stdint.h>
#include <cstring>
#include <frame2/auxiliary.h>

#include "test_binary_format.h"

TEST_CASE( "class frame2::Auxiliary testing", "[Auxiliary]" )
{
  frame2::Auxiliary aux;
  REQUIRE ( aux.ByteSize() == sizeof(BinaryAux));
  REQUIRE_FALSE(aux.has_ampl_data());
  REQUIRE_FALSE(aux.has_ampl_pilot());
  REQUIRE_FALSE(aux.has_phase_std_data());
  REQUIRE_FALSE(aux.has_phase_std_pilot());
  REQUIRE_FALSE(aux.has_data_num());
  REQUIRE_FALSE(aux.has_pilot_num());
  REQUIRE_FALSE(aux.has_dif_phase_ants());

  SECTION ( "check ampl_data" )
  {
    float val = 123;
    aux.set_ampl_data(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.ampl_data())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.ampl_data())) );

    REQUIRE       (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.ampl_data() == val);
  }

  SECTION ( "check ampl_pilot" )
  {
    float val = 234;
    aux.set_ampl_pilot(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.ampl_pilot())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.ampl_pilot())) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE       (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.ampl_pilot() == val);
  }

  SECTION ( "check phase_std_data" )
  {
    float val = 345;
    aux.set_phase_std_data(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.phase_std_data())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.phase_std_data())) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE       (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.phase_std_data() == val);
  }

  SECTION ( "check phase_std_pilot" )
  {
    float val = 456;
    aux.set_phase_std_pilot(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.phase_std_pilot())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.phase_std_pilot())) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE       (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.phase_std_pilot() == val);
  }

  SECTION ( "check data_num" )
  {
    uint8_t val = 132;
    aux.set_data_num(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.data_num())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.data_num())) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE       (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.data_num() == val);
  }

  SECTION ( "check pilot_num" )
  {
    uint8_t val = 143;
    aux.set_pilot_num(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.pilot_num())) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(aux.pilot_num())) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE       (aux2.has_pilot_num());
    REQUIRE_FALSE (aux2.has_dif_phase_ants());

    REQUIRE (aux2.pilot_num() == val);
  }

  SECTION ( "check dif_phase_ants" )
  {
    frame2::Auxiliary::ArrayFloat val;
    val.resize(2);
    val[0] = 564.0f;
    val[1] = 471.0f;
    aux.set_dif_phase_ants(val);

    const size_t buf_size = 1024;
    const size_t aux_hdr_size = 6;
    uint8_t buf[buf_size];
    memset( buf, 0x00, buf_size );

    memset( buf, 0x00, buf_size );
    size_t ret_size = aux.SerializeToArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(uint8_t) + val.size() * sizeof(val[0])) );
    frame2::Auxiliary aux2;
    ret_size = aux2.ParseFromArray(buf);
    REQUIRE ( ret_size == (aux_hdr_size + sizeof(uint8_t) + val.size() * sizeof(val[0])) );

    REQUIRE_FALSE (aux2.has_ampl_data());
    REQUIRE_FALSE (aux2.has_ampl_pilot());
    REQUIRE_FALSE (aux2.has_phase_std_data());
    REQUIRE_FALSE (aux2.has_phase_std_pilot());
    REQUIRE_FALSE (aux2.has_data_num());
    REQUIRE_FALSE (aux2.has_pilot_num());
    REQUIRE       (aux2.has_dif_phase_ants());

    REQUIRE (aux2.dif_phase_ants().size() == val.size());
    for (size_t i = 0; i < val.size(); i++)
    {
      REQUIRE (aux2.dif_phase_ants()[i] == val[i]);
    }
  }


}
