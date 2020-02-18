#include "linalg.hpp"
#include <stdlib.h>
#include <cmath>

void plot2Darray(int array[][10], int imax, int jmax) {
    //Don't know how to pass array that at least do not have 1D known before hand
    for (int i = 0; i < imax; i++){
        for (int j = 0; j < jmax; j++){
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}
