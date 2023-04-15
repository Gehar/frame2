/*
 * main.cpp
 *
 *  Created on: 17 июн. 2019 г.
 *      Author: user
 */

/* Подключаем главный заголовочный файл для работы с frame2 */
#include <frame2/frame2.h>
/* Чтобы читать frame2 из файла подключим еще один файл */
#include <frame2/file.h>
//#include <navstr/navstr_types.h>

#include <cstring>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

char* cmd_param_parser(int argc, char ** argv);

constexpr size_t channels_num = 12;

int
main (int argc, char ** argv)
{
  char *path_to_frame2 = cmd_param_parser(argc, argv);
  cout << "Path to Frame2: \"" << path_to_frame2 << "\"\n";

  /*
   * Класс для разбора данных в формате Frame2.
   * Для работы класса ему нужно сообщить максимальное число каналов.
   * Сейчас примемники имеют 12 каналов. Но можно указать больше.
   */
  frame2::Frame2 frame(channels_num);

  /* Класс для чтения кадра в формате Frame2 из файла и передачи его
   * в класс frame2::Frame2 для разбора */
  frame2::File file(path_to_frame2, 'r');

  for (int k = 0; k < 10; k++)
  {
    cout << "==========================================================\n";
    if (file.Read(frame) == 0)
    {
      cout << "File ended\n";
      break;
    }

    /*
     *  frame.UnrecognizedSubframes() и frame.SizeOfOmitedData() - два не обязательных вызова
     *  для диагностики, что кадр верно разобран
     */
    /* После прочтения кадра и его разбора следует проверить, все ли данные были разобраны */
    if (frame.UnrecognizedSubframes() != 0)
    {
      /* В кадре есть не распознанные подкадры */
      cout << "WARNING! Unrecognized subframes was detected: " << frame.UnrecognizedSubframes() << endl;
    }
    if (frame.SizeOfOmitedData())
    {
      /*
       * В кадре остались не обработанные данные, возможно кадр имеет неверный формат
       * Впрочем, возможно класс все равно сможет разобрать все данные верно.
       */
      cout << "ERROR! Omited data was detected: " << frame.SizeOfOmitedData() << " bytes" << endl;
      break;
    }

    /*
     * Далее работаем с кадром
     */
    cout << "Frame index: " << k << ", number of GLONASS channels: " << frame.glo_size() << endl;
    cout << "Hardware timestamp: " << frame.header().timestamp_hw_ms() << " ms\n";
    switch (frame.header().timestamp_id())
    {
      case frame2::Frame2::SYS_UNDEF:
        cout << "timestamp_id: SYS_UNDEF" << endl;
        break;
      case frame2::Frame2::GLO:
        cout << "timestamp_id: GLONASS" << endl;
        break;
      case frame2::Frame2::GPS:
        cout << "timestamp_id: GPS" << endl;
        break;
      case frame2::Frame2::GAL:
        cout << "timestamp_id: Galileo" << endl;
        break;
    }
    cout << "timestamp: " << frame.header().timestamp_ms() << " ms\n";
    for (size_t i = 0; i < frame.glo_size(); i++)
    {
      /*
       * Получаем запись по ГЛОНАСС
       */
      const frame2::Glo &glo = frame.glo(i);

      /*
       * Выводим некоторую информацию по спутнику из блока ID
       */
      cout << "channel: " << glo.id().channel() << endl;
      cout << "  freq_num = " << (int)glo.id().freq_num() << endl;
      cout << "  sat_num = " << (int)glo.id().sys_num() << endl;

      /*
       * Если есть РНП по L1OF, выводим их
       */
      if (glo.has_l1of_obs())
      {
        cout << "  L1OF observation:\n";
        const frame2::Obs &obs = glo.l1of_obs();
        if (glo.has_l1of_aux())
          cout << "    mode = " << (int)glo.l1of_aux().state() << " amplitude = " << glo.l1of_aux().ampl_data();
        else
          cout << "    mode = [aux block is not available]\n    amplitude = [aux block is not available]\n   ";
        cout << " dfs = " << obs.dfs() << endl;
      }

      /*
       * Если есть РНП по L2OF, выводим их
       */
      if (glo.has_l2of_obs())
      {
        cout << "  L2OF observation:\n";
        const frame2::Obs &obs = glo.l2of_obs();
        if (glo.has_l2of_aux())
          cout << "    mode = " << (int)glo.l2of_aux().state() << " amplitude = " << glo.l2of_aux().ampl_data();
        else
          cout << "    mode = [aux block is not available]\n    amplitude = [aux block is not available]\n   ";
        cout << " dfs = " << obs.dfs() << endl;
      }

      /*
       * Если есть ЦИ по L1OF, выводим их
       */
      if (glo.has_l1of_msg())
      {
        cout << "  L1OF DI:\n";
        /*
         * Так же возможна такая запись:
         * const rnl::navstr::GloL1of &str = glo.l1of_msg().str();
         * или
         * const rnl::navstr::GloOf &str = glo.l1of_msg().str();
         */
        auto &str = glo.l1of_msg().str();
        cout << "    str: ";
        for (size_t k = 0; k < glo.l1of_msg().str_size(); k++)
        {
          cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)str[k] << ' ';
        }
        cout << std::dec;
        /*
         * Для доступа к полям ЦИ можно использовать методы field_i32() и другие подобные.
         * Но в сигналах ГЛОНАСС с частотным разделением биты пронумерованы с хвоста строки,
         * а не с начала, а во всех остальных (ГЛОНАСС с кодовым разделением, GPS, Galileo)
         * идет нумерация с начала строки. Поэтому, класс NavString использует нумерацию бит с
         * начала строки. Для их использования с ЦИ ГЛОНАСС сигналов с частотным разделением
         * необходимо вычитать номер старшего бита по ИКД из длины строки в битах + 1:
         */
        constexpr int bit_ofset = str.size_bits() + 1;
        int str_num = str.field_u32(bit_ofset - 84, 4);
        if (str_num > 5)
        {
          if ((str_num % 2) == 0)
          {
            /*
             * Чтобы проверить извлечение параметров из ЦИ в формате frame2
             * попробуем извлечь наклонение орбиты. Полученное значение можно
             * будет сравнить с данными на оф. сайте:
             * https://www.glonass-iac.ru/en/GLONASS/ephemeris.php
             */
            uint32_t na = str.field_u32(bit_ofset - 77, 5); /* Получаем из строки номер системной точки */
            constexpr double resolution = 180.0/static_cast<double>(1 << 20);
            double incl = 63.0 + static_cast<double>(str.field_i32(bit_ofset - 41, 18)) * resolution;
            cout << " (sat_num = " << na << ", orbital inclination = " << incl << ")";
          }
        }
        cout << "\n    crc_error = " << glo.l1of_msg().crc_error() << endl;
      }

      /*
       * Если есть ЦИ по L2OF, выводим их
       */
      if ( glo.has_l2of_msg() )
      {
        cout << "  L2OF DI:\n";
        /*
         * Так же возможна такая запись:
         * const rnl::navstr::GloL2of &str = glo.l2of_msg().str();
         * или
         * const rnl::navstr::GloOf &str = glo.l2of_msg().str();
         */

        auto &str = glo.l2of_msg().str();
        cout << "    str: ";
        for (size_t k = 0; k < glo.l2of_msg().str_size(); k++)
        {
          cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)str[k] << ' ';
        }
        cout << std::dec;
        /*
         * Для доступа к полям ЦИ можно использовать методы field_i32() и другие подобные.
         * Но в сигналах ГЛОНАСС с частотным разделением биты пронумерованы с хвоста строки,
         * а не с начала, а во всех остальных (ГЛОНАСС с кодовым разделением, GPS, Galileo)
         * идет нумерация с начала строки. Поэтому, класс NavString использует нумерацию бит с
         * начала строки. Для их использования с ЦИ ГЛОНАСС сигналов с частотным разделением
         * необходимо вычитать номер старшего бита по ИКД из длины строки в битах + 1:
         */
        constexpr int bit_ofset = str.size_bits() + 1;
        int str_num = str.field_u32(bit_ofset - 84, 4);
        if (str_num > 5)
        {
          if ((str_num % 2) == 0)
          {
            /*
             * Чтобы проверить извлечение параметров из ЦИ в формате frame2
             * попробуем извлечь наклонение орбиты. Полученное значение можно
             * будет сравнить с данными на оф. сайте:
             * https://www.glonass-iac.ru/en/GLONASS/ephemeris.php
             */
            uint32_t na = str.field_u32(bit_ofset - 77, 5); /* Получаем из строки номер системной точки */
            constexpr double resolution = 180.0/static_cast<double>(1 << 20);
            double incl = 63.0 + static_cast<double>(str.field_i32(bit_ofset - 41, 18)) * resolution;
            cout << " (sat_num = " << na << ", orbital inclination = " << incl << ")";
          }
        }
        cout << "\n    crc_error = " << glo.l2of_msg().crc_error() << endl;
      }
    }
  }

  return 0;
}

char*
cmd_param_parser(int argc, char ** argv)
{
  /*
   * Проверяем наличие файлов с РНП и ЦИ
   */
  if ( argc < 2 )
  {
    /*
     * Нулевой аргумент всегда содержит путь к программе и её имя.
     * Получаем из нулевого аргумента имя программы для вывода справки по
     * использованию программы
     */
    char *prog_name = std::strrchr(argv[0], '\\');
    if ( !prog_name )
      prog_name = std::strrchr(argv[0], '/');
    if ( !prog_name )
      prog_name = argv[0];
    else
      prog_name++;
    /*
     * Выводим маленькую справку по использованию программы
     */
    cout << "usage: " << prog_name << " <path to file with frame2>\n";
    exit(1);
  }
  return argv[1];
}
