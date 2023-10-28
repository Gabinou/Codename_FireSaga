#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "types.h"
#include "globals.h"
#include "SDL2/SDL.h"

/* --- LOG --- */
enum SOTA_LOG {
    /* Only for SDL_LogVerbose(SOTA_LOG_FPS, ""); */
    SOTA_LOG_FPS = SDL_LOG_CATEGORY_CUSTOM,
};

extern void Log2file(void *d, i32 c, SDL_LogPriority p, const char *m);

#endif /* LOG_H */
