#ifndef UNIT_LOADOUT_H
#define UNIT_LOADOUT_H

#include "types.h"
#include "enums.h"
#include "item.h"
#include "weapon.h"
#include "unit/unit.h"
#include "structs.h"
#include "unit/unit.h"

/* --- Transformation --- */
i32 eq2equipped(i32 eq);
i32 equipped2eq(i32 equipped);

/* --- Loadout --- */
i32  Loadout_Eq(        Loadout *loadout, i32 hand);
void Loadout_Set(       Loadout *loadout, i32 hand, i32 eq);
void Loadout_None(      Loadout *loadout, i32 hand);
i32  Loadout_isEquipped(Loadout *loadout, i32 hand);

/* --- canEquip --- */
void canEquip_Eq(           canEquip *e, i32 eq);
void canEquip_Loadout(      canEquip *e, i32 h, i32 eq);
void canEquip_Loadout_None( canEquip *e, i32 h);

#endif /* UNIT_LOADOUT_H */
