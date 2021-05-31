
#include "platform.h"

char dir_separator = 0;

uint8_t platform_fromSDL() {
    uint8_t out = NO_PLATFORM;
    const char * platform_name = SDL_GetPlatform();
    if (strcmp(platform_name, "Windows") == 0) {
        out = WINDOWS_32;
    } else if (strcmp(platform_name, "Mac OS X") == 0) {
        out = MAC_OS_X;
    } else if (strcmp(platform_name, "Linux") == 0) {
        out = LINUX;
    } else if (strcmp(platform_name, "Android") == 0) {
        out = ANDROID;
    } else if (strcmp(platform_name, "iOS") == 0) {
        out = iOS;
    }
    return (out);
}