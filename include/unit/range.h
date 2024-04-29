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

/* --- Rangemap --- */
int  Unit_Rangemap_Get(      struct Unit *u);
void Unit_Rangemap_Toggle(   struct Unit *u);
void Unit_Rangemap_Default(  struct Unit *u);
void Unit_Rangemap_Equipment(struct Unit *u);

b32 Range_Valid(    struct Range r1);
void Ranges_Combine( struct Range *r1, struct Range r2);
struct Range  _Ranges_Combine(struct Range r1, struct Range r2);
b32 Range_toCombine(struct Unit *u, struct Weapon *w);

/* -- Equipment Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/
struct Range *Unit_Range_Combine(          struct Unit *u, b32 eq);
struct Range *Unit_Range_Combine_Staves(   struct Unit *u, b32 eq);
struct Range *Unit_Range_Combine_Weapons(  struct Unit *u, b32 eq);
struct Range *Unit_Range_Combine_Equipment(struct Unit *u);

struct Range *_Unit_Range_Combine(struct Unit  *u, struct Range *r, b32 e, int a);

/* -- Loadout Range -- */
/* Compute range of loadout:
*  - Range of weapon in strong hand
*   - UNLESS dual-wielding. Ranges combine, BUT only add stats if in range.
*/
struct Range *Unit_Range_Item(   struct Unit *u, int i);
struct Range *Unit_Range_Loadout(struct Unit *u);

#endif /* UNIT_RANGE_H */
