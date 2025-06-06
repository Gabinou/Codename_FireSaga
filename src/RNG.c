
#include "RNG.h"
#include "structs.h"
#include "cJSON.h"

/* --- FIRST AND ONLY DEFINITION OF GLOBALS --- */
b32 rng_sequence_breaker_miss_growth = true;

i16 URN_debug = -1;

// TODO: Different tables for difficulties?
/* -- Sequence Breaker tables -- */
/* Effective rate is base rate multiplied by this factor */
const float sb_rise_table[RNG_SB_BASE_NUM] = {
    /*    0-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-89, 90-99  Base Rates */
    1.20f, 1.20f, 1.15f, 1.20f, 1.10f, 1.10f, 1.10f, 1.08f, 1.05f, 1.05f /* Factor */
};

const float sb_drop_table[RNG_SB_BASE_NUM] = {
    /*    0-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-89, 90-99  Base Rates */
    1.02f, 1.02f, 1.05f, 1.05f, 1.10f, 1.10f, 1.05f, 1.02f, 1.01f, 1.00f /* Factor */
};

/* --- xoroshiro256** --- */

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
   generators. It has excellent (sub-ns) speed, a state (256 bits) that is
   large enough for any parallel application, and it passes all tests we
   are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

static inline u64 rotl( u64 x, int k) {
    return (x << k) | (x >> (64 - k));
}

static u64 s[4];

u64 next_xoshiro256ss(void) {
    u64 result = rotl(s[1] * 5, 7) * 9;
    u64      t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
}

/* --- RNG --- */
void RNG_Init_xoroshiro256ss(void) {
    s[0] = 1990ULL;
    s[1] =    5ULL;
    s[2] =    8ULL;
    s[3] =  777ULL;
}

void RNG_Set_xoroshiro256ss(u64 s1, u64 s2, u64 s3, u64 s4) {
    s[0] = s1;
    s[1] = s2;
    s[2] = s3;
    s[3] = s4;
}

void RNG_Get_xoroshiro256ss(u64 *s_out) {
    SDL_assert(s     != NULL);
    SDL_assert(s_out != NULL);
    s_out[0] = s[0];
    s_out[1] = s[1];
    s_out[2] = s[2];
    s_out[3] = s[3];
}

u8 RNG_URN(void) {
    u8 out = (u8)RNG_openBSD_u64(RN_MIN, RN_MAX);
    return (URN_debug >= 0 ? URN_debug : out);
}

b32 RNG_single_roll(u8 RN, u8 rate) {
    return (RN < rate);
}

b32 RNG_double_roll(u8 RN1, u8 RN2, u8 rate) {
    return (((RN1 + RN2) / 2) < rate);
}

u8 *RNG_boxmuller( u8 RN_U[INTERVAL_BOUNDS_NUM], float avg, float std_dev) {
    // Box-Muller Transform.
    // 2 U distributed RNs -> 2 G distributed RNs
    // RN_G can be < 0 and > 100.
    static u8 RN_G[INTERVAL_BOUNDS_NUM];
    float RNreal_U[INTERVAL_BOUNDS_NUM];
    if (RN_U[0] == 0)
        RNreal_U[0] = 0.00001;
    else
        RNreal_U[0] = ((float)RN_U[0]) / RN_MAX;

    if (RN_U[1] == 0)
        RNreal_U[1] = 0.00001;
    else
        RNreal_U[1] = ((float)RN_U[1]) / RN_MAX;

    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];
    RN_G[0] = (u8)((term1 * sin(term2)) * std_dev + avg);
    RN_G[1] = (u8)((term1 * cos(term2)) * std_dev + avg);
    return (RN_G);
}

u64 RNG_openBSD_u64(u64 min, u64 max) {
    /* "Scales" uniform integer from [0 and 2**64 - 1] to [min, max[        */
    /* Unbiased according to: Fast Random Integer Generation in an Interval */
    u64 x;
    u64 width = max - min;
    u64 limit = (UINT64_MAX - width + 1) % width; //.+1 cause.umax is 2^n - 1
    do {
        x = next_xoshiro256ss();
    } while (x < limit);
    u64 out = min + (x % width);
    return (out);
}

b32 RNG_checkRate(i16 rate, i16 mode) {
    b32 hit = false;
    u8 RN1, RN2;
    switch (mode) {
        case SOTA_RN_SINGLE:
            RN1 = RNG_URN();
            hit = RNG_single_roll(RN1, rate);
            break;
        case SOTA_RN_DOUBLE:
        default:
            RN1 = RNG_URN();
            RN2 = RNG_URN();
            hit = RNG_double_roll(RN1, RN2, rate);
            break;
    }
    return (hit);
}
/* --- I/O --- */
void RNG_readJSON(void *input, cJSON *jRNG) {
    u64 *array = (u64 *)input;
    SDL_assert(cJSON_IsArray(jRNG));

    int i = 0;
    cJSON *jnum;
    cJSON_ArrayForEach(jnum, jRNG) {
        array[i++] = cJSON_GetNumberValue(jnum);
    }
}

void RNG_writeJSON(void *input, cJSON *jRNG) {
    u64 *array = (u64 *)input;
    SDL_assert(cJSON_IsArray(jRNG));

    for (int i = 0; i < RN_SET_NUM; i++) {
        cJSON *jnum = cJSON_CreateNumber(array[i]);
        cJSON_AddItemToArray(jRNG, jnum);
    }
}

/* --- RNG SEQUENCE BREAKER (SB) --- */
void RNG_checkSequence_twoWay(struct RNG_Sequence *sequence, b32 draw) {
    /* Two way Sequence Breaker. Saves sequence.len for both hit or miss sequences.
        When a sequence is broken, the sequence->hit value is updated.
        If the next draw value is equal to the new hit value, a new sequence begins.
    */
    if (sequence->hit == draw)
        sequence->len = sequence->len < RNG_SEQ_MAX ? sequence->len + 1 : RNG_SEQ_MAX;
    else {
        sequence->hit = draw;
        sequence->len = 1;
    }
}

void RNG_checkSequence_oneWay(struct RNG_Sequence *sequence, b32 draw) {
    /* One way Sequence Breaker. Input sequence->hit is never changed here.
        Only saves sequence->len for user sequence->hit.
    */
    if (sequence->hit == draw)
        sequence->len = sequence->len < RNG_SEQ_MAX ? sequence->len + 1 : RNG_SEQ_MAX;
    else {
        sequence->len = 0;
    }
}

u16 SB_Rate_Drop(u16 rate, u16 n) {
    float fout = rate, base = sb_drop_table[rate / RNG_SB_BASE_NUM];
    i8 exponent = (i8)(n - RNG_SB_SEQ_OFFSET);
    fout /= sota_slowpow(base, exponent);
    return ((u16)fout);
}
