
#include "log.h"

/* --- LOG --- */
/* Copied from SDL_log.c */
static const char *SDL_priority_prefixes[SDL_NUM_LOG_PRIORITIES] = {
    NULL,
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "CRITICAL"
};

void Log2file(void *userdata, int category, SDL_LogPriority priority,
              const char *message) {
    /* -- Log to file -- */
    FILE *logf = fopen(LOGFILE, "a");
    fprintf(logf, "%s", message);
    fprintf(logf, "%s", "\n");
    fclose(logf);

    /* -- Log  -- */
    /* Copied from SDL_log.c */
    fprintf(stderr, "%s: %s\n", SDL_priority_prefixes[priority], message);
}
