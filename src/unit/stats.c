
#include "unit/stats.h"

/* --- Unit stats --- */

struct Unit_stats Unit_stats_plus_cst(struct Unit_stats in_stats1, int cst) {

    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(in_stats1.hp,   cst, UINT8_MAX);
    out_stats.str   = nmath_bplus(in_stats1.str,  cst, UINT8_MAX);
    out_stats.mag   = nmath_bplus(in_stats1.mag,  cst, UINT8_MAX);
    out_stats.agi   = nmath_bplus(in_stats1.agi,  cst, UINT8_MAX);
    out_stats.dex   = nmath_bplus(in_stats1.dex,  cst, UINT8_MAX);
    out_stats.luck  = nmath_bplus(in_stats1.luck, cst, UINT8_MAX);
    out_stats.def   = nmath_bplus(in_stats1.def,  cst, UINT8_MAX);
    out_stats.res   = nmath_bplus(in_stats1.res,  cst, UINT8_MAX);
    out_stats.con   = nmath_bplus(in_stats1.con,  cst, UINT8_MAX);
    out_stats.move  = nmath_bplus(in_stats1.move, cst, UINT8_MAX);
    out_stats.prof  = nmath_bplus(in_stats1.prof, cst, UINT8_MAX);
    return (out_stats);
}

struct Unit_stats Unit_stats_plus(struct Unit_stats in_stats1,
                                  struct Unit_stats in_stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(in_stats1.hp,    in_stats2.hp,   UINT8_MAX);
    out_stats.str   = nmath_bplus(in_stats1.str,   in_stats2.str,  UINT8_MAX);
    out_stats.mag   = nmath_bplus(in_stats1.mag,   in_stats2.mag,  UINT8_MAX);
    out_stats.agi   = nmath_bplus(in_stats1.agi,   in_stats2.agi,  UINT8_MAX);
    out_stats.dex   = nmath_bplus(in_stats1.dex,   in_stats2.dex,  UINT8_MAX);
    out_stats.luck  = nmath_bplus(in_stats1.luck,  in_stats2.luck, UINT8_MAX);
    out_stats.def   = nmath_bplus(in_stats1.def,   in_stats2.def,  UINT8_MAX);
    out_stats.res   = nmath_bplus(in_stats1.res,   in_stats2.res,  UINT8_MAX);
    out_stats.con   = nmath_bplus(in_stats1.con,   in_stats2.con,  UINT8_MAX);
    out_stats.move  = nmath_bplus(in_stats1.move,  in_stats2.move, UINT8_MAX);
    out_stats.prof  = nmath_bplus(in_stats1.prof,  in_stats2.prof, UINT8_MAX);
    return (out_stats);
}

struct Unit_stats Unit_stats_minus_cst(struct Unit_stats in_stats1, int cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(in_stats1.hp,   cst, 0);
    out_stats.str   = nmath_bminus(in_stats1.str,  cst, 0);
    out_stats.mag   = nmath_bminus(in_stats1.mag,  cst, 0);
    out_stats.agi   = nmath_bminus(in_stats1.agi,  cst, 0);
    out_stats.dex   = nmath_bminus(in_stats1.dex,  cst, 0);
    out_stats.luck  = nmath_bminus(in_stats1.luck, cst, 0);
    out_stats.def   = nmath_bminus(in_stats1.def,  cst, 0);
    out_stats.res   = nmath_bminus(in_stats1.res,  cst, 0);
    out_stats.con   = nmath_bminus(in_stats1.con,  cst, 0);
    out_stats.move  = nmath_bminus(in_stats1.move, cst, 0);
    out_stats.prof  = nmath_bminus(in_stats1.prof, cst, 0);
    return (out_stats);
}

struct Unit_stats Unit_stats_div_cst(struct Unit_stats in_stats1, int cst) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = in_stats1.hp    / cst;
    out_stats.str   = in_stats1.str   / cst;
    out_stats.mag   = in_stats1.mag   / cst;
    out_stats.agi   = in_stats1.agi   / cst;
    out_stats.dex   = in_stats1.dex   / cst;
    out_stats.luck  = in_stats1.luck  / cst;
    out_stats.def   = in_stats1.def   / cst;
    out_stats.res   = in_stats1.res   / cst;
    out_stats.con   = in_stats1.con   / cst;
    out_stats.move  = in_stats1.move  / cst;
    out_stats.prof  = in_stats1.prof  / cst;
    return (out_stats);
}

struct Unit_stats Unit_stats_minus(struct Unit_stats in_stats1, struct Unit_stats in_stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(in_stats1.hp,    in_stats2.hp,   0);
    out_stats.str   = nmath_bminus(in_stats1.str,   in_stats2.str,  0);
    out_stats.mag   = nmath_bminus(in_stats1.mag,   in_stats2.mag,  0);
    out_stats.agi   = nmath_bminus(in_stats1.agi,   in_stats2.agi,  0);
    out_stats.dex   = nmath_bminus(in_stats1.dex,   in_stats2.dex,  0);
    out_stats.luck  = nmath_bminus(in_stats1.luck,  in_stats2.luck, 0);
    out_stats.def   = nmath_bminus(in_stats1.def,   in_stats2.def,  0);
    out_stats.res   = nmath_bminus(in_stats1.res,   in_stats2.res,  0);
    out_stats.con   = nmath_bminus(in_stats1.con,   in_stats2.con,  0);
    out_stats.move  = nmath_bminus(in_stats1.move,  in_stats2.move, 0);
    out_stats.prof  = nmath_bminus(in_stats1.prof,  in_stats2.prof, 0);
    return (out_stats);
}