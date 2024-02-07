/*
 * directory_lib.h
 *
 *  Created on: 19 окт. 2023 г.
 *      Author: user
 */

#ifndef DIRECTORY_LIB_H_
#define DIRECTORY_LIB_H_

#include <string>

/*
 * Проверяет существование директории
 * Взято от сюда: https://stackoverflow.com/a/29828907
 */
bool is_dir_exist(const std::string& path);

bool make_path(const std::string& path);

#endif /* DIRECTORY_LIB_H_ */
