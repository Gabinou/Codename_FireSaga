#ifndef PROBABILITY_HPP
#define PROBABILITY_HPP

#include <stdio.h>
#include <zlib.h>
#include <string>
#include <vector>
#include <memory>
#include <bits/stdc++.h>
#include <algorithm>

extern std::mt19937 mt;
extern std::mt19937_64 mt_64;

extern int getRN();
extern int * getGRNs(float avg = 50., float std_dev = 20.);
extern int * boxmuller(int RN_U[2], float avg = 50., float std_dev = 20.);

extern bool single_roll(const int RN, const int hit);
extern bool double_roll(const int RN1, const int RN2, const int hit);


#endif /* PROBABILITY_HPP */