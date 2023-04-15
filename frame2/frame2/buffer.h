/*
 * buffer.h
 *
 *  Created on: 15 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <cstdint>
#include <cstddef>

namespace frame2
{
  /*
   * Класс предоставляет интерфейс для сборки кадров в плоский массив
   */
  class Buffer
  {
  public:
    Buffer (void* Pointer, size_t Size);
    ~Buffer ();

    /* Функция запоминает указатель на буфер и далее работает с ним. size - доступный для работы размер в буфере */
    inline void SetPointer(void* Pointer, const size_t Size);
    inline bool IsSpaceAvailable(size_t RequiredSize) const;
    inline void* CurrentPointer(void) const;
    inline size_t Position(void) const;

    inline Buffer& operator +=(const size_t &size);

    inline void Add(uint8_t value);
    inline void Add(int8_t value);

    template <typename T>
    inline void Add(T value);


  private:

    uint8_t *mpBuffer;
    size_t mBufferSize;
    size_t mBufferPos;
  };

  inline void
  Buffer::SetPointer (void* Pointer, const size_t Size)
  {
    mpBuffer = reinterpret_cast<uint8_t*>(Pointer);
    mBufferSize = Size;
    mBufferPos = 0;
  }

  inline bool
  Buffer::IsSpaceAvailable (size_t RequiredSize) const
  {
    return RequiredSize <= (mBufferSize - mBufferPos);
  }

  inline void*
  Buffer::CurrentPointer(void) const
  {
    return mpBuffer + mBufferPos;
  }

  inline size_t
  Buffer::Position(void) const
  {
    return mBufferPos;
  }

  inline Buffer&
  Buffer::operator +=(const size_t &size)
  {
    this->mBufferPos += size;
    return *this;
  }

  inline void
  Buffer::Add (uint8_t value)
  {
    mpBuffer[mBufferPos] = value;
    mBufferPos += sizeof(value);
  }

  inline void
  Buffer::Add (int8_t value)
  {
    mpBuffer[mBufferPos] = static_cast<uint8_t>(value);
    mBufferPos += sizeof(value);
  }

  template <typename T>
  inline void
  Buffer::Add (T value)
  {
    *(reinterpret_cast<T*>(mpBuffer + mBufferPos)) = value;
    mBufferPos += sizeof(value);
  }

} /* namespace frame2 */

#endif /* BUFFER_H_ */
