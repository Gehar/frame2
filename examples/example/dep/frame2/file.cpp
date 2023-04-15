/*
 * За основу был взят файл mrkframe12:source:libmrkframe12/mrkframe12/file.h
 * Логика работы функций не изменялась, изменились только типы данных и подключены
 * файлы библиотек, без которых код не компилировался.
 */

#include <cstring>
#include <stdexcept>

#include "file.h"

#ifndef FRAME2_NO_FILE

namespace frame2
{

File::File(const std::string &path, char mode) :
    path_(path), mode_(mode)
{
  if (mode_ == 'r')
  {
    fp_ = fopen(path.c_str(), "rb");
    if (!fp_)
      throw std::runtime_error("Error! Frame2: fopen(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\".");
  }
  else if (mode_ == 'w')
  {
    fp_ = fopen(path.c_str(), "wb");
    if (!fp_)
      throw std::runtime_error("Error! Frame2: fopen(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\".");
  }
  else
  {
    std::runtime_error("Error! Frame2: invalid mode argument.");
  }
  buf_ = std::unique_ptr<uint8_t>(new uint8_t[buf_size_]);
  current_pos_ = 0;
}

File::~File()
{
  std::fclose(fp_);
}

void File::SetPos(int32_t index)
{
  if (mode_ != 'r')
    throw std::runtime_error("Error! Frame2::File.SetPos(): can't move in file not opened for reading.");

  if (index == current_pos_)
    return;

  if ((index == 0) or (index < current_pos_))
  {
    // установка указателя на начало файла:
    fpos_t begin = 0;
    if (fsetpos64(fp_, &begin) != 0)
      throw std::runtime_error(
          "Error! Frame2::File.SetPos(): fsetpos64(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\", index = " + std::to_string(index));

    if (index == 0)
      return;
  }

  // перемещение по файлу от начала, либо от текущей позиции:
  int32_t shift;
  if (index < current_pos_)
  {
    shift = index;
  }
  else
  {
    shift = index - current_pos_;
  }

  size_t result;
  uint32_t frame_length;

  for (int32_t i = 0; i < shift; i++)
  {
    // первые три байта, включая размер кадра:
    result = fread(buf_.get(), 1, 8, fp_);
    if (result != 8)
    {
      if (feof(fp_))
        throw std::runtime_error("Error! Frame2::File.SetPos(): end of file was reached, filename = \"" + path_ + "\", index = " + std::to_string(index));
      else
        throw std::runtime_error(
            "Error! Frame2::File.SetPos(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\", index = " + std::to_string(index));
    }

    memcpy(&frame_length, buf_.get() + 4, sizeof(frame_length)); // +1: смещение поля с размером кадра

    if (frame_length == 0)
      throw std::runtime_error("Error! Frame2::File.SetPos(): frame_length = 0. Can't move file position.");

    result = fread(buf_.get() + 8, 1, frame_length - 8, fp_);
    if (result != size_t(frame_length - 8))
    {
      if (feof(fp_))
        throw std::runtime_error("Error! Frame2::File.SetPos(): end of file was reached, filename = \"" + path_ + "\", index = " + std::to_string(index));
      else
        throw std::runtime_error(
            "Error! Frame2::File.SetPos(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\", index = " + std::to_string(index));
    }
  }
  current_pos_ = index;
}

void File::Write(std::vector<frame2::Frame2> &frames)
{
  if (mode_ != 'w')
  {
    throw std::runtime_error("Error! Frame2::File.Write(): file is not opened for writing.");
  }
  for (auto &f : frames)
  {
    Write(f);
  }
}

int32_t File::Read(frame2::Frame2 &frame)
{
  if (mode_ != 'r')
  {
    throw std::runtime_error("Error! Frame2::File.Read(): file is not opened for reading, filename = \"" + path_ + "\".");
  }

  size_t result;
  uint32_t frame_length;

  // первые три байта, включая размер кадра:
  result = fread(buf_.get(), 1, 8, fp_);
  if (result != 8)
    return 0; /* TODO: вернуть файловый указатель на место */

  memcpy(&frame_length, buf_.get() + 4, sizeof(frame_length)); // +1: смещение поля с размером кадра

  // чтение кадра:
  if (frame_length != 0)
  {
    result = fread(buf_.get() + 8, 1, frame_length - 8, fp_);
    if (result != size_t(frame_length - 8))
      return 0; /* TODO: вернуть файловый указатель на место */

    // разбор кадра:
    result = frame.ParseFromArray(buf_.get(), frame_length);
    if (result == 0)
    {
      throw std::runtime_error("Error! Frame2::File.Read(): ParseFromArray() return 0");
    }

    current_pos_++;
    return frame_length;
  }
  else
  {
    frame.Clear();
    return 0; /* TODO: вернуть файловый указатель на место */
  }
}
void File::Write(const frame2::Frame2 &frame)
{
  if (mode_ != 'w')
  {
    throw std::runtime_error("Error! Frame2::File.Write(): file is not opened for writing.");
  }

  size_t size = frame.SerializeToArray(buf_.get(), buf_size_);

  if (fwrite(buf_.get(), 1, size, fp_) != size)
    throw std::runtime_error("Error! Frame2::File.Write(): " + std::string(strerror(errno)) + ", filename = \"" + path_ + "\".");
}

}  // namespace frame2

#endif // FRAME2_NO_FILE
