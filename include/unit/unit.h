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
extern b32 army_isPC[ARMY_END];

/* --- UNIT STATS --- */


/* --- Constructors/Destructors --- */
void Unit_Init(        struct Unit *u);
void Unit_Free(        struct Unit *u);
void Unit_Allocs(      struct Unit *u);
void Unit_InitWweapons(struct Unit *u, struct dtab *weapons_dtab);
void Unit_Reinforcement_Load(struct Unit *u, struct Reinforcement *r);

int Unit_Hand_Strong(struct Unit *u);
int SotA_Hand_Strong(i8 handedness);

// TODO: Remove <<<<<<<<<<<<<<<
int Unit_Hand_Strong2Side(struct Unit *unit, int i);
int Unit_Hand_Side2Strong(struct Unit *unit, int i);
// TODO: Remove >>>>>>>>>>>>>>>

void Unit_setid(      struct Unit *u, i16 id);
void Unit_setStats(   struct Unit *u, struct Unit_stats stats);
void Unit_setBases(   struct Unit *u, struct Unit_stats stats);
void Unit_setClassind(struct Unit *u, i8 class_i);

struct Unit_stats Unit_getStats(struct Unit *u);

/* --- Supports --- */
void Unit_supportUp(struct Unit *u, i16 id);

/* --- Second-order info --- */
b32 SotA_isPC(          u8 a);
u8  SotA_army2alignment(u8 a);

u8  Unit_mvtType(struct Unit *u);

/* --- Skills --- */
b32 Unit_hasSkill(struct Unit *u, u64 s);

/* --- Stat computation --- */
/* Gives weapon stat if distance is in range.
*    DEBUG: input -1 to always be in_range
*/
/* Distance-dependent stats */
i32 Unit_computeHit(     struct Unit *u, int dist);
i32 Unit_computeDodge(   struct Unit *u, int dist);
i32 Unit_computeFavor(   struct Unit *u, int dist);
i32 Unit_computeSpeed(   struct Unit *u, int dist);
i32 Unit_computeCritical(struct Unit *u, int dist);

/* Distance-independent stats */
i32 Unit_computeMove(    struct Unit *u);
i32 Unit_computeAgony(   struct Unit *u);
i32 Unit_computeRegrets( struct Unit *u);
i32 Unit_computeEffectivefactor(struct Unit *a, struct Unit *d);

i32 *Unit_computeAttack( struct Unit *u, int dist);
i32 *Unit_computeDefense(struct Unit *u);

struct Unit_stats Unit_effectiveStats(   struct Unit *u);
struct Unit_stats Unit_effectiveGrowths( struct Unit *u);
struct Computed_Stats Unit_computedStats(struct Unit *u, int dist);
struct Computed_Stats Unit_computedStats_wLoadout(struct Unit *u, int lh, int rh, int dist);

/* --- Debug Utils --- */
void Unit_Cap_Stats(    struct Unit *u);
void Unit_HalfCap_Stats(struct Unit *u);

/* --- Unit State change --- */
void Unit_wait(    struct Unit *u);
void Unit_dies(    struct Unit *u);
void Unit_refresh( struct Unit *u);
void Unit_agonizes(struct Unit *u);

void Unit_getsHealed( struct Unit *u, u8 healing);
void Unit_takesDamage(struct Unit *u, u8 dmg, b32 ct);

/* --- I/O --- */
void Unit_readJSON( void *u, cJSON *junit);
void Unit_writeJSON(void *u, cJSON *junit);

/* --- Unit Can --- */
b32 Unit_canDance(struct Unit *u);

/* -- Can Carry -- */
b32 Unit_canCarry(struct Unit *u1, struct Unit *u2);

/* -- Can Attack -- */
b32 _Unit_canAttack(   struct Unit *u, b32 hand);  /* with weapon in hand       */
b32  Unit_canAttack(   struct Unit *u);            /* with equipped weapon      */
b32  Unit_canAttack_Eq(struct Unit *u);            /* with any wpn in equipment */

/* -- Can Staff -- */
int Unit_canStaff(        struct Unit *u);
int Unit_canStaff_Eq(     struct Unit *u);
int Unit_canStaff_oneHand(struct Unit *u);

/* -- HP is full -- */
b32 Unit_HP_isFull(struct Unit *u);

/* -- Brave -- */
u8 Unit_Brave(struct Unit *u);

/* --- Lvlup && Promotion --- */
void Unit_lvlUp(  struct Unit *u);
i16  Unit_getLvl( struct Unit *u);
void Unit_Promote(struct Unit *u, i8 new_class_i);

/* -- Unit_id -- */
b32 Unit_ID_Valid(u16 id);

#endif /* UNIT_H */
