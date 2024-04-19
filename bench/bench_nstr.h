#include "nourstest.h"
#include "nstr.h"

#undef ITERATIONS
#define ITERATIONS 100000
void bench_s8() {
    char string[33] = "Hello, my mother's name is baka.\0";

    /* Benchmark n8 */
    u64 before_ns = tnecs_get_ns();
    s8 s8_string = s8_mut(string);
    for (int i = 0; i < ITERATIONS; i++) {
        s8_toLower(s8_string);
        s8_toUpper(s8_string);
    }

    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("s8 %d ms", elapsed_ms);

    /* Benchmark null terminated string */
    before_ns = tnecs_get_ns();
    for (int i = 0; i < ITERATIONS; i++) {
        nstr_toLower(string);
        nstr_toUpper(string);
    }

    after_ns    = tnecs_get_ns();
    elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("null %d ms", elapsed_ms);

    /* --- FREE --- */
    s8_free(&s8_string);

    /* --- CONCLUSION --- */
    // n8 string 3x-4x faster than null-terminated strings!
}

void bench_nstr() {
    bench_s8();
}

#undef ITERATIONS
