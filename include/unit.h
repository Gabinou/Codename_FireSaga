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

/* --- CONSTANTS--- */
enum STATUSES {
    STATUS_DEFAULT_TURNS = 5,
};

enum UNIT_SEX {
    UNIT_SEX_F   = 0,
    UNIT_SEX_M   = 1,
    UNIT_SEX_NUM = 2
};

/* -- Unit second-order info -- */
extern uf8  army_alignment[ARMY_END];
extern bool army_isPC[ARMY_END];

/* --- UNIT STATUS --- */
struct Unit_status {
    // Number of turns to be in this state.
    // <0 means forever.
    if8 status;
    if8 turns;
};
extern struct Unit_status Unit_status_default;

/* --- UNIT STATS --- */

/* --- UNIT --- */
typedef struct Unit {
    uf8     json_element; /* JSON_ELEM_bOFFSET = 0 */
    if16    class;
    if8     mvt_type;
    if8     army;
    uf8     current_agony;
    uf8     current_hp;
    if8     handedness;
    uf16    talkable;
    if8     agony; /* turns left before death (-1 not agonizing) */
    uf8     regrets;

    // Status with least remaining turns on top.
    struct Unit_status *status_queue;

    struct Support supports[SOTA_MAX_SUPPORTS];
    uf16 support_type;
    uf16 support_num;
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

    uf16 equippable;
    uf16 base_exp;
    uf16 exp;
    uf16 _id;
    uf16 rescuee;

    if8 rangemap;
    if8 user_rangemap; /* reset to NULL when equipment changes */

    bitflag16_t job_talent;

    /* Defendant position (self is Aggressor.) */
    struct Point dft_pos; /* Used to compute stats in case of dual wielding */

    bool hands   [UNIT_HANDS_NUM];      /* Does unit have hands?     */
    bool equipped[UNIT_HANDS_NUM];      /* Is Item in hand equipped? */

    char **skill_names;

    struct dtab *weapons_dtab;
    struct dtab *items_dtab;

    /* _equipment is in side space: [left, right, 2, 3, 4, 5]   */
    /* Most functions are in side space unless stated otherwise */
    struct Inventory_item _equipment[DEFAULT_EQUIPMENT_SIZE];

    /* For twohanding when computing computedstats */
    struct Inventory_item temp;

    if8 eq_usable[DEFAULT_EQUIPMENT_SIZE];
    if8 num_equipment;
    if8 num_usable;

    struct Mount *mount;
    bool mounted;

    char name [DEFAULT_BUFFER_SIZE / 4];
    char title[DEFAULT_BUFFER_SIZE / 4];    /* Lord, Duke, etc. */
    struct Computed_Stats computed_stats;   /* Computed from Unit_Stats */
    struct Computed_Stats support_bonuses[SOTA_MAX_SUPPORTS];
    struct Computed_Stats support_bonus;

    bool sex            : 1; /* 0:F, 1:M. eg. hasPenis. */
    bool waits          : 1;
    bool is_alive       : 1;
    bool agonizes       : 1;
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
extern void Unit_Init(        struct Unit *u);
extern void Unit_Free(        struct Unit *u);
extern void Unit_InitWweapons(struct Unit *u, struct dtab *weapons_dtab);

/* --- Setters/Getters --- */
extern int Unit_Id_Strong(struct Unit *u, int i);
/* -- Item spaces -- */
/* Equipment, items                         ->    side space */
/* Menus, popups are top hand/strong hand   ->  strong space */
extern struct Inventory_item *Unit_Item_Side(  struct Unit *u, int i); /*   side space */
extern struct Inventory_item *Unit_Item_Strong(struct Unit *u, int i); /* strong space */

extern struct WpnorItem Unit_WpnorItem(struct Unit *u, int i);         /*   side space */

extern void Unit_Set_Item_Side(struct Unit *u, int i);                 /*   side space */

extern int Unit_Hand_Strong(const struct Unit *u);
extern int SotA_Hand_Strong(if8 handedness);

extern int Unit_Hand_Strong2Side(const struct Unit *unit, int i);
extern int Unit_Hand_Side2Strong(const struct Unit *unit, int i);

extern void Unit_setid(      struct Unit *u, if16 id);
extern void Unit_setStats(   struct Unit *u, struct Unit_stats stats);
extern void Unit_setStats(   struct Unit *u, struct Unit_stats stats);
extern void Unit_setBases(   struct Unit *u, struct Unit_stats stats);
extern void Unit_setClassind(struct Unit *u, if8 class_i);

extern struct Unit_stats Unit_getStats(struct Unit *u);

/* --- Items --- */
/* -- Deplete: decrease durability -- */
extern void _Unit_Item_Deplete(           struct Unit *u, int  i, int a);
extern void _Unit_Equipped_Deplete(       struct Unit *u, bool h, int a);

extern void Unit_Item_Deplete(            struct Unit *u, int i);
extern void Unit_Equipped_Staff_Deplete(  struct Unit *u, bool h);
extern void Unit_Equipped_Weapons_Deplete(struct Unit *u);
extern void Unit_Equipped_Shields_Deplete(struct Unit *u);

/* -- Trading -- */
extern void  Unit_Item_Take(  struct Unit *u, struct Inventory_item item);
extern void  Unit_Item_Swap(  struct Unit *u, if16 ind1, if16 ind2);
extern void  Unit_Item_Trade( struct Unit *g, struct Unit *t, if16 giver_i, if16 taker_i);
extern void  Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);
extern void _Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);
extern struct Inventory_item Unit_Item_Drop(struct Unit *u, if16 ind1);

