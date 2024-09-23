#ifndef UNIT_LOADOUT_H
#define UNIT_LOADOUT_H

#include "types.h"
#include "enums.h"
#include "item.h"
#include "weapon.h"
#include "unit/unit.h"
#include "structs.h"
#include "unit/unit.h"

i32  Loadout_Eq(        Loadout *loadout, i32 hand);
void Loadout_Set(       Loadout *loadout, i32 hand, i32 eq);
void Loadout_None(      Loadout *loadout, i32 hand);
i32  Loadout_isEquipped(Loadout *loadout, i32 hand);

#endif /* UNIT_LOADOUT_H */
