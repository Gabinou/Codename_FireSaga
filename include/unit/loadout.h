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
b32 eq_valid(       i32 eq);
b32 equipped_valid( i32 equipped);

/* --- Loadout --- */
// unit._equipped:      Currently equipped equipment
// loadout._loadout:    Possible equipped equipment
i32  Loadout_Eq(        Loadout *loadout, i32 hand);
void Loadout_Set(       Loadout *loadout, i32 hand, i32 eq);
void Loadout_None(      Loadout *loadout, i32 hand);
b32  Loadout_isEquipped(Loadout *loadout, i32 hand);

b32 Loadout_istwoHanding(Loadout *loadout);
b32 _istwoHanding(i32 eq_left, i32 eq_right);


/* --- canEquip --- */
void canEquip_Eq(           canEquip *e, i32 eq);
void canEquip_Loadout(      canEquip *e, i32 h, i32 eq);
void canEquip_Loadout_None( canEquip *e, i32 h);

/* --- Importing/Exporting --- */
// - Export: unit       -> loadout
// - Import: loadout    -> unit

void Loadout_Copy(Loadout *dest, const Loadout *const src);
void Equipped_Copy(i32 *_dest, const i32 *const _src);

void Unit_Equipped_Import(Unit *u, i32 *_loadout);
void Unit_Equipped_Export(Unit *u, i32 *_loadout);

void Unit_Loadout_Import(Unit *u, Loadout *loadout);
void Unit_Loadout_Export(Unit *u, Loadout *loadout);

void Unit_Equipment_Import(Unit *u, Inventory_item *equipment);
void Unit_Equipment_Export(Unit *u, Inventory_item *equipment);

#endif /* UNIT_LOADOUT_H */
