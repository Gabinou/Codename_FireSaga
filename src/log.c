
#include "log.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

/* --- LOG --- */
/* Copied from SDL_log.c */
static const char *SDL_priority_prefixes[SDL_NUM_LOG_PRIORITIES] = {
    NULL,
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "CRITICAL",
};

void Log_Init() {
    /* -- Clear logfile -- */
    fclose(fopen(LOGFILE, "w"));

    /* -- SOTA_LOG_FPS -- */
    #ifndef SOTA_VERBOSE
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    // #else /* SOTA_VERBOSE */
    // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
    #endif /* SOTA_VERBOSE */

    /* -- SOTA_LOG_FPS -- */
    /* Change to SDL_LOG_PRIORITY_DEBUG to see system logs */
    SDL_LogSetPriority(SOTA_LOG_SYSTEM, SDL_LOG_PRIORITY_INFO);

    /* Change to SDL_LOG_PRIORITY_VERBOSE to see FPS */
    SDL_LogSetPriority(SOTA_LOG_FPS, SDL_LOG_PRIORITY_CRITICAL);

    #if HAVE_STDIO_H
    SDL_LogSetOutputFunction(Log2file, NULL);
    #endif
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Logfile set\n");
}

void Log2file(void *userdata, int category, SDL_LogPriority priority,
              const char *message) {
    /* -- Log to file -- */
    FILE *logf = fopen(LOGFILE, "a");
    fprintf(logf, "%s:\t%s\n", SDL_priority_prefixes[priority], message);
    fclose(logf);

    /* -- Log  -- */
    /* Copied from SDL_log.c */
    fprintf(stderr, "%s:    %s\n", SDL_priority_prefixes[priority], message);
}

