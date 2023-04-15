/*
 * Msg.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#ifndef MSG_H_
#define MSG_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

#include <frame2/msg_base.h>
#include <navstr/navstr_types.h>

#ifdef DEBUG_FRAME2
#include <iomanip>
#endif

namespace frame2
{
  template <size_t n_bits>
  class Msg : public MsgBase
  {
  public:
    /*
     * Конструктор:
     *   1. Msg () : mCrcError(0), mStr(rnl::NavString<n_bits>({0})) { }
     * работает медленнее, чем:
     *   2. Msg () { Clear(); }
     */
    Msg () { Clear(); }
    ~Msg (){ }

    void Clear();
    inline size_t ByteSize() const;

    inline size_t SerializeToArray(void* data) const;
    inline size_t ParseFromArray(const void* data);

//    void CopyFrom(const Msg<di_length>& from);

    // accessors -------------------------------------------------------

    // str - строка ЦИ
    inline const navstr::NavStr<n_bits>& str() const;
    inline size_t str_size() const;
    inline void set_str(const navstr::NavStr<n_bits>& value);
    inline uint8_t& mutable_str_at(int index);
    inline void clear_str();

    // crc - ошибка контрольной суммы (КС), если 0, то КС прошла проверку.
    inline uint32_t crc_error() const;          /* Ошибка проверки ЦК. Если 0, то нет ошибки */
    inline void set_crc_error(uint8_t flag);    /* Ошибка проверки ЦК. Если 0, то нет ошибки */

  private:

#pragma pack(push,1)
    struct Binary
    {
      uint8_t size; /* длина структуры в байтах */
      uint8_t crc_error;
      uint8_t str_byte0;    /* Первый байт строки ЦИ. Объявили его здесь только для удобства получения указателя */
    };
#pragma pack(pop)

    uint8_t mCrcError;
    navstr::NavStr<n_bits> mStr;
    static_assert(navstr::NavStr<n_bits>::nbytes() <= 253, "Size of NavString can not be bigger, then 253 byte");

  };

  // functions definition

  template<size_t n_bits>
    inline void
    Msg<n_bits>::Clear ()
    {
      mCrcError = 0;
      mStr.fill(0);
//      mStr.set(nullptr, 0);
    }

  template<size_t n_bits>
    inline size_t
    Msg<n_bits>::ByteSize () const
    {
      return mStr.size() + offsetof(Binary, str_byte0);
    }

  template<size_t n_bits>
    inline size_t
    Msg<n_bits>::SerializeToArray (void* data) const
    {
      Binary &bin = *(reinterpret_cast<Binary*> (data));
      bin.size = static_cast<uint8_t>(ByteSize ());
      bin.crc_error = mCrcError;
      size_t str_size = bin.size - offsetof(Binary, str_byte0);
      std::copy(mStr.begin(), mStr.begin() + str_size, &bin.str_byte0);
      return bin.size;
    }

  template<size_t n_bits>
    inline size_t
    Msg<n_bits>::ParseFromArray (const void* data)
    {
      const Binary &bin =  *(reinterpret_cast<const Binary*> (data));
      if (bin.size < offsetof(Binary, str_byte0))
        return 0;
      mCrcError = bin.crc_error;
      size_t str_n = static_cast<uint8_t>(bin.size - offsetof(Binary, str_byte0));
      mStr.set(&bin.str_byte0, str_n);
      return bin.size;
    }
//
//  template<size_t di_length>
//    inline void
//    Msg<di_length>::CopyFrom (const Msg<di_length>& from)
//    {
//    }

  template<size_t n_bits>
    inline const navstr::NavStr<n_bits>&
    Msg<n_bits>::str () const
    {
      return mStr;
    }

  template<size_t n_bits>
    inline size_t
    Msg<n_bits>::str_size () const
    {
      /* Не знаю, что лучше возвращать: sizeof(Str) или di_length */
      return mStr.size();
    }

  template<size_t n_bits>
    inline void
    Msg<n_bits>::set_str (const navstr::NavStr<n_bits>& value)
    {
      mStr = value;
    }

  template<size_t n_bits>
    inline uint8_t&
    Msg<n_bits>::mutable_str_at (int index)
    {
      if (index < (int)this->str_size())
      {
        return mStr[index];
      }
      else
      {
        /* Функция должна вернуть корректную ссылку */
        static uint8_t stub_element = 0;
        stub_element = 0; /* Стираем ранее записанное значение */
        return stub_element;
      }
    }

  template<size_t n_bits>
    inline void
    Msg<n_bits>::clear_str ()
    {
//      mStr.set(nullptr, 0);
      mStr.fill(0);
    }

  template<size_t n_bits>
    inline uint32_t
    Msg<n_bits>::crc_error () const
    {
      return mCrcError;
    }

  template<size_t n_bits>
    inline void
    Msg<n_bits>::set_crc_error (uint8_t value)
    {
      mCrcError = value;
    }

} /* namespace frame2 */

#endif /* MSG_H_ */
