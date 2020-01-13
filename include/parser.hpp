#ifndef PARSER_HPP
#define PARSER_HPP

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>

extern void read(const char * filename);
extern void readcsv(const char * filename, const int header = 0, const char * delim = ",", const int out_size = 255);
extern std::vector<std::vector<int>> readcsv_vec(const char * filename, const int header = 0, const char * delim = ",", const int out_size = 255);

#endif /* PARSER_HPP */