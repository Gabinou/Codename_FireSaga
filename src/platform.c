
#include "platform.h"

u8 platform_fromSDL(void) {
    u8 out  = NO_PLATFORM;
    s8 name = s8_var(SDL_GetPlatform());

    if (s8equal(name, s8_literal("Windows")))
        out = WINDOWS_32;
    else if (s8equal(name, s8_literal("Mac OS X")))
        out = MAC_OS_X;
    else if (s8equal(name, s8_literal("Linux")))
        out = LINUX;
    else if (s8equal(name, s8_literal("Android")))
        out = ANDROID;
    else if (s8equal(name, s8_literal("iOS")))
        out = iOS;
    return (out);
}

int sota_mkdir(char *name) {
#if defined(__unix__)     || defined(__linux__)
    mkdir(name, 0777);
#elif defined(__APPLE__)  || defined(__MACH__)
    mkdir(name, 0777);
#elif defined(_WIN32)     || defined(WIN32)
    _mkdir(name);
#elif defined(_WIN64)     || defined(WIN64)
    _mkdir(name);
#endif
    return (1);
}