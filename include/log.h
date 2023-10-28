#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "types.h"
#include "globals.h"
#include "SDL2/SDL.h"

/* --- LOG --- */
extern void Log2file(void *d, i32 c, SDL_LogPriority p, const char *m);

#endif /* LOG_H */
