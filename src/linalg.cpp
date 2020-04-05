
#include "linalg.hpp"

std::vector<std::vector<int>> list2matrix(std::vector<std::vector<int>> list, int col, int row) {
    std::vector<std::vector<int>> matrix(col, std::vector<int> (row, 0));
    for (int ind = 0; ind < list.size(); ind++) {
        matrix[list[ind][0]][list[ind][1]] = 1;
    }
    return(matrix);
}