/* --- Equipping --- */
extern bool                  Unit_Equip(           struct Unit *u, bool h, int i);
extern void                  Unit_Unequip(         struct Unit *u, bool h);
extern bool                  Unit_Equip_inHand(    struct Unit *u, bool h);
extern struct Inventory_item Unit_Equip_TwoHanding(struct Unit *u);

/* --- Checking Equipment: de-equip if broken --- */
extern void _Unit_Check_Equipped(struct Unit *u, bool hand);
extern void  Unit_Check_Equipped(struct Unit *u);

/* --- Supports --- */
extern void Unit_supportUp(struct Unit *u, if16 id);

/* --- Second-order info --- */
extern bool SotA_isPC(          uf8 a);
extern uf8  SotA_army2alignment(uf8 a);

extern uf8  Unit_mvtType(const struct Unit *u);

/* --- Unit status --- */
extern void Unit_Status_Add(       struct Unit *u, struct Unit_status);
extern if16 Unit_Status_Find(      struct Unit *u, if16 status);
extern void Unit_Status_Remove(    struct Unit *u, if16 i);
extern void Unit_Status_Decrement( struct Unit *u);
extern if16 Unit_Status_Find_Turns(struct Unit *unit, if16 turns);

/* --- Unit stats --- */
extern struct Unit_stats Unit_stats_plus(     struct Unit_stats stats1, struct Unit_stats stats2);
extern struct Unit_stats Unit_stats_minus(    struct Unit_stats stats1, struct Unit_stats stats2);
extern struct Unit_stats Unit_stats_div_cst(  struct Unit_stats stats,  int cst);
extern struct Unit_stats Unit_stats_plus_cst( struct Unit_stats stats1, int cst);
extern struct Unit_stats Unit_stats_minus_cst(struct Unit_stats stats,  int cst);

/* --- Mount --- */
extern void Unit_Mount_Mount(   struct Unit *u, if16 id);
extern void Unit_Mount_Bonus(   struct Unit *u, if16 id);
extern void Unit_Mount_Assign(  struct Unit *u, if16 id);
extern void Unit_Mount_Unmount( struct Unit *u, if16 id);
extern void Unit_Mount_Deassign(struct Unit *u, if16 id);

/* --- Debug --- */
extern bool Unit_Equipment_Full( const struct Unit *u);
extern void Unit_Equipment_Print(const struct Unit *u);

/* --- Usability --- */
extern bool Unit_Eq_Usable(  const struct Unit *u, int a, int i);
extern bool Unit_All_Usable(       struct Unit *u);
extern void Unit_Find_Usable(      struct Unit *u, int a);
extern bool Unit_Item_Usable(const struct Unit *u, int a, int i);

/* --- Skills --- */
extern bool Unit_hasSkill(const struct Unit *u, u64 s);

/* --- Stat computation --- */
/* Gives weapon stat if distance is in range.
*    DEBUG: input -1 to always be in_range
*/
/* Distance-dependent stats */
extern uf8 Unit_computeHit(     struct Unit *u, int dist);
extern if8 Unit_computeDodge(   struct Unit *u, int dist);
extern uf8 Unit_computeFavor(   struct Unit *u, int dist);
extern if8 Unit_computeSpeed(   struct Unit *u, int dist);
extern uf8 Unit_computeCritical(struct Unit *u, int dist);

/* Distance-independent stats */
extern if8 Unit_computeMove(    struct Unit *u);
extern uf8 Unit_computeAgony(   struct Unit *u);
extern if8 Unit_computeRegrets( struct Unit *u);
extern uf8 Unit_computeEffectivefactor(struct Unit *a, struct Unit *d);

extern uf8 *Unit_computeAttack( struct Unit *u, int dist);
extern uf8 *Unit_computeDefense(struct Unit *u);

extern struct Unit_stats Unit_effectiveStats(struct     Unit *u);
extern struct Unit_stats Unit_effectiveGrowths(struct   Unit *u);
extern struct Computed_Stats Unit_supportBonus(struct   Unit *u);
extern struct Computed_Stats Unit_computedStats(struct  Unit *u, int dist);
extern struct Computed_Stats Unit_computedStats_wLoadout(struct Unit *u, int lh, int rh, int dist);

