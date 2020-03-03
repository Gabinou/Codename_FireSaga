#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>

namespace FILESYSTEM {
void loadFileToMemory(const char * name, unsigned char ** mem,
                      size_t * len, bool addnull = false);
}

#endif /* FILESYSTEM_HPP */