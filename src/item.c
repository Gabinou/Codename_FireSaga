
#include "item.h"
#include "names.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "aura.h"
#include "equations.h"
#include "weapon.h"
#include "platform.h"
#include "nmath.h"
#include "jsonio.h"
#include "stb_sprintf.h"

const struct Item Item_default = {
    .jsonio_header.json_element   = JSON_ITEM,
    .stats              = {1000, 10, 10},
    .target             = ITEM_TARGET_ENEMY,
    .range              = {0, 1},
    .flags.canUse       = true,
    .flags.canSell      = true,
};

#define REGISTER_ENUM(x, y) ITEM_EFFECT_ID_##x,
const i16 item_effect_ids[ITEM_EFFECT_NUM] = {
    ITEM_EFFECT_NULL,
#include "names/items_effects.h"
};

const use_function_t item_effect_funcs[ITEM_EFFECT_NUM] = {
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
i32 useEffect_STAFF_HEAL(struct Item *item,
                         struct Unit *user,
                         struct Unit *target) {
    // HEALING ITEMS CAN BE USED ON OTHER UNITS/PEGASUSES/ENEMIES.
    u8 healing = Equation_Staff_Healing(item->stats.AP, user->stats.current.mag);
    Unit_getsHealed(target, healing);
    return (-1);
}

i32 useEffect_USE_DIVINE_SHIELD(struct Item *item,
                                struct Unit *user,
                                struct Unit *target) {
    Unit_DivineShield_set(target, true);
    return (-1);
}

i32 useEffect_BLOW_HORN(struct Item *item,
                        struct Unit *user,
                        struct Unit *target) {
    return (-1);
}

i32 useEffect_USE_PROMOTE(struct Item *item,
                          struct Unit *user,
                          struct Unit *target) {
    // Unit_Promote(unit, i8 new_class_index);
    return (-1);
}

i32 useEffect_USE_LVL_UP(struct Item *item,
                         struct Unit *user,
                         struct Unit *target) {
    Unit_lvlUp(user);
    return (-1);
}

i32 useEffect_USE_GAIN_SKILL(struct Item *item,
                             struct Unit *user,
                             struct Unit *target) {
    Unit_Skill_Add(user, item->stats.AP);
    // why return -1
    return (-1);
}

i32 useEffect_USE_GAIN_STATS(struct Item *item,
                             struct Unit *user,
                             struct Unit *target) {
    switch (item->id) {
        case ITEM_ID_TALISMAN_HP:
            target->stats.current.hp += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_STR:
            target->stats.current.str += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_MAG:
            target->stats.current.mag += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_DEX:
            target->stats.current.dex += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_AGI:
            target->stats.current.agi += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_LUCK:
            target->stats.current.luck += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_DEF:
            target->stats.current.def += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_RES:
            target->stats.current.res += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_CON:
            target->stats.current.con += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_PROF:
            target->stats.current.prof += item->stats.AP;
            break;
        case ITEM_ID_TALISMAN_MOVE:
            target->stats.current.move += item->stats.AP;
            break;
        default:
            SDL_Log("Wrong id for stat booster.");
            exit(ERROR_Generic);
    }
    return (-1);
}

i32 useEffect_CALL_PEGASUS(struct Item *item,
                           struct Unit *user,
                           struct Unit *target) {
    return (-1);
}

i32 useEffect_CALL_HORSE(struct Item *item,
                         struct Unit *user,
                         struct Unit *target) {
    return (-1);
}

/* --- ITEM --- */
void Inventory_item_Swap(struct Inventory_item *items, u8 i1, u8 i2) {
    struct Inventory_item buffer = items[i1];
    items[i1] = items[i2];
    items[i2] = buffer;
}

void Inventory_item_Deplete(struct Inventory_item *inventory_item, i32 uses) {
    /* Decrease Durability */
    inventory_item->used++;
    if (inventory_item->used >= uses)
        Inventory_item_Break(inventory_item);
}

void Inventory_item_Break(struct Inventory_item *inventory_item) {
    // TODO: Game animation/notification of some kind.
    *inventory_item = Inventory_item_broken;
}

b32 Item_ID_isValid(u16 id) {
    b32 valid = false;
    valid |= ((id > ITEM_ID_ITEM_START) && (id < ITEM_ID_ITEM_END));
    return (valid);
}

b32 Item_canUse(struct Item *item,  struct Unit *unit) {
    /* ITEM Checks if current unit can use item. GAME checks if target is in range */
    SDL_assert(item != NULL);
    SDL_assert(unit != NULL);

    if (!item->flags.canUse) {
        /* Item has no active, can't use */
        return (0);
    }

    if (item->effect.active == ITEM_EFFECT_NULL) {
        /* Item has no active, can't use */
        return (0);
    }

    /* Check if unit is a user */
    if (item->users.id != NULL) {
        b32 is_user = false;
        for (size_t i = 0; i < DARR_NUM(item->users.id); i++) {
            if (Unit_id(unit) == item->users.id[i]) {
                is_user = true;
                break;
            }
        }

        if (is_user == false) {
            /* Unit id is not in user list */
            return (0);
        }
    }

    // Unit is in list of id, OR users list is NULL
    // Both -> canUse is true

    /* Check if unit class is in the classes */
    if (item->users.class != NULL) {
        b32 is_class = false;
        /* Check if correct class */
        for (size_t i = 0; i < DARR_NUM(item->users.class); i++) {
            if (Unit_Class(unit) == item->users.class[i]) {
                is_class = true;
                break;
            }
        }
        if (is_class == false) {
            /* Class id is not in class list */
            return (0);
        }
    }

    // Class is in list of classes, OR class list is NULL
    // Both -> canUse is true

    return (1);
}

// TODO: move to unit responsibility
void Item_Use(struct Item *item, struct Unit *user,
              struct Unit *targets) {
    /* Game takes charge of uses-- */
    SDL_assert(item != NULL);
    SDL_assert(item->effect.active > ITEM_EFFECT_NULL);
    use_function_t active_func = item_effect_funcs[item->effect.active];

    for (i16 i = 0; i < DARR_NUM(targets); i++) {
        active_func(item, user, &targets[i]);
    }
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
    if (!Item_ID_isValid(id))
        return;
    SDL_assert(items_dtab != NULL);

    if (DTAB_GET(items_dtab, id) != NULL) {
        Item_Free(DTAB_GET(items_dtab, id));
        DTAB_DEL(items_dtab, id);
    }

    s8 filename = s8_mut("items"PHYSFS_SEPARATOR"Item"PHYSFS_SEPARATOR);
    filename    = Item_Filename(filename, id);

    struct Item temp_item = Item_default;
    SDL_assert(temp_item.jsonio_header.json_element == JSON_ITEM);

    /* - read weapon - */
    jsonio_readJSON(filename, &temp_item);

    /* - Add weapon to dtab - */
    DTAB_ADD(items_dtab, &temp_item, id);

    s8_free(&filename);
}

void Item_All_Load(struct dtab *items_dtab) {
    for (size_t i = ITEM_ID_ITEM_START; i < ITEM_ID_ITEM_END; i++) {
        if (Item_ID_isValid(i))
            Item_Load(items_dtab, i);
    }
}

void Item_All_Reload(struct dtab *items_dtab) {
    for (size_t i = ITEM_ID_ITEM_START; i < ITEM_ID_ITEM_END; i++) {
        if (Item_ID_isValid(i))
            Item_Reload(items_dtab, i);
    }
}

void Item_writeJSON(const void *_input, cJSON *jitem) {
    /* - Preliminaries - */
    const struct Item *_item = _input;
    SDL_assert(jitem != NULL);
    cJSON *jid        = cJSON_CreateNumber(_item->id);
    cJSON *jname      = cJSON_CreateString(_item->name.data);
    cJSON *jaura      = cJSON_CreateObject();
    cJSON *jcanSell   = cJSON_CreateBool(_item->flags.canSell);
    cJSON *jusers     = cJSON_CreateObject();
    cJSON *jeffects   = cJSON_CreateObject();
    cJSON *jusers_ids = cJSON_CreateArray();
    cJSON *jusers_id  = NULL;
    cJSON *jclass     = cJSON_CreateObject();
    cJSON *jclass_ids = cJSON_CreateArray();
    cJSON *jclass_id  = NULL;

    /* - Users - */
    if (_item->users.id != NULL) {
        for (i16 i = 0; i < DARR_NUM(_item->users.id); i++) {
            jusers_id = cJSON_CreateNumber(_item->users.id[i]);
            cJSON_AddItemToArray(jusers_ids, jusers_id);
        }
    }
    cJSON_AddItemToObject(jusers, "id", jusers_ids);

    /* - Classes - */
    if (_item->users.class != NULL) {
        for (i16 i = 0; i < DARR_NUM(_item->users.class); i++) {
            jclass_id = cJSON_CreateNumber(_item->users.class[i]);
            cJSON_AddItemToArray(jclass_ids, jclass_id);
        }
    }
    cJSON_AddItemToObject(jclass, "id", jclass_ids);

    /* - Passive effects - */
    // TODO: Fix writing effects
    cJSON *jpassive = cJSON_CreateNumber(_item->effect.passive);
    cJSON_AddItemToObject(jeffects, "passive", jpassive);

    /* - Types - */
    cJSON *jtypes = cJSON_CreateObject();
    cJSON *jtype2 = NULL;
    jtype2 = cJSON_CreateNumber(_item->ids.type);
    cJSON_AddItemToObject(jtypes, "id", jtype2);
    s8 *types = Names_wpnType(_item->ids.type);
    for (i16 i = 0; i < DARR_NUM(types); i++) {
        jtype2 = cJSON_CreateString(types[i].data);
        cJSON_AddItemToObject(jtypes, "Type", jtype2);
        s8_free(&types[i]);
    }
    DARR_FREE(types);

    /* - Aura - */
    Aura_writeJSON(&(_item->aura), jaura);

    /* -- Adding to JSON -- */
    cJSON_AddItemToObject(jitem,   "Name",        jname);
    cJSON_AddStringToObject(jitem, "Description", _item->description);
    cJSON_AddItemToObject(jitem,   "id",          jid);
    cJSON_AddItemToObject(jitem,   "Aura",        jaura);
    cJSON_AddItemToObject(jitem,   "Users",       jusers);
    cJSON_AddItemToObject(jitem,   "Class",       jclass);
    cJSON_AddItemToObject(jitem,   "canSell",     jcanSell);
    cJSON_AddItemToObject(jitem,   "Effects",     jeffects);
    cJSON_AddItemToObject(jitem,   "Types",       jtypes);

    /* - Writing stats - */
    if (_item->flags.write_stats) {
        cJSON *jstats = cJSON_CreateObject();
        const struct Item_stats *_stats = &(_item->stats);
        Item_stats_writeJSON(_stats, jstats);
        cJSON_AddItemToObject(jitem, "Stats", jstats);
    }
}

void Item_readJSON(void *input, const cJSON *_jitem) {
    SDL_assert(_jitem != NULL);

    /* - Preliminaries - */
    struct Item *item = (struct Item *)input;
    Item_Free(item);
    // cJSON *jname        = cJSON_GetObjectItemCaseSensitive(_jitem,      "Name");
    cJSON *jid          = cJSON_GetObjectItemCaseSensitive(_jitem,      "id");
    cJSON *jdescription = cJSON_GetObjectItemCaseSensitive(_jitem,      "Description");
    cJSON *jaura        = cJSON_GetObjectItemCaseSensitive(_jitem,      "Aura");
    cJSON *jcanSell     = cJSON_GetObjectItemCaseSensitive(_jitem,      "canSell");
    cJSON *jcanRepair   = cJSON_GetObjectItemCaseSensitive(_jitem,      "canRepair");
    cJSON *jusers       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Users");
    cJSON *jstats       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Stats");
    cJSON *jeffects     = cJSON_GetObjectItemCaseSensitive(_jitem,      "Effects");
    cJSON *jpassive     = cJSON_GetObjectItemCaseSensitive(jeffects,    "passive");
    cJSON *jactive      = cJSON_GetObjectItemCaseSensitive(jeffects,    "active");
    cJSON *jtarget      = cJSON_GetObjectItemCaseSensitive(_jitem,      "Target");
    // cJSON *jprice       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Price");
    cJSON *jtypes       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Types");
    cJSON *jtypeid      = cJSON_GetObjectItemCaseSensitive(jtypes,      "id");

    item->id            = cJSON_GetNumberValue(jid); /* returns 0 if junit is NULL */

    /* - Users - */
    cJSON *jusers_ids = cJSON_GetObjectItem(jusers, "id");
    struct cJSON *jusers_id;
    SDL_assert(item->id > 0);
    SDL_assert(global_itemOrders != NULL);
    item->users.id = DARR_INIT(item->users.id, u16, 16);
    if (jusers_ids  != NULL) {
        cJSON_ArrayForEach(jusers_id, jusers_ids) {
            DARR_PUT(item->users.id, cJSON_GetNumberValue(jusers_id));
        }
    }

    /* - Item order - */
    SDL_assert(global_itemOrders != NULL);
    i32 item_order = *(i32 *)DTAB_GET(global_itemOrders, item->id);
    // i32 max_order  = DARR_NUM(global_itemNames);
    // SDL_Log("%d %d", item_order, max_order);
    // SDL_assert((item_order > 0));
    SDL_assert(item_order < ITEM_NUM);

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
    Aura_readJSON(&item->aura, jaura);

    /* - Effects - */
    if (jpassive != NULL)
        item->effect.passive = cJSON_GetNumberValue(jpassive);

    if (jactive != NULL) {
        i32 active_order = 0, active_id = cJSON_GetNumberValue(jactive);
        for (i32 i = 0; i < ITEM_EFFECT_NUM; i++) {
            if (item_effect_ids[i] == active_id) {
                active_order = i;
                break;
            }
        }
        item->effect.active = active_order;
    }

    /* - Target - */

    if (Weapon_ID_isValid(item->id)) {
        if (Weapon_isStaff(item->id)) {
            item->target = ITEM_TARGET_FRIENDLY;
        }
    }
    if (jtarget != NULL) {
        item->target = cJSON_GetNumberValue(jtarget);
    }

    /* - Stats - */
    if (jstats != NULL)
        Item_stats_readJSON(&(item->stats), jstats);

    /* - Type - */
    if (jtypeid != NULL)
        item->ids.type = cJSON_GetNumberValue(jtypeid);

    /* - Sellable - */
    if (jcanSell != NULL)
        item->flags.canSell = cJSON_IsTrue(jcanSell);

    /* - Repairable - */
    if (jcanRepair != NULL)
        item->flags.canRepair = cJSON_IsTrue(jcanRepair);
}

void Item_Free(struct Item *item) {
    if (item->users.id != NULL) {
        DARR_FREE(item->users.id);
        item->users.id = NULL;
    }
    s8_free(&item->jsonio_header.json_filename);
    s8_free(&item->name);
}

/* --- Is --- */
u64 Item_Archetype(i32 id) {
  // TODO : wpn_is funcs vs archetypes: redundant?
    u64 archetype = ITEM_ARCHETYPE_ITEM;
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

u16 Item_Typecode(const struct Item *const item) {
    return(item->ids.type);
}

b32 Item_hasType(struct Item *item, u64 type) {
  // TODO: use flag isin macro
    return ((type & item->ids.type) > 0);
}

b32 Item_isOffhand(i16  id) {
    return (Weapon_isOffhand(id));
}

b32 Item_isShield(i16  id) {
    /* Must be equivalent to using shield item archetype */
    return (Weapon_isShield(id));
}

b32 Item_isStaff(i16  id) {
    /* Must be equivalent to using staff item archetype */
    return (Weapon_isStaff(id));
}

b32 Item_isWeapon(i16 id) {
    /* Must be equivalent to using weapon item archetype */
    return (Item_Archetype(id) == ITEM_ARCHETYPE_WEAPON);
}

i32 Item_Stat( struct Item *item, i16 stattype)  {
    SDL_assert((stattype > ITEM_STAT_START) && (stattype < ITEM_STAT_END));
    i32 *item_stats_arr = &item->stats.price;
    i32 stat = item_stats_arr[stattype - 1];
    return (stat);
}
