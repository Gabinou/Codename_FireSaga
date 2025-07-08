
#include "types.h"

/********* 0.1 MICROSECOND RESOLUTION CLOCK ********/
#if defined(__linux)
    #define MICROSECOND_CLOCK
    #define HAVE_POSIX_TIMER
    #include <time.h>
    #ifdef CLOCK_MONOTONIC
        #define CLOCKID CLOCK_MONOTONIC
    #else
        #define CLOCKID CLOCK_REALTIME
    #endif
#elif defined(__APPLE__)
    #define MICROSECOND_CLOCK
    #define HAVE_MACH_TIMER
    #include <mach/mach_time.h>
#elif defined(_WIN32)
    #define MICROSECOND_CLOCK
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

u64 IES_get_ns(void);
u64 IES_get_us(void);
