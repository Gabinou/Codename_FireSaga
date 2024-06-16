#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include "cJSON.h"
#include "utilities.h"
#include "structs.h"
#include "unit/unit.h"
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;

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

/* --- DEFINITIONS --- */
typedef i8(* use_function_t)(struct Item *, struct Unit *, struct Unit *);

typedef struct Item {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    u8 target;  /* units by which item is usable. */
    struct Range range;
    struct Aura aura; /* Aura with range [0, 0] to for wielder-only bonus */

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

    b32 canSell     : 1;
    b32 write_stats : 1;
    b32 canUse      : 1;
    b32 canRepair   : 1; /* TODO: Move to weapon? */
} Item;
extern struct Item Item_default;

/* --- Inventory Item --- */
void Inventory_item_Swap(struct Inventory_item *items, u8 i1, u8 i2);

/* --- Constructors/Destructors --- */
void Item_Free(struct Item *item);

/* --- I/O --- */
void Item_Load(      struct dtab *items_dtab, i16 id);
void Item_Reload(    struct dtab *items_dtab, i16 id);
void Item_All_Load(  struct dtab *items_dtab);
void Item_All_Reload(struct dtab *items_dtab);

s8 Item_Filename(s8 filename, i16 id);

void Item_readJSON( void *input, cJSON *jitem);
void Item_writeJSON(void *input, cJSON *jitem);

/* --- Type --- */
u64 Item_Archetype(i16 id);

/* --- Is --- */
b32 Item_isStaff(  i16 id);
b32 Item_isShield( i16 id);
b32 Item_isWeapon( i16 id);
b32 Item_isOffhand(i16 id);

b32 Item_canUse(struct Item *item,  struct Unit *unit);

/* --- Use --- */
void Item_Use(              struct Item *i, struct Unit *u, struct Unit *t);
void Inventory_item_Break(  struct Inventory_item *invitem);
void Inventory_item_Deplete(struct Inventory_item *invitem, int uses);

/* --- Check --- */
b32 Item_ID_isValid(u16 id); /* NOT for weapons */

/* --- Stat --- */
int Item_Stat( struct Item *item, i16 s);

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
extern i16            item_effect_ids  [ITEM_EFFECT_NUM];

#endif /* ITEM_H */
