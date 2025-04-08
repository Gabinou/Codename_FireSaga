#ifndef UNIT_FLAGS_H
#define UNIT_FLAGS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

// TODO: toggle functions for flags
b32 Unit_Sex(const Unit *u);
b32 Unit_isAlive(const Unit *u);
b32 Unit_isWaiting(const Unit *u);
b32 Unit_showsDanger(const Unit *u);
b32 Unit_isDivineShield(const Unit *u);
b32 Unit_isDualWielding(const Unit *u);

u64 Unit_Skills(const Unit *u);
void Unit_Skill_Add(Unit *u, u64 skill);

struct Unit_stats *Unit_Stats_Grown(const Unit *u);
struct Unit_stats *Unit_Stats_Growths(Unit *u);

struct dtab *Unit_dtab_Items(const Unit *u);
struct dtab *Unit_dtab_Weapons(const Unit *u);

i32 Unit_Current_HP(const Unit *u);
i32 Unit_Current_Agony(const Unit *u);
i32 Unit_Current_Regrets(const Unit *u);

i32 Unit_Army(const Unit *u);
i32 Unit_Class(const Unit *u);
i32 Unit_AI(const Unit *u);
void Unit_Class_set(Unit *unit, i32 class);
void Unit_Army_set(Unit *unit, i32 army);
void Unit_AI_set(Unit *u, i32 ai);

#endif /* UNIT_H */
