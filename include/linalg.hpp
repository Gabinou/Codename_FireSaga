#ifndef LINALG_HPP
#define LINALG_HPP

#include <vector>
#include <stdio.h>

extern void plot2Darray(int array[][10], int imax, int jmax);
extern void plot2Dvector(std::vector<std::vector<int>> matrix);
extern std::vector<int> unique2D(std::vector<std::vector<int>> matrix);
extern std::vector<std::vector<int>> matrix_plus(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2, int sign = 1);


#endif /* LINALG_HPP */