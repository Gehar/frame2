/*
 * frame2.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#ifndef FRAME2_H_
#define FRAME2_H_

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <vector>

#ifdef DEBUG_FRAME2
#include <iomanip>
#endif

#include <frame2/gal.h>
#include <frame2/glo.h>
#include <frame2/gps.h>
#include <frame2/header.h>

namespace frame2
{

  class Frame2
  {
  public:

      enum:uint8_t {SYS_UNDEF = 0, GLO = 1, GPS = 2, GAL = 3,};

    /* Класс проектировался для работы в приемнике. Поэтому память выделяется только
     * при создании объекта класса. Если во время использования объекта его память
     * закончится, то выделение дополнительной памяти не произойдет (не смотря на
     * использование vector). Объем выделяемой памяти определяется параметром
     * конструктора MaxChannelsCnt. */
    Frame2 (size_t MaxChannelsCnt);
    Frame2 (const Frame2 &frame) ;
    ~Frame2 ();

    inline void Clear();
    size_t ByteSize() const;

    inline size_t SerializeToArray(void* data, size_t size) const;
    inline size_t ParseFromArray(const void* data, size_t size);

//    void CopyFrom(const Frame2& from);

    // diagnostics -----------------------------------------------------

    /*
     * Возвращает число не распознанных функцией ParseFromArray() подкадров
     */
    inline size_t UnrecognizedSubframes() const;
    /*
     * Возвращает общее чило пропущеных (не обработанных) байт при
     * разборе извесных классу подкадров. Объем нераспозанных кадров
     * не возвращается этой функцией
     */
    inline size_t SizeOfOmitedData() const;

    // accessors -------------------------------------------------------

    inline uint32_t version() const;

    // header
    inline const Header& header() const;
    inline Header* mutable_header();

    // glo
    inline size_t glo_size() const;            /* Returns number of elements */
    inline void clear_glo();
    /* Возвращает константную ссылку на объект с измерениями по НКА.
     * Если объекта не существует, будет возвращена работающая ссылка на пустой объект */
    inline const Glo& glo(size_t index) const;
    /* Возвращает указатель на внутренний объект для добавлени/изменения данных.
     * Если объекта не существует, будет возвращен нулевой указатель */
    inline Glo* mutable_glo(size_t index);
    /* Добавляет еще один НКА в конец массива и возвращает указатель на
     * новый объект. Если на момент вызова функции в массиве будет число объектов, равное заданному
     * в конструкторе числу, то будет возвращен нулевой указатель. */
    inline Glo* add_glo();

    // gps
    inline size_t gps_size() const;            /* Returns number of elements */
    inline void clear_gps();
    /* Возвращает константную ссылку на объект с измерениями по НКА.
     * Если объекта не существует, будет возвращена работающая ссылка на пустой объект */
    inline const Gps& gps(size_t index) const;
    /* Возвращает указатель на внутренний объект для добавлени/изменения данных.
     * Если объекта не существует, будет возвращен нулевой указатель */
    inline Gps* mutable_gps(size_t index);
    /* Добавляет еще один НКА в конец массива и возвращает указатель на
     * новый объект. Если на момент вызова функции в массиве будет число объектов, равное заданному
     * в конструкторе числу, то будет возвращен нулевой указатель. */
    inline Gps* add_gps();

    // gal
    inline size_t gal_size() const;            /* Returns number of elements */
    inline void clear_gal();
    /* Возвращает константную ссылку на объект с измерениями по НКА.
     * Если объекта не существует, будет возвращена работающая ссылка на пустой объект */
    inline const Gal& gal(size_t index) const;
    /* Возвращает указатель на внутренний объект для добавлени/изменения данных.
     * Если объекта не существует, будет возвращен нулевой указатель */
    inline Gal* mutable_gal(size_t index);
    /* Добавляет еще один НКА в конец массива и возвращает указатель на
     * новый объект. Если на момент вызова функции в массиве будет число объектов, равное заданному
     * в конструкторе числу, то будет возвращен нулевой указатель. */
    inline Gal* add_gal();

  private:

    /*
     * Может быть все поля uint32_t данного уровня кодировать как в protobuf? См. ссылку:
     * https://developers.google.com/protocol-buffers/docs/encoding#varints
     * Принять решение нужно уже сейчас, так как позднее лучше не менять формат
     * первых двух полей с длиной и версией.
     *
     * Структура бинарных данных:
     *   uint32_t - идентификатор кадра;
     *   uint32_t - длина кадра, в том числе поле "идентификатор кадра";
     *   uint32_t - версия кадра;
     *   mHeader  - заголово кадра в бинарном формате;
     *       Далее массив подкадров (сейчас подкадр занимает
     *       uint32_t - объем байт, который занимает подкадр;
     *       uint8_t  - идентификатор подкадра;
     *       []       - данные подкадра.
     *
     *   На данный момент кадр frame2 может содержать только один подкадр с измерениями по ГЛОНАСС.
     *   После завершения разработки подкадра для ГЛОНАСС будут дабавлены подкадры GPS и Galileo.
     *   В перспективе могут появиться подкадры с телеметрией и лог-сообщениями.
     */

