
#include "nourstest.h"
#include "types.h"
#include "tnecs.h"
#include "RNG.h"

#undef ITERATIONS
#define ITERATIONS 10000000

void bench_RNG() {
    u64 before_ns = tnecs_get_ns();
    struct TINYMT32_T tinymt;
    RNG_Init_tinymt(&tinymt);
    for (int i = 0; i < ITERATIONS; ++i) {
        tinymt32_generate_uint32(&tinymt);
    }
    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ns  = (after_ns - before_ns);
    u64 elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("Tinymt %d ns", elapsed_ns);
    SDL_Log("Tinymt %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("Tinymt %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    for (int i = 0; i < ITERATIONS; ++i) {
        RNG_openBSD_u32(&tinymt, 0, 99);
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("RNG_openBSD_u32 %d ns", elapsed_ns);
    SDL_Log("RNG_openBSD_u32 %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("RNG_openBSD_u32 %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    RNG_Init_xoroshiro256ss();
    for (int i = 0; i < ITERATIONS; ++i) {
        next_xoshiro256ss();
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("xoshiro256ss %d ns", elapsed_ns);
    SDL_Log("xoshiro256ss %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("xoshiro256ss %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    for (int i = 0; i < ITERATIONS; ++i) {
        RNG_openBSD_u64(0ULL, 99ULL);
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("RNG_openBSD_u64 %d ns", elapsed_ns);
    SDL_Log("RNG_openBSD_u64 %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("RNG_openBSD_u64 %d ms", elapsed_ms);

    /* --- CONCLUSION --- */
    /* xoshiro256ss < 2x faster than tinymt32 */
}
#undef ITERATIONS
