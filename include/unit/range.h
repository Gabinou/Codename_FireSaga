#ifndef UNIT_RANGE_H
#define UNIT_RANGE_H

#include "types.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Position;

// Range is valid if:
//  - min range >= max range
//  - min range and max range in valid range values
b32 Range_Valid(struct Range r1);

// Range of a single piece of equipment, if can be equipped
void Unit_Range_Eq(Unit *u, i32 eq, i64 archetype, struct Range *range );
// Range of a single piece of equipment -> CAN'T CHECK canEquip!
void Unit_Range_Id(Unit *u, i32 id, i64 archetype, struct Range *range);

// Combine ranges of items currently equipped
// Item was previously equipped, no need to check if CAN equip
void Unit_Range_Equipped(Unit *u, i64 archetype, struct Range *range);

/* Combines range of all weapons in equipment, if can be equipped */
// - Combine ranges of all equippable equipment, assuming NO LOADOUT
// - Combined range may no reflect actual loadout range
//      - Ex: will combine range of two two-hand only weapons
void Unit_Range_Equipment(Unit *u, i64 archetype, struct Range *range);

b32 Unit_inRange_Loadout(struct Unit        *agg,
                         struct Position    *agg_pos,
                         struct Position    *dft_pos,
                         i64                archetype);

/* --- Combine --- */
void          Ranges_Combine(struct Range *r1, struct Range r2);
struct Range _Ranges_Combine(struct Range  r1, struct Range r2);

b32 _Range_Archetype_Match(const struct Weapon *wpn, i64 archetype);

/* --- Rangemap --- */
int  Unit_Rangemap(             Unit *u);
int  Unit_User_Rangemap(        Unit *u);
void Unit_Rangemap_set(         Unit *u, int rangemap);
void Unit_User_Rangemap_set(    Unit *u, int rangemap);

int  Unit_Rangemap_Get(         Unit *u);
void Unit_Rangemap_default(     Unit *u);
void Unit_RangeMap_Act_Toggle(  Unit *u);

#endif /* UNIT_RANGE_H */
