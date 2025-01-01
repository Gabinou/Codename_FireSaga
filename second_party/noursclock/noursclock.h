
#include <stdint.h>

/********************** 0.1 MICROSECOND RESOLUTION CLOCK **********************/
//  Modified from: https://gist.github.com/ForeverZer0/0a4f80fc02b96e19380ebb7a3debbee5
#if defined(__linux)
#  define MICROSECOND_CLOCK
#  define HAVE_POSIX_TIMER
#  include <time.h>
#  ifdef CLOCK_MONOTONIC
#     define CLOCKID CLOCK_MONOTONIC
#  else
#     define CLOCKID CLOCK_REALTIME
#  endif
#elif defined(__APPLE__)
#  define MICROSECOND_CLOCK
#  define HAVE_MACH_TIMER
#  include <mach/mach_time.h>
#elif defined(_WIN32)
#  define MICROSECOND_CLOCK
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

extern uint64_t  tnecs_get_ns();
extern double    tnecs_get_us();
