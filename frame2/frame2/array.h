/*
 * Array.h
 *
 *  Created on: 20 мар. 2021 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

namespace frame2
{
template<typename T, size_t n>
  class Array
  {
  public:
    Array() : mSize(0), mArray{ 0 } {}
    Array(size_t size) : mArray{ 0 }
    {
      mSize = (size < n)? size : n;
    }
    Array(size_t size, T *array)
    {
      if (array == nullptr)
      {
        mSize = 0;
        for (size_t i = 0; i < n; i++)
          mArray[i] = static_cast<T>(0);
        return;
      }
      mSize = (size < n)? size : n;
      for (size_t i = 0; i < mSize; i++)
        mArray[i] = array[i];
    }

    size_t size() const noexcept {return mSize;}
    // Класс не может содержать элементов больше, чем задано шаблонным параметром n
    void resize(size_t size) {mSize = (size < n)? size : n;}

    T& operator[](size_t indx) noexcept /* В std::array оператор [] тоже помечен как noexcept */
    {
      return mArray[indx];
    }
    const T& operator[](size_t indx) const noexcept /* В std::array оператор [] тоже помечен как noexcept */
    {
      return mArray[indx];
    }

  private:
    size_t mSize;
    T mArray[n];
  };

} /* namespace frame2 */

#endif /* ARRAY_H_ */
