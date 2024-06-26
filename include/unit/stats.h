#ifndef UNIT_STATS_H
#define UNIT_STATS_H

#include "types.h"
#include "enums.h"
#include "unit/range.h"
#include "structs.h"


/* --- Unit stats --- */
struct Unit_stats Unit_stats_plus(     struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_plus_cst( struct Unit_stats stats1, i32 cst);
struct Computed_Stats Computed_Stats_plus(struct Computed_Stats s1, struct Computed_Stats s2);

#endif /* UNIT_STATS_H */
