/*
 * miscellaneous.cpp
 *
 *  Created on: 22 авг. 2019 г.
 *      Author: user
 */

#include <cstdint>
#include <cstdio>

#include "miscellaneous.h"

void
hexdump(const void* ptr, size_t size)
{
  const uint8_t *buf = reinterpret_cast<const uint8_t*>(ptr);
  for (size_t i = 0; i < size; i++)
  {
    printf("%02x ", static_cast<unsigned int>(buf[i]));
  }
  printf("\n");
}
