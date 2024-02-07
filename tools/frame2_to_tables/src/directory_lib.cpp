/*
 * directory_lib.cpp
 *
 *  Created on: 19 окт. 2023 г.
 *      Author: user
 */

#include <sys/stat.h>

#include "directory_lib.h"

bool is_dir_exist(const std::string &path)
{
  /*
   * Взято от сюда: https://stackoverflow.com/a/29828907
   */

    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool make_path(const std::string &path)
{
    int ret = mkdir(path.c_str());
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            int pos = path.find_last_of('/');
            if (pos == static_cast<int>(std::string::npos))
                return false;
            if (!make_path( path.substr(0, pos) ))
                return false;
        }
        // now, try to create again
        return 0 == mkdir(path.c_str());

    case EEXIST:
        // done!
        return is_dir_exist(path);

    default:
        return false;
    }
}
