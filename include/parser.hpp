#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "structs.hpp"
#include "enums.hpp"

extern char * slice_char(const char * in, int start, int end);

extern short unsigned int count_words(char line[DEFAULT::LINE_LENGTH], const char * counted = ",");

template <typename T> std::vector<T> extern parse_line_vec(char line[DEFAULT::LINE_LENGTH], const char * until = ",") {
    std::vector<T> out;
    int start = 0;
    char * end;
    char * current;
    end = strchr(line, *until);

    while (end != NULL) {
        current = slice_char(line, start, (int)(end - line));
        start = end + 1 - line;
        end = strchr(end + 1, *until);
        out.push_back(atoi(current));
    }

    current = slice_char(line, start, (int)(end));
    out.push_back(atoi(current));
    return (out);
}

template <typename T> extern std::vector<std::vector<T>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",") {
    FILE * fp;
    char line_c[DEFAULT::LINE_LENGTH];
    long int current_line = 0;
    static short unsigned int line_length;
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
        line_length = count_words(line_c);
        col = parse_line_vec<T>(line_c);
        matrix.push_back(col);
    }

    return (matrix);
}

#endif /* PARSER_HPP */