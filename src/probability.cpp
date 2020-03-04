
#include "probability.hpp"

short int seed = 1990; 
std::mt19937_64 mt_64(1990);
std::mt19937 mt(1990); // negligible perfomance difference to _64
std::uniform_int_distribution<unsigned char> U_99(0, 99); // more twice faster than Ureal_1

bool single_roll(const unsigned char RN, const unsigned char hit) {
    return((RN < hit));
}

bool double_roll(const unsigned char RN1, const unsigned char RN2, const unsigned char hit) {
    return((((RN1 + RN2) / 2) < hit));
}

unsigned char getRN(){
    return(U_99(mt_64));
}

unsigned char boxmuller_switch(const unsigned char RN_U[2], const bool sin_switch = true, const float avg = 50., const float std_dev = 20.) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the unsigned char that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
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
    // prunsigned charf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    if (sin_switch) {
        RN_G = (unsigned char) ((term1 * sin(term2))*std_dev + avg);
    } else {
        RN_G = (unsigned char) ((term1 * cos(term2))*std_dev + avg);
    }
    return(RN_G);
}

unsigned char * boxmuller(const unsigned char RN_U[2], const float avg, const float std_dev) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the unsigned char that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
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
    // prunsigned charf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (unsigned char) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (unsigned char) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}

unsigned char * getGRNs(const float avg, const float std_dev) {
    // get Gaussian Random Numbers
    // RNs frm 0 to 100. Uses box-muller tranform and getRN function to compute it.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the unsigned char that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    static unsigned char RN_G[2];
    float RNreal_U[2];
    unsigned char RN_U[2];
    RN_U[0] = getRN();
    RN_U[1] = getRN();
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