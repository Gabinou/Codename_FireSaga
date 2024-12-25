
#include "nourstest.h"
#include "types.h"
#include "macros.h"
#include "platform.h"
#include "utilities.h"
#include "nmath.h"
#include "unit/unit.h"
#include "jsonio.h"
#include "SDL.h"
#include "SDL_thread.h"

#undef ITERATIONS
#define ITERATIONS 1000  

#define MAX_THREADS 2
int jsonio_threadfunc(void *data) {
    s8 path = s8_literal(PATH_JOIN("units", "Silou_test.json"));
    jsonio_readJSON(path, data);
    return(0);
}

void bench_jsonio() {
    SDL_Log("Benchmarking jsonio: %d iterations", ITERATIONS);
    Filesystem_Init(0);
    Utilities_Load();

    struct Unit Silou = Unit_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(&Silou, weapons_dtab);

    u64 before_ns = tnecs_get_ns();
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

    // SDL_Thread thread_queue[MAX_THREADS];
    struct Unit units[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; ++i) {
        units[i] = Unit_default;
        Unit_InitWweapons(&units[i], weapons_dtab);
    }

    SDL_Thread **thread_queue = DARR_INIT(thread_queue, SDL_Thread*, MAX_THREADS);

    before_ns = tnecs_get_ns();

    for (int i = 0; i < ITERATIONS; ++i) {
        jsonio_readJSON(path, &Silou);
        SDL_Thread * thread = SDL_CreateThread(jsonio_threadfunc, "allo", &units[i % MAX_THREADS]);
        DARR_PUT(thread_queue, thread);
        if (DARR_NUM(thread_queue) >= MAX_THREADS) {
            SDL_WaitThread(thread_queue[0], NULL);
            DARR_DEL(thread_queue, 0);
        }
    }

    while (DARR_NUM(thread_queue) > 0) {
        SDL_WaitThread(thread_queue[0], NULL);
        DARR_DEL(thread_queue, 0);
    }

    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("jsonio %d ns", elapsed_ns);
    SDL_Log("jsonio %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("jsonio %d ms", elapsed_ms);
    // multithreading slower than serial for file reading
}
#undef ITERATIONS
#undef MAX_THREADS
