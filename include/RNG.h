#ifndef RNG_H
#define RNG_H

#include <stdint.h>
#include "enums.h"
#include "globals.h"
#include "debug.h"
#include "macros.h"
#include "types.h"
#include "tinymt32.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
float sota_slowpow(float base, int exponent);

/* -- GLOSSARY: -- */
/* RN:      Random number           */
/* RNG:     Random number generator */
/* SB:      sequence breaker        */
/* U:       Uniform(ly)             */
/* G:       Gaussian/Normal(ly)     */
/* avg:     average                 */
/* std_dev: standard deviation      */

/* --- TYPEDEFS --- */
typedef u8(*RNG_URN_ptr)(struct TINYMT32_T *);

/* --- CONSTANTS --- */
enum SOTA_RN {
    RN_MAX          = 100, /* 100 excluded */
    RN_5050         = 50,  /* < for 50/50  */
    RN_MIN          = 0,
    RN_TABLE_SIZE   = 100,
};


/* --- xoshiro256** --- */
/* 2x faster than tinymt */
u64  next_xoshiro256ss(void);
void RNG_Init_xoroshiro256ss(void);

extern struct TINYMT32_T *tinyMT_global;

/* -- API -- */
bool RNG_checkRate(  struct TINYMT32_T *tinymt, i16 rate, i16 mode);
void RNG_Init_tinymt(struct TINYMT32_T *tinymt);

// Pointer to RNG function used everywhere in SOTA
extern RNG_URN_ptr global_RNG_URN;

/* -- Internals -- */
/* - Uniform - */
u8  RNG_URN(             struct TINYMT32_T *tinymt);
static u32 RNG_openBSD_u32(struct TINYMT32_T *tinymt, u32 max, u32 min);
static u64 RNG_openBSD_u64(struct TINYMT32_T *tinymt, u64 max, u64 min);

/* - Checkers - */
bool RNG_single_roll(u8 RN,  u8 hit);
bool RNG_double_roll(u8 RN1, u8 RN2, u8 hit);

/* - Gaussian - */
u8 *RNG_boxmuller(const u8 RN_U[INTERVAL_BOUNDS_NUM], float avg, float std_dev);
/* defaults: avg = 50, std_dev = 20 */

/* -- Debug -- */
u8 RNG_URN_debug(struct TINYMT32_T *tinymt);
extern u8 URN_debug; /* Value returned by RNG_URN_debug */

/* --- RNG SEQUENCE BREAKER (SB) --- */
/* Sequence breaker increases or decreases probability of next draw
*   exponential increase/decrease by base^sequence_length (harmonic sucks, too long)
*   HIT/MISS: rate can only RISE. rate drop for HIT sequence feels terrible?
*   CRIT/NOCRIT: rate can only RISE. rate drop for CRIT sequence is baka not relevant
*   every 10% rate strata has a different base, higher for lower rates
*   rate is cast to int, so values are floored.
*
*   sequence breaking for crit is probably useless? -> implement anyway, easy to remove later
*   sequence breaking should be GLOBAL? maybe. if so make global sequence of HIT/CRITS
*/
struct RNG_Sequence { /* Sequence of hits/misses in a row */
    i8 len;
    i8 eff_rate;
    bool hit; /* 0 if sequence of misses, 1 of hits */
};

/* -- Constants -- */
#define RNG_SB_BASE_NUM         10
#define RNG_SB_GROWTH_OFFSET     0 /* hit/misses crits/nocrits to ignore */
#define RNG_SB_SEQ_OFFSET        1 /* hit/misses crits/nocrits to ignore */
#define RNG_SEQ_MAX             21 /* length until rate 1 gets close enough to 50% */

/* -- Tables -- */
extern float sb_rise_table[RNG_SB_BASE_NUM];
extern float sb_drop_table[RNG_SB_BASE_NUM];

/* -- Macros -- */
#define SB_GROWTH_RISE(rate, n) (u16) ( (float) rate * sota_slowpow((float)sb_rise_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_GROWTH_OFFSET ? n : RNG_SB_GROWTH_OFFSET) - RNG_SB_GROWTH_OFFSET)))
#define SB_RATE_RISE(rate, n) (u16) ( (float) rate * sota_slowpow((float)sb_rise_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_SEQ_OFFSET ? n : RNG_SB_SEQ_OFFSET) - RNG_SB_SEQ_OFFSET)))
#define SB_RATE_DROP(rate, n) (u16) ( (float) rate / sota_slowpow((float)sb_drop_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_SEQ_OFFSET ? n : RNG_SB_SEQ_OFFSET) - RNG_SB_SEQ_OFFSET)))

/* -- Functions -- */
u16 SB_Rate_Drop(u16 rate, u16 n);
void RNG_checkSequence_twoWay(struct RNG_Sequence *sequence, bool hit);
void RNG_checkSequence_oneWay(struct RNG_Sequence *sequence, bool hit);

/* REFERENCES */
/* [1] Lemire, Daniel. "Fast random integer generation in an interval." ACM Transactions on Modeling and Computer Simulation (TOMACS) 29.1 (2019): 1-12. */
/* [2] Matsumoto, Makoto, and Takuji Nishimura. "Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator." ACM Transactions on Modeling and Computer Simulation (TOMACS) 8.1 (1998): 3-30. */

#endif /* RNG_H */