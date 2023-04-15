/*
 * buffer.cpp
 *
 *  Created on: 15 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#include <frame2/buffer.h>

namespace frame2
{
/* no comments */
  Buffer::Buffer (void* Pointer, size_t Size)
  {
    mpBuffer = reinterpret_cast<uint8_t*>(Pointer);
    mBufferSize = Size;
    mBufferPos = 0;
  }

  Buffer::~Buffer ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
