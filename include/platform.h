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

int IES_Platform(void);

/* --- IES defines --- */
/* Make core functionality platform independent */
int sota_mkdir(char *name);

#ifdef SDL_malloc
#define IES_malloc SDL_malloc
#else
#define IES_malloc malloc
#endif /* SDL_malloc */

#ifdef SDL_calloc
#define IES_calloc SDL_calloc
#else
#define IES_calloc calloc
#endif /* SDL_calloc */

#ifdef SDL_assert
#define IES_assert SDL_assert
#else
#define IES_assert assert
#endif /* SDL_assert */

#ifdef SDL_free
#define IES_free SDL_free
#else
#define IES_free free
#endif /* SDL_assert */

#endif /* PLATFORM_H */
