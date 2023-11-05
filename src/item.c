
#include "item.h"

struct Item Item_default = {
    .json_element   = JSON_ITEM,
    .json_filename  = NULL,

    .bonus_stats  = {0},
    .malus_stats  = {0},
    .id           = 0, // 0 means empty.
    .type         = 0,
    .stats        = {1000, 10, 10},
    .target       = ITEM_TARGET_ENEMY,
    .range        = {0, 1},
    .passive      = 0,
    .active       = NULL,
    .users        = NULL, /* item only usable by users.   empty = everyone */
    .classes      = NULL, /* item only usable by classes. empty = everyone */
    .canUse       = true,
    .canSell      = true,
    .canRepair    = false,
};

#define REGISTER_ENUM(x, y) ITEM_EFFECT_ID_##x,
i16 item_effect_ids[ITEM_EFFECT_NUM] = {
    ITEM_EFFECT_NULL,
#include "names/items_effects.h"
};

use_function_t item_effect_funcs[ITEM_EFFECT_NUM] = {
    /* NULL */                 NULL,
    /* STAFF_HEAL */           useEffect_STAFF_HEAL,
    /* KILL1P */               NULL,
    /* BRAVE2X */              NULL,
    /* BRAVE3X */              NULL,
    /* BRAVE4X */              NULL,
    /* BRAVE7X */              NULL,
    /* NOCOUNTER */            NULL,
    /* IGNORE_DEF */           NULL,
    /* IGNORE_RES */           NULL,
    /* IGNORE_SHIELD */        NULL,
    /* USE_HEAL */             NULL,
    /* USE_BUFF */             NULL,
    /* USE_DIVINE_SHIELD */    NULL,
    /* NO_CRIT */              NULL,
    /* NO_ATTACK */            NULL,
    /* CHARM */                NULL,
    /* IMMUNE_MAGIC */         NULL,
    /* IMMUNE_PHYS */          NULL,
    /* COUP_DE_GRACE */        NULL,
    /* BREAK_WEAPON */         NULL,
    /* EXP2X */                NULL,
    /* LIFESTEAL */            NULL,
    /* POISON */               NULL,
    /* CURSED */               NULL,
    /* MASOCHISM */            NULL,
    /* STUN */                 NULL,
    /* PASS */                 NULL,
    /* JOUST */                NULL,
    /* SPARE */                NULL,
    /* CAPTURE */              NULL,
    /* HALF_DMG */             NULL,
    /* BREAK_SHIELD */         NULL,
    /* MAGIC_SHIELD */         NULL,
    /* RESTORE */              NULL,
    /* KILLY_CIBO_PAIR */      NULL,
    /* NO_KILL */              NULL,
    /* SHIELD_SPIKE */         NULL,
    /* ANGELIC */              NULL,
    /* DEMONIC */              NULL,
    /* ELEMENTAL */            NULL,
    /* BLOW_HORN */            NULL,
    /* CURE_STATUSES */        NULL,
    /* PROMOTION */            NULL,
    /* REPAIR */               NULL,
    /* REPAIR_MAGIC */         NULL,
    /* REPAIR_BOWS */          NULL,
    /* AOE_HEAL */             NULL,
    /* HOOK */                 NULL,
    /* USE_PROMOTE */          NULL,
    /* USE_LVL_UP */           NULL,
    /* USE_GAIN_SKILL */       NULL,
    /* USE_GAIN_STATS */       NULL,
    /* CALL_PEGASUS */         NULL,
    /* CALL_HORSE */           NULL,
};

/* --- USE_EFFECTS --- */
i8 useEffect_STAFF_HEAL(struct Item *restrict item,
                        struct Unit *restrict user,
                        struct Unit *restrict target) {
    // HEALING ITEMS CAN BE USED ON OTHER UNITS/PEGASUSES/ENEMIES.
    u8 healing = Equation_Staff_Healing(item->stats.AP, user->current_stats.mag);
    Unit_getsHealed(target, healing);
    return (-1);
}

i8 useEffect_USE_DIVINE_SHIELD(struct Item *restrict item,
                               struct Unit *restrict user,
                               struct Unit *restrict target) {
    target->divine_shield = true;
    return (-1);
}

