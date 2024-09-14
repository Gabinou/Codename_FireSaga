#ifndef UNIT_RANGE_H
#define UNIT_RANGE_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"
#include "types.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/boss.h"
#include "unit/stats.h"
#include "unit/mount.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Weapon;
struct Damage;
struct Position;
struct Map;
struct RNG_Sequence;
struct Reinforcement;

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

b32 Unit_inRange_Loadout(struct Unit     *agg,
                         struct Position *agg_pos,
                         struct Position *dft_pos);

/* --- Rangemap --- */
int  Unit_Rangemap_Get(      struct Unit *u);
void Unit_Rangemap_Toggle(   struct Unit *u);
void Unit_Rangemap_Default(  struct Unit *u);
void Unit_Rangemap_Equipment(struct Unit *u);


#endif /* UNIT_RANGE_H */
