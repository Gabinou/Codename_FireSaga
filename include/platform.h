#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "SDL2/SDL.h"

enum PLATFORMS {
    NO_PLATFORM = 0,
    WINDOWS_32 = 1,
    WINDOWS_64 = 2,
    LINUX = 3,
    ANDROID = 4,
    MAC_OS_X = 4,
    iOS = 5,
};

// Compiler macros: __TINYC__ for tcc
#define PHYSFS_SEPARATOR "/" // -> for PHYSFS_Mount() directories
#define PHYSICSFS_SEPARATOR PHYSFS_SEPARATOR

#if defined(__unix__) || defined(__linux__)
#define PLATFORM LINUX
#define PLATFORM_NAME "Linux"
#define DIR_SEPARATOR "/"
#elif defined(__APPLE__) || defined(__MACH__)
#define PLATFORM MAC_OS_X
#define PLATFORM_NAME "Mac OS X"
#define DIR_SEPARATOR ":"
#elif defined(_WIN32) || defined(WIN32)
#define PLATFORM WINDOWS_32
#define PLATFORM_NAME "Windows_32"
#define DIR_SEPARATOR "\\"
#elif defined(_WIN64) || defined(WIN64)
#define PLATFORM WINDOWS_64
#define PLATFORM_NAME "Windows_64"
#define DIR_SEPARATOR "\\"
#else
#define PLATFORM NO_PLATFORM
#define PLATFORM_NAME "NO_PLATFORM"
#define DIR_SEPARATOR NULL
#endif

extern char dir_separator;
extern uint8_t platform_fromSDL();

#endif /* PLATFORM_H */
