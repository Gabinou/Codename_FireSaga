#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include "physfs.h"
#include "cJSON.h"
#include "utilities.h"
#include "string.h"
#include "structs.h"
#include "flecs.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

struct Unit;

typedef struct Item {
    uint8_t json_element;
    struct Unit_stats bonus_stats;
    struct Unit_stats malus_stats;
    uint16_t price;
    uint64_t use_power; // for heal (%), blowHorn, gainStats, gainSkill
    // largest contained int is Skill->uint32_t
    bool sellable;
    bool equippable; // unused by weapons
    uint16_t id; // 0 means empty.
    uint16_t type;
    uint64_t passive_effect;
    int8_t (* use_effectp)(struct Item *, struct Unit *, struct Unit *);
    uint16_t * users; // item only usable by users. empty = everyone
    uint8_t users_num;
    char * name;
    char * description;
} Item;
extern struct Item Item_default;

typedef struct ItemModule {
    ECS_DECLARE_COMPONENT(Item);
} ItemModule;

void ItemModuleImport(ecs_world_t * in_world);
#define ItemModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Item);

extern void Item_writeJSON(struct Item * in_item, cJSON * in_jtile);
extern void Item_readJSON(struct Item * in_item, cJSON * in_jtile);

// effect's specific is determined by item
// useEffect_heal's heal, gainSkill's skill, gain Stats' stat....
extern int8_t useEffect_heal(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_divineShield(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_blowHorn(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_promote(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_lvlUp(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_gainSkill(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_gainStats(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_callPegasus(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);
extern int8_t useEffect_callHorse(struct Item * in_item, struct Unit * in_user, struct Unit * in_target);

#endif /* ITEM_H */
