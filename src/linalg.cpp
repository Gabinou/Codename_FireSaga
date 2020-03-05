
#include "linalg.hpp"

void plot2Darray(int array[][10], short unsigned int imax, short unsigned int jmax) {
    //Don't know how to pass array that at least do not have 1D known before hand
    for (short unsigned int i = 0; i < imax; i++){
        for (short unsigned int j = 0; j < jmax; j++){
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

std::vector<std::vector<int>> list2matrix(std::vector<std::vector<int>> list, int col, int row) {
    std::vector<std::vector<int>> matrix(col, std::vector<int> (row, 0));
    for (int ind = 0; ind < list.size(); ind++) {
        matrix[list[ind][0]][list[ind][1]] = 1;
    }
    return(matrix);
}


