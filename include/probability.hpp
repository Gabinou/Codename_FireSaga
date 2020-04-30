#ifndef PROBABILITY_HPP
#define PROBABILITY_HPP

#include <cmath>
#include "tinymt32.h"

#define TWOPOWER32 0x100000000
#define RN_MAX 100
#define RN_MIN 0

extern tinymt32_t tinyMT;

extern unsigned char getURN(tinymt32_t & tinymt = tinyMT);
extern unsigned char * boxmuller(const unsigned char RN_U[2], const float avg = 50., const float std_dev = 20.);

extern bool single_roll(const unsigned char RN, const unsigned char hit);
extern bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit);

extern unsigned int Uuint32_openBSD(tinymt32_t & tinymt = tinyMT, unsigned int max = RN_MAX, unsigned int min = RN_MIN);
extern void init_tinyMT(tinymt32_t & tinymt = tinyMT);

// REFERENCES
// [1] Lemire, Daniel. "Fast random integer generation in an interval." ACM Transactions on Modeling and Computer Simulation (TOMACS) 29.1 (2019): 1-12.
// [2] Matsumoto, Makoto, and Takuji Nishimura. "Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator." ACM Transactions on Modeling and Computer Simulation (TOMACS) 8.1 (1998): 3-30.

#endif /* PROBABILITY_HPP */