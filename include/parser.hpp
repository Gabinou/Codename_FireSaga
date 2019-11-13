#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <iostream>
#include <string>
#include <ostream>
#include <istream>
#include <ctype.h>
#include <vector>
#include <math.h>
#include <random>
#include <bits/stdc++.h>

template <typename T>
extern void print2D_vec(std::vector<std::vector<T>> in_mat) {
    for (int row = 0; row < in_mat.size(); row++) {
        for (int col = 0; col < in_mat[row].size(); col++) {
            printf("%d,", in_mat[row][col]);
            // printf("%d,%d",row, col);
        }

        printf("\n");
    }
}


extern void read(const char * filename);
extern void readcsv(const char * filename, const int header = 0, const char * delim = ",", const int out_size = 255);
extern std::vector<std::vector<int>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",", const int out_size = 255);

#endif /* PARSER_HPP */