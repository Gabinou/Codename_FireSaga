#ifndef PROBABILITY_HPP
#define PROBABILITY_HPP

#include <cmath>
#include "tinymt32.h"

#define TWOPOWER32 0x100000000
#define RN_MAX 100
#define RN_MIN 0

extern tinymt32_t tinyMT;

extern uint8_t getURN(tinymt32_t & tinymt = tinyMT);
extern uint8_t * boxmuller(const uint8_t RN_U[2], const float avg = 50., const float std_dev = 20.);

extern bool single_roll(const uint8_t RN, const uint8_t hit);
extern bool double_roll(const uint8_t RN1, const uint8_t RN2, const uint8_t hit);

extern uint32_t Uuint32_openBSD(tinymt32_t & tinymt = tinyMT, uint32_t max = RN_MAX, uint32_t min = RN_MIN);
extern void init_tinyMT(tinymt32_t & tinymt = tinyMT);

// REFERENCES
// [1] Lemire, Daniel. "Fast random integer generation in an interval." ACM Transactions on Modeling and Computer Simulation (TOMACS) 29.1 (2019): 1-12.
// [2] Matsumoto, Makoto, and Takuji Nishimura. "Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator." ACM Transactions on Modeling and Computer Simulation (TOMACS) 8.1 (1998): 3-30.

#endif /* PROBABILITY_HPP */