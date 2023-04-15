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

  char** argv_local = argv;

#ifdef STANDALONE_CACH_SUPPORT
  argc = 1;
  char app_name[] = "frame2-test";
  char *papp_name = app_name;
  argv_local = &papp_name;
#endif

  int result = Catch::Session().run(argc, argv_local);

  // global clean-up...

  return result;
}