i8 useEffect_BLOW_HORN(struct Item *restrict item,
                       struct Unit *restrict user,
                       struct Unit *restrict target) {
    return (-1);
}

i8 useEffect_USE_PROMOTE(struct Item *restrict item,
                         struct Unit *restrict user,
                         struct Unit *restrict target) {
    // Unit_Promote(unit, i8 new_class_index);
    return (-1);
}

i8 useEffect_USE_LVL_UP(struct Item *restrict item,
                        struct Unit *restrict user,
                        struct Unit *restrict target) {
    Unit_lvlUp(user);
    return (-1);
}

i8 useEffect_USE_GAIN_SKILL(struct Item *restrict item,
                            struct Unit *restrict user,
                            struct Unit *restrict target) {
    target->skills += item->stats.AP;
    return (-1);
}

i8 useEffect_USE_GAIN_STATS(struct Item *restrict item,
                            struct Unit *restrict user,
                            struct Unit *restrict target) {
    switch (item->id) {
        case ITEM_ID_TALISMAN_HP:
            target->current_stats.hp += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_STR:
            target->current_stats.str += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_MAG:
            target->current_stats.mag += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_DEX:
            target->current_stats.dex += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_AGI:
            target->current_stats.agi += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_LUCK:
            target->current_stats.luck += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_DEF:
            target->current_stats.def += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_RES:
            target->current_stats.res += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_CON:
            target->current_stats.con += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_PROF:
            target->current_stats.prof += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_MOVE:
            target->current_stats.move += item->stats.AP;
            break;
        default:
            SDL_Log("Wrong id for stat booster.");
            exit(ERROR_Generic);
    }
    return (-1);
}

i8 useEffect_CALL_PEGASUS(struct Item *restrict item,
                          struct Unit *restrict user,
                          struct Unit *restrict target) {
    return (-1);
}

i8 useEffect_CALL_HORSE(struct Item *restrict item,
                        struct Unit *restrict user,
                        struct Unit *restrict target) {
    return (-1);
}

/* --- ITEM --- */
void Inventory_item_Swap(struct Inventory_item *restrict items, u8 i1, u8 i2) {
    struct Inventory_item buffer = items[i1];
    items[i1] = items[i2];
    items[i2] = buffer;
}

void Inventory_item_Deplete(struct Inventory_item *restrict inventory_item, int uses) {
    /* Decrease Durability */
    inventory_item->used++;
    if (inventory_item->used >= uses)
        Inventory_item_Break(inventory_item);
}

void Inventory_item_Break(struct Inventory_item *restrict inventory_item) {
    // TODO: Game animation/notification of some kind.
    *inventory_item = Inventory_item_broken;
}

bool Item_ID_isValid(u16 id) {
    bool valid = false;
    valid |= ((id > ITEM_ID_ITEM_START) && (id < ITEM_ID_ITEM_END));
    valid |= (id == ITEM_ID_BROKEN);
    return (valid);
}

bool Item_canUse(struct Item *restrict item, const struct Unit *restrict unit) {
    /* ITEM Checks if current unit can use item. GAME checks if target is in range */
    SDL_assert(item != NULL);
    SDL_assert(unit != NULL);

    /* Item has effect, no specfic users, can use */
    bool has_effect = (item->active != NULL);

    /* Check if unit is a user */
    bool is_user  = false;
    if (item->users == NULL) {
        is_user = true;
    } else {
        /* Check if unit is user */
        for (size_t i = 0; i < DARR_NUM(item->users); i++) {
            if (unit->_id == item->users[i]) {
                is_user = true;
                break;
            }
        }
    }

    /* Check if unit class is in the classes */
    bool is_class = false;
    if (item->classes == NULL) {
        is_class = true;
    } else {
        /* Check if unit is user */
        for (size_t i = 0; i < DARR_NUM(item->classes); i++) {
            if (unit->class == item->classes[i]) {
                is_class = true;
                break;
            }
        }
    }

    return (item->canUse = (has_effect && is_user && is_class));
}

void Item_Use(struct Item *restrict item, struct Unit *restrict user,
              struct Unit *restrict targets) {
    /* Game takes charge of uses-- */
    SDL_assert(item != NULL);
    SDL_assert(item->active != NULL);
    for (i16 i = 0; i < DARR_NUM(targets); i++)
        item->active(item, user, &targets[i]);
}

