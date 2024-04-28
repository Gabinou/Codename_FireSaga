#ifndef UNIT_BONUS_H
#define UNIT_BONUS_H

#include "enums.h"
#include "structs.h"

void Unit_Bonus_Add(struct Unit *unit, struct Aura *aura, tnecs_entity source, struct Item,);
void Unit_Bonus_Decay(struct Unit *unit);
void Unit_Bonus_Refresh(struct Unit *unit, struct Aura *aura, tnecs_entity source, struct Item,);

#endif /* UNIT_BONUS_H */
