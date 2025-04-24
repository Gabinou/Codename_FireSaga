#ifndef RNG_H
#define RNG_H

#include <stdint.h>
#include <math.h>
#include "enums.h"
#include "types.h"
#include "cJSON.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif /* M_PI */

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
struct RNG_Sequence;
typedef int cJSON_bool;
cJSON *cJSON_GetObjectItem(const cJSON *const object, const char *const string);
cJSON *cJSON_CreateNumber(double num);
cJSON_bool cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
double cJSON_GetNumberValue(const cJSON *const item);
float sota_slowpow(float base, int exponent);

/* -- GLOSSARY: -- */
/* RN:      Random number                       */
/* RNG:     (Pseudo-)Random number generator    */
/* SB:      sequence breaker                    */
/* U:       Uniform(ly)                         */
/* G:       Gaussian/Normal(ly)                 */
/* avg:     average                             */
/* std_dev: standard deviation                  */

/* --- TYPEDEFS --- */
typedef u8(*RNG_URN_ptr)(void);

/* --- ANTS --- */
/* FIRST OF MANY POSSIBLE GLOBAL DECLARATIONS */
extern b32 rng_sequence_breaker_miss_growth;
extern i16 URN_debug;
enum SOTA_RN {
    RN_MAX          = 100, /* 100 excluded */
    RN_5050         =  50,  /* < for 50/50  */
    RN_MIN          =   0,
    RN_TABLE_SIZE   = 100,
    RN_SET_NUM      =   4,
};

/* --- xoshiro256** --- */
/* 2x faster than tinymt */
u64  next_xoshiro256ss(void);
void RNG_Init_xoroshiro256ss(void);

void RNG_Set_xoroshiro256ss(u64 s1, u64 s2, u64 s3, u64 s4);
void RNG_Get_xoroshiro256ss(u64 *s);

/* -- API -- */
b32 RNG_checkRate(i16 rate, i16 mode);

/* -- Internals -- */
/* - Uniform - */
u8 RNG_URN(void);
u64 RNG_openBSD_u64(u64 min, u64 max);

/* - Checkers - */
b32 RNG_single_roll(u8 RN,  u8 hit);
b32 RNG_double_roll(u8 RN1, u8 RN2, u8 hit);

/* - Gaussian - */
u8 *RNG_boxmuller( u8 RN_U[INTERVAL_BOUNDS_NUM], float avg, float std_dev);
/* defaults: avg = 50, std_dev = 20 */

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

/* -- ants -- */
#define RNG_SB_BASE_NUM         10
#define RNG_SB_GROWTH_OFFSET     0 /* hit/misses crits/nocrits to ignore */
#define RNG_SB_SEQ_OFFSET        1 /* hit/misses crits/nocrits to ignore */
#define RNG_SEQ_MAX             21 /* length until rate 1 gets close enough to 50% */

/* -- Tables -- */
extern const float sb_rise_table[RNG_SB_BASE_NUM];
extern const float sb_drop_table[RNG_SB_BASE_NUM];

/* -- Macros -- */
#define SB_GROWTH_RISE(rate, n) (u16) ( (float) rate * sota_slowpow((float)sb_rise_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_GROWTH_OFFSET ? n : RNG_SB_GROWTH_OFFSET) - RNG_SB_GROWTH_OFFSET)))
#define SB_RATE_RISE(rate, n) (u16) ( (float) rate * sota_slowpow((float)sb_rise_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_SEQ_OFFSET ? n : RNG_SB_SEQ_OFFSET) - RNG_SB_SEQ_OFFSET)))
#define SB_RATE_DROP(rate, n) (u16) ( (float) rate / sota_slowpow((float)sb_drop_table[rate/RNG_SB_BASE_NUM], (u16)((n > RNG_SB_SEQ_OFFSET ? n : RNG_SB_SEQ_OFFSET) - RNG_SB_SEQ_OFFSET)))

/* -- Functions -- */
u16 SB_Rate_Drop(u16 rate, u16 n);
void RNG_checkSequence_twoWay(struct RNG_Sequence *sequence, b32 hit);
void RNG_checkSequence_oneWay(struct RNG_Sequence *sequence, b32 hit);

/* --- I/O --- */
void RNG_readJSON( void *input, cJSON *jRNG);
void RNG_writeJSON(void *input, cJSON *jRNG);

/* REFERENCES */
/* [1] Lemire, Daniel. "Fast random integer generation in an interval." ACM Transactions on Modeling and Computer Simulation (TOMACS) 29.1 (2019): 1-12. */
/* [2] Matsumoto, Makoto, and Takuji Nishimura. "Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator." ACM Transactions on Modeling and Computer Simulation (TOMACS) 8.1 (1998): 3-30. */

#endif /* RNG_H */
