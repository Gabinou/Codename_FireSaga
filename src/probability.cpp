
#include "probability.hpp"

// GLOSSARY:
// RN: Random number
// U: Uniform(ly)
// G: Gaussian/Normal(ly)
// std_dev: standard deviation
// avg: average

bool single_roll(const unsigned char RN, const unsigned char hit) {
    return((RN < hit));
}

bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit) {
    return((((RN1 + RN2) / 2) < hit));
}

unsigned char getURN(tinymt32_t & tinymt){
    return(Uuint32_openBSD(tinymt, RN_MAX, RN_MIN));
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
        RNreal_U[0] = ((float)RN_U[0])/100.;
    }
    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1])/100.;
    }
    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];

    RN_G[0] = (unsigned char) ((term1 * sin(term2)) * std_dev + avg);
    RN_G[1] = (unsigned char) ((term1 * cos(term2)) * std_dev + avg);
    return(RN_G);
}

// unsigned char * getGRNs(const float avg, const float std_dev) {
//     // Get a pair of G distributed RNs.
//     // RN_G can be < 0 and > 100
//     static unsigned char * RN_G;
//     unsigned char RN_U[2];
//     RN_U[0] = getURN();
//     RN_U[1] = getURN();
//     RN_G = boxmuller(RN_U, avg, std_dev);
//     return(RN_G);
// }

unsigned int Uuint32_openBSD(tinymt32_t & tinymt, unsigned int max, unsigned int min) {
    // According to [1], it is unbiased.
    unsigned int t = -max % max;
    unsigned int x;
    do {
        x = tinymt32_generate_uint32(&tinymt);
        // Rejects the last pigeonhole.
        // Ex: 32 with max=5 -> values of 30,31,32 are rejected. 
    } while (x < t);
    return(min + (x % max));
}