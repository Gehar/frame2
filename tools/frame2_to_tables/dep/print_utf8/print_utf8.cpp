#include "print_utf8.h"
#include <iostream>

#ifdef __WIN32__

std::wstring utf8_decode(const std::string &str)
{
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0);
  std::wstring wstrTo(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}

void print_utf8(std::string str)
{
  std::wcout << utf8_decode(str);
}

void cout_utf8(std::string str)
{
  std::wcout << utf8_decode(str);
}

#else

void print_utf8(std::string str)
{
  std::cout << str << std::endl;
}

#endif
