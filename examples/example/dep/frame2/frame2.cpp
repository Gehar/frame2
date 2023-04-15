/*
 * frame2.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#include <algorithm>

#include <frame2/frame2.h>

namespace frame2
{
  /* Класс проектировался для работы в приемнике. Поэтому память выделяется только
   * при создании объекта класса. Если во время использования объекта его память
   * закончится, то выделение дополнительной памяти не произойдет (не смотря на
   * использование vector). Объем выделяемой памяти определяется параметром
   * конструктора MaxChannelsCnt. */
  Frame2::Frame2 (size_t MaxChannelsCnt):
    mSize(0), mSubframeSize(0), mSubframeId(0), mGloCnt(0), mGpsCnt(0), mGalCnt(0), mUnrecognizedSubframes(0), mSizeOfOmitedData(0)
  {
    mGlo.resize(MaxChannelsCnt);
    mGps.resize(MaxChannelsCnt);
    mGal.resize(MaxChannelsCnt);
  }

  Frame2::Frame2 (const Frame2 &frame)
  {
    mSize = frame.mSize;
    mHeader = frame.mHeader;
    mSubframeSize = frame.mSubframeSize;
    mSubframeId = frame.mSubframeId;
    mGloCnt = frame.mGloCnt;
    mGlo.resize(frame.mGlo.size());
    std::copy(frame.mGlo.begin(), frame.mGlo.end(), mGlo.begin());
    mGpsCnt = frame.mGpsCnt;
    mGps.resize(frame.mGps.size());
    std::copy(frame.mGps.begin(), frame.mGps.end(), mGps.begin());
    mGalCnt = frame.mGalCnt;
    mGal.resize(frame.mGal.size());
    std::copy(frame.mGal.begin(), frame.mGal.end(), mGal.begin());

    mUnrecognizedSubframes = frame.mUnrecognizedSubframes;
    mSizeOfOmitedData = frame.mSizeOfOmitedData;
  }

  Frame2::~Frame2 ()
  {
    // TODO Auto-generated destructor stub
  }

  size_t
  Frame2::ByteSize () const
  {
    size_t size = 0;
    size += sizeof(scFrameId);
    size += sizeof(mSize);
    size += sizeof(scVersion);
    size += mHeader.ByteSize();
    if (glo_size() > 0)
    {
      size += sizeof(mSubframeSize);
      size += sizeof(mSubframeId);
      for (size_t i = 0; i < glo_size(); i++)
      {
        size += mGlo[i].ByteSize();
      }
    }
    if (gps_size() > 0)
    {
      size += sizeof(mSubframeSize);
      size += sizeof(mSubframeId);
      for (size_t i = 0; i < gps_size(); i++)
      {
        size += mGps[i].ByteSize();
      }
    }
    if (gal_size() > 0)
    {
      size += sizeof(mSubframeSize);
      size += sizeof(mSubframeId);
      for (size_t i = 0; i < gal_size(); i++)
      {
        size += mGal[i].ByteSize();
      }
    }
    return size;
  }

} /* namespace frame2 */
