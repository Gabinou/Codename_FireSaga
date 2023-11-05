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
typedef i8(* use_function_t)(struct Item *, struct Unit *, struct Unit *);

typedef struct Item {
    u8      json_element;  /* JSON_ELEM_bOFFSET = 0 */
    // TODO: Change to s8
    char   *json_filename; /* JSON_ELEM_bOFFSET = 1 */

    u8 target;  /* units on which item is usable. */
    struct Range range;

    struct Unit_stats bonus_stats;
    struct Unit_stats malus_stats;

    struct Item_stats stats;
    u16  id;           /* 0 is NULL */
    u16  type;         /* and not type_exp */
    u16 *users;        /* item only usable by users.   NULL -> everyone */
    u16 *classes;      /* item only usable by classes. NULL -> everyone */

    /* -- Item Effects -- */
    u64 passive;

    /* Use function is used for Staves effects too. */
    use_function_t active; /* NULL if not usable */

    // TODO: Change to s8
    s8 name;
    char description[DEFAULT_BUFFER_SIZE * 2];

    bool canSell     : 1;
    bool write_stats : 1;
    bool canUse      : 1;
    bool canRepair   : 1; /* TODO: Move to weapon? */
} Item;
extern struct Item Item_default;

/* --- Inventory Item --- */
void Inventory_item_Swap(struct Inventory_item *items, u8 i1, u8 i2);

/* --- Constructors/Destructors --- */
void Item_SDL_free(struct Item *item);

/* --- I/O --- */
void Item_Load(      struct dtab *items_dtab, i16 id);
void Item_Reload(    struct dtab *items_dtab, i16 id);
void Item_All_Load(  struct dtab *items_dtab);
void Item_All_Reload(struct dtab *items_dtab);

s8 Item_Filename(s8 filename, i16 id);

void Item_readJSON(       void *input, const cJSON *jitem);
void Item_writeJSON(const void *input,       cJSON *jitem);

/* --- Type --- */
int Item_Archetype(i16 id);

/* --- Is --- */
bool Item_isStaff(  i16 id);
bool Item_isShield( i16 id);
bool Item_isWeapon( i16 id);
bool Item_isOffhand(i16 id);

bool Item_canUse(struct Item *item, const struct Unit *unit);

/* --- Use --- */
void Item_Use(              struct Item *i, struct Unit *u, struct Unit *t);
void Inventory_item_Break(  struct Inventory_item *invitem);
void Inventory_item_Deplete(struct Inventory_item *invitem, int uses);

/* --- Check --- */
bool Item_ID_isValid(u16 id); /* NOT for weapons */

/* --- Stat --- */
int Item_Stat(const struct Item *item, i16 s);

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

#define REGISTER_ENUM(x, y) i8 useEffect_##x(struct Item *i, struct Unit *u, struct Unit *t);
#include "names/items_effects.h"
#undef REGISTER_ENUM

extern use_function_t item_effect_funcs[ITEM_EFFECT_NUM];
extern i16           item_effect_ids  [ITEM_EFFECT_NUM];


#endif /* ITEM_H */
