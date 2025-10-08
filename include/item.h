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
**      1. Occupy equipment slots (InventoryItem)
**      2. Have limited uses #
**      3. Need to be equipped to be used
**      4. May have stats, users, active/passive effects...
**  Many things are items:
**      1. Weapons      are items
**      2. Staves       are items
**          * Staves are not pure items
**      3. Pure items   are items
**
*/

#include "nstr.h"
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
    // TODO: DARR -> array w/ max number
    // users -> id
    u16 *id;        /* item only usable by users.   NULL -> everyone */
    // classes -> class
    u16 *class;      /* item only usable by classes. NULL -> everyone */
} Item_Users;

typedef struct Item_Flags {
    /* Note:
    - One handed item CAN'T be two handed
    - Two handed item CAN be two handed, with Malus
    */
    i32 handedness;

    b32 canSell;
    /* Note:
    ** canUse can be used to turn off canUse.
    ** Delete if uncessary. */
    b32 canUse;
    b32 canRepair;
    b32 repairEveryChapter;

    /* TODO: remove write_stats? */
    b32 write_stats;

    /* id of CanUse function */
    i32 canUse_Full;

} Item_Flags;

typedef struct Item_Effect {
    /* Note: passives excluding auras. Examples:    */
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

// TODO: delete Item_Type
// Types are derived from ID. -> function API always
typedef struct Item_Type {
    u64 top;   /* not type_exp */
    i32 sub;   /* e.g. thrust sword */
} Item_Type;

typedef struct Item {
    struct jsonIO_Header jsonio_header;

    /* Item.range: for use, staff, effects (mostly active). */
    //  - Some weapons are usable, so item & wpn need range.
    //      - Ex: magic sword that can be used to heal others.
    //  - effects exclude auras.
    struct Range    range;

    // Note: all auras need item equipped
    struct Aura     aura;/* only if equipped */

    struct Item_IDs     ids;
    struct Item_Type    type;
    struct Item_stats   stats;
    struct Item_Users   users;
    struct Item_Flags   flags;
    // Note: all effects need item equipped
    struct Item_Effect  effect;
    // TODO:
    //  1- Design all weapons, check if bonuses necessary
    //  2- Remove, implement as necessary
    struct Bonus_Stats  bonus_equip;
    struct Bonus_Stats  bonus_inventory;

    char description[ITEM_DESCRIPTION_LEN];
} Item;
extern const struct Item Item_default;

/* --- CONSTANTS --- */
#define ITEM_NAME_INVALID " "

/* --- Inventory Item --- */
void Inventory_item_Swap(   struct Inventory_item *items,
                            u8 i1, u8 i2);

/* --- Constructors/Destructors --- */
void Item_Free(struct Item *item);

/* --- Getter --- */
struct Item *Item_Get(struct Inventory_item *items);
struct Item *_Item_Get(i32 id);

/* --- I/O --- */
// Note: all input dtab to be more general.
//  - Can load from gl_dtab, or local dtab
void Item_Load(     i32 id);
void Item_Reload(   i32 id);
void Item_All_Load(void);
void Item_All_Reload(void);

b32 Unit_isItemUser(    const Item *item, const Unit *user);
b32 Unit_isItemClass(   const Item *item, const Unit *user);

/* Pure item filename */
s8 Item_Filename(s8 filename, i16 id);
/* Item name */
s8 Item_Name(i32 id);

void Item_readJSON( void *input, const cJSON *jitem);
void Item_writeJSON(const void *input, cJSON *jitem);

/* --- Type --- */
// Item -> id of struct Item OR struct Weapon
u16 Item_ID2Type(   i32 id);
u64 Item_Archetype( i32 id);
// _Item -> struct Item
u16 Item_Typecode(  const struct Item *const item);
b32 Item_hasType(   const struct Item *const item, u64 type);

struct Range Item_Range(const struct Item *const item);

/* --- Getters --- */
b32     Item_TwoHand_Only(const Item *item);
b32     Item_OneHand_Only(const Item *item);
i32     Item_Handedness(const Item *item);
void    Item_Handedness_Set(Item *item, i32 hand);

/* --- Is --- */
b32 Item_isShield( i16 id);
b32 Item_isWeapon( i16 id);
b32 Item_isOffhand(i16 id);

b32  Item_canUse(i32 id);
b32 _Item_canUse(const Item *item);

/* --- Use --- */
void Item_Use(  const struct Item *i, struct Unit *u,
                struct Unit **t, int num);
i32 Pure_Item_Uses(   const Item *i,
                      const Inventory_item *inv);
i32 Item_Uses(i32 id, const Inventory_item *inv);
void Inventory_item_Break(  Inventory_item  *invitem);
void Inventory_item_Deplete(Inventory_item  *invitem,
                            Item            *item);

/* --- Check --- */
/* Check if id is of a *pure* item */
b32 Item_Pure_ID_isValid(u16 id);

/* --- Stat --- */
int Item_Stat(const struct Item *const item, i16 s);

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
extern const i16            item_effect_ids  [ITEM_EFFECT_NUM];

#define REGISTER_ENUM(x, y) i32 useEffect_##x(const struct Item * const i, struct Unit *u, struct Unit *t);
#include "names/items_effects.h"
#undef REGISTER_ENUM

/* -- CanUse_Full -- */
/* full: depends on game state
**      Except item and unit_C. */
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
