/*
 * main.cpp
 *
 *  Created on: 11 июн. 2019 г.
 *      Author: user
 */

#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

int
main (int argc, char* argv[])
{
  // global setup...

  std::cout << "Start Frame2 test" << std::endl;
  std::cout << "Test will take few minutes..." << std::endl;

  char** argv_local = argv;

  int result = Catch::Session().run(argc, argv_local);

  // global clean-up...

  return result;
}
