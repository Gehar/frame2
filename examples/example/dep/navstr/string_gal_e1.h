/*
 * string_gal_e1.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: user
 */

#ifndef STRING_GAL_E1_H_
#define STRING_GAL_E1_H_

#include <array>
#include <cstdio>
#include <navstr/navstr_types.h>

namespace navstr
{

  class StringGalE1
  {
  public:
    StringGalE1 ();
    ~StringGalE1 ();

    static const int scINavDataLen = 128/8;
    typedef std::array<uint8_t, scINavDataLen> INavData;

    /*
     * Проверяет строку и в случае успешной проверки разбирает её.
     * Метод осуществляет следующие проверки:
     *   Поля Even/odd: 29 байт, бит 7 = 0; байт 14, бит 7 = 1;
     *   Поля Page Type: 29 байт, бит 6 = 0; байт 14, бит 6 = 0;
     *   Поля Tail = 0.
     * Если указанные поля имеют отличные значения, то строка не разбирается
     * и функция вернет true (есть ошибка).
     *   Поле CRCj не проверяется. Контрольная сумма должна проверяться перед
     * загрузкой строки в класс.
     *
     * Входные параметры:
     *   str - строка Galileo длиной 30 байт в формате frame2.
     * Возвращаемое значение:
     *   false - строка успешно разобрана;
     *   true - Данные в строке имеют ошибки, строка не разобрана.
     *
     * Примечание: Так как Reserved2 не защищено КС, то оно не читается
     * из входной строки и не возвращается.
     */
    inline bool SetStr(const navstr::GalE1 &str);

    // accessors -------------------------------------------------------
    inline uint32_t PageType() {return mPageType;};
    inline const INavData& Data() {return mINavData;};
    inline uint64_t Reserved1() {return mReserved1;};
    inline uint32_t Sar() {return mSar;};
    inline uint32_t Spare() {return mSpare;};
    inline uint32_t Crc() {return mCrc;};

  protected:
    inline bool CheckFields(const navstr::GalE1 &str);
    inline void ReadPageType(const navstr::GalE1 &str);   /* Читает бит из четной страницы */
    inline void ReadINavData(const navstr::GalE1 &str);
    inline void ReadReserved1(const navstr::GalE1 &str);
    inline void ReadSar(const navstr::GalE1 &str);
    inline void ReadSpare(const navstr::GalE1 &str);
    inline void ReadCrc(const navstr::GalE1 &str);

  private:
    /* Так как Zynq 32-битный, то используем uint32_t и кратные ему по размеру типы */
    uint32_t mPageType;
    INavData mINavData;
    uint64_t mReserved1;
    uint32_t mSar;
    uint32_t mSpare;
    uint32_t mCrc;
  };

  inline bool StringGalE1::SetStr(const navstr::GalE1 &str)
  {
    /* Проверяем, что это I/Nav nominal page */
    if (CheckFields(str))
      return true;
    ReadPageType(str);
    ReadINavData(str);
    ReadReserved1(str);
    ReadSar(str);
    ReadSpare(str);
    ReadCrc(str);
    return false;
  }

  inline bool StringGalE1::CheckFields(const navstr::GalE1 &str)
  {
    if (str.field_u32(1, 1) != 0x00) // Even/Odd = 0
      return true;
    if (str.field_u32(121, 1) != 0x01) // Even/Odd = 1
      return true;
    if (str.field_u32(2, 1) != 0x00) // Page Type = 0
      return true;
    if (str.field_u32(122, 1) != 0x00) // Page Type = 0
      return true;
    if ((str.field_u32(235, 6)) != 0x00) // Tail = 0b000000
      return true;
    if (str.field_u32(115, 6) != 0x00)  // Tail = 0b000000
      return true;
    return false;
  }

  inline void StringGalE1::ReadPageType(const navstr::GalE1 &str)
  {
    mPageType = str.field_u32(2, 1);
  }

  inline void StringGalE1::ReadINavData(const navstr::GalE1 &str)
  {
    uint32_t *p32 = reinterpret_cast<uint32_t*>(mINavData.data());
    p32[0] = str.field_u32(123, 16) | (str.field_u32(99, 16) << 16);
    p32[1] = static_cast<uint32_t>(str.field_u64(67, 32));
    p32[2] = static_cast<uint32_t>(str.field_u64(35, 32));
    p32[3] = static_cast<uint32_t>(str.field_u64(3, 32));
  }

  inline void StringGalE1::ReadReserved1(const navstr::GalE1 &str)
  {
    mReserved1 = str.field_u64(139, 40);
  }
  inline void StringGalE1::ReadSar(const navstr::GalE1 &str)
  {
    mSar = str.field_u32(179, 22);
  }
  inline void StringGalE1::ReadSpare(const navstr::GalE1 &str)
  {
    mSpare = str.field_u32(201, 2);
  }
  inline void StringGalE1::ReadCrc(const navstr::GalE1 &str)
  {
    mCrc = str.field_u32(203, 24);
  }
} /* namespace rnl */

#endif /* STRING_GAL_E1_H_ */
