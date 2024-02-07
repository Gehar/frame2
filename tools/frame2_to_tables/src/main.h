#ifndef MAIN_H
#define MAIN_H

static const char USAGE[] =
R"(
Usage:
  )" prog_name R"( <input> [options]
  )" prog_name R"( -h | --help
  )" prog_name R"( --version

Преобразование данных frame2 в табличные плоские данные.

  <input>                   Имя входного файла с данными в формате frame2.

Options:
  --channels=<channels>     Количество каналов [default: 12].
  --output=<output>         Директория для выходных данных.
  -h --help                 Отображение справки.
  --version                 Версия программы.)";

#endif  // MAIN_H
