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

b32 eq_valid(       i32 eq);
b32 equipped_valid( i32 equipped);

/* --- Loadout --- */
// unit._equipped:      Currently equipped equipment
// loadout._loadout:    Possible equipped equipment
i32  Loadout_Eq(        Loadout *loadout, i32 hand);
void Loadout_Set(       Loadout *loadout, i32 hand, i32 eq);
void Loadout_None(      Loadout *loadout, i32 hand);
b32  Loadout_isEquipped(Loadout *loadout, i32 hand);

/* --- canEquip --- */
void canEquip_Eq(           canEquip *e, i32 eq);
void canEquip_Loadout(      canEquip *e, i32 h, i32 eq);
void canEquip_Loadout_None( canEquip *e, i32 h);

/* --- Importing/Exporting --- */
void Unit_Equipped_Import(Unit *u, i32 *_loadout);
void Unit_Equipped_Export(Unit *u, i32 *_loadout);

void Unit_Loadout_Import(Unit *u, Loadout *loadout);
void Unit_Loadout_Export(Unit *u, Loadout *loadout);

void Unit_Equipment_Import(Unit *u, Inventory_item *equipment);
void Unit_Equipment_Export(Unit *u, Inventory_item *equipment);

#endif /* UNIT_LOADOUT_H */