/* --- I/O --- */
s8 Item_Filename(s8 filename, i16 id) {
    char buffer[DEFAULT_BUFFER_SIZE] = {0};
    char *token;

    /* - add item name to filename - */
    size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);
    SDL_assert(item_order != 0);
    memcpy(buffer, global_itemNames[item_order].data, global_itemNames[item_order].num);
    token = strtok(buffer, " \t");
    while (token != NULL) {
        filename    = s8cat(filename, s8_var(token));
        token       = strtok(NULL, " \t");
    }

    /* - add .json to filename - */
    filename = s8cat(filename, s8_literal(".json"));
    return (filename);
}

void Item_Reload(struct dtab *items_dtab, i16 id) {
    /* Overwrite item ONLY if it already exists */
    if (DTAB_GET(items_dtab, id) != NULL) {
        Item_Free(DTAB_GET(items_dtab, id));
        DTAB_DEL(items_dtab, id);
        Item_Load(items_dtab, id);
    }
}


/* Loads only pure items */
void Item_Load(struct dtab *items_dtab, i16 id) {
    SDL_assert(Item_ID_isValid(id));
    SDL_assert(items_dtab != NULL);

    if (DTAB_GET(items_dtab, id) != NULL) {
        Weapon_Free(DTAB_GET(items_dtab, id));
        DTAB_DEL(items_dtab, id);
    }

    s8 filename = s8_mut("items"PHYSFS_SEPARATOR"Item"PHYSFS_SEPARATOR);
    filename    = Item_Filename(filename, id);

    struct Item temp_item = Item_default;
    SDL_Log("Loading item %ld %s", id, filename.data);
    SDL_assert(temp_item.json_element == JSON_ITEM);

    /* - read weapon - */
    jsonio_readJSON(filename.data, &temp_item);

    /* - Add weapon to dtab - */
    DTAB_ADD(items_dtab, &temp_item, id);

    s8_free(&filename);
}

void Item_All_Load(struct dtab *items_dtab) {
    for (size_t i = ITEM_ID_ITEM_START; i < ITEM_ID_ITEM_END; i++) {
        SDL_Log("%zu", i);
        if (Item_ID_isValid(i))
            Item_Load(items_dtab, i);
    }
}

void Item_All_Reload(struct dtab *items_dtab) {
    for (size_t i = ITEM_ID_ITEM_START; i < ITEM_ID_ITEM_END; i++) {
        SDL_Log("%zu", i);
        if (Item_ID_isValid(i))
            Item_Reload(items_dtab, i);
    }
}

