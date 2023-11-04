#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "types.h"
#include "enums.h"
#include "SDL.h"

/* --- Useful log funcs --- */
//      SDL_LogVerbose(SOTA_LOG_FPS, "");  -> only publishes if category is verbose
//      SDL_LogCritical(0, "");            -> always publishes

/* --- LOG --- */
enum SOTA_LOG {
    SOTA_LOG_APP    = SDL_LOG_CATEGORY_APPLICATION,
    SOTA_LOG_ERROR  = SDL_LOG_CATEGORY_ERROR,
    SOTA_LOG_ASSERT = SDL_LOG_CATEGORY_ASSERT,
    SOTA_LOG_SYSTEM = SDL_LOG_CATEGORY_SYSTEM,
    SOTA_LOG_AUDIO  = SDL_LOG_CATEGORY_AUDIO,
    SOTA_LOG_VIDEO  = SDL_LOG_CATEGORY_VIDEO,
    SOTA_LOG_RENDER = SDL_LOG_CATEGORY_RENDER,
    SOTA_LOG_INPUT  = SDL_LOG_CATEGORY_INPUT,
    SOTA_LOG_TEST   = SDL_LOG_CATEGORY_TEST,

    /* Only for SDL_LogVerbose(SOTA_LOG_FPS, ""); */
    SOTA_LOG_FPS = SDL_LOG_CATEGORY_CUSTOM,
};

void Log_Init();
void Log2file(void *d, i32 c, SDL_LogPriority p, const char *m);

#endif /* LOG_H */
