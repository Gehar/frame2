#include <frame2/array.h>

#include "catch.hpp"

#include <stdint.h>

TEST_CASE( "class frame2::Array testing", "[Array]" )
{
  // Конструктор без параметров
  constexpr size_t n = 10;
  frame2::Array<int32_t, n> a1;
  REQUIRE ( a1.size() == 0);
  for (size_t i = 0; i < n; i++)
    REQUIRE ( a1[i] == 0);

  // Конструктор с параметром по умолчанию
  frame2::Array<int32_t, n> a2(1);
  REQUIRE ( a2.size() == 1);
  for (size_t i = 0; i < n; i++)
    REQUIRE ( a2[i] == 0);

  // Конструктор с параметром по умолчанию и нулевым указателем
  frame2::Array<int32_t, n> a3(1, nullptr);
  REQUIRE ( a3.size() == 0);
  for (size_t i = 0; i < n; i++)
    REQUIRE ( a3[i] == 0);

  // Проверяем оператор []
  for (size_t i = 0; i < n; i++)
  {
    a3[i] = static_cast<int32_t>(i);
    REQUIRE ( a3[i] == i);
  }

  // Проверяем resize()
  for (size_t i = 0; i < 2*n; i++)
  {
    a3.resize(i);
    if (i <= n)
      REQUIRE ( a3.size() == i);
    else
      REQUIRE ( a3.size() == n);
  }

  // Проверяем конструкторы, в которые передан размер массива больше, чем определено шаблонным параметром
  frame2::Array<int32_t, n> a4(n+1);
  REQUIRE ( a4.size() == n);
  std::array<int32_t, 2*n> std_a;
  for (size_t i = 0; i < std_a.size(); i++)
    std_a[i] = static_cast<int32_t>(i);
  frame2::Array<int32_t, n> a5(n+1, &std_a[0]);
  REQUIRE ( a5.size() == n);
  for (size_t i = 0; i < a5.size(); i++)
    REQUIRE ( a5[i] == std_a[i]);
}
