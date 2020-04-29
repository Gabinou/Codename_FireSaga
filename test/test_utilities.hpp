#include "minctest.h"
#include "utilities.hpp"

int current;
int upper1;
int upper2;
int lower;
int out;

void test_utilities() {
    current = 0;
    upper1 = 3;
    upper2 = 6;
    lower = 0;

    out = pingpong(current, upper1);
    lok(out == 0);
    out = pingpong(current, upper2);
    lok(out == 0);
    
    current++;
    out = pingpong(current, upper1);
    lok(out == 1);
    out = pingpong(current, upper2);
    lok(out == 1);

    current++;
    out = pingpong(current, upper1);
    lok(out == 2);
    out = pingpong(current, upper2);
    lok(out == 2);

    current++;
    out = pingpong(current, upper1);
    lok(out == 1);
    out = pingpong(current, upper2);
    lok(out == 3);

    current++;
    out = pingpong(current, upper1);
    lok(out == 0);
    out = pingpong(current, upper2);
    lok(out == 4);
    
    current++;
    out = pingpong(current, upper1);
    lok(out == 1);
    out = pingpong(current, upper2);
    lok(out == 5);

    current++;
    out = pingpong(current, upper1);
    lok(out == 2);
    out = pingpong(current, upper2);
    lok(out == 4);
    

}
