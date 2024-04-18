#ifndef UNIT_H
#define UNIT_H

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
extern u8   army_alignment[ARMY_END];
extern bool army_isPC[ARMY_END];

/* --- UNIT STATS --- */


/* --- Constructors/Destructors --- */
void Unit_Init(        struct Unit *u);
void Unit_Free(        struct Unit *u);
void Unit_InitWweapons(struct Unit *u, struct dtab *weapons_dtab);
void Unit_Reinforcement_Load(struct Unit *u, struct Reinforcement *r);

/* --- Setters/Getters --- */
int Unit_Id_Strong(struct Unit *u, int i);
/* -- Item spaces -- */
/* Equipment, items                         ->    side space */
/* Menus, popups are top hand/strong hand   ->  strong space */
struct Inventory_item *Unit_Item_Side(  struct Unit *u, int i); /*   side space */
struct Inventory_item *Unit_Item_Strong(struct Unit *u, int i); /* strong space */

struct WpnorItem Unit_WpnorItem(struct Unit *u, int i);         /*   side space */

void Unit_Set_Item_Side(struct Unit *u, int i);                 /*   side space */

int Unit_Hand_Strong(struct Unit *u);
int SotA_Hand_Strong(i8 handedness);

int Unit_Hand_Strong2Side(struct Unit *unit, int i);
int Unit_Hand_Side2Strong(struct Unit *unit, int i);

void Unit_setid(      struct Unit *u, i16 id);
void Unit_setStats(   struct Unit *u, struct Unit_stats stats);
void Unit_setStats(   struct Unit *u, struct Unit_stats stats);
void Unit_setBases(   struct Unit *u, struct Unit_stats stats);
void Unit_setClassind(struct Unit *u, i8 class_i);

struct Unit_stats Unit_getStats(struct Unit *u);

/* --- Items --- */
/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(           struct Unit *u, int  i, int a);
void _Unit_Equipped_Deplete(       struct Unit *u, bool h, int a);

void Unit_Item_Deplete(            struct Unit *u, int i);
void Unit_Equipped_Staff_Deplete(  struct Unit *u, bool h);
void Unit_Equipped_Weapons_Deplete(struct Unit *u);
void Unit_Equipped_Shields_Deplete(struct Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  struct Unit *u, struct Inventory_item item);
void  Unit_Item_Swap(  struct Unit *u, i16 ind1, i16 ind2);
void  Unit_Item_Trade( struct Unit *g, struct Unit *t, i16 giver_i, i16 taker_i);
void  Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);
void _Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);

/* -- Dropping -- */
struct Inventory_item Unit_Item_Drop(struct Unit *u, i16 ind1);
void Unit_Equipment_Drop(struct Unit *u);

/* --- Equipping --- */
bool                  Unit_Equip(           struct Unit *u, bool h, int i);
void                  Unit_Unequip(         struct Unit *u, bool h);
bool                  Unit_Equip_inHand(    struct Unit *u, bool h);
struct Inventory_item Unit_Equip_TwoHanding(struct Unit *u);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(struct Unit *u, bool hand);
void  Unit_Check_Equipped(struct Unit *u);

/* --- Supports --- */
void Unit_supportUp(struct Unit *u, i16 id);

/* --- Second-order info --- */
bool SotA_isPC(          u8 a);
u8  SotA_army2alignment(u8 a);

u8  Unit_mvtType(struct Unit *u);

/* --- Debug --- */
bool Unit_Equipment_Full( struct Unit *u);
void Unit_Equipment_Print(struct Unit *u);

/* --- Usability --- */
bool Unit_Eq_Usable(  struct Unit *u, int a, int i);
bool Unit_All_Usable( struct Unit *u);
void Unit_Find_Usable(struct Unit *u, int a);
bool Unit_Item_Usable(struct Unit *u, int a, int i);

/* --- Skills --- */
bool Unit_hasSkill(struct Unit *u, u64 s);

/* --- Stat computation --- */
/* Gives weapon stat if distance is in range.
*    DEBUG: input -1 to always be in_range
*/
/* Distance-dependent stats */
u8 Unit_computeHit(     struct Unit *u, int dist);
i8 Unit_computeDodge(   struct Unit *u, int dist);
u8 Unit_computeFavor(   struct Unit *u, int dist);
i8 Unit_computeSpeed(   struct Unit *u, int dist);
u8 Unit_computeCritical(struct Unit *u, int dist);

