/*
 * nav_system_base.h
 *
 *  Created on: 22 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef FRAME2_NAV_SYSTEM_BASE_H_
#define FRAME2_NAV_SYSTEM_BASE_H_

#include <array>
#include <climits>
#include <cstdint>

#include <frame2/auxiliary.h>
#include <frame2/id.h>
#include <frame2/msg.h>
#include <frame2/msg_base.h>
#include <frame2/obs.h>

#ifdef DEBUG_FRAME2
#include <iostream>
#include <iomanip>
#endif

#if  (__GNUC__ < 7) || \
    ((__GNUC__ == 7) && (__GNUC_MINOR__ < 2))

// gcc версии ниже 7.2.0 не поддерживает "constexpr inline" методы
#define CONSTEXPR_INLINE

#else

// constexpr inline методы поддерживаются gcc, начиная с версии 7.2.0
#define CONSTEXPR_INLINE constexpr inline

#endif

namespace frame2
{

  class NavSystemBase
  {
  protected:
    NavSystemBase (const int NumSignals);
    virtual
    ~NavSystemBase ();

  public:
    inline void Clear();
    inline size_t ByteSize() const;

    /* Возвращает 0 в случае ошибки, либо количество записаных байт */
    inline size_t SerializeToArray(void* data, size_t size) const;
    /* Возвращает 0 в случае ошибки, либо количество прочитанных байт */
    inline size_t ParseFromArray(const void* data, size_t size);

  protected:

    const int cNumSignals;

    /*
     * Следующие поля содержат набор битовых флагов. Каждый флаг
     * отражает наличие одного блока данных. Размер полей был выбран
     * равным 16 бит специально, чтобы Arm мог загрузить битовую
     * маску за одну команду.
     */
    uint16_t mSetOfObsFlags;
    uint16_t mSetOfMsgFlags;
    uint16_t mSetOfAuxFlags;

    /* Блок Id всегда есть, остальные блоки могут отсутствовать */
    Id *pId;
    Auxiliary *pAux;
    Obs *pObs;
    std::array<MsgBase*, CHAR_BIT*sizeof(mSetOfMsgFlags)> pMsg;

    CONSTEXPR_INLINE uint16_t Flag (int component) const
    {
      return static_cast<uint16_t>(1 << component);
    }

    /* Возвращает индекс старшего единичного бита. Если val = 0, то резльтат не определен.
     * пример: MsbBitIndex(1) = 1; MsbBitIndex(2) = 2; MsbBitIndex(6) = 3;*/
    inline uint16_t MsbBitIndex(uint16_t val)
    {
      return static_cast<uint16_t>((CHAR_BIT * sizeof(unsigned int)) - __builtin_clz(static_cast<unsigned int>(val)));
    }
  };

  // functions definition

  inline void
  NavSystemBase::Clear ()
  {

    mSetOfObsFlags = 0;
    mSetOfMsgFlags = 0;
    mSetOfAuxFlags = 0;
    pId->Clear();
    for (int i = 0; i < cNumSignals; i++)
    {
      pObs[i].Clear();
      pAux[i].Clear();
      if (pMsg[i] != nullptr)
        pMsg[i]->Clear();
    }
  }

  inline size_t
  NavSystemBase::ByteSize() const
  {
    size_t size = 0;
    size += sizeof(mSetOfObsFlags);
    size += sizeof(mSetOfMsgFlags);
    size += sizeof(mSetOfAuxFlags);
    size += pId->ByteSize();
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfObsFlags & Flag(i)) != 0)
        size += pObs[i].ByteSize();
      if ((mSetOfMsgFlags & Flag(i)) != 0)
        size += pMsg[i]->ByteSize();
      if ((mSetOfAuxFlags & Flag(i)) != 0)
        size += pAux[i].ByteSize();
    }
    return size;
  }

  inline size_t
  NavSystemBase::SerializeToArray(void* data, size_t size) const
  {
    Buffer buf(data, size);
    if ( !buf.IsSpaceAvailable(ByteSize()) )
      return 0; /* В переданном буфере не достаточно места */

    buf.Add(mSetOfObsFlags);
    buf.Add(mSetOfMsgFlags);
    buf.Add(mSetOfAuxFlags);
    buf += pId->SerializeToArray(buf.CurrentPointer());

    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfAuxFlags & Flag(i)) != 0)
        buf += pAux[i].SerializeToArray(buf.CurrentPointer());
    }
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfObsFlags & Flag(i)) != 0)
        buf += pObs[i].SerializeToArray(buf.CurrentPointer());
    }
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfMsgFlags & Flag(i)) != 0)
        buf += pMsg[i]->SerializeToArray(buf.CurrentPointer());
    }
    return buf.Position();
  }

  inline size_t
  NavSystemBase::ParseFromArray(const void* data, size_t size)
  {
#ifdef DEBUG_FRAME2
    const uint8_t *dbg_buf = reinterpret_cast<const uint8_t*>(data);
#endif

    ConstBuffer buf(data, size);

    size_t min_size = sizeof(mSetOfObsFlags) + sizeof(mSetOfMsgFlags) + pId->ByteSize();
    if ( size < min_size )
      return 0; /* В переданном буфере нет достаточного количества данных */

    buf.Get(mSetOfObsFlags);
    buf.Get(mSetOfMsgFlags);
    buf.Get(mSetOfAuxFlags);
    buf += pId->ParseFromArray(buf.CurrentPointer());

    /*
     * Поля класса mSetOf*Flags уже прочитаны, можно пользоваться методами has_*()
     */

    /* Разбираем блоки aux */
    if (mSetOfAuxFlags != 0)
    {
      int ncomponents = MsbBitIndex(mSetOfAuxFlags);
      for (int i = 0; i < ncomponents; i++)
      {
        if ((mSetOfAuxFlags & Flag(i)) != 0)
        {
          if ( !buf.IsSpaceAvailable(pAux[i].ByteSize()) )
          {
            Clear();
            return 0;
          }
          if (i < cNumSignals)
          {
            /* Эта компонента сигнала поддерживается классом */
            buf += pAux[i].ParseFromArray(buf.CurrentPointer());
          }
          else
          {
            /* Эта компонента сигнала не поддерживается классом */
            Auxiliary aux;
            buf += aux.ParseFromArray(buf.CurrentPointer());
          }
        }
      }
    }

    /* Разбираем блоки obs */
    if (mSetOfObsFlags != 0)
    {
      int ncomponents = MsbBitIndex(mSetOfObsFlags);
      for (int i = 0; i < ncomponents; i++)
      {
        if ((mSetOfObsFlags & Flag(i)) != 0)
        {
          if ( !buf.IsSpaceAvailable(pObs[i].ByteSize()) )
          {
            Clear();
            return 0;
          }
          if (i < cNumSignals)
          {
            /* Эта компонента сигнала поддерживается классом */
            buf += pObs[i].ParseFromArray(buf.CurrentPointer());
          }
          else
          {
            /* Эта компонента сигнала не поддерживается классом */
            Obs obs;
            buf += obs.ParseFromArray(buf.CurrentPointer());
          }
        }
      }
    }

    /* Разбираем блоки msg */
    if (mSetOfMsgFlags != 0)
    {
      int ncomponents = MsbBitIndex(mSetOfMsgFlags);
      for (int i = 0; i < ncomponents; i++)
      {
        if ((mSetOfMsgFlags & Flag(i)) != 0)
        {
          size_t ret_size = 0;
          bool msg_support = false;
          if (i < cNumSignals)
          {
            if (pMsg[i] != nullptr)
              msg_support = true;
          }
          if (msg_support)
          {
            ret_size = pMsg[i]->ParseFromArray(buf.CurrentPointer(), buf.Size() - buf.Position());
          }
          else
          {
            /* Эта версия класса не поддерживает эту компоненту */
            frame2::Msg<CHAR_BIT * (UCHAR_MAX - 3)> msg;
            ret_size = msg.ParseFromArray(buf.CurrentPointer(), buf.Size() - buf.Position());
          }

          if (ret_size == 0)
          {
            /* Не удалось разобрать блок со строкой */
  #ifdef DEBUG_FRAME2
            std::ios_base::fmtflags f(std::cout.flags()); /* save output format */
            std::cout << "[" << typeid(*this).name() << "::" << __func__ << "] ERROR: current Msg flag 0x"
                << std::hex << std::setfill('0') << std::setw(2) << Flag(i)
                << ", Msg block size " << std::dec << pMsg[i]->ByteSize()
                << ", available space of buffer " << buf.Size() - buf.Position() << std::endl;

            std::cout << "[" << typeid(*this).name() << "::" << __func__ << "]        Buffer tail: ";
            for (size_t i = buf.Position(); i < buf.Size(); i++)
            {
              std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)dbg_buf[i] << ' ';
            }
            std::cout << std::dec << std::endl;
            std::cout.flags(f); /* restore output format */
  #endif
            Clear();
            return 0;
          }
          buf += ret_size;
        }
      }
    }
#ifdef DEBUG_FRAME2
    std::ios_base::fmtflags f(std::cout.flags()); /* save output format */
    std::cout << "[" << typeid(*this).name() << "::" << __func__ << "] ";
    for (size_t i = 0; i < buf.Position(); i++)
    {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)dbg_buf[i] << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout.flags(f); /* restore output format */
#endif
    return buf.Position();
  }

} /* namespace frame2 */

#endif /* FRAME2_NAV_SYSTEM_BASE_H_ */
