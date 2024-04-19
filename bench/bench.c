#include "filesystem.h"
#include "nourstest.h"
#include "bench_RNG.h"
#include "bench_nstr.h"

int main(int argc, char *argv[]) {
    /* -- Preliminaries -- */
    Log_Init();

    /* -- Running benchmarks -- */
    SDL_Log("Benchmarking nstr");
    bench_nstr();
    SDL_Log("Benchmarking RNG");
    bench_RNG();

    /* -- Postliminaries -- */
    SDL_Quit();
    fclose(stdout);
    return (0);
}