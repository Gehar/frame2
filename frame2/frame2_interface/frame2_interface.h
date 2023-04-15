/*
 * frame2_interface.h
 *
 *  Created on: 9 июн. 2021 г.
 *      Author: user
 */

#ifndef FRAME2_INTERFACE_H_
#define FRAME2_INTERFACE_H_

#include <frame2/frame2.h>
#include <frame2_interface/frame2_interface_types.h>

namespace frame2
{
  template<typename T>
    class Frame2Interface
    {
    public:
      inline Frame2Interface (const frame2::Frame2 &Frm2);

      inline size_t size() const;

      inline const frame2::Id& id (size_t index) const;

      inline bool has_obs(size_t index) const;
      inline const frame2::Obs& obs (size_t index) const;

      inline bool has_aux(size_t index) const;
      inline const frame2::Auxiliary& aux (size_t index) const;

#if __cplusplus >= 201402L
      /*
       * Возвращаемый тип для строк разный для разных компонент,
       * поэтому используется возвращаемый тип auto. Данная воможность
       * появилсь начиная с C++14. Чтобы использовать эти функции
       * используйте C++14 или новее
       */
      inline bool has_msg(size_t index) const;
      inline auto& msg (size_t index) const;
#endif

    protected:
      const T mT;
    };

  template<typename T>
    inline Frame2Interface<T>::Frame2Interface (const frame2::Frame2 &Frm2) : mT(Frm2)
    {
    }

  template<typename T>
    inline size_t Frame2Interface<T>::size() const
    {
      return mT.size();
    }

  template<typename T>
    inline const frame2::Id& Frame2Interface<T>::id (size_t index) const
    {
      return mT.id(index);
    }

  template<typename T>
    inline bool Frame2Interface<T>::has_obs (size_t index) const
    {
      return mT.has_obs(index);
    }

  template<typename T>
    inline const frame2::Obs& Frame2Interface<T>::obs (size_t index) const
    {
      return mT.obs(index);
    }

  template<typename T>
    inline bool Frame2Interface<T>::has_aux (size_t index) const
    {
      return mT.has_aux(index);
    }

  template<typename T>
    inline const frame2::Auxiliary& Frame2Interface<T>::aux (size_t index) const
    {
      return mT.aux(index);
    }

#if __cplusplus >= 201402L
  /* см. комментарий в объявлении функций */
  template<typename T>
    inline bool Frame2Interface<T>::has_msg (size_t index) const
    {
      return mT.has_msg(index);
    }

  template<typename T>
    inline auto& Frame2Interface<T>::msg (size_t index) const
    {
      return mT.msg(index);
    }
#endif

} /* namespace frame2 */

#endif /* FRAME2_INTERFACE_H_ */
