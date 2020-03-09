#ifndef PROBABILITY_HPP
#define PROBABILITY_HPP

#include <random>
#include "tinymt32.h"

#define TWOPOWER32 0x100000000
#define SEED 19900508

extern std::mt19937 mt;
extern std::mt19937_64 mt_64;

extern unsigned char getURN();
extern unsigned char * getGRNs(const float avg = 50., const float std_dev = 20.);
extern unsigned char * boxmuller(const unsigned char RN_U[2], const float avg = 50., const float std_dev = 20.);

extern bool single_roll(const unsigned char RN, const unsigned char hit);
extern bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit);

extern unsigned int UUINT32_openBSD(unsigned int max, tinymt32_t & tinymt)

#endif /* PROBABILITY_HPP */