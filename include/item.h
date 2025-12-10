#ifndef ITEM_H
#define ITEM_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
**  Items:
**      1. Occupy equipment slots (InvItem)
**      2. Have limited uses #
**      3. Need to be equipped to be used
**      4. May have stats, users, active/passive effects...
**  Many things are items:
**      1. Weapons      are items
**      2. Staves       are items
**      3. Pure items   are items
**          * Staves are not pure items
**  Item vs InvItem:
**      1.
**          Item:       constant, shared            stats
**          InvItem:    variable, instance-specific stats
**      2.
**          Item:       only one in gl_items_dtab, for reference
**          InvItem:    in equipment, trade, buy, sell, etc.
** todo:rn  Item    /   InvItem 
**          RefItem /   Item
**          Item    /   
*/

#include "nstr.h"
#include "aura.h"
#include "enums.h"
#include "cJSON.h"
#include "structs.h"
#include "globals.h"
#include "cooldown.h"

/* --- FORWARD DECLARATIONS --- */
struct dtab;
struct Unit;
struct Item;
struct Game;
struct cJSON;

/* --- USAGE CRITERIA FUNC DEFINITIONS --- */
// full: depends on game state
typedef b32 (*item_CanUse_full_t)( struct Game  *IES,
                                   struct Unit  *user,
                                   struct Unit  *target,
                                   struct Item  *item);


/* --- STRUCT DEFINITIONS --- */
typedef struct Item_Users {
    i32 *id;        /* NULL -> everyone */
    i32 *class;     /* NULL -> every class */
} Item_Users;

typedef struct Item_Flags {
    /* Note:
    - One handed item CAN'T be two handed
    - Two handed item CAN   be one handed, with Malus
    */
    i32 handedness;

    /* --- Override flags --- */
    b32 canSell;
    b32 canUse;
    b32 canRepair;

    b32 repairEveryChapter;

    /* id of CanUse function */
    i32 canUse_Full;

} Item_Flags;

typedef struct Item_Effect {
    /* 1. All effects need item equipped            */
    /* 2. Passives excluding auras. Examples:       */
    /*  - Holder gets a skill                       */
    /*  - Holder gets a status                      */
    /*  - Holder gets cleansed (rm statuses)        */
    /*  - Holder loses/gains HP every turn          */
    /*  - Holder gains divine shield every turn     */
    u64 passive;
    i32 active; /* e.g. use */
} Item_Effect;

typedef struct Item_IDs {
    i32 id;
    i32 target;
} Item_IDs;

typedef struct Item {
    struct jsonIO_Header jsonio_header;

    /* Item.range: for use, staff, effects (mostly active). */
    //  - Some weapons are usable, so item & wpn need range.
    //      - Ex: magic sword that can be used to heal others.
    //  - effects exclude auras.
    struct Range    range;

    struct Aura     aura; /* only if equipped */

    struct Item_IDs     ids;
    struct Item_stats   stats;
    struct Item_Users   users;
    struct Item_Flags   flags;
    struct Item_Effect  effect;

    // TODO:
    //  1- Design all weapons, check if bonuses necessary
    struct Bonus_Stats  bonus_equip;
    struct Bonus_Stats  bonus_inventory;

    char description[ITEM_DESCRIPTION_LEN];
} Item;
extern const struct Item Item_default;

/* --- CONSTANTS --- */
#define ITEM_NAME_INVALID " "

/* --- Inventory Item --- */
void InvItem_Swap(   struct InvItem *items,
                     i32 i1, i32 i2);

/* --- Constructors/Destructors --- */
void Item_Free(struct Item *item);

/* --- Getter --- */
struct Item * Item_Get(struct InvItem *items);
struct Item *_Item_Get(i32 id);

/* --- I/O --- */
void Item_Load(     i32 id);
void Item_Reload(   i32 id);

void Item_All_Load(     void);
void Item_All_Reload(   void);

b32 Item_isUnitUser(    const Item *item, const Unit *user);
b32 Unit_isUnitClass(   const Item *item, const Unit *user);

/* --- Names --- */
s8 Item_Filename(s8 filename, i32 id);
s8 Item_Name(   i32      id);
s8 InvItem_Name(InvItem *invitem);

void Item_readJSON( void *input, const cJSON *jitem);
void Item_writeJSON(const void *input, cJSON *jitem);

/* --- Type --- */
u64  Item_Type(     const struct Item *const item);
u64 _Item_Type(     i32 id);
i32  Item_Type_Exp( const struct Item *const item);
i32 _Item_Type_Exp( i32 id);

u64  Item_SubType(  const struct Item *const item);
u64 _Item_SubType(  i32 id);
u64 Item_Archetype( i32 id);
b32 Item_hasType(   const struct Item *const item, u64 type);

struct Range Item_Range(const struct Item *const item);

/* --- Getters --- */
b32     Item_TwoHand_Only(  const   Item *item);
b32     Item_OneHand_Only(  const   Item *item);
i32     Item_Handedness(    const   Item *item);
void    Item_Handedness_Set(        Item *item, i32 hand);

/* --- Is --- */
b32 Item_isShield( i32 id);
b32 Item_isWeapon( i32 id);
b32 Item_isOffhand(i32 id);

b32  Item_canUse(const Item *item);
b32 _Item_canUse(i32 id);

/* --- Use --- */
void Item_Use(  const struct Item *i, struct Unit *u,
                struct Unit **t, int num);

i32 Pure_Item_remUses(  const Item *i,
                        const InvItem *inv);
i32 Item_remUses(       const InvItem *inv);

void InvItem_Break(     InvItem *invitem);
void InvItem_Deplete(   InvItem *invitem, Item *item);
void InvItem_Repair(    InvItem *invitem, i32 AP);

/* --- Check --- */
b32 Item_ID_isValid(i32 id);
b32 Item_Pure_ID_isValid(i32 id);

/* --- Stat --- */
i32          Item_Stat(     const struct Item *item, i32 s);
const i32   *Item_Stat_Arr( const struct Item *item);

/* -- Effects -- */
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y) ITEM_EFFECT_ID_##x = y,
enum ITEM_EFFECTS_ID {
    ITEM_EFFECT_NULL = 0,
#include "names/items_effects.h"
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) ITEM_EFFECT_ORDER_##x,
enum ITEM_EFFECTS_ORDER {
    ITEM_EFFECT_ZERO = 0,
#include "names/items_effects.h"
    ITEM_EFFECT_NUM,
};
#undef REGISTER_ENUM

extern const use_function_t item_effect_funcs[ITEM_EFFECT_NUM];
extern const i32            item_effect_ids  [ITEM_EFFECT_NUM];

#define REGISTER_ENUM(x, y) i32 useEffect_##x(const struct Item * const i, struct Unit *u, struct Unit *t);
#include "names/items_effects.h"
#undef REGISTER_ENUM

/* -- CanUse_Full --
** full: depends on game state.
**     Note: Item and Unit have their own canUse
*/
enum ITEM_CanUse_Full_ID {
    ITEM_CanUse_Full_NULL   = 0,
    ITEM_CanUse_Full_HP_LT  = 1,
    ITEM_CanUse_Full_NUM,
};
extern const item_CanUse_full_t item_CanUse_full[ITEM_CanUse_Full_NUM];

item_CanUse_full_t Item_CanUse_Func(i32 id);
b32 Item_CanUse_Full_HP_LT( struct Game *IES,
                            Unit        *user,
                            Unit        *target,
                            Item        *item);

#endif /* ITEM_H */
