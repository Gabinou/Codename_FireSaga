
#include "unit/stats.h"

/* --- Unit stats --- */
struct Unit_stats Unit_stats_default = {
    .hp =    0,
    .str =   0,
    .mag =   0,
    .dex =   0,
    .agi =   0,
    .fth =   0,
    .luck =  0,
    .def =   0,
    .res =   0,
    .move =  0,
    .con =   0,
    .prof =  0
};

struct Unit_stats Unit_stats_plus_cst(struct Unit_stats stats1, int cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(stats1.hp,   cst, UINT8_MAX);
    out_stats.str   = nmath_bplus(stats1.str,  cst, UINT8_MAX);
    out_stats.mag   = nmath_bplus(stats1.mag,  cst, UINT8_MAX);
    out_stats.agi   = nmath_bplus(stats1.agi,  cst, UINT8_MAX);
    out_stats.dex   = nmath_bplus(stats1.dex,  cst, UINT8_MAX);
    out_stats.luck  = nmath_bplus(stats1.luck, cst, UINT8_MAX);
    out_stats.def   = nmath_bplus(stats1.def,  cst, UINT8_MAX);
    out_stats.res   = nmath_bplus(stats1.res,  cst, UINT8_MAX);
    out_stats.con   = nmath_bplus(stats1.con,  cst, UINT8_MAX);
    out_stats.move  = nmath_bplus(stats1.move, cst, UINT8_MAX);
    out_stats.prof  = nmath_bplus(stats1.prof, cst, UINT8_MAX);
    return (out_stats);
}

struct Unit_stats Unit_stats_plus(struct Unit_stats stats1,
                                  struct Unit_stats stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(stats1.hp,    stats2.hp,   UINT8_MAX);
    out_stats.str   = nmath_bplus(stats1.str,   stats2.str,  UINT8_MAX);
    out_stats.mag   = nmath_bplus(stats1.mag,   stats2.mag,  UINT8_MAX);
    out_stats.agi   = nmath_bplus(stats1.agi,   stats2.agi,  UINT8_MAX);
    out_stats.dex   = nmath_bplus(stats1.dex,   stats2.dex,  UINT8_MAX);
    out_stats.luck  = nmath_bplus(stats1.luck,  stats2.luck, UINT8_MAX);
    out_stats.def   = nmath_bplus(stats1.def,   stats2.def,  UINT8_MAX);
    out_stats.res   = nmath_bplus(stats1.res,   stats2.res,  UINT8_MAX);
    out_stats.con   = nmath_bplus(stats1.con,   stats2.con,  UINT8_MAX);
    out_stats.move  = nmath_bplus(stats1.move,  stats2.move, UINT8_MAX);
    out_stats.prof  = nmath_bplus(stats1.prof,  stats2.prof, UINT8_MAX);
    return (out_stats);
}

struct Unit_stats Unit_stats_minus_cst(struct Unit_stats stats1, int cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(stats1.hp,   cst, 0);
    out_stats.str   = nmath_bminus(stats1.str,  cst, 0);
    out_stats.mag   = nmath_bminus(stats1.mag,  cst, 0);
    out_stats.agi   = nmath_bminus(stats1.agi,  cst, 0);
    out_stats.dex   = nmath_bminus(stats1.dex,  cst, 0);
    out_stats.luck  = nmath_bminus(stats1.luck, cst, 0);
    out_stats.def   = nmath_bminus(stats1.def,  cst, 0);
    out_stats.res   = nmath_bminus(stats1.res,  cst, 0);
    out_stats.con   = nmath_bminus(stats1.con,  cst, 0);
    out_stats.move  = nmath_bminus(stats1.move, cst, 0);
    out_stats.prof  = nmath_bminus(stats1.prof, cst, 0);
    return (out_stats);
}

struct Unit_stats Unit_stats_div_cst(struct Unit_stats stats1, int cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = stats1.hp    / cst;
    out_stats.str   = stats1.str   / cst;
    out_stats.mag   = stats1.mag   / cst;
    out_stats.agi   = stats1.agi   / cst;
    out_stats.dex   = stats1.dex   / cst;
    out_stats.luck  = stats1.luck  / cst;
    out_stats.def   = stats1.def   / cst;
    out_stats.res   = stats1.res   / cst;
    out_stats.con   = stats1.con   / cst;
    out_stats.move  = stats1.move  / cst;
    out_stats.prof  = stats1.prof  / cst;
    return (out_stats);
}

struct Unit_stats Unit_stats_minus(struct Unit_stats stats1,
                                   struct Unit_stats stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(stats1.hp,    stats2.hp,   0);
    out_stats.str   = nmath_bminus(stats1.str,   stats2.str,  0);
    out_stats.mag   = nmath_bminus(stats1.mag,   stats2.mag,  0);
    out_stats.agi   = nmath_bminus(stats1.agi,   stats2.agi,  0);
    out_stats.dex   = nmath_bminus(stats1.dex,   stats2.dex,  0);
    out_stats.luck  = nmath_bminus(stats1.luck,  stats2.luck, 0);
    out_stats.def   = nmath_bminus(stats1.def,   stats2.def,  0);
    out_stats.res   = nmath_bminus(stats1.res,   stats2.res,  0);
    out_stats.con   = nmath_bminus(stats1.con,   stats2.con,  0);
    out_stats.move  = nmath_bminus(stats1.move,  stats2.move, 0);
    out_stats.prof  = nmath_bminus(stats1.prof,  stats2.prof, 0);
    return (out_stats);
}
