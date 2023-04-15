/*
 * glo.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#ifndef GLO_H_
#define GLO_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

#include <frame2/auxiliary.h>
#include <frame2/buffer.h>
#include <frame2/const_buffer.h>
#include <frame2/id.h>
#include <frame2/msg.h>
#include <frame2/nav_system_base.h>
#include <frame2/obs.h>

namespace frame2
{
  class Glo : public NavSystemBase
  {

  public:
    Glo ();
    Glo (const Glo &glo);
    ~Glo ();

    Glo& operator=(const Glo& glo);

    /* Объекты блоков для работы со строками кадра */
    typedef frame2::Msg<navstr::GloOfBitsNum> MsgOf;
    typedef frame2::Msg<navstr::GloL1ofBitsNum> MsgL1of;
    typedef frame2::Msg<navstr::GloL1cfgBitsNum> MsgL1cfg;
    typedef frame2::Msg<navstr::GloL2ofBitsNum> MsgL2of;
    typedef frame2::Msg<navstr::GloL2cfgBitsNum> MsgL2cfg;
    /* Аномальные строки не поддерживаются */
    typedef frame2::Msg<navstr::GloL1ocBitsNum> MsgL1oc;
    typedef frame2::Msg<navstr::GloL3ocBitsNum> MsgL3oc;
    typedef frame2::Msg<navstr::GloSrlBitsNum> MsgSrl;


    /*
     * Следующие методы, доступные для этого класса, определены в базовом классе:
     *   inline void Clear();
     *   inline size_t ByteSize() const;
     *   inline size_t SerializeToArray(void* data, int size) const;
     *   inline size_t ParseFromArray(const void* data, int size);
     */

    // accessors -------------------------------------------------------

    // id - всегда пристутсвует
    inline const Id& id() const;
    inline Id* mutable_id();

    //=========================================================================
    // l1of_obs
    inline bool has_l1of_obs() const;
    inline void clear_l1of_obs();
    inline const Obs& l1of_obs() const;
    inline Obs* mutable_l1of_obs();
    inline Obs* add_l1of_obs();

    // l1of_aux
    inline bool has_l1of_aux() const;
    inline void clear_l1of_aux();
    inline const Auxiliary& l1of_aux() const;
    inline Auxiliary* mutable_l1of_aux();
    inline Auxiliary* add_l1of_aux();

    // l1of_msg
    inline bool has_l1of_msg() const;
    inline void clear_l1of_msg();
    inline const MsgL1of& l1of_msg() const;
    inline MsgL1of* mutable_l1of_msg();
    inline MsgL1of* add_l1of_msg();

    //=========================================================================
    // l2of_obs
    inline bool has_l2of_obs() const;
    inline void clear_l2of_obs();
    inline const Obs& l2of_obs() const;
    inline Obs* mutable_l2of_obs();
    inline Obs* add_l2of_obs();

    // l2of_aux
    inline bool has_l2of_aux() const;
    inline void clear_l2of_aux();
    inline const Auxiliary& l2of_aux() const;
    inline Auxiliary* mutable_l2of_aux();
    inline Auxiliary* add_l2of_aux();

    // l2of_msg
    inline bool has_l2of_msg() const;
    inline void clear_l2of_msg();
    inline const MsgL2of& l2of_msg() const;
    inline MsgL2of* mutable_l2of_msg();
    inline MsgL2of* add_l2of_msg();

    //=========================================================================
    // l1oc_obs
    inline bool has_l1oc_obs() const;
    inline void clear_l1oc_obs();
    inline const Obs& l1oc_obs() const;
    inline Obs* mutable_l1oc_obs();
    inline Obs* add_l1oc_obs();

    // l1oc_aux
    inline bool has_l1oc_aux() const;
    inline void clear_l1oc_aux();
    inline const Auxiliary& l1oc_aux() const;
    inline Auxiliary* mutable_l1oc_aux();
    inline Auxiliary* add_l1oc_aux();

    // l1oc_msg - Аномальные строки не поддерживаются
    inline bool has_l1oc_msg() const;
    inline void clear_l1oc_msg();
    inline const MsgL1oc& l1oc_msg() const;
    inline MsgL1oc* mutable_l1oc_msg();
    inline MsgL1oc* add_l1oc_msg();

    //=========================================================================
    // l2oc_obs
    inline bool has_l2oc_obs() const;
    inline void clear_l2oc_obs();
    inline const Obs& l2oc_obs() const;
    inline Obs* mutable_l2oc_obs();
    inline Obs* add_l2oc_obs();

    // l2oc_aux
    inline bool has_l2oc_aux() const;
    inline void clear_l2oc_aux();
    inline const Auxiliary& l2oc_aux() const;
    inline Auxiliary* mutable_l2oc_aux();
    inline Auxiliary* add_l2oc_aux();

    //=========================================================================
    // l3oc_obs
    inline bool has_l3oc_obs() const;
    inline void clear_l3oc_obs();
    inline const Obs& l3oc_obs() const;
    inline Obs* mutable_l3oc_obs();
    inline Obs* add_l3oc_obs();

    // l3oc_aux
    inline bool has_l3oc_aux() const;
    inline void clear_l3oc_aux();
    inline const Auxiliary& l3oc_aux() const;
    inline Auxiliary* mutable_l3oc_aux();
    inline Auxiliary* add_l3oc_aux();

    // l3oc_msg - Аномальные строки не поддерживаются
    inline bool has_l3oc_msg() const;
    inline void clear_l3oc_msg();
    inline const MsgL3oc& l3oc_msg() const;
    inline MsgL3oc* mutable_l3oc_msg();
    inline MsgL3oc* add_l3oc_msg();

    //=========================================================================
    // l1cfg_obs
    inline bool has_l1cfg_obs() const;
    inline void clear_l1cfg_obs();
    inline const Obs& l1cfg_obs() const;
    inline Obs* mutable_l1cfg_obs();
    inline Obs* add_l1cfg_obs();

    // l1cfg_aux
    inline bool has_l1cfg_aux() const;
    inline void clear_l1cfg_aux();
    inline const Auxiliary& l1cfg_aux() const;
    inline Auxiliary* mutable_l1cfg_aux();
    inline Auxiliary* add_l1cfg_aux();

    // l1cfg_msg
    inline bool has_l1cfg_msg() const;
    inline void clear_l1cfg_msg();
    inline const MsgL1cfg& l1cfg_msg() const;
    inline MsgL1cfg* mutable_l1cfg_msg();
    inline MsgL1cfg* add_l1cfg_msg();

    //=========================================================================
    // l2cfg_obs
    inline bool has_l2cfg_obs() const;
    inline void clear_l2cfg_obs();
    inline const Obs& l2cfg_obs() const;
    inline Obs* mutable_l2cfg_obs();
    inline Obs* add_l2cfg_obs();

    // l2cfg_aux
    inline bool has_l2cfg_aux() const;
    inline void clear_l2cfg_aux();
    inline const Auxiliary& l2cfg_aux() const;
    inline Auxiliary* mutable_l2cfg_aux();
    inline Auxiliary* add_l2cfg_aux();

    // l2cfg_msg
    inline bool has_l2cfg_msg() const;
    inline void clear_l2cfg_msg();
    inline const MsgL2cfg& l2cfg_msg() const;
    inline MsgL2cfg* mutable_l2cfg_msg();
    inline MsgL2cfg* add_l2cfg_msg();

    //=========================================================================
    // srl_obs
    inline bool has_srl_obs() const;
    inline void clear_srl_obs();
    inline const Obs& srl_obs() const;
    inline Obs* mutable_srl_obs();
    inline Obs* add_srl_obs();

    // srl_aux
    inline bool has_srl_aux() const;
    inline void clear_srl_aux();
    inline const Auxiliary& srl_aux() const;
    inline Auxiliary* mutable_srl_aux();
    inline Auxiliary* add_srl_aux();

    // srl_msg
    inline bool has_srl_msg() const;
    inline void clear_srl_msg();
    inline const MsgSrl& srl_msg() const;
    inline MsgSrl* mutable_srl_msg();
    inline MsgSrl* add_srl_msg();

  private:

    /*
     * Короче, я хотел использовать enum в качестве индексов полей
     * в массиве и чтобы этот enum имел последнее поле, равное числу
     * констант в нем (чтобы создавать массивы с таким же числом элементов).
     * Это делается легко с помощью enum. Я так же
     * хотел иметь возможность обращаться к ним через Component::L1OF.
     * Казалось бы, я должен использовать enum class... Но в C++ в качестве
     * длины массива можно использовать только "unscoped enumeration":
     * https://stackoverflow.com/a/17359774
     */
    struct Component
    {
      enum c
      {
        L1OF = 0, L2OF, L1OC, L2OC, L3OC, L1CFG, L2CFG, SRL/*, L1SC, L2SC*/, MAX,
      };
    };

    /*=======================================================================================*/
    /*    Порядок следования полей класса отражает их порядок в бинарной структуре данных    */

    /*
     * Следующие поля содержат набор битовых флагов. Каждый флаг
     * отражает наличие одного блока данных. Размер полей был выбран
     * равным 16 бит специально, чтобы Arm мог загрузить битовую
     * маску за одну команду.
     * Три закоментированных поля ниже определены в базовом классе,
     * здесь они приведены для отображения структуры бинарной информации.
     */
