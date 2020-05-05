#include "minctest.h"
#include "utilities.hpp"

void test_utilities() {
    int current;
    int upper1;
    int upper2;
    int lower;
    int out;
    int geo_factor;
    int distance;

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


    lok(fequal("test_results.txt", "test_results.txt"));


    int arrtest1[5] = {3, 4, 5, 1, 2};
    quicksort(arrtest1, 0, 4);
    lok(arrtest1[0] == 1);
    lok(arrtest1[1] == 2);
    lok(arrtest1[2] == 3);
    lok(arrtest1[3] == 4);
    lok(arrtest1[4] == 5);

    short unsigned int arrtest2[5] = {3, 4, 5, 1, 2};
    short unsigned int high = 4;
    short unsigned int low = 0;
    quicksort(arrtest2, low, high);
    lok(arrtest2[0] == 1);
    lok(arrtest2[1] == 2);
    lok(arrtest2[2] == 3);
    lok(arrtest2[3] == 4);
    lok(arrtest2[4] == 5);

    std::vector<int> vectest1 = {3, 4, 5, 1, 2};
    cppquicksort(vectest1, 0, 4);
    lok(vectest1[0] == 1);
    lok(vectest1[1] == 2);
    lok(vectest1[2] == 3);
    lok(vectest1[3] == 4);
    lok(vectest1[4] == 5);

    // std::vector<short unsigned int> vectest2 = {3, 4, 5, 1, 2};
    // cppquicksort(vectest2, 0, 4);
    // lok(vectest2[0] == 1);
    // lok(vectest2[1] == 2);
    // lok(vectest2[2] == 3);
    // lok(vectest2[3] == 4);
    // lok(vectest2[4] == 5);
}
