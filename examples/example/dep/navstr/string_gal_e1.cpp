/*
 * string_gal_e1.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: user
 */

#include <navstr/string_gal_e1.h>

namespace navstr
{

  StringGalE1::StringGalE1 () :
      mPageType(0), mINavData
        { 0 }, mReserved1(0), mSar(0), mSpare(0), mCrc(0)
  {
    // TODO Auto-generated constructor stub

  }

  StringGalE1::~StringGalE1 ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace rnl */