void Item_writeJSON(const void *restrict input, cJSON *restrict jitem) {
    /* - Preliminaries - */
    const struct Item *item = input;
    SDL_assert(jitem != NULL);
    cJSON *jid        = cJSON_CreateNumber(item->id);
    cJSON *jname      = cJSON_CreateString(item->name.data);
    cJSON *jbonus     = cJSON_CreateObject();
    cJSON *jmalus     = cJSON_CreateObject();
    cJSON *jcanSell   = cJSON_CreateBool(item->canSell);
    cJSON *jusers     = cJSON_CreateObject();
    cJSON *jusers_ids = cJSON_CreateArray();
    cJSON *jusers_id  = NULL;
    cJSON *jclass     = cJSON_CreateObject();
    cJSON *jclass_ids = cJSON_CreateArray();
    cJSON *jclass_id  = NULL;

    Filesystem_writeJSON_Unitstats(jmalus, &(item->malus_stats));
    Filesystem_writeJSON_Unitstats(jbonus, &(item->bonus_stats));

    /* - Users - */
    if (item->users != NULL) {
        for (i16 i = 0; i < DARR_NUM(item->users); i++) {
            jusers_id = cJSON_CreateNumber(item->users[i]);
            cJSON_AddItemToArray(jusers_ids, jusers_id);
        }
    }
    cJSON_AddItemToObject(jusers, "id", jusers_ids);

    /* - Classes - */
    if (item->classes != NULL) {
        for (i16 i = 0; i < DARR_NUM(item->classes); i++) {
            jclass_id = cJSON_CreateNumber(item->classes[i]);
            cJSON_AddItemToArray(jclass_ids, jclass_id);
        }
    }
    cJSON_AddItemToObject(jclass, "id", jclass_ids);

    /* - Passive effects - */
    cJSON *jpassives = cJSON_CreateObject();
    cJSON *jpassive = NULL;
    jpassive = cJSON_CreateNumber(item->passive);
    cJSON_AddItemToObject(jpassives, "id", jpassive);
    s8 *effects = Names_wpnEffects(item->passive);
    for (i16 i = 0; i < DARR_NUM(effects); i++) {
        jpassive = cJSON_CreateString(effects[i].data);
        cJSON_AddItemToObject(jpassives, "Effect", jpassive);
        s8_free(&effects[i]);
    }
    DARR_FREE(effects);

    /* - Types - */
    cJSON *jtypes = cJSON_CreateObject();
    cJSON *jtype2 = NULL;
    jtype2 = cJSON_CreateNumber(item->type);
    cJSON_AddItemToObject(jtypes, "id", jtype2);
    s8 *types = Names_wpnType(item->type);
    for (i16 i = 0; i < DARR_NUM(types); i++) {
        jtype2 = cJSON_CreateString(types[i].data);
        cJSON_AddItemToObject(jtypes, "Type", jtype2);
        s8_free(&types[i]);
    }
    DARR_FREE(types);

    /* -- Adding to JSON -- */
    cJSON_AddItemToObject(jitem,   "Name",        jname);
    cJSON_AddItemToObject(jitem,   "id",          jid);
    cJSON_AddStringToObject(jitem, "Description", item->description);
    cJSON_AddItemToObject(jitem,   "Bonus",       jbonus);
    cJSON_AddItemToObject(jitem,   "Users",       jusers);
    cJSON_AddItemToObject(jitem,   "Class",       jclass);
    cJSON_AddItemToObject(jitem,   "Malus",       jmalus);
    cJSON_AddItemToObject(jitem,   "canSell",     jcanSell);
    cJSON_AddItemToObject(jitem,   "Effects",     jpassives);
    cJSON_AddItemToObject(jitem,   "Types",       jtypes);

    /* - Writing stats - */
    if (item->write_stats) {
        cJSON *jstats = cJSON_CreateObject();
        const struct Item_stats *stats = &(item->stats);
        Filesystem_writeJSON_Itemstats(jstats, stats);
        cJSON_AddItemToObject(jitem, "Stats", jstats);
    }
}

