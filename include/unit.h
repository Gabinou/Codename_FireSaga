#ifndef UNIT_H
#define UNIT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Weapon;
struct Damage;
struct RNG_Sequence;

/* --- ANTS--- */
enum STATUSES {
    STATUS_DEFAULT_TURNS = 5,
};

/* -- Unit second-order info -- */
extern u8   army_alignment[ARMY_END];
extern bool army_isPC[ARMY_END];

/* --- UnitMoveAnimation --- */
typedef struct UnitMoveAnimation {
    i64 time_ns;
    struct Point target;
    tnecs_entity unit;
} UnitMoveAnimation;
extern struct UnitMoveAnimation UnitMoveAnimation_default;


/* --- UNIT STATUS --- */
struct Unit_status {
    /* Number of turns to be in this state. *
    /* <0 means forever. */
    i8 status;
    i8 turns;
};
extern struct Unit_status Unit_status_default;

/* --- UNIT STATS --- */

/* --- UNIT --- */
typedef struct Unit {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    i16 class;
    i8  mvt_type;
    i8  army;
    u8  current_hp;
    i8  handedness;
    u16 talkable;
    u8  current_agony;
    i8  agony; /* turns left before death (-1 not agonizing) */
    u8  regrets;

    // Status with least remaining turns on top.
    struct Unit_status *status_queue;

    // TODO: change into ID for AI?
    s8 ai_filename; /* Default AI for unit */

    struct Support supports[SOTA_MAX_SUPPORTS];
    u16 support_type;
    u16 support_num;
    struct Damage damage;

    struct Unit_stats base_stats;
    struct Unit_stats bonus_stats;
    struct Unit_stats malus_stats;
    struct Unit_stats caps_stats;
    struct Unit_stats current_stats;    /* base_stats + all growths */
    struct Unit_stats effective_stats;  /* current_stats + bonuses/maluses */
    struct Unit_stats growths;
    struct Unit_stats bonus_growths;
    struct Unit_stats effective_growths;
    struct Unit_stats *grown_stats;

    u64 skills;

    struct RNG_Sequence hit_sequence;
    struct RNG_Sequence crit_sequence;

    struct RNG_Sequence hp_sequence;
    struct RNG_Sequence str_sequence;
    struct RNG_Sequence mag_sequence;
    struct RNG_Sequence dex_sequence;
    struct RNG_Sequence agi_sequence;
    struct RNG_Sequence fth_sequence;
    struct RNG_Sequence luck_sequence;
    struct RNG_Sequence def_sequence;
    struct RNG_Sequence res_sequence;
    struct RNG_Sequence con_sequence;
    struct RNG_Sequence move_sequence;
    struct RNG_Sequence prof_sequence;

    u16 equippable;
    u16 base_exp;
    u16 exp;
    u16 _id;
    u16 rescuee;

    i8 rangemap;
    i8 user_rangemap; /* reset to NULL when equipment changes */

    bitflag16_t job_talent;

    /* Defendant position (self is Aggressor.) */
    struct Point dft_pos; /* Used to compute stats in case of dual wielding */

    bool hands   [UNIT_HANDS_NUM];      /* Does unit have hands?     */
    bool equipped[UNIT_HANDS_NUM];      /* Is Item in hand equipped? */

    s8 *skill_names;

    struct dtab *weapons_dtab;
    struct dtab *items_dtab;


    /* _equipment is in side space: [left, right, 2, 3, 4, 5]   */
    /* Most functions are in side space unless stated otherwise */
    struct Inventory_item _equipment[DEFAULT_EQUIPMENT_SIZE];

    /* For twohanding when computing computedstats */
    struct Inventory_item temp;

    i8 eq_usable[DEFAULT_EQUIPMENT_SIZE];
    i8 num_equipment;
    i8 num_usable;

    struct Mount *mount;
    bool mounted;

    s8 name;
    s8 title;    /* Lord, Duke, etc. */
    struct Computed_Stats computed_stats;   /* Computed from Unit_Stats */
    struct Computed_Stats support_bonuses[SOTA_MAX_SUPPORTS];
    struct Computed_Stats support_bonus;

    bool sex            : 1; /* 0:F, 1:M. eg. hasPenis. */
    bool waits          : 1;
    bool alive          : 1;
    bool literate       : 1; /* Reading/writing for scribe job. */
    bool courageous     : 1; /* For reaction to story events    */
    bool show_danger    : 1;
    bool update_stats   : 1;
    bool isTwoHanding   : 1; /* If true, one Inventory_Item in hands is a copy */
    bool divine_shield  : 1;
    bool isDualWielding : 1;
} Unit;
extern struct Unit Unit_default;

/* --- Constructors/Destructors --- */
void Unit_Init(        struct Unit *u);
void Unit_Free(        struct Unit *u);
void Unit_InitWweapons(struct Unit *u, struct dtab *weapons_dtab);

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

/* --- Unit status --- */
void Unit_Status_Add(      struct Unit *u, struct Unit_status);
i16 Unit_Status_Find(      struct Unit *u, i16 status);
void Unit_Status_Remove(   struct Unit *u, i16 i);
void Unit_Status_Decrement(struct Unit *u);
i16 Unit_Status_Find_Turns(struct Unit *unit, i16 turns);

/* --- Unit stats --- */
struct Unit_stats Unit_stats_plus(     struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_minus(    struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_div_cst(  struct Unit_stats stats,  int cst);
struct Unit_stats Unit_stats_plus_cst( struct Unit_stats stats1, int cst);
struct Unit_stats Unit_stats_minus_cst(struct Unit_stats stats,  int cst);

/* --- Mount --- */
void Unit_Mount_Mount(   struct Unit *u, i16 id);
void Unit_Mount_Bonus(   struct Unit *u, i16 id);
void Unit_Mount_Assign(  struct Unit *u, i16 id);
void Unit_Mount_Unmount( struct Unit *u, i16 id);
void Unit_Mount_Deassign(struct Unit *u, i16 id);

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

bool Range_Valid(    struct Range r1);
void Ranges_Combine( struct Range *r1, struct Range r2);
bool Range_toCombine(struct Unit *u, struct Weapon *w);


/* --- Rangemap --- */
int  Unit_Rangemap_Get(      struct Unit *u);
void Unit_Rangemap_Toggle(   struct Unit *u);
void Unit_Rangemap_Default(  struct Unit *u);
void Unit_Rangemap_Equipment(struct Unit *u);

/* -- Equipment Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/
struct Range *Unit_Range_Combine(          struct Unit *u, bool eq);
struct Range *Unit_Range_Combine_Staves(   struct Unit *u, bool eq);
struct Range *Unit_Range_Combine_Weapons(  struct Unit *u, bool eq);
struct Range *Unit_Range_Combine_Equipment(struct Unit *u);

struct Range *_Unit_Range_Combine(struct Unit  *u, struct Range *r, bool e, int a);

/* -- Loadout Range -- */
/* Compute range of loadout:
*  - Range of weapon in strong hand
*   - UNLESS dual-wielding. Ranges combine, BUT only add stats if in range.
*/
struct Range *Unit_Range_Item(   struct Unit *u, int i);
struct Range *Unit_Range_Loadout(struct Unit *u);

/* -- Unit_id -- */
bool Unit_ID_Valid(u16 id);

#endif /* UNIT_H */
