/*
 * header.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#include <frame2/header.h>

namespace frame2
{

  const Header::Binary Header::scDefaultBinary = {0, 0, 0};

  Header::Header () : mBinary (Header::scDefaultBinary)
  {
//    Clear();
  }

  Header::~Header ()
  {
    // TODO Auto-generated destructor stub
  }

} /* namespace frame2 */
