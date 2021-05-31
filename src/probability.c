
#include "probability.h"

// GLOSSARY:
// RN: Random number
// U: Uniform(ly)
// G: Gaussian/Normal(ly)
// std_dev: standard deviation
// avg: average

struct TINYMT32_T tinyMT_global;

void init_tinyMT(struct TINYMT32_T * tinymt) {
    tinymt->mat1 = 1990UL;
    tinymt->mat2 = 5UL;
    tinymt->tmat = 8UL;
    tinymt32_init(tinymt, 777UL);
}

bool single_roll(const uint8_t RN, const uint8_t hit) {
    return ((RN < hit));
}

bool double_roll(const uint8_t RN1, const uint8_t RN2, const uint8_t hit) {
    return ((((RN1 + RN2) / 2) < hit));
}

uint8_t URN_global() {
    return (Uuint32_openBSD(&tinyMT_global, RN_MAX, RN_MIN));
}

uint8_t URN_local(struct TINYMT32_T * tinymt) {
    return (Uuint32_openBSD(tinymt, RN_MAX, RN_MIN));
}

uint8_t * boxmuller(const uint8_t RN_U[2], const float avg, const float std_dev) {
    // Box-Muller Transform.
    // 2 U distributed RNs -> 2 G distributed RNs
    // RN_G can be < 0 and > 100.
    static uint8_t RN_G[2];
    float RNreal_U[2];
    if (RN_U[0] == 0) {
        RNreal_U[0] = 0.00001;
    } else {
        RNreal_U[0] = ((float)RN_U[0]) / RN_MAX;
    }
    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1]) / RN_MAX;
    }
    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];
    RN_G[0] = (uint8_t)((term1 * sin(term2)) * std_dev + avg);
    RN_G[1] = (uint8_t)((term1 * cos(term2)) * std_dev + avg);
    return (RN_G);
}

uint16_t Uuint32_openBSD(struct TINYMT32_T * tinymt, uint32_t max, uint32_t min) {
    // "Scales" uniform integer from [0 and 2**32 - 1] to [min, max[
    // Unbiased according to [1]
    // [1]: Fast Random Integer Generation in an Interval
    uint32_t t = -(max - min) % (max - min);
    uint32_t x;
    do {
        x = tinymt32_generate_uint32(tinymt);
        // Rejects the last pigeonhole
        // Ex: 32 with max=5 -> rejects 30,31,32
    } while (x < t);
    uint16_t out = min + (x % max);
    return (out);
}

bool checkRate(struct TINYMT32_T * in_tinymt, int16_t rate, int16_t mode) {
    // Uses tinyMT_global if (in_tinymt==NULL)
    bool hit = false;
    uint8_t RN1;
    uint8_t RN2;

    switch (mode) {

        case GAME_RN_SINGLE:
            if (in_tinymt == NULL) {
                RN1 = URN_global();
            } else {
                RN1 = URN_local(in_tinymt);
            }
            hit = (RN1 < rate);
            break;

        case GAME_RN_DOUBLE:
            if (in_tinymt == NULL) {
                RN1 = URN_global();
                RN2 = URN_global();
            } else {
                RN1 = URN_local(in_tinymt);
                RN2 = URN_local(in_tinymt);
            }
            hit = (((RN1 + RN2) / 2) < rate);
            // printf("RNs, rate: %d %d %d\n", RN1, RN2, rate);
            break;

        // case GAME_RN_GAUSSIAN:
        // I don't think I will use GAME_RN_GAUSSIAN...
        // If I do, precomputed RN table should be implemented.
        //     uint8_t * RNs;
        //     RNs[0] = getURN();
        //     RNs[1] = getURN();
        //     RNs = boxmuller(RNs);
        //     hitcrit[0] = (RNs[0] < hit_rate);
        //     hitcrit[1] = (RNs[1] < crit_rate);
        //     break;

        default:
            SDL_Log("RN mode is invalid");
    }
    return (hit);
}
