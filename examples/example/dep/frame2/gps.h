/*
 * Gps.h
 *
 *  Created on: 20 авг. 2019 г.
 *      Author: user
 */

#ifndef FRAME2_GPS_H_
#define FRAME2_GPS_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

#include <frame2/auxiliary.h>
#include <frame2/buffer.h>
#include <frame2/const_buffer.h>
#include <frame2/id.h>
#include <frame2/msg.h>
#include <frame2/obs.h>

namespace frame2
{

  class Gps
  {
  public:
    Gps ();
    ~Gps ();

    Gps& operator=(const Gps& gps);

    /* Тип объекта для работы со блоками frame2, содержащих строки */
    typedef frame2::Msg<navstr::GpsLnavBitsNum> MsgL1ca;

    inline void Clear();
    inline size_t ByteSize() const;

    /* Возвращает 0 в случае ошибки, либо количество записаных байт */
    inline size_t SerializeToArray(void* data, size_t size) const;
    /* Возвращает 0 в случае ошибки, либо количество прочитанных байт */
    inline size_t ParseFromArray(const void* data, size_t size);

//    void CopyFrom(const Gps& from);

    // accessors -------------------------------------------------------

    // id - всегда пристутсвует
    inline const Id& id() const;
    inline Id* mutable_id();

    // l1ca_obs
    inline bool has_l1ca_obs() const;
    inline void clear_l1ca_obs();
    inline const Obs& l1ca_obs() const;
    inline Obs* mutable_l1ca_obs();
    inline Obs* add_l1ca_obs();

    // l1ca_aux
    inline bool has_l1ca_aux() const;
    inline void clear_l1ca_aux();
    inline const Auxiliary& l1ca_aux() const;
    inline Auxiliary* mutable_l1ca_aux();
    inline Auxiliary* add_l1ca_aux();

    // l1ca_msg
    inline bool has_l1ca_msg() const;
    inline void clear_l1ca_msg();
    inline const MsgL1ca& l1ca_msg() const;
    inline MsgL1ca* mutable_l1ca_msg();
    inline MsgL1ca* add_l1ca_msg();

  private:
    struct Component
    {
      enum c
      {
        L1CA = 0, MAX,
      };
    };

    /*=======================================================================================*/
    /*    Порядок следования полей класса отражает их порядок в бинарной структуре данных    */

    /*
     * Следующие поля содержат набор битовых флагов. Каждый флаг
     * отражает наличие одного блока данных. Размер полей был выбран
     * равным 16 бит специально, чтобы Arm мог загрузить битовую
     * маску за одну команду.
     */
    uint16_t mSetObsFlags;
    uint16_t mSetMsgFlags;
    uint16_t mSetAuxFlags;

    /* Блок Id всегда есть, остальные блоки могут отсутствовать */
    Id mId;
    Auxiliary mAux[Component::MAX];
    Obs mObs[Component::MAX];

    /*
     * Объекты для ЦИ нельзя объявить одним массивом,
     * так как для разных сигналов разная длина строк,
     * следовательно разные типы объектов (у них
     * разная длина)
     * Update: Вообще-то можно, если объявлить объединение,
     * в нем перечислить все возможные типы Msg, то тогда
     * можно создать массив этих объединений
     * */
    MsgL1ca mMsgL1ca;

    /* Конец части класса, описывающей порядок следования полей в бинарной структуре */
    /*=======================================================================================*/

    constexpr inline uint16_t Flag (Component::c component) const
    {
      return static_cast<uint16_t>(1 << component);
    }
  };

  inline void
  Gps::Clear()
  {
    mSetObsFlags = 0;
    mSetMsgFlags = 0;
    mSetAuxFlags = 0;
    mId.Clear();
    for (int i = 0; i < Component::MAX; i++)
    {
      mObs[i].Clear();
      mAux[i].Clear();
    }
    mMsgL1ca.Clear();
  }

  inline size_t
  Gps::ByteSize () const
  {
    size_t size = 0;
    size += sizeof(mSetObsFlags);
    size += sizeof(mSetMsgFlags);
    size += sizeof(mSetAuxFlags);
    size += mId.ByteSize();
    if (has_l1ca_aux())
      size += l1ca_aux().ByteSize();
    if (has_l1ca_obs())
      size += l1ca_obs().ByteSize();
    if (has_l1ca_msg())
      size += l1ca_msg().ByteSize();
    return size;
  }

  inline size_t
  Gps::SerializeToArray (void* data, size_t size) const
  {
    Buffer buf(data, size);
    if ( !buf.IsSpaceAvailable(ByteSize()) )
      return 0; /* В переданном буфере не достаточно места */

    buf.Add(mSetObsFlags);
    buf.Add(mSetMsgFlags);
    buf.Add(mSetAuxFlags);
    buf += mId.SerializeToArray(buf.CurrentPointer());

    if (has_l1ca_aux())
      buf += l1ca_aux().SerializeToArray(buf.CurrentPointer());

    if (has_l1ca_obs())
      buf += l1ca_obs().SerializeToArray(buf.CurrentPointer());

    if (has_l1ca_msg())
      buf += l1ca_msg().SerializeToArray(buf.CurrentPointer());

    return buf.Position();
  }

