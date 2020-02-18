#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>

extern char * slice_char(const char * in, int start, int end);
short unsigned int count(char * line, const char * counted);
unsigned int count(char * line, const char * counted);

extern std::vector<short int> parse_line_vec(char * line, const char * until = ",");
extern std::vector<int> parse_line_vec(char * line, const char * until = ",");

extern std::vector<std::vector<int>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",");
extern std::vector<std::vector<short int>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",");


#endif /* PARSER_HPP */