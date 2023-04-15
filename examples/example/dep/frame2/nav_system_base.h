/*
 * nav_system_base.h
 *
 *  Created on: 22 авг. 2019 г.
 *      Author: user
 */

#ifndef FRAME2_NAV_SYSTEM_BASE_H_
#define FRAME2_NAV_SYSTEM_BASE_H_

#include <array>
#include <climits>
#include <cstdint>

#include <frame2/auxiliary.h>
#include <frame2/id.h>
#include <frame2/msg_base.h>
#include <frame2/obs.h>

#ifdef DEBUG_FRAME2
#include <iostream>
#include <iomanip>
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

    constexpr inline uint16_t Flag (int component) const
    {
      return static_cast<uint16_t>(1 << component);
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
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfAuxFlags & Flag(i)) != 0)
      {
        if ( !buf.IsSpaceAvailable(pAux[i].ByteSize()) )
        {
          Clear();
          return 0;
        }
        buf += pAux[i].ParseFromArray(buf.CurrentPointer());
      }
    }
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfObsFlags & Flag(i)) != 0)
      {
        if ( !buf.IsSpaceAvailable(pObs[i].ByteSize()) )
        {
          Clear();
          return 0;
        }
        buf += pObs[i].ParseFromArray(buf.CurrentPointer());
      }
    }
    for (int i = 0; i < cNumSignals; i++)
    {
      if ((mSetOfMsgFlags & Flag(i)) != 0)
      {
        if (pMsg[i] == nullptr)
        {
          /* Эта версия класса не поддерживает эту компоненту */
          // TODO: Добавилть правильную обработку неподдерживаемых компонент
          buf += static_cast<size_t>(*reinterpret_cast<const uint8_t*>(buf.CurrentPointer())) + 2;
          continue;
        }

        size_t byte_size = pMsg[i]->ByteSize();
        if ( !buf.IsSpaceAvailable(byte_size) )
        {
          Clear();
          return 0;
        }
        buf += pMsg[i]->ParseFromArray(buf.CurrentPointer());
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
