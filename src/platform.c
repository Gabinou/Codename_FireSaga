
#include "platform.h"

u8 platform_fromSDL() {
    u8 out = NO_PLATFORM;
    const char *platform_name = SDL_GetPlatform();
    if (strcmp(platform_name, "Windows")        == 0)
        out = WINDOWS_32;
    else if (strcmp(platform_name, "Mac OS X")  == 0)
        out = MAC_OS_X;
    else if (strcmp(platform_name, "Linux")     == 0)
        out = LINUX;
    else if (strcmp(platform_name, "Android")   == 0)
        out = ANDROID;
    else if (strcmp(platform_name, "iOS")       == 0)
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