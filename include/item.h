#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include "structs.h"
#include "nstr.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;
struct cJSON;

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

s8 Item_Filename(s8 filename, i16 id);

void Item_readJSON( void *input, const cJSON *jitem);
void Item_writeJSON(const void *input, cJSON *jitem);

/* --- Type --- */
u64 Item_Archetype(i16 id);
b32 Item_hasType(struct Item *item, u64 type);

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

extern const use_function_t item_effect_funcs[ITEM_EFFECT_NUM];
extern const i16            item_effect_ids  [ITEM_EFFECT_NUM];

#define REGISTER_ENUM(x, y) i32 useEffect_##x(struct Item *i, struct Unit *u, struct Unit *t);
#include "names/items_effects.h"
#undef REGISTER_ENUM

#endif /* ITEM_H */
