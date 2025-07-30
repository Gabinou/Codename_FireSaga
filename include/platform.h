#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <string.h>
#include "types.h"
#include "SDL.h"

/* --- IDs --- */
enum PLATFORMS {
    NO_PLATFORM = 0,
    WINDOWS_32  = 1,
    WINDOWS_64  = 2,
    LINUX       = 3,
    ANDROID     = 4,
    MAC_OS_X    = 4,
    iOS         = 5,
    SDL         = 6,
};

enum COMPILERS {
    NO_COMPILER = 0,
    TCC         = 1,
    GCC         = 2,
    CLANG       = 3,
};
/* --- COMPILER IDENTIFICATION --- */
// Predefined macros: "echo | tcc -dM -E -"
#if defined(__TINYC__)
    #define COMPILER TCC
    #define COMPILER_NAME "tcc"
#elif defined(__GNUC__)
    #define COMPILER GCC
    #define COMPILER_NAME "gcc"
#elif defined(__clang__)
    #define COMPILER CLANG
    #define COMPILER_NAME "clang"
#endif /* COMPILER IDENTIFICATION */

/* --- PLATFORM IDENTIFICATION & SEPARATOR SPECIFICATION --- */
#define PHYSFS_SEPARATOR "/" /* -> PHYSFS_Mount() dirs */
#if defined(__unix__) || defined(__linux__)
    #define PLATFORM LINUX
    #define PLATFORM_NAME "Linux"
    #define DIR_SEPARATOR "/"
    #include <sys/stat.h>
#elif defined(__APPLE__) || defined(__MACH__)
    #define PLATFORM MAC_OS_X
    #define PLATFORM_NAME "Mac OS X"
    #define DIR_SEPARATOR ":"
    #include <sys/stat.h>
#elif defined(_WIN32) || defined(WIN32)
    #define PLATFORM WINDOWS_32
    #define PLATFORM_NAME "Windows_32"
    #define DIR_SEPARATOR "\\"
    #include <direct.h>
#elif defined(_WIN64) || defined(WIN64)
    #define PLATFORM WINDOWS_64
    #define PLATFORM_NAME "Windows_64"
    #define DIR_SEPARATOR "\\"
    #include <direct.h>
#else
    #define PLATFORM NO_PLATFORM
    #define PLATFORM_NAME "NO_PLATFORM"
    #define DIR_SEPARATOR NULL
#endif /* PLATFORM IDENTIFICATION */

int sota_mkdir(char *name);
u8 platform_fromSDL(void);

#endif /* PLATFORM_H */
