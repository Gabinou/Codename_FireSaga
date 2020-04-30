#include "minctest.h"
#include "test_linalg.hpp"
#include "test_pathfinding.hpp"
#include "test_utilities.hpp"
#include "test_position.hpp"
#include "test_tile.hpp"
#include "tile.hpp"

int main(int argc, char * argv[]) {
    // freopen("test_results.txt", "w+", stdout);
    printf("Running tests\n");

    lrun("Test_linalg", test_linalg);
    lrun("Test_pathfinding ", test_pathfinding);
    lrun("Test_utilities ", test_utilities);
    lrun("Test_position ", test_position);
    lrun("Test_tile ", test_tile);
    lresults();

    // getchar();
    return (lfails != 0);
}