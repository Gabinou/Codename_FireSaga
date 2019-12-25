// Code créé par Gabriel Taillon
#include "linalg.hpp"

void plot2Darray(int array[][10], int imax, int jmax) {
    //Don't know how to pass array that at least do not have 1D known before hand
    for (int i = 0; i < imax; i++){
        for (int j = 0; j < jmax; j++){
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

void plot2Dvector(std::vector<std::vector<int>> matrix) {
    for (int i = 0; i < matrix.size(); i++){
        for (int j = 0; j < matrix[i].size(); j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

std::vector<std::vector<int>> matrix_plus(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2, int sign) {
    //both matrices should have the same shape
    std::vector<std::vector<int>> out = matrix1;
    for (int i = 0; i < out.size(); i++){
        for (int j = 0; j < out[i].size(); j++){
            out[i][j] += sign * matrix2[i][j];
        }
    }
    return(out);
}