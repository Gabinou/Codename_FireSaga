#include "minctest.h"
#include "utilities.hpp"

int current;
int upper;
int lower;
int out;

void test_utilities() {
    current = 0;
    upper = 3;
    lower = 0;

    out = pingpong(current, upper);
    lok(out == 0);
    
    current++;
    out = pingpong(current, upper);
    lok(out == 1);

    current++;
    out = pingpong(current, upper);
    lok(out == 2);

    current++;
    out = pingpong(current, upper);
    lok(out == 1);

    current++;
    out = pingpong(current, upper);
    lok(out == 0);
    
    current++;
    out = pingpong(current, upper);
    lok(out == 1);

    current++;
    out = pingpong(current, upper);
    lok(out == 2);
    

}
