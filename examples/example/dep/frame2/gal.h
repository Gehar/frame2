/*
 * gal.h
 *
 *  Created on: 27 авг. 2019 г.
 *      Author: user
 */

#ifndef FRAME2_GAL_H_
#define FRAME2_GAL_H_

#include <cstdint>
#include <cstddef>

#include <frame2/auxiliary.h>
#include <frame2/buffer.h>
#include <frame2/const_buffer.h>
#include <frame2/id.h>
#include <frame2/msg.h>
#include <frame2/nav_system_base.h>
#include <frame2/obs.h>

namespace frame2
{

  class Gal : public NavSystemBase
  {
  public:
    Gal ();
    Gal (const Gal &gal);
    ~Gal ();

    Gal& operator=(const Gal& gal);

    /* Объекты блоков для работы со строками кадра */
    typedef frame2::Msg<navstr::GalE1BitsNum> MsgE1bc;

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

    // e1bc_obs
    inline bool has_e1bc_obs() const;
    inline void clear_e1bc_obs();
    inline const Obs& e1bc_obs() const;
    inline Obs* mutable_e1bc_obs();
    inline Obs* add_e1bc_obs();

    // e1bc_aux
    inline bool has_e1bc_aux() const;
    inline void clear_e1bc_aux();
    inline const Auxiliary& e1bc_aux() const;
    inline Auxiliary* mutable_e1bc_aux();
    inline Auxiliary* add_e1bc_aux();

    // e1bc_msg
    inline bool has_e1bc_msg() const;
    inline void clear_e1bc_msg();
    inline const MsgE1bc& e1bc_msg() const;
    inline MsgE1bc* mutable_e1bc_msg();
    inline MsgE1bc* add_e1bc_msg();

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
        E1BC = 0, MAX,
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
    MsgE1bc mMsgE1bc;

    /* Конец части класса, описывающей порядок следования полей в бинарной структуре */
    /*=======================================================================================*/
  };

  // id - всегда пристутсвует
  inline const Id&
  Gal::id() const
  {
    return mId;
  }
  inline Id*
  Gal::mutable_id()
  {
    return &mId;
  }

  // e1bc_obs
  inline bool
  Gal::has_e1bc_obs() const
  {
    return (mSetOfObsFlags & Flag(Component::E1BC)) != 0;
  }
  inline void
  Gal::clear_e1bc_obs()
  {
    mSetOfObsFlags = mSetOfObsFlags & static_cast<uint16_t>(~Flag(Component::E1BC));
    mObs[Component::E1BC].Clear();
  }
  inline const Obs&
  Gal::e1bc_obs() const
  {
    return mObs[Component::E1BC];
  }
  inline Obs*
  Gal::mutable_e1bc_obs()
  {
    return &mObs[Component::E1BC];
  }
  inline Obs*
  Gal::add_e1bc_obs()
  {
    mObs[Component::E1BC].Clear();
    mSetOfObsFlags = mSetOfObsFlags | Flag(Component::E1BC);
    return &mObs[Component::E1BC];
  }

  // e1bc_aux
  inline bool
  Gal::has_e1bc_aux() const
  {
    return (mSetOfAuxFlags & Flag(Component::E1BC)) != 0;
  }
  inline void
  Gal::clear_e1bc_aux()
  {
    mSetOfAuxFlags = mSetOfAuxFlags & static_cast<uint16_t>(~Flag(Component::E1BC));
    mAux[Component::E1BC].Clear();
  }
  inline const Auxiliary&
  Gal::e1bc_aux() const
  {
    return mAux[Component::E1BC];
  }
  inline Auxiliary*
  Gal::mutable_e1bc_aux()
  {
    return &mAux[Component::E1BC];
  }
  inline Auxiliary*
  Gal::add_e1bc_aux()
  {
    mAux[Component::E1BC].Clear();
    mSetOfAuxFlags = mSetOfAuxFlags | Flag(Component::E1BC);
    return &mAux[Component::E1BC];
  }

  // e1bc_msg
  inline bool
  Gal::has_e1bc_msg() const
  {
    return (mSetOfMsgFlags & Flag(Component::E1BC)) != 0;
  }
  inline void
  Gal::clear_e1bc_msg()
  {
    mSetOfMsgFlags = mSetOfMsgFlags & static_cast<uint16_t>(~Flag(Component::E1BC));
    mMsgE1bc.Clear();
  }
  inline const Gal::MsgE1bc&
  Gal::e1bc_msg() const
  {
    return mMsgE1bc;
  }
  inline Gal::MsgE1bc*
  Gal::mutable_e1bc_msg()
  {
    return &mMsgE1bc;
  }
  inline Gal::MsgE1bc*
  Gal::add_e1bc_msg()
  {
    mMsgE1bc.Clear();
    mSetOfMsgFlags = mSetOfMsgFlags | Flag(Component::E1BC);
    return &mMsgE1bc;
  }

} /* namespace frame2 */

#endif /* FRAME2_GAL_H_ */
