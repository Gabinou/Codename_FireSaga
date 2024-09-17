#ifndef UNIT_H
#define UNIT_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "inventory.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/equipment.h"
#include "unit/boss.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Weapon;
struct Damage;
struct Game;
struct Position;
struct Map;
struct RNG_Sequence;
struct Reinforcement;

/* -- Unit second-order info -- */
extern i32 army_alignment[ARMY_END];
extern b32      army_isPC[ARMY_END];

/* --- UNIT STATS --- */


/* --- Constructors/Destructors --- */

void Unit_Init(         Unit *u);
void Unit_InitWweapons( Unit *u, struct dtab *weapons_dtab);
void Unit_Alloc_Members(Unit *u);

void Unit_Free(         Unit *u);

void Unit_Reinforcement_Load(Unit *u, struct Reinforcement *r);

int Unit_Hand_Weak(  Unit *u);
int Unit_Hand_Strong(Unit *u);
int SotA_Hand_Strong(i8 handedness);

void Unit_setid(      Unit *u, i16 id);
void Unit_setStats(   Unit *u, Unit_stats stats);
void Unit_setBases(   Unit *u, Unit_stats stats);
void Unit_setClassind(Unit *u, i8 class_i);

Unit_stats Unit_getStats(Unit *u);

/* --- Supports --- */
void Unit_supportUp(Unit *u, i16 id);

/* --- Second-order info --- */
b32 SotA_isPC(          u8 a);
u8  SotA_army2alignment(u8 a);

u8  Unit_mvtType(Unit *u);

/* --- Skills --- */
b32 Unit_hasSkill(Unit *u, u64 s);

/* --- Stat computation --- */
/* Gives weapon stat if distance is in range.
*    DEBUG: input -1 to always be in_range
*/
/* Distance-dependent stats */
i32 Unit_computeHit(     Unit *u, int dist);
i32 Unit_computeDodge(   Unit *u, int dist);
i32 Unit_computeFavor(   Unit *u, int dist);
i32 Unit_computeSpeed(   Unit *u, int dist);
i32 Unit_computeCritical(Unit *u, int dist);

/* Distance-independent stats */
i32 Unit_computeMove(    Unit *u);
i32 Unit_computeAgony(   Unit *u);
i32 Unit_computeRegrets( Unit *u);
i32 Unit_computeEffectivefactor(Unit *a, Unit *d);

i32 *Unit_computeAttack( Unit *u, int dist);
i32 *Unit_computeDefense(Unit *u);

Unit_stats Unit_effectiveStats(   Unit *u);
Unit_stats Unit_effectiveGrowths( Unit *u);
struct Computed_Stats Unit_computedStats(Unit *u, int dist);
struct Computed_Stats Unit_computedStats_wLoadout(Unit *u, int lh, int rh, int dist);

/* --- Debug Utils --- */
void Unit_Cap_Stats(    Unit *u);
void Unit_HalfCap_Stats(Unit *u);

/* --- Unit State change --- */
void Unit_wait(    Unit *u);
void Unit_dies(    Unit *u);
void Unit_refresh( Unit *u);
void Unit_agonizes(Unit *u);

void Unit_getsHealed( Unit *u, u8 healing);
void Unit_takesDamage(Unit *u, u8 dmg, b32 ct);

/* --- I/O --- */
void Unit_readJSON( void *u, cJSON *junit);
void Unit_writeJSON(void *u, cJSON *junit);

/* --- Unit Can --- */
b32 Unit_canDance(Unit *u);

/* -- Can Carry -- */
b32 Unit_canCarry(Unit *u1, Unit *u2);

/* -- Can Attack -- */
b32 _Unit_canAttack(   Unit *u, b32 hand);  /* with weapon in hand       */
b32  Unit_canAttack(   Unit *u);            /* with equipped weapon      */
b32  Unit_canAttack_Eq(Unit *u);            /* with any wpn in equipment */

/* -- Can Staff -- */
int Unit_canStaff(        Unit *u);
int Unit_canStaff_Eq(     Unit *u);

int Unit_canStaff_oneHand(Unit *u);
int Unit_canMagic_oneHand(Unit *u);

/* -- HP is full -- */
b32 Unit_HP_isFull(Unit *u);

/* -- Brave -- */
u8 Unit_Brave(Unit *u);

/* --- Lvlup && Promotion --- */
void Unit_lvlUp(  Unit *u);
i16  Unit_getLvl( Unit *u);
void Unit_Promote(Unit *u, i8 new_class_i);

/* -- Unit_id -- */
b32 Unit_ID_Valid(u16 id);

#endif /* UNIT_H */
