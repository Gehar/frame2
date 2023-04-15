/*
 * msg_base.h
 *
 *  Created on: 22 авг. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#ifndef FRAME2_MSG_BASE_H_
#define FRAME2_MSG_BASE_H_

#include <cstddef>
#include <cstdint>

namespace frame2
{

  class MsgBase
  {
  public:
    MsgBase ();
    virtual
    ~MsgBase ();

    virtual void
    Clear ()
    {
    }
    virtual inline size_t
    ByteSize () const
    {
      return 0;
    }
    virtual inline size_t
    SerializeToArray (void* data) const
    {
      (void) data;
      return 0;
    }
    virtual inline size_t
    ParseFromArray (const void* data)
    {
      (void) data;
      return 0;
    }
    virtual inline size_t
    ParseFromArray (const void* data, size_t size)
    {
      (void) data;
      (void) size;
      return 0;
    }
  };

} /* namespace frame2 */

#endif /* FRAME2_MSG_BASE_H_ */
