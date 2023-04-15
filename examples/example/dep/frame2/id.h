/*
 * ID.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#ifndef ID_H_
#define ID_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

namespace frame2
{

  /* Класс идентификатора НКА для ГЛОНАСС */
  class Id
  {
  public:
    Id ();
    ~Id ();

    static const uint8_t scValueUndefined = 0xff;
    static const int8_t  scFreqNumUndefined = 0x7f;

    inline void Clear();
    inline size_t ByteSize() const;

    inline size_t SerializeToArray(void* data) const;
    inline size_t ParseFromArray(const void* data);

//    void CopyFrom(const Id& from);

    // accessors -------------------------------------------------------

    // channel - номер канала, по которому выданы измерения
    inline uint16_t channel() const;
    inline void set_channel(uint16_t value);

    // prn_num - номер псп, задается при установке канала на поиск (для сигналов с кодовым разделением).
    // Если содержит значение 255, то значение не задано
    inline uint8_t prn_num() const;
    inline void set_prn_num(uint8_t value);

    // freq_num - номер литерной частоты сигналов ГЛОНАСС с частотным разделением. Задается при установке канала на поиск.
    // Если содержит значение 255, то значение не задано
    inline int8_t freq_num() const;
    inline void set_freq_num(int8_t value);

    /*
     * sys_num:
     *      - для сигналов ГЛОНАСС - системный номер НКА в ОГ (параметр n A в ЦИ). Если содержит значение 255, то не определен;
     *      - для GPS (сигнал C/A) - space vehicle identifier (параметр SV ID в ЦИ). Если содержит значение 255, то не определен;
     *      - для GALILEO (сигнал E1bc) - space vehicle identifier (параметр SVID в ЦИ). Если содержит значение 255, то не определен;
     */
    inline uint8_t sys_num() const;
    inline void set_sys_num(uint8_t value);

    // azimuth - [градусы] азимут КА, если не определен, то NAN
    inline float azimuth() const;
    inline void set_azimuth(float value);

    // elevation - [градусы] угол места КА, если не определен, то NAN
    inline float elevation() const;
    inline void set_elevation(float value);

  private:

#pragma pack(push,1)
    struct Binary /* 14 байт */
    {
      /*
       * Следует ли перейти на битовую упаковку?
       * По ИКД у плей следующие размеры:
       *   prn_num   - 5 бит
       *   freq_num  - 5 бит
       *   sys_num - 6 бит
       *   data_num  - 6 бит
       * Итого: 22 бита достаточно, сейчас эти поля занимают 32 бита.
       * Если перейти на битовую упаковку, то под номер канала отведем 10 бит,
       * чего должно будет хватить. И размер структуры уменьшится с текущих 16 байт
       * до 12 байт.
       */
      uint16_t channel;
      uint8_t prn_num;
      int8_t  freq_num;
      uint8_t sys_num;
      float azimuth;
      float elevation;
    };
#pragma pack(pop)

    Binary mBinary;

    static const float fNaN;
    static const Binary scDefaultBinary;
  };

  // functions definition

  inline  void
  Id::Clear()
  {
    mBinary = scDefaultBinary;
  }

  inline size_t
  Id::ByteSize () const
  {
    return sizeof(Binary);
  }

  inline size_t
  Id::SerializeToArray (void* data) const
  {
    *(reinterpret_cast<Binary*>(data)) = mBinary;
    return ByteSize ();
  }

  inline size_t
  Id::ParseFromArray (const void* data)
  {
    mBinary = *(reinterpret_cast<const Binary*>(data));
    return ByteSize ();
  }

  inline uint16_t
  Id::channel () const
  {
    return mBinary.channel;
  }

  inline void
  Id::set_channel (uint16_t value)
  {
    mBinary.channel = value;
  }

  inline uint8_t
  Id::prn_num () const
  {
    return mBinary.prn_num;
  }

  inline void
  Id::set_prn_num (uint8_t value)
  {
    mBinary.prn_num = value;
  }

  inline int8_t
  Id::freq_num () const
  {
    return mBinary.freq_num;
  }

  inline void
  Id::set_freq_num (int8_t value)
  {
    mBinary.freq_num = value;
  }

  inline uint8_t
  Id::sys_num () const
  {
    return mBinary.sys_num;
  }

  inline void
  Id::set_sys_num (uint8_t value)
  {
    mBinary.sys_num = value;
  }

  inline float
  Id::azimuth () const
  {
    return mBinary.azimuth;
  }

  inline void
  Id::set_azimuth (float value)
  {
    mBinary.azimuth = value;
  }

  inline float
  Id::elevation () const
  {
    return mBinary.elevation;
  }

  inline void
  Id::set_elevation (float value)
  {
    mBinary.elevation = value;
  }


} /* namespace frame2 */

#endif /* ID_H_ */
