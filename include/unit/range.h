#ifndef UNIT_RANGE_H
#define UNIT_RANGE_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "weapon.h"

// Range is valid if:
//  - min range >= max range
//  - min range and max range in valid range values
b32 Range_Valid(struct Range r1);

// Range of a single piece of equipment, if can be equipped
struct Range *Unit_Range_Eq(Unit *u, i32 eq, i64 archetype);
struct Range *Unit_Range_Id(Unit *u, i32 id, i64 archetype);

// Combine ranges of items in current loadout
// Item was previously equipped, no need to check if CAN equip
struct Range *Unit_Range_Loadout(Unit *u, i64 archetype);

/* Combines range of all weapons in equipment, if can be equipped */
// - Combine ranges of all equippable equipment, assuming NO LOADOUT
// - Combined range may no reflect actual loadout range
//      - Ex: will combine range of two two-hand only weapons
struct Range *Unit_Range_Equipment(Unit *u, i64 archetype);

b32 Unit_inRange_Loadout(struct Unit        *agg,
                         struct Position    *agg_pos,
                         struct Position    *dft_pos,
                         i64          archetype);

/* --- Combine --- */
void          Ranges_Combine(struct Range *r1, struct Range r2);
struct Range _Ranges_Combine(struct Range  r1, struct Range r2);

/* --- Rangemap --- */
int  Unit_Rangemap_Get(      struct Unit *u);
void Unit_Rangemap_Toggle(   struct Unit *u);
void Unit_Rangemap_Default(  struct Unit *u);
void Unit_Rangemap_Equipment(struct Unit *u);

#endif /* UNIT_RANGE_H */
