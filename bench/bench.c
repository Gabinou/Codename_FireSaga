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
    bench_nstr();
    bench_RNG();
    bench_jsonio();

    /* -- Postliminaries -- */
    return (0);
}