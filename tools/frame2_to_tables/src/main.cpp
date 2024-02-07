/*
 * main.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

#define prog_name "frame2_to_tables"

#include <cstring>
#include <array>

#include <docopt/docopt.h>
#include <fmt/format.h>
#include <frame2/file.h>
#include <print_utf8/print_utf8.h>

#include "main.h"
#include "directory_lib.h"
#include "frame2_time_conv.h"

FILE* open_file(const std::string &dir, const std::string &prefix, uint8_t nfile);

int
main (int argc, char ** argv)
{
#ifdef __WIN32__
  setlocale(LC_ALL, "Russian_Russia.1251");
#endif

  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
  { argv + 1, argv + argc }, true, std::string(prog_name) + " 0.1");

  print_utf8("Входные аргументы:");

  // Количество каналов
  size_t      nchannels    = args["--channels"].asLong();
  fmt::print("channels = {}\n", nchannels);

  // Входной кадр
  std::string input_frame2 = args["<input>"].asString();
  fmt::print("<input>:   {}\n", input_frame2.c_str());

  // Выходная аудитория
  std::string output_dir;
  if (args["--output"].isString())
  {
    output_dir = args["--output"].asString();
  }
  if (output_dir == "")
  {
    output_dir = input_frame2 + "_tables";
  }
  fmt::print("<output>:  {}\n", output_dir.c_str());

  /*
   * Запускаем обработку frame2
   */
  frame2::File file(input_frame2.c_str(), 'r');
  frame2::Frame2 frame(nchannels);

  // Так как в начале frame2 шкала времени могла быть не определена, первым проходом ищем кадр с определенной ШВ
  uint8_t first_ts_id = frame2::Header::TS_UNDEF;
  int64_t first_ts_hw_ms = 0;
  int64_t first_ts_ms = 0;
  while(file.Read(frame))
  {
    if (frame.header().timestamp_id() != frame2::Header::TS_UNDEF)
    {
      first_ts_id = frame.header().timestamp_id();
      first_ts_hw_ms = frame.header().timestamp_hw_ms();
      first_ts_ms = frame.header().timestamp_ms();

      // ШВ определили, завершаем цикл
      break;
    }
  }

  fmt::print("first_ts_id    = {}\n", (int)first_ts_id);
  fmt::print("first_ts_hw_ms = {}\n", first_ts_hw_ms);
  fmt::print("first_ts_ms    = {}\n", first_ts_ms);

  // Возвращаемся в начало файла frame2
  file.SetPos(0);

  // Вычисляем и запоминаме время первого кадра
  file.Read(frame);
  frame2::Header zero_hdr = frame.header();
  fmt::print("zero_ts_id     = {}\n", (int)zero_hdr.timestamp_id());
  fmt::print("zero_ts_hw_ms  = {}\n", zero_hdr.timestamp_hw_ms());
  fmt::print("zero_ts_ms     = {}\n", zero_hdr.timestamp_ms());
  zero_hdr.set_timestamp_id(first_ts_id);
  zero_hdr.set_timestamp_ms(first_ts_ms - (first_ts_hw_ms - zero_hdr.timestamp_hw_ms()));
  fmt::print("New zero_ts_ms = {}\n", zero_hdr.timestamp_ms());
  std::string str_timestamp;
  int64_t zero_timestamp = frame2_time_to_utc(zero_hdr, str_timestamp);
  zero_timestamp = zero_timestamp * 1000ll + (frame.header().timestamp_ms() % 1000ll);
  fmt::print(str_timestamp);

  /*
   * Создаем директорию под выходные файлы
   */
  if (is_dir_exist(output_dir) == false)
  {
    if (make_path(output_dir) == false)
    {
      print_utf8(fmt::format("ERROR! Не удалось создать директорию для выходны файлов: {}", output_dir));
      return -1;
    }
  }
  if ((output_dir.back() != '\\') and (output_dir.back() != '/'))
  {
    output_dir += '\\';
  }

  /*
   * Запускаем основной цикл конвертора
   */
  int64_t zero_ts_hw_ms = zero_hdr.timestamp_hw_ms();
  file.SetPos(0);
  constexpr size_t n_sat_glo_max = 32;
  std::array<FILE*, n_sat_glo_max> glo_tables;
  constexpr size_t n_sat_gps_max = 64;
  std::array<FILE*, n_sat_gps_max> gps_tables;
  glo_tables.fill(nullptr);
  gps_tables.fill(nullptr);
  while(file.Read(frame))
  {

    int64_t ts_offset = frame.header().timestamp_hw_ms() - zero_ts_hw_ms;

    /*
     * Создаем таблицы по ГЛОНАСС
     */
    for(size_t i = 0; i < frame.glo_size(); i++)
    {
      const frame2::Glo &glo = frame.glo(i);

      // системный номер НКА ГЛОНАСС определяет файл, в который писать данные
      uint8_t sysnum = glo.id().sys_num();
      if ((sysnum == frame2::Id::scValueUndefined) or
          ((glo.l1of_aux().flags() & 0x01) == 0))
        continue;

      // открываем файл, если он не открыт
      if (glo_tables.at(sysnum) == nullptr)
      {
        glo_tables.at(sysnum) = open_file(output_dir, "r", sysnum);
        if (glo_tables.at(sysnum) == nullptr)
          continue;
        fmt::print(glo_tables[sysnum], "# timestamp_offset_ms state valid l1of_ampl l1of_dfs l1of_phase_std\n");
        fmt::print(glo_tables[sysnum], "{}\n{} # ms from 01.01.1970\n", str_timestamp, zero_timestamp);
      }

      // пишем измерения
      fmt::print(glo_tables[sysnum], "{} {} {} {:6e} {:6e} {:6e}\n", ts_offset, static_cast<int>(glo.l1of_aux().state()), (glo.l1of_aux().flags() & 0x01),
                 glo.l1of_aux().ampl_data(), glo.l1of_obs().dfs(), glo.l1of_aux().phase_std_data());
    }

    /*
     * Создаем таблицы по GPS
     */
    for(size_t i = 0; i < frame.gps_size(); i++)
    {
      const frame2::Gps &gps = frame.gps(i);

      // номер prn НКА GPS определяет файл, в который писать данные
      uint8_t prnnum = gps.id().prn_num();
      if ((prnnum == frame2::Id::scValueUndefined) or
          (prnnum == 0) or
          ((gps.l1ca_aux().flags() & 0x01) == 0))
        continue;

      // открываем файл, если он не открыт
      if (gps_tables.at(prnnum) == nullptr)
      {
        gps_tables.at(prnnum) = open_file(output_dir, "g", prnnum);
        if (gps_tables.at(prnnum) == nullptr)
          continue;
        fmt::print(gps_tables[prnnum], "# timestamp_offset_ms state valid l1ca_ampl l1ca_dfs l1ca_phase_std\n");
        fmt::print(gps_tables[prnnum], "{}\n{} # ms from 01.01.1970\n", str_timestamp, zero_timestamp);
      }

      // пишем измерения
      fmt::print(gps_tables[prnnum], "{} {} {} {:6e} {:6e} {:6e}\n", ts_offset, static_cast<int>(gps.l1ca_aux().state()), (gps.l1ca_aux().flags() & 0x01),
                 gps.l1ca_aux().ampl_data(), gps.l1ca_obs().dfs(), gps.l1ca_aux().phase_std_data());
    }
  }
  return 0;
}

FILE* open_file(const std::string &dir, const std::string &prefix, uint8_t nfile)
{
  std::string name = fmt::format("{}{:02}.txt", prefix, static_cast<int>(nfile));
  FILE* f = fopen((dir + name).c_str(), "w");
  return f;
}
