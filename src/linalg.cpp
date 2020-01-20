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

std::vector<std::vector<int>> tilesatdistance(int start[], int distance, int xmax, int ymax) {
    // assumes the minimum is 0.
    std::vector<std::vector<int>> tiles_list;
    std::vector<int> temp_point = {start[0] + distance - 1, start[1] + 1};
    int xtemp, ytemp;
    tiles_list.push_back(temp_point);
    // printf("start %d %d \n", start[0], start[1]);    
    // printf("out %d %d \n", tiles_list.back()[0], tiles_list.back()[1]);    
    int factor[2] = {-1, 1};
    while ((tiles_list.back()[0] != (start[0] + distance)) || ((tiles_list.back()[1] != (start[1])))) {
        // printf("in %d %d \n", tiles_list.back()[0], tiles_list.back()[1]);
        xtemp = std::min(std::max(tiles_list.back()[0] + factor[0], 0), xmax);
        ytemp = std::min(std::max(tiles_list.back()[1] + factor[1], 0), ymax);
        temp_point = {xtemp, ytemp};

        tiles_list.push_back(temp_point);
        if (tiles_list.back()[0] == (start[0] + distance)){
        factor[0] = -1;
        }         
        if (tiles_list.back()[0] == (start[0] - distance)){
        factor[0] = 1;
        }         
        if (tiles_list.back()[1] == (start[1] + distance)){
        factor[1] = -1;
        }         
        if (tiles_list.back()[1] == (start[1] - distance)){
        factor[1] = 1;
        }
    }
    printf("out %d %d \n", tiles_list.back()[0], tiles_list.back()[1]);

    // std::vector<int> x;
    // int y;
    // int index[2] = {-1, 1};
    // // printf("%d %d \n", start[0], start[1]);
    // // printf("%d \n", distance);
    // for (int dist = 0; dist <= distance; dist++) {
    //     x.push_back(start[0] - dist);
    //     printf("%d \n", x[x.size()-1]);
    //     x.push_back(start[0] + dist);
    //     printf("%d \n", x[x.size()-1]);
    // }
    // getchar();
    // for (int i = 0; i < x.size(); i++) {
    //     // printf("%d \n", x[i]);
    //     temp_point[0] = x[i];
    //     temp_point[1] = start[1] + distance - std::abs(x[i]-start[0]);
    //     tiles_list.push_back(temp_point);
    //     printf("%d %d \n", temp_point[0], temp_point[1]);
    //     temp_point[1] = start[1] - distance - std::abs(x[i]-start[0]);
    //     tiles_list.push_back(temp_point);
    //     printf("%d %d \n", temp_point[0], temp_point[1]);
    //     // printf("%d \n", start[i] + distance - std::abs(x[i]-start[0]));
    //     // temp_point[1] = start[i] - distance - std::abs((x[i]-start[0]));
    //     // tiles_list.push_back(temp_point);
    //     // printf("%d %d \n", temp_point[0], temp_point[1]);
        
    // }
    // for (int dist = 0; dist < distance; dist++) {
    //     for (int i = 0; i <= 1; i++) {
    //         for (int j = 0; j <= 1; j++) {
    //             if ((dist == 0) && (i == 1)) {
    //                 temp_point[0] = start[0] + index[j]*(distance - dist);
    //                 temp_point[1] = start[1] + index[i]*dist;
    //             } else {
    //                 temp_point[0] = start[0] + index[i]*dist;
    //                 temp_point[1] = start[1] + index[j]*(distance - dist);
    //             }
    //             tiles_list.push_back(temp_point);
    //             printf("%d %d \n", temp_point[0], temp_point[1]);
    //         }
    //     }
    // }
    return(tiles_list);
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
    std::vector<std::vector<int>> edges = matrix;

    for (int i = 0; i < edges.size(); i++){
        std::fill(edges[i].begin(), edges[i].end(), 0);
    }

    bool edge;
    for (int i = 0; i < edges.size(); i++){
        for (int j = 0; j < edges[i].size(); j++){
            edge = false;
            if (i != 0) {
                edge = (abs(matrix[i-1][j] - matrix[i][j]) > 0);
            }
            if (j != 0) {
                edge = (abs(matrix[i][j-1] - matrix[i][j]) > 0);
            }
            if (i != (edges.size() - 1)) {
                edge = (abs(matrix[i+1][j] - matrix[i][j]) > 0);
            }
            if (j != (edges.size() - 1)) {
                edge = (abs(matrix[i][j+1] - matrix[i][j]) > 0);
            }
            if (edge) {
               edges[i][j] = 1;
            }
        }
    }
    return(edges);
}