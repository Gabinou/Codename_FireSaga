#ifndef UNIT_STATS_H
#define UNIT_STATS_H

#include "types.h"
#include "enums.h"
#include "structs.h"

/* --- Unit stats --- */
Unit_stats Unit_stats_plus(     Unit_stats stats1, Unit_stats stats2);
Unit_stats Unit_stats_plus_cst( Unit_stats stats1, i32 cst);
Unit_stats Unit_stats_max(Unit_stats stats1, Unit_stats stats2);

i32 *Unit_stats_arr(Unit_stats *stats1);

struct Unit_stats Unit_Stats_Caps(Unit *u);
struct Unit_stats Unit_Stats_Bases(Unit *u);

struct Unit_stats *Unit_Stats_Grown(const Unit *u);
struct Unit_stats *Unit_Stats_Growths(Unit *u);
void Unit_Stats_Growths_Set(Unit *u, Unit_stats growths);

/* --- Computed stats --- */
Computed_Stats Computed_Stats_plus(Computed_Stats s1, Computed_Stats s2);

#endif /* UNIT_STATS_H */
