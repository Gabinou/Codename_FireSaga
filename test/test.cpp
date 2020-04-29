#include "minctest.h"
#include "test_linalg.hpp"
#include "test_pathfinding.hpp"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);
    printf("Running tests\n");

    lrun("Test_linalg", test_linalg);
    lrun("Test_pathfinding ", test_pathfinding);
    lresults();
    return (lfails != 0);
}