  inline size_t
  Gps::ParseFromArray (const void* data, size_t size)
  {
#ifdef DEBUG_FRAME2
    const uint8_t *dbg_buf = reinterpret_cast<const uint8_t*>(data);
#endif

    ConstBuffer buf(data, size);

    size_t min_size = sizeof(mSetObsFlags) + sizeof(mSetMsgFlags) + mId.ByteSize();
    if ( size < min_size )
      return 0; /* В переданном буфере нет достаточного количества данных */

    buf.Get(mSetObsFlags);
    buf.Get(mSetMsgFlags);
    buf.Get(mSetAuxFlags);
    buf += mId.ParseFromArray(buf.CurrentPointer());

    /*
     * Поля класса mSetOf*Flags уже прочитаны, можно пользоваться методами has_*()
     */
    if (has_l1ca_aux())
    {
      if ( !buf.IsSpaceAvailable(l1ca_aux().ByteSize()) )
      {
        Clear();
        return 0;
      }
      buf += mutable_l1ca_aux()->ParseFromArray(buf.CurrentPointer());
    }

    if (has_l1ca_obs())
    {
      if ( !buf.IsSpaceAvailable(l1ca_obs().ByteSize()) )
      {
        Clear();
        return 0;
      }
      buf += mutable_l1ca_obs()->ParseFromArray(buf.CurrentPointer());
    }

    if (has_l1ca_msg())
    {
      if ( !buf.IsSpaceAvailable(l1ca_msg().ByteSize()) )
      {
        Clear();
        return 0;
      }
      buf += mutable_l1ca_msg()->ParseFromArray(buf.CurrentPointer());
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

//  void
//  Gps::CopyFrom (const Gps& from)
//  {
//  }

  inline const Id&
  Gps::id () const
  {
    return mId;
  }
  inline Id*
  Gps::mutable_id ()
  {
    return &mId;
  }

  // l1ca_obs
  inline bool
  Gps::has_l1ca_obs () const
  {
    return (mSetObsFlags & Flag(Component::L1CA)) != 0;
  }
  inline void
  Gps::clear_l1ca_obs ()
  {
    mSetObsFlags = mSetObsFlags & static_cast<uint16_t>(~Flag(Component::L1CA));
    mObs[Component::L1CA].Clear();
  }
  inline const Obs&
  Gps::l1ca_obs () const
  {
    return mObs[Component::L1CA];
  }
  inline Obs*
  Gps::mutable_l1ca_obs ()
  {
    return &mObs[Component::L1CA];
  }
  inline Obs*
  Gps::add_l1ca_obs ()
  {
    mObs[Component::L1CA].Clear();
    mSetObsFlags = mSetObsFlags | Flag(Component::L1CA);
    return &mObs[Component::L1CA];
  }

  // l1ca_aux
  inline bool
  Gps::has_l1ca_aux () const
  {
    return (mSetAuxFlags & Flag(Component::L1CA)) != 0;
  }
  inline void
  Gps::clear_l1ca_aux ()
  {
    mSetAuxFlags = mSetAuxFlags & static_cast<uint16_t>(~Flag(Component::L1CA));
    mAux[Component::L1CA].Clear();
  }
  inline const Auxiliary&
  Gps::l1ca_aux () const
  {
    return mAux[Component::L1CA];
  }
  inline Auxiliary*
  Gps::mutable_l1ca_aux ()
  {
    return &mAux[Component::L1CA];
  }
  inline Auxiliary*
  Gps::add_l1ca_aux ()
  {
    mAux[Component::L1CA].Clear();
    mSetAuxFlags = mSetAuxFlags | Flag(Component::L1CA);
    return &mAux[Component::L1CA];
  }

  // l1ca_msg
  inline bool
  Gps::has_l1ca_msg () const
  {
    return (mSetMsgFlags & Flag(Component::L1CA)) != 0;
  }
  inline void
  Gps::clear_l1ca_msg ()
  {
    mSetMsgFlags = (mSetMsgFlags & static_cast<uint16_t>(~Flag(Component::L1CA)));
    mMsgL1ca.Clear();
  }
  inline const Gps::MsgL1ca&
  Gps::l1ca_msg () const
  {
    return mMsgL1ca;
  }
  inline Gps::MsgL1ca*
  Gps::mutable_l1ca_msg ()
  {
    return &mMsgL1ca;
  }
  inline Gps::MsgL1ca*
  Gps::add_l1ca_msg ()
  {
    mMsgL1ca.Clear();
    mSetMsgFlags = mSetMsgFlags | Flag(Component::L1CA);
    return &mMsgL1ca;
  }

} /* namespace frame2 */

#endif /* FRAME2_GPS_H_ */
