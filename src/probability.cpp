
#include "probability.hpp"

int seed = 1990; 
std::mt19937_64 mt_64(1990);
std::mt19937 mt(1990); // negligible perfomance difference
std::uniform_int_distribution<int> Uint_99(0, 99); // more twice faster than Ureal_1

/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
std::uniform_int_distribution<int> dir_99(0, 99); // more twice faster than Ureal_1

bool single_roll(const int RN, const int hit) {
    return((RN<hit));
}

bool double_roll(const int RN1, const int RN2, const int hit) {
    return((((RN1+RN2)/2)<hit));
}

int getRN(){
    return(Uint_99(mt_64));
}

int boxmuller_switch(int RN_U[2], bool sin_switch = true, float avg = 50., float std_dev = 20.) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    int RN_G;
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
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    if (sin_switch) {
        RN_G = (int) ((term1 * sin(term2))*std_dev + avg);
    } else {
        RN_G = (int) ((term1 * cos(term2))*std_dev + avg);
    }
    return(RN_G);
}

int * boxmuller(int RN_U[2], float avg, float std_dev) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    static int RN_G[2];
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
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (int) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (int) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}

int * getGRNs(float avg, float std_dev) {
    // get Gaussian Random Numbers
    // RNs frm 0 to 100. Uses box-muller tranform and getRN function to compute it.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    static int RN_G[2];
    float RNreal_U[2];
    int RN_U[2];
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
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (int) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (int) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}