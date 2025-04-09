#ifndef UNIT_FLAGS_H
#define UNIT_FLAGS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

// TODO: toggle functions for flags
b32 Unit_Sex(const Unit *u);
b32 Unit_isAlive(const Unit *u);
i8  Unit_Movement(const Unit *u);
b32 Unit_isWaiting(const Unit *u);
i8  Unit_Handedness(const Unit *u);
u16 Unit_Equippable(const Unit *u);
b32 Unit_isTalkable(const Unit *u);
b32 Unit_showsDanger(const Unit *u);
b32 Unit_isDivineShield(const Unit *u);
b32 Unit_isDualWielding(const Unit *u);

void Unit_Alive_set(Unit *u, b32 alive);
void Unit_Waiting_set(Unit *u, b32 wait);
void Unit_Handedness_set(Unit *u, i8 handedness);
void Unit_showsDanger_set(Unit *u, b32 show_danger);
void Unit_DivineShield_set(Unit *u, b32 divine_shield);

u64 Unit_Skills(const Unit *u);
void Unit_Skill_Add(Unit *u, u64 skill);

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
