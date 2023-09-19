
#include "enums.h"

#define REGISTER_ENUM(x) const u64 UNIT_SKILL_##x = 1ULL << UNIT_SKILL_EXP_##x;
#include "names/skills_passive.h"
#include "names/skills_active.h"
#undef REGISTER_ENUM

// #define REGISTER_ENUM(x,y) const u64 ITEM_EFFECT_##x = 1ULL << ITEM_EFFECT_EXP_##x;
#define REGISTER_ENUM(x,y) const u64 ITEM_EFFECT_##x = ITEM_EFFECT_EXP_##x;
#include "names/items_effects.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) const u64 UNIT_PC_ALIVE_##x = 1ULL << UNIT_ID_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) const u64 UNIT_PC_RECRUITED_##x = 1ULL << UNIT_ID_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

int dtabs_id[ITEM_ARCHETYPE_NUM] = {
    /* ITEM_ARCHETYPE_ITEM   */ SOTA_DTAB_ITEMS,
    /* ITEM_ARCHETYPE_STAFF  */ SOTA_DTAB_WEAPONS,
    /* ITEM_ARCHETYPE_WEAPON */ SOTA_DTAB_WEAPONS,
    /* ITEM_ARCHETYPE_SHIELD */ SOTA_DTAB_WEAPONS
};

/* Direction array index from ternary index */
int direction_arr_i[SOTA_DIRECTIONS_NUM + 1] = {
    /* SOTA_DIRECTION_NULL */        -1,
    /* SOTA_DIRECTION_LEFT */         2,
    /* SOTA_DIRECTION_RIGHT */        0,
    /* SOTA_DIRECTION_BOTTOM */       3,
    /* SOTA_DIRECTION_DIAGONAL_BL */  7,
    /* SOTA_DIRECTION_DIAGONAL_BR */  6,
    /* SOTA_DIRECTION_TOP */          1,
    /* SOTA_DIRECTION_DIAGONAL_TL */  5,
    /* SOTA_DIRECTION_DIAGONAL_TR */  4,
};


// #define REGISTER_ENUM(x, y) const u64 UNIT_NPC_ALIVE_##x = 1ULL << (UNIT_ID_##x - UNIT_ID_NPC_START);
// #include "names/units_NPC.h"
// #undef REGISTER_ENUM