#ifndef PROBABILITY_H
#define PROBABILITY_H

#undef M_PI
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tinymt32.h"
#include "enums.h"
#include "SDL2/SDL.h"

#define TWOPOWER32 0x100000000
#define RN_MAX 100
#define RN_MIN 0
#define RN_TABLE_SIZE 100

extern struct TINYMT32_T tinyMT_global;

extern uint8_t RN_table[RN_TABLE_SIZE];
extern uint8_t RN_tableind;

extern uint8_t URN_global();
extern uint8_t URN_local(struct TINYMT32_T * tinymt);
// default avg = 50. and default std_dev is 20.
extern uint8_t * boxmuller(const uint8_t RN_U[2], const float avg, const float std_dev);

extern bool single_roll(const uint8_t RN, const uint8_t hit);
extern bool double_roll(const uint8_t RN1, const uint8_t RN2, const uint8_t hit);

extern uint16_t Uuint32_openBSD(struct TINYMT32_T * tinymt, uint32_t max, uint32_t min);

extern void init_tinyMT(struct TINYMT32_T * tinymt);

extern bool checkRate(struct TINYMT32_T * tinymt, int16_t rate, int16_t mode);


// REFERENCES
// [1] Lemire, Daniel. "Fast random integer generation in an interval." ACM Transactions on Modeling and Computer Simulation (TOMACS) 29.1 (2019): 1-12.
// [2] Matsumoto, Makoto, and Takuji Nishimura. "Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator." ACM Transactions on Modeling and Computer Simulation (TOMACS) 8.1 (1998): 3-30.

#endif /* PROBABILITY_H */