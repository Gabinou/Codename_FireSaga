
#include "unit/stats.h"

/* --- Unit stats --- */
struct Unit_stats Unit_stats_default = {
    .hp     = 0,
    .str    = 0,
    .mag    = 0,
    .dex    = 0,
    .agi    = 0,
    .fth    = 0,
    .luck   = 0,
    .def    = 0,
    .res    = 0,
    .move   = 0,
    .con    = 0,
    .prof   = 0
};

struct Unit_stats Unit_stats_plus_cst(struct Unit_stats stats1, i32 cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_inbounds_int32_t((stats1.hp     + cst), SOTA_MIN_HP,    SOTA_MAX_HP);
    out_stats.str   = nmath_inbounds_int32_t((stats1.str    + cst), SOTA_MIN_STR,   SOTA_MAX_STR);
    out_stats.mag   = nmath_inbounds_int32_t((stats1.mag    + cst), SOTA_MIN_MAG,   SOTA_MAX_MAG);
    out_stats.agi   = nmath_inbounds_int32_t((stats1.agi    + cst), SOTA_MIN_AGI,   SOTA_MAX_AGI);
    out_stats.dex   = nmath_inbounds_int32_t((stats1.dex    + cst), SOTA_MIN_DEX,   SOTA_MAX_DEX);
    out_stats.fth   = nmath_inbounds_int32_t((stats1.fth    + cst), SOTA_MIN_FTH,   SOTA_MAX_FTH);
    out_stats.luck  = nmath_inbounds_int32_t((stats1.luck   + cst), SOTA_MIN_LUCK,  SOTA_MAX_LUCK);
    out_stats.def   = nmath_inbounds_int32_t((stats1.def    + cst), SOTA_MIN_DEF,   SOTA_MAX_DEF);
    out_stats.res   = nmath_inbounds_int32_t((stats1.res    + cst), SOTA_MIN_RES,   SOTA_MAX_RES);
    out_stats.con   = nmath_inbounds_int32_t((stats1.con    + cst), SOTA_MIN_CON,   SOTA_MAX_CON);
    out_stats.move  = nmath_inbounds_int32_t((stats1.move   + cst), SOTA_MIN_MOVE,  SOTA_MAX_MOVE);
    out_stats.prof  = nmath_inbounds_int32_t((stats1.prof   + cst), SOTA_MIN_PROF,  SOTA_MAX_PROF);
    return (out_stats);
}

struct Unit_stats Unit_stats_plus(struct Unit_stats stats1, struct Unit_stats stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_inbounds_int32_t((stats1.hp     + stats2.hp),    SOTA_MIN_HP, SOTA_MAX_HP);
    out_stats.str   = nmath_inbounds_int32_t((stats1.str    + stats2.str),   SOTA_MIN_STR,
                                             SOTA_MAX_STR);
    out_stats.mag   = nmath_inbounds_int32_t((stats1.mag    + stats2.mag),   SOTA_MIN_MAG,
                                             SOTA_MAX_MAG);
    out_stats.agi   = nmath_inbounds_int32_t((stats1.agi    + stats2.agi),   SOTA_MIN_AGI,
                                             SOTA_MAX_AGI);
    out_stats.dex   = nmath_inbounds_int32_t((stats1.dex    + stats2.dex),   SOTA_MIN_DEX,
                                             SOTA_MAX_DEX);
    out_stats.fth   = nmath_inbounds_int32_t((stats1.fth    + stats2.fth),   SOTA_MIN_FTH,
                                             SOTA_MAX_FTH);
    out_stats.luck  = nmath_inbounds_int32_t((stats1.luck   + stats2.luck),  SOTA_MIN_LUCK,
                                             SOTA_MAX_LUCK);
    out_stats.def   = nmath_inbounds_int32_t((stats1.def    + stats2.def),   SOTA_MIN_DEF,
                                             SOTA_MAX_DEF);
    out_stats.res   = nmath_inbounds_int32_t((stats1.res    + stats2.res),   SOTA_MIN_RES,
                                             SOTA_MAX_RES);
    out_stats.con   = nmath_inbounds_int32_t((stats1.con    + stats2.con),   SOTA_MIN_CON,
                                             SOTA_MAX_CON);
    out_stats.move  = nmath_inbounds_int32_t((stats1.move   + stats2.move),  SOTA_MIN_MOVE,
                                             SOTA_MAX_MOVE);
    out_stats.prof  = nmath_inbounds_int32_t((stats1.prof   + stats2.prof),  SOTA_MIN_PROF,
                                             SOTA_MAX_PROF);
    return (out_stats);
}

struct Computed_Stats Computed_Stats_plus(struct Computed_Stats stats1,
                                          struct Computed_Stats stats2) {
    struct Computed_Stats out_stats = Computed_Stats_default;

    out_stats.attack[0] = nmath_inbounds_int32_t((stats1.attack[0] + stats2.attack[0]),
                                                    SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    out_stats.attack[1] = nmath_inbounds_int32_t((stats1.attack[1] + stats2.attack[1]),
                                                    SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    out_stats.attack[2] = nmath_inbounds_int32_t((stats1.attack[2] + stats2.attack[2]),
                                                    SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    out_stats.protection[0] = nmath_inbounds_int32_t((stats1.protection[0] + stats2.protection[0]),
                                                    SOTA_MIN_PROTECTION, SOTA_MAX_PROTECTION);
    out_stats.protection[1] = nmath_inbounds_int32_t((stats1.protection[1] + stats2.protection[1]),
                                                    SOTA_MIN_PROTECTION, SOTA_MAX_PROTECTION);
    out_stats.hit  = nmath_inbounds_int32_t((stats1.hit + stats2.hit),  SOTA_MIN_HIT, SOTA_MAX_HIT);
    out_stats.dodge  = nmath_inbounds_int32_t((stats1.dodge + stats2.dodge),  SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    out_stats.crit  = nmath_inbounds_int32_t((stats1.crit + stats2.crit),  SOTA_MIN_CRIT, SOTA_MAX_CRIT);
    out_stats.favor  = nmath_inbounds_int32_t((stats1.favor + stats2.favor),  SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    out_stats.move  = nmath_inbounds_int32_t((stats1.move + stats2.move),  SOTA_MIN_MOVE, SOTA_MAX_MOVE);
    out_stats.speed  = nmath_inbounds_int32_t((stats1.speed + stats2.speed),  SOTA_MIN_SPEED, SOTA_MAX_SPEED);
    out_stats.agony  = nmath_inbounds_int32_t((stats1.agony + stats2.agony),  SOTA_MIN_AGONY, SOTA_MAX_AGONY);
    out_stats.hit  = nmath_inbounds_int32_t((stats1.hit + stats2.hit),  SOTA_MIN_HIT, SOTA_MAX_HIT);
    out_stats.hit  = nmath_inbounds_int32_t((stats1.hit + stats2.hit),  SOTA_MIN_HIT, SOTA_MAX_HIT);

    // i32 dodge; /* can be negative */
    // i32 crit;
    // i32 favor;
    // i32 move;
    // i32 speed; /* relative to agi so +/- */
    // i32 agony;
    // struct Range range_combined;    /* Range of all equipment */
    // struct Range range_loadout;     /* Range of equipped weapons */
    return (out_stats);
}
