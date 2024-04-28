#ifndef UNIT_BONUS_H
#define UNIT_BONUS_H

#include "enums.h"
#include "tnecs.h"
#include "structs.h"
#include "unit/unit.h"

void Unit_Bonus_Decay(struct Unit *unit);
void Unit_Bonus_Add(struct Unit *unit, struct Bonus_Stats bonus);
void Unit_Bonus_Refresh(struct Unit *unit, struct Bonus_Stats bonus); 
struct Bonus_Stats Aura2Bonus(struct Aura *a, tnecs_entity u, u16 i, u16 s, b32 a);

/* Bonus utils */
b32 Bonus_Stats_Compare(struct Bonus_Stats bonus1, struct Bonus_Stats bonus2);

#endif /* UNIT_BONUS_H */
