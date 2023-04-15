/*
 * obs.h
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#ifndef OBS_H_
#define OBS_H_

#include <cstdint>
#include <cstddef>
#include <iostream>

namespace frame2
{

  class Obs
  {
  public:
    Obs ();
    ~Obs ();

    void Clear();
    inline size_t ByteSize() const;

    inline size_t SerializeToArray(void* data) const;
    inline size_t ParseFromArray(const void* data);

//    void CopyFrom(const Obs& from);

    // accessors -------------------------------------------------------

    // code_pr - [метры] кодовая псевдодальность
    inline double code_pr(/* metres */) const;
    inline void set_code_pr(double value_m);

    // phase_pr - [метры] фазовая псевдодальность
    inline double phase_pr(/* metres */) const;
    inline void set_phase_pr(double value_m);

    // dfs - [Гц] допплеровский сдвиг несущей частоты сигнала
    inline float dfs(/* Hz */) const;
    inline void set_dfs(float value_hz);

    // snr - [дБ*Гц] отношение сигнал/шум в Герце
    inline float snr(/* dBHz */) const;
    inline void set_snr(float value_dbhz);

  private:

#pragma pack(push,1)
    struct Binary   /* 33 байт */
    {
      double code_pr;
      double phase_pr;
      float dfs;
      float snr;
    };
#pragma pack(pop)

    Binary mBinary;

    static const float fNaN;
    static const double NaN;
    static const Binary scDefaultBinary;
  };

  // functions definition

  inline void
  Obs::Clear()
  {
    mBinary = scDefaultBinary;
  }

  inline size_t
  Obs::ByteSize () const
  {
    return sizeof(Binary);
  }

  inline size_t
  Obs::SerializeToArray (void* data) const
  {
    *(reinterpret_cast<Binary*>(data)) = mBinary;
    return ByteSize ();
  }

  inline size_t
  Obs::ParseFromArray (const void* data)
  {
    mBinary = *(reinterpret_cast<const Binary*>(data));
    return ByteSize ();
  }

  inline double
  Obs::code_pr () const
  {
    return mBinary.code_pr;
  }

  inline void
  Obs::set_code_pr (double value_m)
  {
    mBinary.code_pr = value_m;
  }

  inline double
  Obs::phase_pr () const
  {
    return mBinary.phase_pr;
  }

  inline void
  Obs::set_phase_pr (double value_m)
  {
    mBinary.phase_pr = value_m;
  }

  inline float
  Obs::dfs () const
  {
    return mBinary.dfs;
  }

  inline void
  Obs::set_dfs (float value_hz)
  {
    mBinary.dfs = value_hz;
  }

  /* dBHz */
  inline float
  /* dBHz */
  Obs::snr () const
  {
    return mBinary.snr;
  }

  inline void
  Obs::set_snr (float value_dbhz)
  {
    mBinary.snr = value_dbhz;
  }


} /* namespace frame2 */

#endif /* OBS_H_ */
