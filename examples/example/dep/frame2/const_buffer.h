/*
 * const_buffer.h
 *
 *  Created on: 15 июн. 2019 г.
 *      Author: user
 */

#ifndef CONST_BUFFER_H_
#define CONST_BUFFER_H_

#include <cstdint>
#include <cstddef>

namespace frame2
{

  /*
   * Класс предоставляет интерфейс для разбора кадров из плоского массива
   */
  class ConstBuffer
  {
  public:
    ConstBuffer (const void* Pointer, size_t Size);
    ~ConstBuffer ();

    /* Функция запоминает указатель на буфер и далее работает с ним. size - доступный для работы размер в буфере */
    inline void SetPointer(void* Pointer, const size_t Size);
    inline bool IsSpaceAvailable(size_t RequiredSize) const;
    inline const void* CurrentPointer(void) const;
    inline size_t Position(void) const;

    inline ConstBuffer& operator +=(const size_t &size);

    inline void Get(uint8_t &value);
    inline void Get(int8_t &value);

    template <typename T>
    inline void Get(T &value);

//    inline void Get(uint16_t &value);
//    inline void Get(int16_t &value);


  private:

    const uint8_t *mpBuffer;
    size_t mBufferSize;
    size_t mBufferPos;

  };


  inline void
  ConstBuffer::SetPointer (void* Pointer, const size_t Size)
  {
    mpBuffer = reinterpret_cast<const uint8_t*>(Pointer);
    mBufferSize = Size;
    mBufferPos = 0;
  }

  inline bool
  ConstBuffer::IsSpaceAvailable (size_t RequiredSize) const
  {
    return RequiredSize <= (mBufferSize - mBufferPos);
  }

  inline const void*
  ConstBuffer::CurrentPointer(void) const
  {
    return mpBuffer + mBufferPos;
  }

  inline size_t
  ConstBuffer::Position(void) const
  {
    return mBufferPos;
  }

  inline ConstBuffer&
  ConstBuffer::operator +=(const size_t &size)
  {
    this->mBufferPos += size;
    return *this;
  }

  inline void
  ConstBuffer::Get (uint8_t &value)
  {
    value = mpBuffer[mBufferPos];
    mBufferPos += sizeof(value);
  }

  inline void
  ConstBuffer::Get (int8_t &value)
  {
    value = static_cast<int8_t>(mpBuffer[mBufferPos]);
    mBufferPos += sizeof(value);
  }

  template <typename T>
  inline void
  ConstBuffer::Get(T &value)
  {
    value = *(reinterpret_cast<const T*>(mpBuffer + mBufferPos));
    mBufferPos += sizeof(value);
  }

} /* namespace frame2 */

#endif /* CONST_BUFFER_H_ */
