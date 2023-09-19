#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include "cJSON.h"
#include "utilities.h"
#include "structs.h"
#include "unit.h"
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;

/* --- CONSTANTS --- */
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

/* --- DEFINITIONS --- */
typedef if8(* use_function_t)(struct Item *, struct Unit *, struct Unit *);

typedef struct Item {
    uf8 json_element;   /* JSON_ELEM_bOFFSET = 0 */
    uf8 target;         /* units on which item is usable. */
    struct Range range;

    struct Unit_stats bonus_stats;
    struct Unit_stats malus_stats;

    struct Item_stats stats;
    uf16  id;           /* 0 is NULL */
    uf16  type;         /* and not type_exp */
    uf16 *users;        /* item only usable by users.   NULL -> everyone */
    uf16 *classes;      /* item only usable by classes. NULL -> everyone */

    /* -- Item Effects -- */
    u64 passive;

    /* Use function is used for Staves effects too. */
    use_function_t active; /* NULL if not usable */

    char name[DEFAULT_BUFFER_SIZE];
    char description[DEFAULT_BUFFER_SIZE * 2];

    bool canSell     : 1;
    bool write_stats : 1;
    bool canUse      : 1;
    bool canRepair   : 1; /* TODO: Move to weapon? */
} Item;
extern struct Item Item_default;

/* --- Inventory Item --- */
extern void Inventory_item_Swap(struct Inventory_item *items, uf8 i1, uf8 i2);

/* --- (De)Constructors --- */
extern void Item_Free(struct Item *item);

/* --- I/O --- */
extern void Item_Load(    struct dtab *items_dtab, if16 id);
extern void Item_readJSON(       void *input, const cJSON *jitem);
extern void Item_writeJSON(const void *input,       cJSON *jitem);

/* --- Type --- */
extern int Item_Archetype(if16 id);

/* --- Is --- */
extern bool Item_isStaff(  if16 id);
extern bool Item_isShield( if16 id);
extern bool Item_isWeapon( if16 id);
extern bool Item_isOffhand(if16 id);

extern bool Item_canUse(struct Item *item, const struct Unit *unit);

/* --- Use --- */
extern void Item_Use(              struct Item *i, struct Unit *u, struct Unit *t);
extern void Inventory_item_Break(  struct Inventory_item *invitem);
extern void Inventory_item_Deplete(struct Inventory_item *invitem, int uses);

/* --- Check --- */
extern bool Item_ID_isValid(uf16 id); /* NOT for weapons */

/* --- Stat --- */
extern int Item_Stat(const struct Item *item, if16 s);

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

#define REGISTER_ENUM(x, y) extern if8 useEffect_##x(struct Item *i, struct Unit *u, struct Unit *t);
#include "names/items_effects.h"
#undef REGISTER_ENUM

extern use_function_t item_effect_funcs[ITEM_EFFECT_NUM];
extern if16           item_effect_ids  [ITEM_EFFECT_NUM];


#endif /* ITEM_H */
