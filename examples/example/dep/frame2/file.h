/*
 * За основу был взят файл mrkframe12:source:libmrkframe12/mrkframe12/file.h
 * Логика работы функций не изменялась, изменились только типы данных и подключены
 * файлы библиотек, без которых код не компилировался.
 */

#ifndef FRAME2_FILE_H_
#define FRAME2_FILE_H_

#if !(defined(_WIN32) || defined(_WIN64) \
    || defined(unix) || defined(__unix) || defined(__unix__)\
    || defined(__APPLE__) || defined(__MACH__)\
    || defined(__linux__) || defined(linux) || defined(__linux) || defined(__FreeBSD__))
#define FRAME2_NO_FILE
#endif

#include <memory>
#include <string>
#include <cstdint>
#include <cstddef>


#include "frame2.h"

#ifndef FRAME2_NO_FILE

namespace frame2
{

#define BUF_SIZE 65536

class File
{
public:
  File(const std::string &path, char mode); // mode: 'r' или 'w'
  ~File();
  void SetPos(int32_t index);

  /* В случае успеха возвращает число прочитаных байт. Если
   * обнаруживается ошибка, то генерирует исключение или
   * возвращает 0 */
  int32_t Read(frame2::Frame2 &frame);

  void Write(const frame2::Frame2 &frame);
  void Write(std::vector<frame2::Frame2> &frames);
private:

  std::string path_;
  char mode_;
  std::FILE* fp_;
  const size_t buf_size_ = 65536;
  std::unique_ptr<uint8_t> buf_;
  int32_t current_pos_;
};

}  // namespace frame2

#endif // FRAME2_NO_FILE

#endif  // FRAME2_FILE_H_