//    static const uint32_t scFrameId = 0x326d7266; /* = "frm2" */
    static constexpr uint32_t scFrameId = 0x326d7266; /* = "frm2" */
    uint32_t mSize;
    static constexpr uint32_t scVersion = 5;
    Header mHeader;
    uint32_t mSubframeSize;
    uint8_t mSubframeId;
    size_t mGloCnt;
    std::vector<Glo> mGlo;
    size_t mGpsCnt;
    std::vector<Gps> mGps;
    size_t mGalCnt;
    std::vector<Gal> mGal;

    size_t mUnrecognizedSubframes;
    size_t mSizeOfOmitedData;
  };

  // functions definition

  inline void
  Frame2::Clear ()
  {
    mHeader.Clear();
    mGloCnt = 0;
    mGpsCnt = 0;
    mGalCnt = 0;
    mSize = 0;
    mSubframeSize = 0;
    mSubframeId = 0;
    mUnrecognizedSubframes = 0;
    mSizeOfOmitedData = 0;
  }

  inline size_t
  Frame2::SerializeToArray (void* data, size_t size) const
  {
    Buffer buf(data, size);
    uint32_t data_size = static_cast<uint32_t>(ByteSize());
    if ( !buf.IsSpaceAvailable(data_size) )
      return 0; /* В переданном буфере не достаточно места */

    buf.Add(scFrameId);
    buf.Add(data_size);
    buf.Add(scVersion);
    buf += mHeader.SerializeToArray(buf.CurrentPointer());

    if (glo_size() > 0)
    {
      /* Сейчас не известен размер всех данных по ГЛОНАСС. Запомним ссылку
       * но место в буфере, где они хранятся, и продолжим сериализацию.
       * После завершения сериализации данных ГЛОНАСС сохраним в ссылку размер данных */
      uint32_t &glo_subframe_size = *(reinterpret_cast<uint32_t*>(buf.CurrentPointer()));
      buf += sizeof(mSubframeSize);

      uint8_t glo_id = GLO;
      buf.Add(glo_id);

      /* Можно, конечно,  */
      size_t glo_data_size = sizeof(mSubframeSize) + sizeof(mSubframeId);
      for (size_t i = 0; i < glo_size(); i++)
      {
        size_t ret_size = mGlo[i].SerializeToArray(buf.CurrentPointer(), size - static_cast<int>(buf.Position()));
        buf += ret_size;
        glo_data_size += ret_size;
      }

      glo_subframe_size = static_cast<uint32_t>(glo_data_size);
    }

    if (gps_size() > 0)
    {
      /* Сейчас не известен размер всех данных по нав. системе. Запомним ссылку
       * но место в буфере, где они хранятся, и продолжим сериализацию.
       * После завершения сериализации данных по нав. системе сохраним в ссылку размер данных */
      uint32_t &sys_subframe_size = *(reinterpret_cast<uint32_t*>(buf.CurrentPointer()));
      buf += sizeof(mSubframeSize);

      uint8_t navsys_id = GPS;
      buf.Add(navsys_id);

      size_t navsys_data_size = sizeof(mSubframeSize) + sizeof(mSubframeId);
      for (size_t i = 0; i < gps_size(); i++)
      {
        size_t ret_size = mGps[i].SerializeToArray(buf.CurrentPointer(), size - buf.Position());
        buf += ret_size;
        navsys_data_size += ret_size;
      }

      sys_subframe_size = static_cast<uint32_t>(navsys_data_size);
    }

    if (gal_size() > 0)
    {
      /* Сейчас не известен размер всех данных по нав. системе. Запомним ссылку
       * но место в буфере, где они хранятся, и продолжим сериализацию.
       * После завершения сериализации данных по нав. системе сохраним в ссылку размер данных */
      uint32_t &sys_subframe_size = *(reinterpret_cast<uint32_t*>(buf.CurrentPointer()));
      buf += sizeof(mSubframeSize);

      uint8_t navsys_id = GAL;
      buf.Add(navsys_id);

      size_t navsys_data_size = sizeof(mSubframeSize) + sizeof(mSubframeId);
      for (size_t i = 0; i < gal_size(); i++)
      {
        size_t ret_size = mGal[i].SerializeToArray(buf.CurrentPointer(), size - buf.Position());
        buf += ret_size;
        navsys_data_size += ret_size;
      }

      sys_subframe_size = static_cast<uint32_t>(navsys_data_size);
    }
    return buf.Position();
  }

  /*
   * При разборе кадра, если встретится неподдерживаемый
   * подкадр (subframe), то он будет пропущен. Если в подкадре будет слишком
   * много данных, то будет считано столько, сколько выделено памяти, остальное
   * будет пропущено. Узнать о пропущенных подкадрах и данных можно с помощью функций
   * UnrecognizedSubframes() и OmitedDataSize().
   * Функция возвращает количество разобранных байт (в том числе пропущенных).
   * После завершения работы функции рекомендуется сравнить переданный в неё размер
   * и возвращенное значение, чтобы убедиться, что функция отработала штатно
   * и разобрала весь кадр.
   */
  inline size_t
  Frame2::ParseFromArray (const void* data, size_t size)
  {
#ifdef DEBUG_FRAME2
    const uint8_t *dbg_buf = reinterpret_cast<const uint8_t*>(data);
    std::ios_base::fmtflags f(std::cout.flags()); /* save output format */
    std::cout << "[" << typeid(*this).name() << "::" << __func__ << "] full Frame2: ";
    for (size_t i = 0; i < size; i++)
    {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)dbg_buf[i] << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout.flags(f); /* restore output format */
#endif

    Clear();

    ConstBuffer buf(data, size);

    /* Проверяем, что есть поля "идентификатор", "длина" и "версия" кадра */
    size_t expected_size = sizeof(scFrameId) + sizeof(mSize) + sizeof(scVersion);
    if ( size < expected_size )
      return 0; /* В переданном буфере нет достаточного количества данных */

    uint32_t frame_id = 0;
    uint32_t version = 0;
    buf.Get(frame_id);
    buf.Get(mSize);
    buf.Get(version);

    /* Проверяем длину кадра */
    if (size < mSize)
      return 0; /* Размер буфера меньше размера кадра */

    /* В буфере достаточно данных, ограничиваем размер читаемых данных */
    size = mSize;

    /* Проверяем идентификатор кадра */
    if (frame_id != scFrameId)
      return 0; /* Это не Frame2 */

    /* Проверяем версию кадра */
    if (version != scVersion)
      return 0; /* Версия кадра не поддерживается */

    expected_size += mHeader.ByteSize();
    if ( size < expected_size)
    {
      /* Нет заголовка, а он должен быть всегда */
      Clear();
      return 0;
    }
    buf += mHeader.ParseFromArray(buf.CurrentPointer());

#ifdef DEBUG_FRAME2
    f = std::ios_base::fmtflags(std::cout.flags()); /* save output format */
    std::cout << "[" << typeid(*this).name() << "::" << __func__ << "] full header: ";
    for (; dbg_buf < buf.CurrentPointer(); dbg_buf++)
    {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(*dbg_buf) << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout.flags(f); /* restore output format */
#endif

    while (expected_size < size)
    {
      /*
       * Читаем подкадры. Варианты развития событий при чтении данных:
       *   1. Подкадр отсутствует. В этом случае не будет полей mSubframeSize
       *      и mSubframeId - значит кажр закончился;
       *   2. Подкадр не поддерживается ПО. Тогда пропускаем этот подкадр и
       *      переходим к следующему подкадру;
       *   3. Подкадр поддерживается, но содержит много данных. Читаем столько
       *      данных, сколькод доступно памяти, оставшиеся пропускаем.
       *   4. Подкадр поддерживается, все данные разобраны. Разбор прошел штатно.
       */

      constexpr uint32_t subframe_header_size = sizeof(mSubframeSize) + sizeof(mSubframeId);
      expected_size += subframe_header_size;
      if ( size < expected_size)
      {
        /* 1. Подкадр отсутствует. Кадр закончился */
        return buf.Position();
      }
      buf.Get(mSubframeSize);
      buf.Get(mSubframeId);

      if (mSubframeSize < subframe_header_size)
      {
        /* Так как подзаголовок явно имеет неверный формат, то считаем,
         * что эти данные не разобраны. */
        mSizeOfOmitedData += (mSize - (expected_size - subframe_header_size));
        return buf.Position() - subframe_header_size;
      }

      switch (mSubframeId)
      {
        case GLO:
        {
          size_t parsed_subframe_size = subframe_header_size;
          while (parsed_subframe_size < mSubframeSize)
          {
            /* Чтобы недопустить выделения памяти, если в данных окажется больше элементов, чем было выделено памяти */
            if (glo_size() < mGlo.size())
            {
              size_t glo_len = add_glo()->ParseFromArray(buf.CurrentPointer(), mSubframeSize - parsed_subframe_size);
              buf += glo_len;
              parsed_subframe_size += glo_len;
            }
            else
            {
              /* 3. Подкадр поддерживается, но содержит много данных. */
              buf += mSubframeSize - parsed_subframe_size;
              mSizeOfOmitedData += mSubframeSize - parsed_subframe_size;
              break;
            }
          }
          /* Закончили работать с подкадром */
          expected_size += mSubframeSize - subframe_header_size;
          break;
        }
        case GPS:
        {
          size_t parsed_subframe_size = subframe_header_size;
          while (parsed_subframe_size < mSubframeSize)
          {
            /* Чтобы недопустить выделения памяти, если в данных окажется больше элементов, чем было выделено памяти */
            if (gps_size() < mGps.size())
            {
              size_t sat_len = add_gps()->ParseFromArray(buf.CurrentPointer(), mSubframeSize - parsed_subframe_size);
              buf += sat_len;
              parsed_subframe_size += sat_len;
            }
            else
            {
              /* 3. Подкадр поддерживается, но содержит много данных. */
              buf += mSubframeSize - parsed_subframe_size;
              mSizeOfOmitedData += mSubframeSize - parsed_subframe_size;
              break;
            }
          }
          /* Закончили работать с подкадром */
          expected_size += mSubframeSize - subframe_header_size;
          break;
        }
        case GAL:
        {
          size_t parsed_subframe_size = subframe_header_size;
          while (parsed_subframe_size < mSubframeSize)
          {
            /* Чтобы недопустить выделения памяти, если в данных окажется больше элементов, чем было выделено памяти */
            if (gal_size() < mGal.size())
            {
              size_t sat_len = add_gal()->ParseFromArray(buf.CurrentPointer(), mSubframeSize - parsed_subframe_size);
              buf += sat_len;
              parsed_subframe_size += sat_len;
            }
            else
            {
              /* 3. Подкадр поддерживается, но содержит много данных. */
              buf += mSubframeSize - parsed_subframe_size;
              mSizeOfOmitedData += mSubframeSize - parsed_subframe_size;
              break;
            }
          }
          /* Закончили работать с подкадром */
          expected_size += mSubframeSize - subframe_header_size;
          break;
        }
        default:
          /* 2. Подкадр не поддерживается ПО. Пропускаем подкадр.*/
          expected_size += mSubframeSize - subframe_header_size;
          buf += mSubframeSize - subframe_header_size;
          mUnrecognizedSubframes++; /* Увеличиваем счетчик нераспознанных подкадров */
          break;
      }
    }
    return buf.Position();
  }

//  void
//  Frame2::CopyFrom (const Frame2& from)
//  {
//  }

  inline size_t
  Frame2::UnrecognizedSubframes() const
  {
    return mUnrecognizedSubframes;
  }

  inline size_t
  Frame2::SizeOfOmitedData() const
  {
    return mSizeOfOmitedData;
  }

  inline uint32_t
  Frame2::version() const
  {
    return scVersion;
  }

  inline const Header&
  Frame2::header () const
  {
    return mHeader;
  }

  inline Header*
  Frame2::mutable_header ()
  {
    return &mHeader;
  }

  //===========================================================================
  // glo
  inline size_t
  Frame2::glo_size () const
  {
    return mGloCnt;
  }
  inline void
  Frame2::clear_glo ()
  {
    mGloCnt = 0;;
  }
  inline const Glo&
  Frame2::glo (size_t index) const
  {
    if (index < glo_size())
    {
      return mGlo[index];
    }
    else
    {
      static Glo glo;
      // TODO: добавить очистку glo
      return glo;
    }
  }
  inline Glo*
  Frame2::mutable_glo (size_t index)
  {
    if (index < glo_size())
    {
      return &mGlo[index];
    }
    else
    {
      return NULL;
    }
  }
  inline Glo*
  Frame2::add_glo ()
  {
    if (glo_size() < mGlo.size())
    {
      mGlo[mGloCnt].Clear();
      return &mGlo[mGloCnt++];
    }
    else
    {
      return NULL;
    }
  }

  //===========================================================================
  // gps
  inline size_t
  Frame2::gps_size () const
  {
    return mGpsCnt;
  }
  inline void
  Frame2::clear_gps ()
  {
    mGpsCnt = 0;
  }
  inline const Gps&
  Frame2::gps (size_t index) const
  {
    if (index < gps_size())
    {
      return mGps[index];
    }
    else
    {
      static Gps gps;
      // TODO: добавить очистку gps
      return gps;
    }
  }
  inline Gps*
  Frame2::mutable_gps (size_t index)
  {
    if (index < gps_size())
    {
      return &mGps[index];
    }
    else
    {
      return NULL;
    }
  }
  inline Gps*
  Frame2::add_gps ()
  {
    if (gps_size() < mGps.size())
    {
      mGps[mGpsCnt].Clear();
      return &mGps[mGpsCnt++];
    }
    else
    {
      return NULL;
    }
  }

  //===========================================================================
  // gal
  inline size_t
  Frame2::gal_size () const
  {
    return mGalCnt;
  }
  inline void
  Frame2::clear_gal ()
  {
    mGalCnt = 0;
  }
  inline const Gal&
  Frame2::gal (size_t index) const
  {
    if (index < gal_size())
    {
      return mGal[index];
    }
    else
    {
      static Gal sat;
      return sat;
    }
  }
  inline Gal*
  Frame2::mutable_gal (size_t index)
  {
    if (index < gal_size())
    {
      return &mGal[index];
    }
    else
    {
      return NULL;
    }
  }
  inline Gal*
  Frame2::add_gal ()
  {
    if (gal_size() < mGal.size())
    {
      mGal[mGalCnt].Clear();
      return &mGal[mGalCnt++];
    }
    else
    {
      return NULL;
    }
  }

} /* namespace frame2 */

#endif /* FRAME2_H_ */
