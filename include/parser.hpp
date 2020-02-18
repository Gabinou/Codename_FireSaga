#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>

extern void read(const char * filename);
extern void readcsv(const char * filename, const int header = 0, const char * delim = ",", const int out_size = 255);
extern int count(char * line, const char * counted = ",");

template <typename T> extern std::vector<std::vector<T>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",") {
    FILE * fp;
    char line_c[255];
    long int current_line = 0;
    static int line_length;
    std::vector<T> col;
    std::vector<std::vector<T>> matrix;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file");
    }

    while (current_line < header) {
        fgets(line_c, sizeof(line_c), fp);
        current_line++;
    }

    while (!feof(fp)) {
        fgets(line_c, sizeof(line_c), fp);
        line_length = count(line_c);
        col = parse_line_vec(line_c);
        matrix.push_back(col);
    }

    return (matrix);
}



#endif /* PARSER_HPP */