/*
 * const_buffer.cpp
 *
 *  Created on: 15 июн. 2019 г.
 *      Author: user
 */

#include <frame2/const_buffer.h>

namespace frame2
{

  ConstBuffer::ConstBuffer (const void* Pointer, size_t Size)// : mpBuffer(reinterpret_cast<const uint8_t*>(Pointer))
  {
    mpBuffer = reinterpret_cast<const uint8_t*>(Pointer);;
    mBufferSize = Size;
    mBufferPos = 0;
  }

  ConstBuffer::~ConstBuffer ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
