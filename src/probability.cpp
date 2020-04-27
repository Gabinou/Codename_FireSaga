
#include "probability.hpp"

// GLOSSARY:
// RN: Random number
// U: Uniform(ly)
// G: Gaussian/Normal(ly)
// std_dev: standard deviation
// avg: average
tinymt32_t tinyMT;

void init_tinyMT(tinymt32_t & tinymt) {
    tinymt.mat1 = 1990;
    tinymt.mat2 = 5;
    tinymt.tmat = 8;
    tinymt32_init(&tinymt, 29);
}

bool single_roll(const unsigned char RN, const unsigned char hit) {
    return ((RN < hit));
}

bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit) {
    return ((((RN1 + RN2) / 2) < hit));
}

unsigned char getURN(tinymt32_t & tinymt) {
    return (Uuint32_openBSD(tinymt, RN_MAX, RN_MIN));
}

unsigned char * boxmuller(const unsigned char RN_U[2], const float avg, const float std_dev) {
    // Box-Muller Transform.
    // 2 U distributed RNs -> 2 G distributed RNs
    // RN_G can be < 0 and > 100.
    static unsigned char RN_G[2];
    float RNreal_U[2];

    if (RN_U[0] == 0) {
        RNreal_U[0] = 0.00001;
    } else {
        RNreal_U[0] = ((float)RN_U[0]) / 100.;
    }

    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1]) / 100.;
    }

    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];

    RN_G[0] = (unsigned char)((term1 * sin(term2)) * std_dev + avg);
    RN_G[1] = (unsigned char)((term1 * cos(term2)) * std_dev + avg);
    return (RN_G);
}

unsigned int Uuint32_openBSD(tinymt32_t & tinymt, unsigned int max, unsigned int min) {
    // "Scales" uniform integer from [0 and 2**32 - 1] to [min, max]
    // Unbiased according to [1]
    unsigned int t = -(max - min) % (max - min);
    unsigned int x;

    do {
        x = tinymt32_generate_uint32(&tinymt);
        // Rejects the last pigeonhole
        // Ex: 32 with max=5 -> rejects 30,31,32
    } while (x < t);

    return (min + (x % max));
}

// References:
// [1]: Fast Random Integer Generation in an Interval