#ifndef LINALG_HPP
#define LINALG_HPP

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>


// template <typename T> operator modulo(T divisor, T dividend) {
//     return ()
// }


template <typename T> extern std::vector<std::vector <T>> list2matrix(std::vector<std::vector<T>> list, T col, T row) {
    std::vector<std::vector <T>> matrix(col, std::vector<T> (row, 0));

    for (short unsigned int ind = 0; ind < list.size(); ind++) {
        matrix[list[ind][0]][list[ind][1]] = 1;
    }

    return (matrix);
}

template <typename T> extern void plot2Dvec(std::vector<std::vector<T>> vec) {
    for (short unsigned int i = 0; i < vec.size(); i++) {
        for (short unsigned int j = 0; j < vec[i].size(); j++) {
            printf("%d ", vec[i][j]);
        }

        printf("\n");
    }
}

template <typename T> extern void plot2Darray(T array[][10], T imax, T jmax) {
    //Don't know how to pass array that at least do not have 1D known before hand
    for (short unsigned int i = 0; i < imax; i++) {
        for (short unsigned int j = 0; j < jmax; j++) {
            printf("%d ", array[i][j]);
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

    std::sort(uniques.begin(), uniques.end());
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

template <typename T> extern std::vector<std::vector<T>> matrix2list(std::vector<std::vector<T>> matrix) {
    std::vector<std::vector<T>> list;

    for (T col = 0; col < matrix.size(); col++) {
        for (T row = 0; row < matrix[0].size(); row++) {
            if (matrix[col][row] > 0) {
                list.push_back({col, row});
            }
        }
    }

    return (list);
}

#endif /* LINALG_HPP */