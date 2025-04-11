#ifndef UNIT_H
#define UNIT_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Weapon;
struct Combat_Damage;
struct Game;
struct Position;
struct Map;
struct RNG_Sequence;
struct Reinforcement;
struct cJSON;
typedef struct cJSON cJSON;

/* -- Unit second-order info -- */
extern const i32 army_alignment[ARMY_END];
extern const b32      army_isPC[ARMY_END];

/* --- UNIT STATS --- */

/* --- Tetrabrachios --- */
void Tetrabrachios_Init(   Unit *u);
void Tetrabrachios_default(Unit *u);

/* --- Constructors/Destructors --- */
// TODO: Only one unit_init -> WITH WEAPONS_DTAB
// unit should be USABLE ANYWHERE AFTER INIT.
void Unit_Init(         Unit *u);
void Unit_Members_Alloc(Unit *u);

void Unit_Free(         Unit *u);

void Unit_Reinforcement_Load(Unit *u, struct Reinforcement *r);

i16 Unit_id(Unit *unit);
void Unit_id_set(Unit *unit, i16 id);

void Unit_setStats(   Unit *u, Unit_stats stats);
void Unit_setBases(   Unit *u, Unit_stats stats);
void Unit_setClassind(Unit *u, i8 class_i);

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
// TODO: input effective_stats
void Unit_computeHit(     Unit *u, int dist, i32 *hit);
void Unit_computeDodge(   Unit *u, int dist, i32 *dodge);
void Unit_computeFavor(   Unit *u, int dist, i32 *favor);
void Unit_computeSpeed(   Unit *u, int dist, i32 *speed);
void Unit_computeCritical(Unit *u, int dist, i32 *crit);

/* Distance-independent stats */
void Unit_computeMove(    Unit *u, i32 *mv);
void Unit_computeAgony(   Unit *u, i32 *agony);
void Unit_computeRegrets( Unit *u, Computed_Stats *stats, i32 *regrets);
void Unit_computeEffectivefactor(Unit *a, Unit *d, i32 *factor);

void Unit_computeAttack( Unit *u, int dist, i32 *att);
void Unit_computeDefense(Unit *u, i32* def);

Unit_stats Unit_effectiveStats(   Unit *u);
Unit_stats Unit_effectiveGrowths( Unit *u);
struct Computed_Stats Unit_computedStats(Unit *u, int dist, Unit_stats effective_stats);
struct Computed_Stats Unit_computedStats_wLoadout(Unit *u, Loadout *loadout, int dist);

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

/* -- Hand -- */
b32  Unit_hasHand(       Unit *u, i32 hand);
void Unit_Hand_Set(      Unit *u, i32 hand, b32 has);
i32  Unit_Hand_Weak(     Unit *u);
i32  Unit_Hand_Strong(   Unit *u);

/* -- Can Attack -- */
b32 _Unit_canAttack(   Unit *u, i32 hand);  /* with weapon in hand       */
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
void Unit_lvlUp(        Unit *u);
i16  Unit_Level(        Unit *u);
i16  Unit_Experience(const Unit const *unit);
void Unit_Promote(Unit *u, i8 new_class_i);

/* -- Unit_id -- */
b32 Unit_ID_Valid(u16 id);

Damage_Raw Unit_Shield_Protection(struct Unit *unit, i32 hand);

#endif /* UNIT_H */
