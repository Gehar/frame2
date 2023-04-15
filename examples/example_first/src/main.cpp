/*
 * main.cpp
 *
 *  Created on: 17 июн. 2019 г.
 *      Author: Aleksandr Senchenko
 *      Автор: Сенченко А. А.
 *      email: alsenchenko@mail.ru
 */

/* Подключаем главный заголовочный файл для работы с frame2 */
#include <frame2/frame2.h>
/* Чтобы читать frame2 из файла подключим еще один файл */
#include <frame2/file.h>

#include <cstring>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

/* Простейший парсер командной строки, призванный возвращать путь к файлу с кадром */
char* cmd_param_parser(int argc, char ** argv);

constexpr size_t channels_num = 12;

int
main (int argc, char ** argv)
{
  char *path_to_frame2 = cmd_param_parser(argc, argv);
  cout << "Path to Frame2: \"" << path_to_frame2 << "\"\n";

  /*********************************************************************************\
   *
   * ПЕРЕД ИЗУЧЕНИЕМ ПРИМЕРА РЕКОМЕНДУЕТСЯ ПОСМОТРЕТЬ "Очень кратко о Frame2.pdf"
   *
  \*********************************************************************************/

  /*
   * Класс для разбора данных в формате Frame2.
   * Класс написан на языке C++.
   *
   * !!! ВНИМАНИЕ !!!
   *
   * Для компиляции рекомендуется gcc версии не ниже 7.2.0.
   * Можно откомпилировать в gcc v6.3.0, но производительность будет ниже.
   * Более старые версии gcc не проверялись и класс может не откомпилироваться.
   */

  /*
   * Для создания класса конструктору нужно сообщить максимальное число каналов.
   * Сейчас наши приемники имеют 12 каналов. Но можно указать больше, с запасом.
   */
  frame2::Frame2 frame(channels_num);

  /* Класс для чтения кадра в формате Frame2 из файла и передачи его
   * в класс frame2::Frame2 для разбора */
  frame2::File file(path_to_frame2, 'r');

  /* Читаем каждр из файла. При следующем вызове file.Read(frame)
   * будет прочитан следующий кадр. */
  if (file.Read(frame) == 0)
  {
    cout << "File ended\n";
    return 0;
  }

  /*
   * В заголовке содержится временная метка данных.
   * Значение метки времени зависит от текущей шкалы времени, в которой приемник выдает данные.
   * Выведем в какой шкале времени сейчас приемник выдает данные
   */
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
  /* Теперь выведем метку времени, она хранится в мс */
  cout << "timestamp: " << frame.header().timestamp_ms() << " ms\n";

  /*
   * Кадр содержит заголовок, за которым могут идти:
   *   - Подкадр с измерениями и ЦИ по навигационной системе ГЛОНАСС
   *   - Подкадр с измерениями и ЦИ по навигационной системе GPS
   *   - Подкадр с измерениями и ЦИ по навигационной системе Galileo
   * Проверяем, что есть данные по ГЛОНАСС
   */
  if (frame.glo_size() == 0) // Проверям, что данные по ГЛОНАСС есть
  {
    cout << "Frame2 don't contain GLONASS data\n";
    return 0;
  }

  /*
   * Подкадр с данными по какой-либо навигационной системе состоит
   * из записей по каждому принимаемому НКА.
   * Для удобства дальнейшей работы сохраним ссылку на нулевую запись
   */
  const frame2::Glo &glo = frame.glo(0);

  /* Блок id присутствует всегда. */
  cout << "channel: " << glo.id().channel() << endl;
  cout << "freq_num = " << (int)glo.id().freq_num() << endl;
  cout << "sat_num = " << (int)glo.id().sys_num() << endl;

  /*
   * Амплитуда, номер текущего состояния канала, СКО ошибки на выходе
   * фазового дискриминатора (СКО фазы) хранятся в вспомогательном блоке
   * aux - auxiliary - вспомогательный, дополнительный
   * Большинство полей в этом блоке являются необязательными и могут отсутствовать.
   * Обязательно присутствуют только поля state (номер текущего состояния канала)
   * и поле с флагами.
   */
  if (glo.has_l1of_aux())
  {
    cout << "state = " << (int)glo.l1of_aux().state() << endl;
    if (glo.l1of_aux().has_ampl_data())
      cout << "amplitude = " << (int)glo.l1of_aux().ampl_data() << " units" << endl;
    if (glo.l1of_aux().has_phase_std_data())
      cout << "phase std = " << (int)glo.l1of_aux().phase_std_data() << " deg." << endl;
  }

  /* Проверяем, если ли измерения РНП по L1 СТ */
  if (glo.has_l1of_obs())
  {
    cout << "code pseudorange: " << glo.l1of_obs().code_pr() << " m" << endl;
    cout << "DFS: " << glo.l1of_obs().dfs() << " Hz" << std::endl;
  }

  if (glo.has_l1of_msg())
  {
    /*
     * Сначало выведем строку ЦИ в виде hex кодов,
     * а затем покажем, как с использованием Frame2 легко извлекать поля из ЦИ
     */
    cout << "L1OF DI:\n";
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
     * начала строки. Для использования NavString с ЦИ ГЛОНАСС СТ сигналов (с частотным разделением)
     * необходимо из (длины_строки_в_битах + 1) вычесть номер старшего бита по ИКД:
     */
    constexpr int bit_ofset = str.size_bits() + 1; // (длина_строки_в_битах + 1)
    // Переменная str_num будет содержат номер данной строки
    int str_num = str.field_u32(bit_ofset - 84, 4); // В ИКД старший бит поля m (номер строки) имеет номер 84
    cout << "string number: " << str_num << endl;
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
