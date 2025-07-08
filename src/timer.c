
#include "timer.h"

u64 IES_get_ns(void) {
    static u64 is_init = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (0 == is_init) {
        mach_timebase_info(&info);
        is_init = 1;
    }
    u64 now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
#elif defined(__linux)
    static struct timespec linux_rate;
    if (0 == is_init) {
        clock_getres(CLOCKID, &linux_rate);
        is_init = 1;
    }
    u64 now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
#elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (0 == is_init) {
        QueryPerformanceFrequency(&win_frequency);
        is_init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (u64)((1e9 * now.QuadPart) / win_frequency.QuadPart);
#endif
}
#ifdef MICROSECOND_CLOCK
u64 IES_get_us(void) {
    return (IES_get_ns() / 1e3);
}
#else
#  define FAILSAFE_CLOCK
#  define IES_get_us(void) ((clock())/CLOCKS_PER_SEC*1e6) // [us]
#  define IES_get_ns(void) ((clock())/CLOCKS_PER_SEC*1e9) // [ns]
#endif
