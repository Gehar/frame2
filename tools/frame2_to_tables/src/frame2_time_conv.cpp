/*
 * frame2_time_conv.cpp
 *
 *  Created on: 6 февр. 2024 г.
 *      Author: user
 */

#include <cstdint>
#include <cstdio>
#include <ctime>

#include <frame2/frame2.h>

#include "frame2_time_conv.h"

void frame2_time_conv(const frame2::Header &hdr, int64_t &tglo, int64_t &tgps, int64_t &tgal)
{
    int64_t time = hdr.timestamp_ms()/1000;
    constexpr int64_t utcglo_dif = 3 * 3600;
    constexpr int64_t glo_gps_dif = 504489600;
    constexpr int64_t gal_glo_dif = 114825587;
    constexpr int64_t gps_max = 1024 * 7 * 24 * 3600;
    switch (hdr.timestamp_id())
    {
      case frame2::Header::TS_GLO:
        tglo = time;
        tgps = (time + glo_gps_dif - utcglo_dif) % gps_max;
        tgal = time - gal_glo_dif - utcglo_dif;
        break;
      case frame2::Header::TS_GPS: /* прогноз со ШВ GPS не проверял */
        {
            /*
             * По системному времени определяем, сколько раз переполнился номер недели GPS
             * Делаем это каждый раз, так как время системы неточное и при очередном переполнении
             * номера недели может не верно определено количество переполнений noverflow
             */
            constexpr int64_t noverflow = 2;
            tgps = time;
            time += noverflow * gps_max;
            tglo = time - glo_gps_dif + utcglo_dif;
            tgal = time - glo_gps_dif - gal_glo_dif;
        }
        break;
      case frame2::Header::TS_GAL:
        tglo = time + gal_glo_dif + utcglo_dif;
        tgps = (time + glo_gps_dif + gal_glo_dif) % gps_max;
        tgal = time;
        break;
      default:
        {
            /* приемник не определил ШВ */
            tglo = 0;
            tgps = 0;
            tgal = 0;
        }
        break;
    }
}

int64_t frame2_time_to_utc(const frame2::Header &hdr, int32_t delta_leap_sec, int32_t n_gps_overflow)
{
  /*
   * На момент 09.10.2023 delta_leap_sec = 18 с (разность шкал UTC и GPS)
   * По Галилео текущее значение не известно, но здесь сказано, что смещение такое же, как у GPS (стр. 5):
   * https://www.itu.int/dms_pub/itu-r/oth/0a/0e/R0A0E0000960004PDFE.pdf
   */
  constexpr int64_t utcglo_dif = 3 * 3600;
  constexpr int64_t gps_max = 1024 * 7 * 24 * 3600;
  constexpr int64_t nsec_1970_1996 = 820454400; // Количество секунд с 01.01.1970 по 01.01.1996
  constexpr int64_t nsec_1970_1980 = 315964800; // Количество секунд с 01.01.1970 по 06.01.1980
  constexpr int64_t nsec_1970_1999 = 315964800; // Количество секунд с 01.01.1970 по 22.08.1999

  int64_t utc = hdr.timestamp_ms() / 1000;
  switch (hdr.timestamp_id())
  {
    case frame2::Header::TS_GLO:
      utc += (nsec_1970_1996 - utcglo_dif);
      break;
    case frame2::Header::TS_GPS:
      utc += (nsec_1970_1980 + (gps_max * n_gps_overflow) - delta_leap_sec);
      break;
    case frame2::Header::TS_GAL:
      utc += (nsec_1970_1999 - delta_leap_sec);
      break;
    default:
      return -1;
  }
  return utc;
}

int64_t frame2_time_to_utc(const frame2::Header &hdr, std::string &str_time, int32_t delta_leap_sec, int32_t n_gps_overflow)
{
  int64_t utc = frame2_time_to_utc(hdr, delta_leap_sec, n_gps_overflow);
  if (utc <= 0)
    return -1;
  // Преобразуем число секунд в дату
  std::tm cur_tm = *gmtime(&utc);
  cur_tm.tm_year += 1900;
  cur_tm.tm_mon += 1;
  int time_ms = hdr.timestamp_ms() % 1000;
  char cstr_time[128];
  sprintf(cstr_time, "%02d:%02d:%02d.%03d %02d.%02d.%04d (UTC)", cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec, time_ms, cur_tm.tm_mday, cur_tm.tm_mon, cur_tm.tm_year);
  str_time = cstr_time;
  return utc;
}
