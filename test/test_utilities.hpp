#include "minctest.h"
#include "utilities.hpp"

int current;
int upper1;
int upper2;
int lower;
int out;
int geo_factor;
int distance;

void test_utilities() {
    current = 0;
    upper1 = 3;
    upper2 = 6;
    lower = 0;
    distance = 32;

    distance = geometricslide(distance);
    lok(distance == 16);
    distance = geometricslide(distance);
    lok(distance == 8);
    distance = geometricslide(distance);
    lok(distance == 4);
    distance = geometricslide(distance);
    lok(distance == 2);
    distance = geometricslide(distance);
    lok(distance == 1);
    distance = geometricslide(distance);
    lok(distance == 1);

    geo_factor = 3;
    distance = 243;
    distance = geometricslide(distance, geo_factor);
    lok(distance == 81);
    distance = geometricslide(distance, geo_factor);
    lok(distance == 27);
    distance = geometricslide(distance, geo_factor);
    lok(distance == 9);
    distance = geometricslide(distance, geo_factor);
    lok(distance == 3);
    distance = geometricslide(distance, geo_factor);
    lok(distance == 1);
    distance = geometricslide(distance, geo_factor);
    lok(distance == 1);
    

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
