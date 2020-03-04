
#include "probability.hpp"

// GLOSSARY:
// RN: Random number
// U: Uniform(ly)
// G: Gaussian/Normal(ly)

#define SEED 19900508
std::mt19937_64 mt_64(SEED);
std::mt19937 mt(SEED); // negligible perfomance difference to _64
std::uniform_int_distribution<unsigned char> U_99(0, 99);

bool single_roll(const unsigned char RN, const unsigned char hit) {
    return((RN < hit));
}

bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit) {
    return((((RN1 + RN2) / 2) < hit));
}

unsigned char getURN(){
    return(U_99(mt_64));
}

unsigned char boxmuller_switch(const unsigned char RN_U[2], const bool sin_switch = true, const float avg = 50., const float std_dev = 20.) {
    // Transforms a pair of U distributed RNs into a pair of G distributed RNs
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100.
    unsigned char RN_G;
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

    if (sin_switch) {
        RN_G = (unsigned char) ((term1 * sin(term2))*std_dev + avg);
    } else {
        RN_G = (unsigned char) ((term1 * cos(term2))*std_dev + avg);
    }
    return(RN_G);
}

unsigned char * boxmuller(const unsigned char RN_U[2], const float avg, const float std_dev) {
    // Transforms a pair of U distributed RNs into a pair of G distributed RNs
    // std_dev: standard deviation,  avg: average
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

unsigned char * getGRNs(const float avg, const float std_dev) {
    // Get a pair of G distributed RNs.
    // RNs frm 0 to 100. Uses box-muller tranform and getURN function to compute it.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100
    static unsigned char RN_G[2];
    float RNreal_U[2];
    unsigned char RN_U[2];
    RN_U[0] = getURN();
    RN_U[1] = getURN();
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
    // prunsigned charf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (unsigned char) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (unsigned char) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}