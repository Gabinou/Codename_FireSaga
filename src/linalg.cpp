#include "linalg.hpp"
#include <stdlib.h>

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

std::vector<int> unique2D(std::vector<std::vector<int>> matrix) {
    std::vector<int> uniques;
    bool found;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            found = false;
            for (int k = 0; k < uniques.size(); k++) {
                if (uniques[k] == matrix[i][j]) {
                    found = true;
                }
            }
            if (!found) {
                uniques.push_back(matrix[i][j]);
            }
        }
    }
    return(uniques);
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

std::vector<std::vector<int>> matrix_or(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2) {
    //both matrices should have the same shape
    std::vector<std::vector<int>> out = matrix1;
    for (int i = 0; i < out.size(); i++){
        for (int j = 0; j < out[i].size(); j++){
            out[i][j] = out[i][j] || matrix2[i][j];
        }
    }
    return(out);
}

std::vector<std::vector<int>> matrix_edges(std::vector<std::vector<int>> matrix) {
    //both matrices should have the same shape
    std::vector<std::vector<int>> out = matrix;
    bool edge;
    for (int i = 0; i < out.size(); i++){
        for (int j = 0; j < out[i].size(); j++){
            edge = false;
            if (i != 0) {
                edge = (abs(matrix[i-1][j] - matrix[i][j]) > 0);
            }
            if (j != 0) {
                edge = (abs(matrix[i][j-1] - matrix[i][j]) > 0);
            }
            if (i != (out.size() - 1)) {
                edge = (abs(matrix[i+1][j] - matrix[i][j]) > 0);
            }
            if (j != (out.size() - 1)) {
                edge = (abs(matrix[i][j+1] - matrix[i][j]) > 0);
            }
            if (edge) {
               out[i][j] = 1;
            }
        }
    }
    return(out);
}