void Item_readJSON(void *input, const cJSON *jitem) {
    SDL_assert(jitem != NULL);

    /* - Preliminaries - */
    struct Item *item = (struct Item *)input;
    Item_Free(item);
    cJSON *jname        = cJSON_GetObjectItemCaseSensitive(jitem,    "Name");
    cJSON *jid          = cJSON_GetObjectItemCaseSensitive(jitem,    "id");
    cJSON *jdescription = cJSON_GetObjectItemCaseSensitive(jitem,    "Description");
    cJSON *jbonus_stats = cJSON_GetObjectItemCaseSensitive(jitem,    "Bonus");
    cJSON *jmalus_stats = cJSON_GetObjectItemCaseSensitive(jitem,    "Malus");
    cJSON *jcanSell     = cJSON_GetObjectItemCaseSensitive(jitem,    "canSell");
    cJSON *jcanRepair   = cJSON_GetObjectItemCaseSensitive(jitem,    "canRepair");
    cJSON *jusers       = cJSON_GetObjectItemCaseSensitive(jitem,    "Users");
    cJSON *jstats       = cJSON_GetObjectItemCaseSensitive(jitem,    "Stats");
    cJSON *jeffects     = cJSON_GetObjectItemCaseSensitive(jitem,    "Effects");
    cJSON *jpassive     = cJSON_GetObjectItemCaseSensitive(jeffects, "id");
    cJSON *jactive      = cJSON_GetObjectItemCaseSensitive(jeffects, "active");
    cJSON *jprice       = cJSON_GetObjectItemCaseSensitive(jitem,    "Price");
    cJSON *jtypes       = cJSON_GetObjectItemCaseSensitive(jitem,    "Types");
    cJSON *jtypeid      = cJSON_GetObjectItemCaseSensitive(jtypes,   "id");
    item->id            = cJSON_GetNumberValue(jid); /* returns 0 if junit is NULL */


    /* - Users - */
    cJSON *jusers_ids = cJSON_GetObjectItem(jusers, "id");
    struct cJSON *jusers_id;
    SDL_assert(item->id > 0);
    SDL_assert(global_itemOrders != NULL);
    item->users = DARR_INIT(item->users, u16, 16);
    if (jusers_ids  != NULL) {
        cJSON_ArrayForEach(jusers_id, jusers_ids) {
            DARR_PUT(item->users, cJSON_GetNumberValue(jusers_id));
        }
    }

    /* - Item order - */
    SDL_assert(global_itemOrders != NULL);
    i32 item_order = *(i32 *)DTAB_GET(global_itemOrders, item->id);
    i32 max_order  = DARR_NUM(global_itemNames);
    SDL_assert((item_order > 0));
    SDL_assert(((i32)item_order < (i32)max_order));

    SDL_assert(global_itemNames[item_order].data != NULL);

    /* - Name - */
    // item->name = s8_mut(cJSON_GetStringValue(jname));
    // size_t len = strlen(global_itemNames[item_order]);
    // memcpy(item->name, global_itemNames[item_order], len);
    s8_free(&item->name);
    item->name = s8cpy(item->name, global_itemNames[item_order]);

    /* - Description - */
    char *string = cJSON_GetStringValue(jdescription);
    memcpy(item->description, string, strlen(string));

    /* - Bonus/Malus - */
    Filesystem_readJSON_Unitstats(jbonus_stats, &(item->bonus_stats));
    Filesystem_readJSON_Unitstats(jmalus_stats, &(item->malus_stats));

    /* - Effects - */
    if (jpassive != NULL)
        item->passive = cJSON_GetNumberValue(jpassive);

    if (jactive != NULL) {
        int active_order = 0, active_id = cJSON_GetNumberValue(jactive);
        for (int i = 0; i < ITEM_EFFECT_NUM; i++) {
            if (item_effect_ids[i] == active_id) {
                active_order = i;
                break;
            }
        }
        item->active = item_effect_funcs[active_order];
    }

    /* - Stats - */
    Filesystem_readJSON_Itemstats(jstats, &(item->stats));

    /* - Type - */
    if (jtypeid != NULL)
        item->type = cJSON_GetNumberValue(jtypeid);

    /* - Sellable - */
    if (jcanSell != NULL)
        item->canSell = cJSON_IsTrue(jcanSell);

    /* - Repairable - */
    if (jcanRepair != NULL)
        item->canRepair = cJSON_IsTrue(jcanRepair);
}

void Item_Free(struct Item *restrict item) {
    if (item->users != NULL) {
        DARR_FREE(item->users);
        item->users = NULL;
    }
    if (item->json_filename != NULL) {
        free(item->json_filename);
        item->json_filename = NULL;
    }

}

/* --- Is --- */
int Item_Archetype(i16 id) {
    int archetype = ITEM_ARCHETYPE_ITEM;
    if (Weapon_ID_isValid(id)) {
        if (Weapon_isStaff(id)) {
            archetype = ITEM_ARCHETYPE_STAFF;
        } else if (Weapon_isShield(id)) {
            archetype = ITEM_ARCHETYPE_SHIELD;
        } else {
            archetype = ITEM_ARCHETYPE_WEAPON;
        }
    }
    return (archetype);
}

bool Item_isOffhand(i16  id) {
    return (Weapon_isOffhand(id));
}

bool Item_isShield(i16  id) {
    /* Must be equivalent to using shield item archetype */
    return (Weapon_isShield(id));
}

bool Item_isStaff(i16  id) {
    /* Must be equivalent to using staff item archetype */
    return (Weapon_isStaff(id));
}

bool Item_isWeapon(i16 id) {
    /* Must be equivalent to using weapon item archetype */
    return (Item_Archetype(id) == ITEM_ARCHETYPE_WEAPON);
}

int Item_Stat(const struct Item *item, i16 stattype)  {
    SDL_assert((stattype > ITEM_STAT_START) && (stattype < ITEM_STAT_END));
    u8 *item_stats_arr = (u8 *)&item->stats;
    int stat = item_stats_arr[stattype - 1];
    return (stat);
}


