#ifndef UNIT_STATS_H
#define UNIT_STATS_H

#include "types.h"
#include "enums.h"
#include "unit/range.h"
#include "structs.h"

/* --- Unit stats --- */
Unit_stats Unit_stats_plus(     Unit_stats stats1, Unit_stats stats2);
Unit_stats Unit_stats_plus_cst( Unit_stats stats1, i32 cst);

i32 *Unit_stats_arr(Unit_stats *stats1);
/* --- Computed stats --- */
Computed_Stats Computed_Stats_plus(Computed_Stats s1, Computed_Stats s2);

#endif /* UNIT_STATS_H */
