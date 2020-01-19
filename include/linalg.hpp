#ifndef LINALG_HPP
#define LINALG_HPP

#include <vector>
#include <stdio.h>

extern void plot2Darray(int array[][10], int imax, int jmax);
extern void plot2Dvector(std::vector<std::vector<int>> matrix);
extern std::vector<int> unique2D(std::vector<std::vector<int>> matrix);
extern std::vector<std::vector<int>> matrix_plus(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2, int sign = 1);
extern std::vector<std::vector<int>> matrix_or(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2);
extern std::vector<std::vector<int>> matrix_and(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2);
extern std::vector<std::vector<int>> matrix_edges(std::vector<std::vector<int>> matrix);
extern std::vector<std::vector<int>> tilesatdistance(int start[], int distance);

#endif /* LINALG_HPP */