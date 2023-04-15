/*
 * nav_string.h
 *
 *  Created on: 22 окт. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef NAVSTR_H_
#define NAVSTR_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <climits>

namespace navstr
{
  template<std::size_t n_bits>
    class NavStr : public std::array<uint8_t, (n_bits / 8 + (((n_bits % 8) == 0) ? 0 : 1))>
    {
    public:
      /* Метод nbytes() возвращает размер выделенной памяти под строку в байтах */
      static constexpr size_t nbytes();

      static constexpr size_t scSize = nbytes();

      NavStr();
      NavStr (const std::array<uint8_t, scSize> &arr);
      /*
       * Конструктор копирует из Data строку рамером SizeData, но не более чем nbytes().
       * Чтобы узнать, сколько байт было скопировано следует использовать метод size()
       */
      NavStr (const uint8_t *Data, size_t SizeData);

      void dump(void) const;

      /*
       * Метод копирует из Data строку рамером SizeData, но не более чем nbytes().
       * Чтобы узнать, сколько байт было скопировано следует использовать метод size()
       */
      inline void set(const uint8_t *Data, size_t SizeData);

      /* Метод возвращает размер хранящейся строки */
      size_t size() const noexcept;

      /* Возвращает размер строки в битах */
      inline static constexpr size_t size_bits() noexcept;

      /*
       * Метод для установки полей в строку. Метод устанавливает корректно
       * только поля длиной до (sizeof(T) - 1) * CHAR_BIT + 1] бит включительно.
       * Для вызова достаточно написать:
       *   int32_t field = -1;
       *   set_field(field, 0, 5);
       */
      template<typename T>
        inline void set_field(T field, int start, int length);

      /*
       * Аналогичен методу set_field(), но входное поле преобразует в формат SMR (Signed magnitude representation)
       * Не используйте этот метод для беззнаковых чисел.
       */
      template<typename T>
        inline void set_field_smr (T field, int start, int length);

      /*
       * Методы field_u32 и field_i32 извлекают корректно только поля длиной до 25 бит включительно.
       * Для извлечения полей большей длины следует использовать методы field_u64 и field_i64.
       *
       * Это связано с тем, что поля размером 26 бит и более могут занимать 5 байт, а не четыре:
       * ---------------------------------
       * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
       * ---------------------------------
       * |   |   |   |   |   |   |   | X | байт 0
       * ---------------------------------
       * | X | X | X | X | X | X | X | X | байт 1
       * ---------------------------------
       * | X | X | X | X | X | X | X | X | байт 2
       * ---------------------------------
       * | X | X | X | X | X | X | X | X | байт 3
       * ---------------------------------
       * | X |   |   |   |   |   |   |   | байт 4
       * ---------------------------------
       */
      inline uint32_t
      field_u32(int start, int length) const;

      inline uint32_t
      field_u32(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      inline int32_t
      field_i32(int start, int length) const;

      inline int32_t
      field_i32(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      /*
       * Методы field_u64 и field_i64 извлекают корректно только поля длиной до 57 бит включительно.
       */
      inline uint64_t
      field_u64(int start, int length) const;

      inline uint64_t
      field_u64(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      inline int64_t
      field_i64(int start, int length) const;

      inline int64_t
      field_i64(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      /*
       * Мотоды, возвращающие отрицательные числа, записанные в прямом коде (т.е. когда старший занковый)
       * *_smr() - Signed magnitude representation (см. https://en.wikipedia.org/wiki/Signed_number_representations#Sign-and-magnitude_method)
       */
      inline int32_t
      field_i32_smr(int start, int length) const;

      inline int32_t
      field_i32_smr(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      inline int64_t
      field_i64_smr(int start, int length) const;

      inline int64_t
      field_i64_smr(int start_msb, int length_msb, int start_lsb, int length_lsb) const;

      /*
       * Метод field() извлекает корректно только поля длиной до (sizeof(T) - 1) * CHAR_BIT + 1] бит включительно.
       */
      template<typename T> inline T
      field(int start, int length) const;

    protected:
      constexpr int byte(int start, int length, int word_size) const;
      constexpr int shift(int start, int length, int word_size) const;
      template<typename T> constexpr T mask(int length) const;
      /* field_smr() - use only for signed T */
      template<typename T> inline T
      convert_from_smr(T smr, int length) const;
      template<typename T> inline T
      convert_to_smr(T two_complement, int length) const;
    private:
      size_t mSizeData;
    };

  template<std::size_t n_bits>
    inline constexpr size_t
    NavStr<n_bits>::nbytes ()
    {
      return (n_bits / 8 + (((n_bits % 8) == 0) ? 0 : 1));
    }

  template<std::size_t n_bits>
    inline
    NavStr<n_bits>::NavStr () :
        mSizeData(nbytes())
    {
      this->fill(0);
    }

  template<std::size_t n_bits>
    inline
    NavStr<n_bits>::NavStr (const std::array<uint8_t, scSize> &arr) :
        mSizeData(nbytes())
    {
      std::copy(arr.begin(), arr.end(), this->begin());
    }

  template<std::size_t n_bits>
    inline
    NavStr<n_bits>::NavStr (const uint8_t *Data, size_t SizeData) :
        mSizeData(SizeData)
    {
      if ( SizeData > nbytes() )
      {
        /* Размер входной строки превышает объем выделенной памяти */
        mSizeData = nbytes();
      }
      std::copy(Data, Data + mSizeData, this->begin());
    }

  template<std::size_t n_bits>
    void NavStr<n_bits>::dump (void) const
    {
      /* запоминаем форматирование iostream */
      std::ios_base::fmtflags f( std::cout.flags() );
      char prev = std::cout.fill();

      /* выводим состояние объекта */
      std::cout << "size: " << this->size() << "\n";
      std::cout << "data (hex): ";
      std::cout << std::hex << std::setfill('0');
      for (size_t i = 0; i < this->size(); i++)
        std::cout << std::setw(2) << static_cast<unsigned>(this->data()[i]) << ' ';

      /* восстанавливаем форматирование iostream */
      std::cout << std::setfill(prev) << std::endl;
      std::cout.flags(f);
    }

  template<std::size_t n_bits>
    inline void
    NavStr<n_bits>::set (const uint8_t *Data, size_t SizeData)
    {
      mSizeData = SizeData;
      if ( SizeData > nbytes() )
      {
        /* Размер входной строки превышает объем выделенной памяти */
        mSizeData = nbytes();
      }
      std::copy(Data, Data + mSizeData, this->begin());
    }

  template<std::size_t n_bits>
    inline size_t
    NavStr<n_bits>::size () const noexcept
    {
      return mSizeData;
    }

  template<std::size_t n_bits>
    inline constexpr size_t
    NavStr<n_bits>::size_bits () noexcept
    {
      return n_bits;
    }

  template<std::size_t n_bits>
    inline uint32_t
    NavStr<n_bits>::field_u32 (int start, int length) const
    {
      return field<uint32_t>(start, length);
    }

  template<std::size_t n_bits>
    inline uint32_t
    NavStr<n_bits>::field_u32 (int start_msb, int length_msb, int start_lsb, int length_lsb) const
    {
      return field<uint32_t>(start_lsb, length_lsb) | (field<uint32_t>(start_msb, length_msb) << length_lsb);
    }

  template<std::size_t n_bits>
    inline int32_t
    NavStr<n_bits>::field_i32 (int start, int length) const
    {
      return (field<int32_t>(start, length) << (32 - length)) >> (32 - length);
    }

  template<std::size_t n_bits>
    inline int32_t
    NavStr<n_bits>::field_i32 (int start_msb, int length_msb, int start_lsb, int length_lsb) const
    {
      return field<int32_t>(start_lsb, length_lsb) | ((field<int32_t>(start_msb, length_msb)  << (32 - length_msb)) >> (32 - length_msb)) << length_lsb;
    }

  template<std::size_t n_bits>
    inline uint64_t
    NavStr<n_bits>::field_u64 (int start, int length) const
    {
      return field<uint64_t>(start, length);
    }

  template<std::size_t n_bits>
    inline uint64_t
    NavStr<n_bits>::field_u64 (int start_msb, int length_msb, int start_lsb, int length_lsb) const
    {
      return field<uint64_t>(start_lsb, length_lsb) | (field<uint64_t>(start_msb, length_msb) << length_lsb);
    }

  template<std::size_t n_bits>
    inline int64_t
    NavStr<n_bits>::field_i64 (int start, int length) const
    {
      return (field<int64_t>(start, length) << (64 - length)) >> (64 - length);
    }

  template<std::size_t n_bits>
    inline int64_t
    NavStr<n_bits>::field_i64 (int start_msb, int length_msb, int start_lsb, int length_lsb) const
    {
      return field<int64_t>(start_lsb, length_lsb) | ((field<int64_t>(start_msb, length_msb)  << (64 - length_msb)) >> (64 - length_msb)) << length_lsb;
    }

  template<std::size_t n_bits>
  inline int32_t
  NavStr<n_bits>::field_i32_smr(int start, int length) const
  {
    return convert_from_smr<int32_t>(field<uint32_t>(start, length), length);
  }

  template<std::size_t n_bits>
  inline int32_t
  NavStr<n_bits>::field_i32_smr(int start_msb, int length_msb, int start_lsb, int length_lsb) const
  {
    return convert_from_smr<int32_t>(field_i32(start_msb, length_msb, start_lsb, length_lsb), length_msb + length_lsb);
  }

  template<std::size_t n_bits>
  inline int64_t
  NavStr<n_bits>::field_i64_smr(int start, int length) const
  {
    return convert_from_smr<int64_t>(field<uint64_t>(start, length), length);
  }

  template<std::size_t n_bits>
  inline int64_t
  NavStr<n_bits>::field_i64_smr(int start_msb, int length_msb, int start_lsb, int length_lsb) const
  {
    return convert_from_smr<int64_t>(field_i64(start_msb, length_msb, start_lsb, length_lsb), length_msb + length_lsb);
  }

  template<std::size_t n_bits>
    template<typename T>
      inline T
      NavStr<n_bits>::field (int start, int length) const
      {
        return ((*reinterpret_cast<const T*>(this->data() + byte(start, length, sizeof(T)))) >> shift(start, length, sizeof(T))) & mask<T>(length);
      }

  template<std::size_t n_bits>
    template<typename T>
      inline void
      NavStr<n_bits>::set_field(T field, int start, int length)
      {
        T *ptr = reinterpret_cast<T*>(this->data() + byte(start, length, sizeof(T)));
        *ptr &= (~mask<T>(length));
        *ptr |= (field & mask<T>(length)) << shift(start, length, sizeof(T));
     }

  template<std::size_t n_bits>
    template<typename T>
      inline void
      NavStr<n_bits>::set_field_smr(T field, int start, int length)
      {
        set_field(convert_from_smr(field, length), start, length);
     }

  template<std::size_t n_bits>
    constexpr int
    NavStr<n_bits>::byte (int start, int length, int word_size) const
    {
      /* В связи с ограничениями constexpr функций, следующий закоментированный код: */
//          int n = (this->size() - word_size);
//          /* Если при чтении слова не выйдем за пределы массива, то
//           * вычисляем номер байта и возвращаем его */
//          if ()
//            return (n_bits - (start + length - 1)) / CHAR_BIT;
//          return n;
      /* пришлось записать в одну строку: */
      constexpr int n_bits_loc = static_cast<int>(n_bits);
      constexpr int char_bits = static_cast<int>(CHAR_BIT);
      return ((n_bits_loc - (start + length - 1)) <= ((static_cast<int>(this->size()) - word_size) * char_bits)) ? (n_bits_loc - (start + length - 1)) / char_bits : (static_cast<int>(this->size()) - word_size);
    }

  template<std::size_t n_bits>
    constexpr int
    NavStr<n_bits>::shift (int start, int length, int word_size) const
    {
      /* В связи с ограничениями constexpr функций, следующий закоментированный код: */
      // return (n_bits - (start + length - 1)) - byte(start, length, word_size) * CHAR_BIT;
      /* пришлось развернуть и записать следующим образом: */
      constexpr int n_bits_loc = static_cast<int>(n_bits);
      constexpr int char_bits = static_cast<int>(CHAR_BIT);
      return (n_bits_loc - (start + length - 1)) -
                                (((n_bits_loc - (start + length - 1)) <= ((static_cast<int>(this->size()) - word_size) * char_bits)) ?
                                (n_bits_loc - (start + length - 1)) / char_bits :
                                (static_cast<int>(this->size()) - word_size))
                                                            * char_bits;
    }

  template<std::size_t n_bits>
    template<typename T>
      constexpr T
      NavStr<n_bits>::mask (int length) const
      {
        return (static_cast<T>(1) << length) - static_cast<T>(1);
      }

  template<std::size_t n_bits>
    template<typename T>
      inline T
      NavStr<n_bits>::convert_from_smr (T smr, int length) const
      {
        T s = smr & (~mask<T>(length-1)); /* Выделяем знаковый бит */
        if (s != 0)
        {
          /* Если знаковый бит не равен нулю, то число отрицательное */
          smr = -1 * (smr & (~s)); /* Сбрасываем знаковый бит и умнажаем на -1 */
        }
        return smr;
      }

  template<std::size_t n_bits>
    template<typename T>
      inline T
      NavStr<n_bits>::convert_to_smr (T two_complement, int length) const
      {
        T s = 0;
        if (two_complement < 0)
        {
          s = 1 << (length - 1);
          two_complement *= -1;
        }
        return (two_complement & mask<T>(length - 1)) | s;
      }

} /* namespace navstr */

#endif /* NAVSTR_H_ */
