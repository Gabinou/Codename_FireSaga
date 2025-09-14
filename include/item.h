#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include "structs.h"
#include "cooldown.h"
#include "globals.h"
#include "nstr.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct dtab;
struct Unit;
struct cJSON;

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

typedef struct Item_Type {
    u64 top;   /* not type_exp */
    i32 sub;   /* e.g. thrust sword */
} Item_Type;

typedef struct Item {
    struct jsonIO_Header jsonio_header;

    struct Range    range;
    struct Aura     aura;   /* only if equipped */

    struct Item_IDs     ids;
    struct Item_Type    type;
    struct Item_stats   stats;
    struct Item_Users   users;
    struct Item_Flags   flags;
    struct Item_Effect  effect;
    // TODO:
    //  1- Design all weapons, check if bonuses necessary
    //  2- Remove, implement as necessary
    struct Bonus_Stats  bonus_equip;
    struct Bonus_Stats  bonus_inventory;

    char description[ITEM_DESCRIPTION_LEN];
} Item;
extern const struct Item Item_default;


/* --- ANTS --- */
#define ITEM_NAME_INVALID " "

// Note: How are Breakables categorized?
// Enemy target to all armies I guess
enum ITEM_TARGET {
    ITEM_NO_TARGET          = 0,
    ITEM_TARGET_SELF        = 1,
    ITEM_TARGET_ENEMY       = 2,
    ITEM_TARGET_FRIENDLY    = 3,
    ITEM_TARGET_OTHER       = 4, /* Anyone else */
    ITEM_TARGET_ANYONE      = 5,
    ITEM_TARGET_TILE        = 6, /* No one */
    ITEM_TARGET_NUM,
};
/* --- Inventory Item --- */
void Inventory_item_Swap(struct Inventory_item *items, u8 i1, u8 i2);

/* --- Constructors/Destructors --- */
void Item_Free(struct Item *item);

/* --- I/O --- */
// Note: all input dtab to be more general.
//  - Can load from gl_dtab, or local dtab
void Item_Load(      struct dtab *items_dtab, i16 id);
void Item_Reload(    struct dtab *items_dtab, i16 id);
void Item_All_Load(  struct dtab *items_dtab);
void Item_All_Reload(struct dtab *items_dtab);

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
b32 Item_isStaff(  i16 id);
b32 Item_isShield( i16 id);
b32 Item_isWeapon( i16 id);
b32 Item_isOffhand(i16 id);

b32 Unit_canUse_Item(   const struct Item *item,
                        const struct Unit *unit);
b32 Item_couldbeUsed(const Item *item);

/* --- Use --- */
void Item_Use(  struct Item *i,
                struct Unit *u,
                struct Unit *t);
i32 Pure_Item_Uses(   const Item *i,
                      const Inventory_item *inv);
i32 Item_Uses(i32 id, const Inventory_item *inv);
void Inventory_item_Break(  Inventory_item  *invitem);
void Inventory_item_Deplete(Inventory_item  *invitem,
                            Item            *item);

/* --- Check --- */
/* Check if id is of a *pure* item */
b32 Item_ID_isValid(u16 id);

/* --- Stat --- */
int Item_Stat(const struct Item *const item, i16 s);

/* -- Effects -- */
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

#endif /* ITEM_H */
