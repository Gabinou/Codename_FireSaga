#ifndef LINALG_HPP
#define LINALG_HPP

#include "shared.hpp"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

extern void plot2Darray(int array[][10], int imax, int jmax);
extern std::vector<std::vector<int>> list2matrix(std::vector<std::vector<int>> list);

template <typename T> extern void plot2Dvector(std::vector<std::vector<T>> matrix) {
    for (short unsigned int i = 0; i < matrix.size(); i++) {
        for (short unsigned int j = 0; j < matrix[i].size(); j++) {
            printf("%d ", matrix[i][j]);
        }

        printf("\n");
    }
}

template <typename T> extern std::vector<T> unique2D(std::vector<std::vector<T>> matrix) {
    std::vector<T> uniques;
    bool found;

    for (short unsigned int i = 0; i < matrix.size(); i++) {
        for (short unsigned int j = 0; j < matrix[i].size(); j++) {
            found = false;

            for (short unsigned int k = 0; k < uniques.size(); k++) {
                if (uniques[k] == matrix[i][j]) {
                    found = true;
                }
            }

            if (!found) {
                uniques.push_back(matrix[i][j]);
            }
        }
    }

    return (uniques);
}

template <typename T> extern std::vector<std::vector<T>> matrix_plus(std::vector<std::vector<T>> matrix1, std::vector<std::vector<T>> matrix2, short int sign = 1) {
    //both matrices should have the same shape
    std::vector<std::vector<T>> out = matrix1;

    for (short unsigned int i = 0; i < out.size(); i++) {
        for (short unsigned int j = 0; j < out[i].size(); j++) {
            out[i][j] += sign * matrix2[i][j];
        }
    }

    return (out);
}

template <typename T> extern std::vector<std::vector<T>> matrix_or(std::vector<std::vector<T>> matrix1, std::vector<std::vector<T>> matrix2) {
    //both matrices should have the same shape
    std::vector<std::vector<T>> out = matrix1;

    for (short unsigned int i = 0; i < out.size(); i++) {
        for (short unsigned int j = 0; j < out[i].size(); j++) {
            out[i][j] = out[i][j] || matrix2[i][j];
        }
    }

    return (out);
}

template <typename T> extern std::vector<std::vector<T>> matrix_and(std::vector<std::vector<T>> matrix1, std::vector<std::vector<T>> matrix2) {
    //both matrices should have the same shape
    std::vector<std::vector<T>> out = matrix1;

    for (short unsigned int i = 0; i < out.size(); i++) {
        for (short unsigned int j = 0; j < out[i].size(); j++) {
            out[i][j] = out[i][j] && matrix2[i][j];
        }
    }

    return (out);
}

template <typename T> extern std::vector<std::vector<T>> matrix_edges(std::vector<std::vector<T>> matrix) {
    std::vector<std::vector<T>> edges = matrix;

    for (short unsigned int i = 0; i < edges.size(); i++) {
        std::fill(edges[i].begin(), edges[i].end(), 0);
    }

    bool edge;

    for (short unsigned int i = 0; i < edges.size(); i++) {
        for (short unsigned int j = 0; j < edges[i].size(); j++) {
            edge = false;

            if (i != 0) {
                edge = (abs(matrix[i - 1][j] - matrix[i][j]) > 0);
            }

            if (j != 0) {
                edge = (abs(matrix[i][j - 1] - matrix[i][j]) > 0);
            }

            if (i != (edges.size() - 1)) {
                edge = (abs(matrix[i + 1][j] - matrix[i][j]) > 0);
            }

            if (j != (edges.size() - 1)) {
                edge = (abs(matrix[i][j + 1] - matrix[i][j]) > 0);
            }

            if (edge) {
                edges[i][j] = 1;
            }
        }
    }

    return (edges);
}

#endif /* LINALG_HPP */