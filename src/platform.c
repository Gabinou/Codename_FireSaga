
#include "platform.h"

u8 platform_fromSDL() {
    u8 out = NO_PLATFORM;

    s8 name     = s8_var(SDL_GetPlatform());

    s8 mac      = s8_literal("Mac OS X");
    s8 ios      = s8_literal("iOS");
    s8 slinux   = s8_literal("Linux");
    s8 android  = s8_literal("Android");
    s8 windows  = s8_literal("Windows");

    if (s8equal(&name, &windows))
        out = WINDOWS_32;
    else if (s8equal(&name, &mac))
        out = MAC_OS_X;
    else if (s8equal(&name, &slinux))
        out = LINUX;
    else if (s8equal(&name, &android))
        out = ANDROID;
    else if (s8equal(&name, &ios))
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