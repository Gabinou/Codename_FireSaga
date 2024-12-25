
#include "nourstest.h"
#include "types.h"
#include "macros.h"
#include "platform.h"
#include "jsonio.h"

#undef ITERATIONS
#define ITERATIONS 10000000

void bench_jsonio() {
    u64 before_ns = tnecs_get_ns();

    struct Unit Silou = Unit_default;
    s8 path = s8_literal(PATH_JOIN("units", "Silou_test.json"));
    for (int i = 0; i < ITERATIONS; ++i) {
        jsonio_readJSON(path, &Silou);
    }

    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ns  = (after_ns - before_ns);
    u64 elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("jsonio %d ns", elapsed_ns);
    SDL_Log("jsonio %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("jsonio %d ms", elapsed_ms);
}
#undef ITERATIONS