/* Distance-independent stats */
i8 Unit_computeMove(    struct Unit *u);
u8 Unit_computeAgony(   struct Unit *u);
i8 Unit_computeRegrets( struct Unit *u);
u8 Unit_computeEffectivefactor(struct Unit *a, struct Unit *d);

u8 *Unit_computeAttack( struct Unit *u, int dist);
u8 *Unit_computeDefense(struct Unit *u);

struct Unit_stats Unit_effectiveStats(   struct Unit *u);
struct Unit_stats Unit_effectiveGrowths( struct Unit *u);
struct Computed_Stats Unit_supportBonus( struct Unit *u);
struct Computed_Stats Unit_computedStats(struct Unit *u, int dist);
struct Computed_Stats Unit_computedStats_wLoadout(struct Unit *u, int lh, int rh, int dist);

/* --- Bonus/Malus from items --- */
/* Bonus and Malus should always be computable from skills, equipment, stats */
i8 Unit_computeMalus(struct Unit *u);
i8 Unit_computeBonus(struct Unit *u);

/* --- Loadout Manipulation --- */
bool Unit_Loadout_twoHanding(int lh, int rh);

/* - Public: Chooses between _Unit_Loadout_Swap_Twohanding and _Unit_Loadout_Swap - */
void Unit_Loadout_Swap(        struct Unit *u, int lh, int rh);
void Unit_Loadout_Swap_Reverse(struct Unit *u, int lh, int rh);

/* - Private: Only for not twohanding - */
void _Unit_Loadout_Swap(        struct Unit *u, int lh, int rh);
void _Unit_Loadout_Swap_Reverse(struct Unit *u, int lh, int rh);

/* - Private: Only for twohanding - */
void _Unit_Loadout_Swap_Twohanding(        struct Unit *u, int i);
void _Unit_Loadout_Swap_Reverse_Twohanding(struct Unit *u);

/* --- Debug Utils --- */
void Unit_Cap_Stats(    struct Unit *u);
void Unit_HalfCap_Stats(struct Unit *u);

/* --- Unit State change --- */
void Unit_wait(    struct Unit *u);
void Unit_dies(    struct Unit *u);
void Unit_refresh( struct Unit *u);
void Unit_agonizes(struct Unit *u);

void Unit_getsHealed( struct Unit *u, u8 healing);
void Unit_takesDamage(struct Unit *u, u8 dmg, bool ct);

/* --- I/O --- */
void Unit_readJSON( void *u, cJSON *junit);
void Unit_writeJSON(void *u, cJSON *junit);

/* --- Unit state --- */
bool Unit_isdualWielding(struct Unit *u);
// WrongHanding: using shield or offhand in strong hand and vice vdrsa
bool Unit_iswrongHanding(struct Unit *u);

/* --- Unit Can --- */
bool Unit_canDance(struct Unit *u);

/* -- Can Carry -- */
bool Unit_canCarry(struct Unit *u1, struct Unit *u2);

/* -- Can Equip -- */
bool Unit_canEquip(       struct Unit *u, i16 id);
u8   Unit_Equippables(    struct Unit *u, u8 *eq);
bool Unit_canEquip_Type(  struct Unit *u, i16 id);
bool Unit_canEquip_Hand(  struct Unit *u, i16 id, bool h);
bool Unit_canEquip_inHand(struct Unit *u, bool h);

/* -- Can Attack -- */
bool _Unit_canAttack(   struct Unit *u, bool hand);  /* with weapon in hand       */
bool  Unit_canAttack(   struct Unit *u);             /* with equipped weapon      */
bool  Unit_canAttack_Eq(struct Unit *u);             /* with any wpn in equipment */

/* -- Can Staff -- */
int Unit_canStaff(        struct Unit *u);
int Unit_canStaff_Eq(     struct Unit *u);
int Unit_canStaff_oneHand(struct Unit *u);

u8 Unit_Brave(struct Unit *u);

/* --- Lvlup && Promotion --- */
void Unit_lvlUp(  struct Unit *u);
i16 Unit_getLvl(  struct Unit *u);
void Unit_Promote(struct Unit *u, i8 new_class_i);

/* -- Unit_id -- */
bool Unit_ID_Valid(u16 id);

#endif /* UNIT_H */
