/*
 * frame2_time_conv.h
 *
 *  Created on: 6 февр. 2024 г.
 *      Author: user
 */

#ifndef FRAME2_TIME_CONV_H_
#define FRAME2_TIME_CONV_H_

#include <cstdint>
#include <string>

namespace frame2
{
  class Header;
}

void frame2_time_conv(const frame2::Header &hdr, int64_t &tglo, int64_t &tgps, int64_t &tgal);
int64_t frame2_time_to_utc(const frame2::Header &hdr, int32_t delta_leap_sec = 18, int32_t n_gps_overflow = 2);
int64_t frame2_time_to_utc(const frame2::Header &hdr, std::string &str_time, int32_t delta_leap_sec = 18, int32_t n_gps_overflow = 2);

#endif /* FRAME2_TIME_CONV_H_ */
