/*
 * header.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

namespace frame2
{

  class Header
  {
  public:
    Header ();
    ~Header ();

    enum {TS_UNDEF = 0, TS_GLO = 1, TS_GPS = 2, TS_GAL = 3};

    inline void Clear();
    inline size_t ByteSize() const;

    inline size_t SerializeToArray(void *data) const;
    inline size_t ParseFromArray(const void *data);

//    void CopyFrom(const Header& from);

    // accessors -------------------------------------------------------

    // timestamp_hw_ms - метка времени в аппаратной (hardware) шкале времени, число миллисекунд от запуска блока ЦОС
    inline int64_t timestamp_hw_ms() const;         // = timestamp_hw_ms
    inline void set_timestamp_hw_ms(int64_t value); // timestamp_hw_ms = value

    /*
     * timestamp_id - идентификатор навигационной системы, в которой сформирована метка времени timestamp_ms и выданы измерения в кадре:
     *    * 0 - шкала не определена ни по одной системе, измерения выдаются в аппаратной шкале на момент времени, указанный в поле timestamp_hw_ms;
     *    * 1 - шкала определена по навигационной системе ГЛОНАСС и содержит метку времени, равную числу миллисекунд от 00:00:00 01.01.1996 г. по Москве (UTC SU);
     *    * 2 - шкала определена по навигационной системе GPS и содержит метку времени, равную числу миллисекунд от 00:00:00 06.01.1980 г. в шкале GPS;
     *    * 3 - шкала определена по навигационной системе Galileo и содержит метку времени, равную числу миллисекунд от 23:59:47 21.08.1999 в шкале Galileo;
     */
    inline uint8_t timestamp_id() const;
    inline void set_timestamp_id(uint8_t value);

    // timestamp_ms - [мс] метка времени в миллисекундах, на которую сформированы все данные из этого кадра. Тип шкалы
    // времени, в которой сформирована эта метка времени определяется с помощью поля timestamp_id;
    inline int64_t timestamp_ms() const;            // = utc_timescale_ms
    inline void set_timestamp_ms(int64_t value);    // utc_timescale_ms = value

  private:

#pragma pack(push,1)
    struct Binary
    {
      int64_t timestamp_hw_ms;
      uint8_t timestamp_id;
      int64_t timescale_ms;
    };
#pragma pack(pop)

    Binary mBinary;

    static const Binary scDefaultBinary;
  };

  // functions definition

  inline void
  Header::Clear ()
  {
    mBinary = scDefaultBinary;
  }

  inline size_t
  Header::ByteSize () const
  {
    return sizeof(Binary);
  }

  inline size_t
  Header::SerializeToArray (void *data) const
  {
    *(reinterpret_cast<Binary*>(data)) = mBinary;
     return ByteSize ();
  }

  inline size_t
  Header::ParseFromArray (const void *data)
  {
    mBinary = *(reinterpret_cast<const Binary*>(data));
    return ByteSize ();
  }

  inline int64_t
  Header::timestamp_hw_ms () const
  {
    return mBinary.timestamp_hw_ms;
  }

  inline void
  Header::set_timestamp_hw_ms (int64_t value)
  {
    mBinary.timestamp_hw_ms = value;
  }

  // timestamp_id
  inline uint8_t
  Header::timestamp_id () const
  {
    return mBinary.timestamp_id;
  }
  inline void
  Header::set_timestamp_id (uint8_t value)
  {
    mBinary.timestamp_id = value;
  }

  inline int64_t
  Header::timestamp_ms () const
  {
    return mBinary.timescale_ms;
  }

  inline void
  Header::set_timestamp_ms (int64_t value)
  {
    mBinary.timescale_ms = value;
  }

} /* namespace frame2 */

#endif /* HEADER_H_ */
