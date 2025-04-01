#ifndef UNIT_FLAGS_H
#define UNIT_FLAGS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

// TODO: toggle functions for flags
b32 Unit_Sex(Unit *u);
b32 Unit_isAlive(Unit *u);
b32 Unit_isWaiting(Unit *u);
b32 Unit_showsDanger(Unit *u);
b32 Unit_isUpdateStats(Unit *u);
b32 Unit_isDivineShield(Unit *u);
b32 Unit_isDualWielding(Unit *u);

u64 Unit_Skills(Unit *u);
void Unit_Skill_Add(Unit *u, u64 skill);

struct Unit_stats *Unit_Stats_Grown(Unit *u);
struct Unit_stats *Unit_Stats_Growths(Unit *u);

struct dtab *Unit_dtab_Weapons(Unit *u);
struct dtab *Unit_dtab_Items(Unit *u);

#endif /* UNIT_H */
