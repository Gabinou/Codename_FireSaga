#include "minctest.h"
#include "test_linalg.hpp"
#include "test_pathfinding.hpp"
#include "test_utilities.hpp"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);
    printf("Running tests\n");

    lrun("Test_linalg", test_linalg);
    lrun("Test_pathfinding ", test_pathfinding);
    lrun("Test_utilities ", test_utilities);
    lresults();
    return (lfails != 0);
}