/* --- Bonus/Malus from items --- */
/* Bonus and Malus should always be computable from skills, equipment, stats */
extern if8 Unit_computeMalus(struct Unit *u);
extern if8 Unit_computeBonus(struct Unit *u);

/* --- Loadout Manipulation --- */
extern bool Unit_Loadout_twoHanding(int lh, int rh);

/* - Public: Chooses between _Unit_Loadout_Swap_Twohanding and _Unit_Loadout_Swap - */
extern void Unit_Loadout_Swap(struct Unit *u, int lh, int rh);
extern void Unit_Loadout_Swap_Reverse(struct Unit *u, int lh, int rh);

/* - Private: Only for not twohanding - */
extern void _Unit_Loadout_Swap_Reverse(struct Unit *u, int lh, int rh);
extern void _Unit_Loadout_Swap(struct Unit *u, int lh, int rh);

/* - Private: Only for twohanding - */
extern void _Unit_Loadout_Swap_Twohanding(        struct Unit *u, int i);
extern void _Unit_Loadout_Swap_Reverse_Twohanding(struct Unit *u);

/* --- Debug Utils --- */
extern void Unit_Cap_Stats(struct       Unit *u);
extern void Unit_HalfCap_Stats(struct   Unit *u);

/* --- Unit State change --- */
extern void Unit_wait(struct        Unit *u);
extern void Unit_dies(struct        Unit *u);
extern void Unit_refresh(struct     Unit *u);
extern void Unit_agonizes(struct    Unit *u);

extern void Unit_getsHealed(struct  Unit *u, uf8 healing);
extern void Unit_takesDamage(struct Unit *u, uf8 dmg, bool ct);

/* --- I/O --- */
extern void Unit_readJSON(void        *u, const cJSON *const junit);
extern void Unit_writeJSON(const void *u,       cJSON       *junit);

/* --- Unit state --- */
extern bool Unit_isdualWielding(struct Unit *u);
// WrongHanding: using shield or offhand in strong hand and vice constvdrsa
extern bool Unit_iswrongHanding(struct Unit *u);

/* --- Unit Can --- */
extern bool Unit_canDance(struct Unit *u);

/* -- Can Carry -- */
extern bool Unit_canCarry(struct Unit *u1, struct Unit *u2);

/* -- Can Equip -- */
extern bool Unit_canEquip(       const struct Unit *u, if16 id);
extern bool Unit_canEquip_Type(  const struct Unit *u, if16 id);
extern bool Unit_canEquip_Hand(  const struct Unit *u, if16 id, bool h);
extern bool Unit_canEquip_inHand(const struct Unit *u, bool h);

/* -- Can Attack -- */
extern bool _Unit_canAttack(   struct Unit *u, bool hand);  /* with weapon in hand       */
extern bool  Unit_canAttack(   struct Unit *u);             /* with equipped weapon      */
extern bool  Unit_canAttack_Eq(struct Unit *u);             /* with any wpn in equipment */

/* -- Can Staff -- */
extern int Unit_canStaff(        const struct Unit *u);
extern int Unit_canStaff_Eq(     const struct Unit *u);
extern int Unit_canStaff_oneHand(const struct Unit *u);

extern uf8 Unit_Brave(const struct Unit *u);

/* --- Lvlup && Promotion --- */
extern void Unit_lvlUp(  struct Unit *u);
extern if16 Unit_getLvl( struct Unit *u);
extern void Unit_Promote(struct Unit *u, if8 new_class_i);

extern bool Range_Valid(struct Range r1);
extern struct Range Ranges_Combine(struct Range *r1, struct Range r2);
extern bool Range_toCombine(struct Unit *u, struct Weapon *w);


/* --- Rangemap --- */
extern int  Unit_Rangemap_Get(      struct Unit *u);
extern void Unit_Rangemap_Toggle(   struct Unit *u);
extern void Unit_Rangemap_Default(  struct Unit *u);
extern void Unit_Rangemap_Equipment(struct Unit *u);

/* -- Equipment Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/
extern struct Range *Unit_Range_Combine(          struct Unit *u, bool eq);
extern struct Range *Unit_Range_Combine_Staves(   struct Unit *u, bool eq);
extern struct Range *Unit_Range_Combine_Weapons(  struct Unit *u, bool eq);
extern struct Range *Unit_Range_Combine_Equipment(struct Unit *u);

extern struct Range *_Unit_Range_Combine(const struct Unit  *u, struct Range *r, bool e, int a);

/* -- Loadout Range -- */
/* Compute range of loadout:
*  - Range of weapon in strong hand
*   - UNLESS dual-wielding. Ranges combine, BUT only add stats if in range.
*/
extern struct Range *Unit_Range_Item(   struct Unit *u, int i);
extern struct Range *Unit_Range_Loadout(struct Unit *u);

#endif /* UNIT_H */
