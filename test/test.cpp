#include "minctest.h"
#include "test_linalg.hpp"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);
    printf("Running tests\n");

    lrun("Test_linalg", test_linalg);
    lresults();
    return (lfails != 0);
}