//    uint16_t mSetOfObsFlags;
//    uint16_t mSetOfMsgFlags;
//    uint16_t mSetOfAuxFlags;

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
    MsgL1of mMsgL1of;
    MsgL2of mMsgL2of;
    MsgL1oc mMsgL1oc;
    /* L2OC не имеет информационной компоненты */
    MsgL3oc mMsgL3oc;
    MsgL1cfg mMsgL1cfg;
    MsgL2cfg mMsgL2cfg;
    MsgSrl mMsgSrl;

    /* Конец части класса, описывающей порядок следования полей в бинарной структуре */
    /*=======================================================================================*/
  };

  // functions definition

  inline const Id&
  Glo::id () const
  {
    return mId;
  }
  inline Id*
  Glo::mutable_id ()
  {
    return &mId;
  }

  //=========================================================================
  // l1of
  inline bool
  Glo::has_l1of_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L1OF)) != 0;
  }
  inline void
  Glo::clear_l1of_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L1OF));
    mObs[Component::L1OF].Clear();
  }
  inline const Obs&
  Glo::l1of_obs () const
  {
    return mObs[Component::L1OF];
  }
  inline Obs*
  Glo::mutable_l1of_obs ()
  {
    return &mObs[Component::L1OF];
  }
  inline Obs*
  Glo::add_l1of_obs ()
  {
    mObs[Component::L1OF].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L1OF);
    return &mObs[Component::L1OF];
  }

  // l1of_aux
  inline bool
  Glo::has_l1of_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L1OF)) != 0;
  }
  inline void
  Glo::clear_l1of_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L1OF));
    mAux[Component::L1OF].Clear();
  }
  inline const Auxiliary&
  Glo::l1of_aux () const
  {
    return mAux[Component::L1OF];
  }
  inline Auxiliary*
  Glo::mutable_l1of_aux ()
  {
    return &mAux[Component::L1OF];
  }
  inline Auxiliary*
  Glo::add_l1of_aux ()
  {
    mAux[Component::L1OF].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L1OF);
    return &mAux[Component::L1OF];
  }

  // l1of_msg
  inline bool
  Glo::has_l1of_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L1OF)) != 0;
  }
  inline void
  Glo::clear_l1of_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L1OF));
    mMsgL1of.Clear();
  }
  inline const Glo::MsgL1of&
  Glo::l1of_msg () const
  {
    return mMsgL1of;
  }
  inline Glo::MsgL1of*
  Glo::mutable_l1of_msg ()
  {
    return &mMsgL1of;
  }
  inline Glo::MsgL1of*
  Glo::add_l1of_msg ()
  {
    mMsgL1of.Clear();
    mSetOfMsgFlags = mSetOfMsgFlags | Flag(Component::L1OF);
    return &mMsgL1of;
  }

  //=========================================================================
  // l2of
  inline bool
  Glo::has_l2of_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L2OF)) != 0;
  }
  inline void
  Glo::clear_l2of_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L2OF));
    mObs[Component::L2OF].Clear();
  }
  inline const Obs&
  Glo::l2of_obs () const
  {
    return mObs[Component::L2OF];
  }
  inline Obs*
  Glo::mutable_l2of_obs ()
  {
    return &mObs[Component::L2OF];
  }
  inline Obs*
  Glo::add_l2of_obs ()
  {
    mObs[Component::L2OF].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L2OF);
    return &mObs[Component::L2OF];
  }

  // aux_l2of
  inline bool
  Glo::has_l2of_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L2OF)) != 0;
  }
  inline void
  Glo::clear_l2of_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L2OF));
    mAux[Component::L2OF].Clear();
  }
  inline const Auxiliary&
  Glo::l2of_aux () const
  {
    return mAux[Component::L2OF];
  }
  inline Auxiliary*
  Glo::mutable_l2of_aux ()
  {
    return &mAux[Component::L2OF];
  }
  inline Auxiliary*
  Glo::add_l2of_aux ()
  {
    mAux[Component::L2OF].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L2OF);
    return &mAux[Component::L2OF];
  }

  // l2of_msg
  inline bool
  Glo::has_l2of_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L2OF)) != 0;
  }
  inline void
  Glo::clear_l2of_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L2OF));
    mMsgL2of.Clear();
  }
  inline const Glo::MsgL2of&
  Glo::l2of_msg () const
  {
    return mMsgL2of;
  }
  inline Glo::MsgL2of*
  Glo::mutable_l2of_msg ()
  {
    return &mMsgL2of;
  }
  inline Glo::MsgL2of*
  Glo::add_l2of_msg ()
  {
    mMsgL2of.Clear();
    mSetOfMsgFlags = mSetOfMsgFlags | Flag(Component::L2OF);
    return &mMsgL2of;
  }

  //=========================================================================
  // l1oc
  inline bool
  Glo::has_l1oc_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L1OC)) != 0;
  }
  inline void
  Glo::clear_l1oc_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L1OC));
    mObs[Component::L1OC].Clear();
  }
  inline const Obs&
  Glo::l1oc_obs () const
  {
    return mObs[Component::L1OC];
  }
  inline Obs*
  Glo::mutable_l1oc_obs ()
  {
    return &mObs[Component::L1OC];
  }
  inline Obs*
  Glo::add_l1oc_obs ()
  {
    mObs[Component::L1OC].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L1OC);
    return &mObs[Component::L1OC];
  }

  // l1oc_aux
  inline bool
  Glo::has_l1oc_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L1OC)) != 0;
  }
  inline void
  Glo::clear_l1oc_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L1OC));
    mAux[Component::L1OC].Clear();
  }
  inline const Auxiliary&
  Glo::l1oc_aux () const
  {
    return mAux[Component::L1OC];
  }
  inline Auxiliary*
  Glo::mutable_l1oc_aux ()
  {
    return &mAux[Component::L1OC];
  }
  inline Auxiliary*
  Glo::add_l1oc_aux ()
  {
    mAux[Component::L1OC].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L1OC);
    return &mAux[Component::L1OC];
  }

  // l1oc_msg
  inline bool
  Glo::has_l1oc_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L1OC)) != 0;
  }
  inline void
  Glo::clear_l1oc_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L1OC));
    mMsgL1oc.Clear();
  }
  inline const Glo::MsgL1oc&
  Glo::l1oc_msg () const
  {
    return mMsgL1oc;
  }
  inline Glo::MsgL1oc*
  Glo::mutable_l1oc_msg ()
  {
    return &mMsgL1oc;
  }
  inline Glo::MsgL1oc*
  Glo::add_l1oc_msg ()
  {
    mMsgL1oc.Clear();
    mSetOfMsgFlags = (mSetOfMsgFlags | Flag(Component::L1OC));
    return &mMsgL1oc;
  }

  //=========================================================================
  // l2oc
  inline bool
  Glo::has_l2oc_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L2OC)) != 0;
  }
  inline void
  Glo::clear_l2oc_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L2OC));
    mObs[Component::L2OC].Clear();
  }
  inline const Obs&
  Glo::l2oc_obs () const
  {
    return mObs[Component::L2OC];
  }
  inline Obs*
  Glo::mutable_l2oc_obs ()
  {
    return &mObs[Component::L2OC];
  }
  inline Obs*
  Glo::add_l2oc_obs ()
  {
    mObs[Component::L2OC].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L2OC);
    return &mObs[Component::L2OC];
  }

  // l2oc_aux
  inline bool
  Glo::has_l2oc_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L2OC)) != 0;
  }
  inline void
  Glo::clear_l2oc_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L2OC));
    mAux[Component::L2OC].Clear();
  }
  inline const Auxiliary&
  Glo::l2oc_aux () const
  {
    return mAux[Component::L2OC];
  }
  inline Auxiliary*
  Glo::mutable_l2oc_aux ()
  {
    return &mAux[Component::L2OC];
  }
  inline Auxiliary*
  Glo::add_l2oc_aux ()
  {
    mAux[Component::L2OC].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L2OC);
    return &mAux[Component::L2OC];
  }

  //=========================================================================
  // l3oc
  inline bool
  Glo::has_l3oc_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L3OC)) != 0;
  }
  inline void
  Glo::clear_l3oc_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L3OC));
    mObs[Component::L3OC].Clear();
  }
  inline const Obs&
  Glo::l3oc_obs () const
  {
    return mObs[Component::L3OC];
  }
  inline Obs*
  Glo::mutable_l3oc_obs ()
  {
    return &mObs[Component::L3OC];
  }
  inline Obs*
  Glo::add_l3oc_obs ()
  {
    mObs[Component::L3OC].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L3OC);
    return &mObs[Component::L3OC];
  }

  // l3oc_aux
  inline bool
  Glo::has_l3oc_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L3OC)) != 0;
  }
  inline void
  Glo::clear_l3oc_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L3OC));
    mAux[Component::L3OC].Clear();
  }
  inline const Auxiliary&
  Glo::l3oc_aux () const
  {
    return mAux[Component::L3OC];
  }
  inline Auxiliary*
  Glo::mutable_l3oc_aux ()
  {
    return &mAux[Component::L3OC];
  }
  inline Auxiliary*
  Glo::add_l3oc_aux ()
  {
    mAux[Component::L3OC].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L3OC);
    return &mAux[Component::L3OC];
  }

  // l3oc_msg
  inline bool
  Glo::has_l3oc_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L3OC)) != 0;
  }
  inline void
  Glo::clear_l3oc_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L3OC));
    mMsgL3oc.Clear();
  }
  inline const Glo::MsgL3oc&
  Glo::l3oc_msg () const
  {
    return mMsgL3oc;
  }
  inline Glo::MsgL3oc*
  Glo::mutable_l3oc_msg ()
  {
    return &mMsgL3oc;
  }
  inline Glo::MsgL3oc*
  Glo::add_l3oc_msg ()
  {
    mMsgL3oc.Clear();
    mSetOfMsgFlags = (mSetOfMsgFlags | Flag(Component::L3OC));
    return &mMsgL3oc;
  }

  //=========================================================================
  // l1cfg
  inline bool
  Glo::has_l1cfg_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L1CFG)) != 0;
  }
  inline void
  Glo::clear_l1cfg_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L1CFG));
    mObs[Component::L1CFG].Clear();
  }
  inline const Obs&
  Glo::l1cfg_obs () const
  {
    return mObs[Component::L1CFG];
  }
  inline Obs*
  Glo::mutable_l1cfg_obs ()
  {
    return &mObs[Component::L1CFG];
  }
  inline Obs*
  Glo::add_l1cfg_obs ()
  {
    mObs[Component::L1CFG].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L1CFG);
    return &mObs[Component::L1CFG];
  }

  // l1cfg_aux
  inline bool
  Glo::has_l1cfg_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L1CFG)) != 0;
  }
  inline void
  Glo::clear_l1cfg_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L1CFG));
    mAux[Component::L1CFG].Clear();
  }
  inline const Auxiliary&
  Glo::l1cfg_aux () const
  {
    return mAux[Component::L1CFG];
  }
  inline Auxiliary*
  Glo::mutable_l1cfg_aux ()
  {
    return &mAux[Component::L1CFG];
  }
  inline Auxiliary*
  Glo::add_l1cfg_aux ()
  {
    mAux[Component::L1CFG].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L1CFG);
    return &mAux[Component::L1CFG];
  }

  // l1cfg_msg
  inline bool
  Glo::has_l1cfg_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L1CFG)) != 0;
  }
  inline void
  Glo::clear_l1cfg_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L1CFG));
    mMsgL1cfg.Clear();
  }
  inline const Glo::MsgL1cfg&
  Glo::l1cfg_msg () const
  {
    return mMsgL1cfg;
  }
  inline Glo::MsgL1cfg*
  Glo::mutable_l1cfg_msg ()
  {
    return &mMsgL1cfg;
  }
  inline Glo::MsgL1cfg*
  Glo::add_l1cfg_msg ()
  {
    mMsgL1cfg.Clear();
    mSetOfMsgFlags = mSetOfMsgFlags | Flag(Component::L1CFG);
    return &mMsgL1cfg;
  }

  //=========================================================================
  // l2cfg
  inline bool
  Glo::has_l2cfg_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::L2CFG)) != 0;
  }
  inline void
  Glo::clear_l2cfg_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::L2CFG));
    mObs[Component::L2CFG].Clear();
  }
  inline const Obs&
  Glo::l2cfg_obs () const
  {
    return mObs[Component::L2CFG];
  }
  inline Obs*
  Glo::mutable_l2cfg_obs ()
  {
    return &mObs[Component::L2CFG];
  }
  inline Obs*
  Glo::add_l2cfg_obs ()
  {
    mObs[Component::L2CFG].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::L2CFG);
    return &mObs[Component::L2CFG];
  }

  // l2cfg_aux
  inline bool
  Glo::has_l2cfg_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::L2CFG)) != 0;
  }
  inline void
  Glo::clear_l2cfg_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::L2CFG));
    mAux[Component::L2CFG].Clear();
  }
  inline const Auxiliary&
  Glo::l2cfg_aux () const
  {
    return mAux[Component::L2CFG];
  }
  inline Auxiliary*
  Glo::mutable_l2cfg_aux ()
  {
    return &mAux[Component::L2CFG];
  }
  inline Auxiliary*
  Glo::add_l2cfg_aux ()
  {
    mAux[Component::L2CFG].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::L2CFG);
    return &mAux[Component::L2CFG];
  }

  // l2cfg_msg
  inline bool
  Glo::has_l2cfg_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::L2CFG)) != 0;
  }
  inline void
  Glo::clear_l2cfg_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::L2CFG));
    mMsgL2cfg.Clear();
  }
  inline const Glo::MsgL2cfg&
  Glo::l2cfg_msg () const
  {
    return mMsgL2cfg;
  }
  inline Glo::MsgL2cfg*
  Glo::mutable_l2cfg_msg ()
  {
    return &mMsgL2cfg;
  }
  inline Glo::MsgL2cfg*
  Glo::add_l2cfg_msg ()
  {
    mMsgL2cfg.Clear();
    mSetOfMsgFlags = mSetOfMsgFlags | Flag(Component::L2CFG);
    return &mMsgL2cfg;
  }

  //=========================================================================
  // srl
  inline bool
  Glo::has_srl_obs () const
  {
    return (mSetOfObsFlags & Flag(Component::SRL)) != 0;
  }
  inline void
  Glo::clear_srl_obs ()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::SRL));
    mObs[Component::SRL].Clear();
  }
  inline const Obs&
  Glo::srl_obs () const
  {
    return mObs[Component::SRL];
  }
  inline Obs*
  Glo::mutable_srl_obs ()
  {
    return &mObs[Component::SRL];
  }
  inline Obs*
  Glo::add_srl_obs ()
  {
    mObs[Component::SRL].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::SRL);
    return &mObs[Component::SRL];
  }

  // srl_aux
  inline bool
  Glo::has_srl_aux () const
  {
    return (mSetOfAuxFlags & Flag(Component::SRL)) != 0;
  }
  inline void
  Glo::clear_srl_aux ()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::SRL));
    mAux[Component::SRL].Clear();
  }
  inline const Auxiliary&
  Glo::srl_aux () const
  {
    return mAux[Component::SRL];
  }
  inline Auxiliary*
  Glo::mutable_srl_aux ()
  {
    return &mAux[Component::SRL];
  }
  inline Auxiliary*
  Glo::add_srl_aux ()
  {
    mAux[Component::SRL].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::SRL);
    return &mAux[Component::SRL];
  }

  // srl_msg
  inline bool
  Glo::has_srl_msg () const
  {
    return (mSetOfMsgFlags & Flag(Component::SRL)) != 0;
  }
  inline void
  Glo::clear_srl_msg ()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::SRL));
    mMsgSrl.Clear();
  }
  inline const Glo::MsgSrl&
  Glo::srl_msg () const
  {
    return mMsgSrl;
  }
  inline Glo::MsgSrl*
  Glo::mutable_srl_msg ()
  {
    return &mMsgSrl;
  }
  inline Glo::MsgSrl*
  Glo::add_srl_msg ()
  {
    mMsgSrl.Clear();
    mSetOfMsgFlags = (mSetOfMsgFlags | Flag(Component::SRL));
    return &mMsgSrl;
  }

} /* namespace frame2 */

#endif /* GLO_H_ */
