#include "RNG.h"

RNG_URN_ptr global_RNG_URN = RNG_URN;
u8 URN_debug = 1;

struct TINYMT32_T *tinyMT_global = NULL;

// TODO: Different tables for difficulties?
/* -- Sequence Breaker tables -- */
/* Effective rate is base rate multiplied by this factor */
float sb_rise_table[RNG_SB_BASE_NUM] = {
    /*    0-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-89, 90-99  Base Rates */
    1.20f, 1.20f, 1.15f, 1.20f, 1.10f, 1.10f, 1.10f, 1.08f, 1.05f, 1.05f /* Factor */
};

float sb_drop_table[RNG_SB_BASE_NUM] = {
    /*    0-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-89, 90-99  Base Rates */
    1.02f, 1.02f, 1.05f, 1.05f, 1.10f, 1.10f, 1.05f, 1.02f, 1.01f, 1.00f /* Factor */
};

/* --- xoroshiro128** --- */

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is xoroshiro128** 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static uint64_t s[2];

uint64_t next(void) {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = rotl(s0 * 5, 7) * 9;

    s1 ^= s0;
    s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = rotl(s1, 37); // c

    return result;
}

/* --- RNG --- */
void RNG_Init_xoroshiro128ss(struct TINYMT32_T *tinymt) {
    SDL_assert(tinymt);
    tinymt->mat1 = 1990UL;
    tinymt->mat2 = 5UL;
    tinymt->tmat = 8UL;
    tinymt32_init(tinymt, 777UL);
}

void RNG_Init_tinymt(struct TINYMT32_T *tinymt) {
    SDL_assert(tinymt);
    tinymt->mat1 = 1990UL;
    tinymt->mat2 = 5UL;
    tinymt->tmat = 8UL;
    tinymt32_init(tinymt, 777UL);
}

u8 RNG_URN_debug(struct TINYMT32_T *tinymt) {
    return (URN_debug);
}

u8 RNG_URN(struct TINYMT32_T *tinymt) {
    return ((u8)RNG_openBSD_uint32_t(tinymt, RN_MIN, RN_MAX));
}

bool RNG_single_roll(u8 RN, u8 rate) {
    return (RN < rate);
}

bool RNG_double_roll(u8 RN1, u8 RN2, u8 rate) {
    return (((RN1 + RN2) / 2) < rate);
}

u8 *RNG_boxmuller(const u8 RN_U[INTERVAL_BOUNDS_NUM], float avg, float std_dev) {
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

u32 RNG_openBSD_uint32_t(struct TINYMT32_T *tinymt, u32 min, u32 max) {
    // "Scales" uniform integer from [0 and 2**32 - 1] to [min, max[
    // Unbiased according to: Fast Random Integer Generation in an Interval
    u32 s = max - min;
    u32 t = (UINT32_T_MAX - s + 1) % s;
    u32 x;
    do {
        x = tinymt32_generate_uint32(tinymt);
        // Rejects the last pigeonhole
        // Ex: 32 with max=5 -> rejects 30,31,32
    } while (x < t);
    u32 out = min + (x % s);
    return (out);
}

bool RNG_checkRate(struct TINYMT32_T *in_tinymt, i16 rate, i16 mode) {
    // Uses tinyMT_global if (in_tinymt==NULL)
    bool hit = false;
    u8 RN1, RN2;
    switch (mode) {
        case GAME_RN_SINGLE:
            RN1 = global_RNG_URN(in_tinymt);
            hit = RNG_single_roll(RN1, rate);
            break;
        case GAME_RN_DOUBLE:
        default:
            RN1 = global_RNG_URN(in_tinymt);
            RN2 = global_RNG_URN(in_tinymt);
            hit = RNG_double_roll(RN1, RN2, rate);
            break;
    }
    return (hit);
}

/* --- RNG SEQUENCE BREAKER (SB) --- */
void RNG_checkSequence_twoWay(struct RNG_Sequence *sequence, bool draw) {
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

void RNG_checkSequence_oneWay(struct RNG_Sequence *sequence, bool draw) {
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


