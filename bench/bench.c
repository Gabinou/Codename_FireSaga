#include "filesystem.h"
#include "nourstest.h"
#include "log.h"
#include "bench_RNG.h"
#include "bench_nstr.h"
#include "bench_jsonio.h"

int main(int argc, char *argv[]) {
    /* -- Preliminaries -- */
    Log_Init();

    /* -- Running benchmarks -- */
    SDL_Log("Benchmarking nstr");
    bench_nstr();
    SDL_Log("Benchmarking RNG");
    bench_RNG();
    SDL_Log("Benchmarking jsonio");
    bench_jsonio();

    /* -- Postliminaries -- */
    return (0);
}