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

#include "aura.h"
#include "item.h"
#include "nmath.h"
#include "names.h"
#include "weapon.h"
#include "jsonio.h"
#include "platform.h"
#include "equations.h"

#include "game/game.h"

#include "unit/unit.h"
#include "unit/flags.h"

#include "stb_sprintf.h"

const struct Item Item_default = {
    .jsonio_header.json_element   = JSON_ITEM,
    .stats              = {1000, 10, 10},
    .flags.handedness   = WEAPON_HAND_ANY,
    .ids.target         = TARGET_ENEMY,
    .range              = {0, 1},
    .flags.canUse       = true,
    .flags.canSell      = true,
};

#define REGISTER_ENUM(x, y) ITEM_EFFECT_ID_##x,
const i32 item_effect_ids[ITEM_EFFECT_NUM] = {
    ITEM_EFFECT_NULL,
#include "names/items_effects.h"
};

const item_CanUse_full_t item_CanUse_full[ITEM_CanUse_Full_NUM] = {
    /* NULL */  NULL,
    /* HP_LT */ &Item_CanUse_Full_HP_LT,
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
    /* USE_HEAL */             useEffect_USE_HEAL,
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
i32 useEffect_STAFF_HEAL(const  Item *const item,
                         Unit *user,
                         Unit *target) {
    i32 heal = Eq_Staff_Healing(item->stats.AP,
                                user->stats.current.mag);
    Unit_Heal(target, heal);
    return (1);
}

i32 useEffect_USE_HEAL(const    Item *const item,
                       Unit *user, Unit *target) {
    i32 heal_percent = Eq_Item_Healing(item->stats.AP);
    Unit_Heal_Percent(target, heal_percent);
    return (1);
}

i32 useEffect_USE_DIVINE_SHIELD(const   Item *const item,
                                Unit *user,
                                Unit *target) {
    Unit_DivineShield_set(target, true);
    return (1);
}

i32 useEffect_BLOW_HORN(const   Item *const item,
                        Unit *user,
                        Unit *target) {
    return (1);
}

i32 useEffect_USE_PROMOTE(const Item *const item,
                          Unit *user,
                          Unit *target) {
    // Unit_Promote(unit, i8 new_class_index);
    return (1);
}

i32 useEffect_USE_LVL_UP(const  Item *const item,
                         Unit *user,
                         Unit *target) {
    Unit_lvlUp(user);
    return (1);
}

i32 useEffect_USE_GAIN_SKILL(const  Item *const item,
                             Unit *user,
                             Unit *target) {
    Unit_Skill_Add(user, item->stats.AP);
    // why return -1
    return (1);
}

i32 useEffect_USE_GAIN_STATS(const  Item *const item,
                             Unit *user,
                             Unit *target) {
    switch (item->ids.id) {
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
    return (1);
}

i32 useEffect_CALL_PEGASUS(const    Item *const item,
                           Unit *user,
                           Unit *target) {
    return (1);
}

i32 useEffect_CALL_HORSE(const  Item *const item,
                         Unit *user,
                         Unit *target) {
    return (1);
}

/* --- ITEM --- */
void InvItem_Swap(InvItem *items, i32 i1, i32 i2) {
    InvItem buffer = items[i1];
    items[i1] = items[i2];
    items[i2] = buffer;
}

void InvItem_Deplete(InvItem  *invitem,
                     Item            *item) {
    IES_check(invitem);
    IES_check(item);
    SDL_assert(invitem->used >= 0);

    /* Decrease Durability */
    invitem->used++;

    if (invitem->used >= item->stats.uses) {
        InvItem_Break(invitem);
    }
}

void InvItem_Break(InvItem *invitem) {
    IES_check(invitem);

    /* TODO: Game animation/notification of some kind. */
    *invitem = InvItem_broken;
}

b32 Item_ID_isValid(i32 id) {
    if (Item_Pure_ID_isValid(id)) {
        return (1);
    }
    if (Staff_ID_isValid(id)) {
        return (1);
    }
    if (Weapon_ID_isValid(id)) {
        return (1);
    }
    return (0);
}

b32 Item_Pure_ID_isValid(i32 id) {
    b32 valid = false;
    valid |= (  (id > ITEM_ID_ITEM_START) &&
                (id < ITEM_ID_ITEM_END));
    valid |= (id == ITEM_ID_BROKEN);
    return (valid);
}

/* --- Item_canUse_<> ---
** ITEM can be used in isolation:
**  1. Item has effect.active
**  2. Item canUser flag is true
**  3. Item is NOT a staff
**      - Staves:   STAFF   option
**      - Items:    USE     option
*/
b32 Item_canUse(const Item *item) {
    IES_check_ret(item, 0);

    if (Staff_ID_isValid(item->ids.id)) {
        /* Staves have STAFF menu option, not USE. */
        return (0);
    }

    if (!item->flags.canUse) {
        /* Flag overrides active effect */
        return (0);
    }

    /* Actual active effect check: by id */
    if ((item->effect.active <= ITEM_EFFECT_NULL) ||
        (item->effect.active >= ITEM_EFFECT_NUM)) {
        return (0);
    }

    return (1);
}

b32 _Item_canUse(i32 id) {
    if (Staff_ID_isValid(id)) {
        /* Staves have STAFF menu option, not USE. */
        return (0);
    }
    const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
    IES_check_ret(item, 0);

    SDL_assert(item->ids.id == id);
    return (Item_canUse(item));
}

b32 Item_isUnitUser(const Item *item, const Unit *user) {
    IES_check_ret(item, 0);
    IES_check_ret(user, 0);

    /* -- skips: Anyone can use -- */
    if (item->users.id == NULL) {
        return (1);
    }

    int num_users = DARR_NUM(item->users.id);
    if (num_users <= 0)
        return (1);

    /* -- Check if unit is in list of users -- */
    for (size_t i = 0; i < num_users; i++) {
        if (Unit_id(user) == item->users.id[i]) {
            return (1);
        }
    }

    return (0);
}

b32 Unit_isUnitClass(const Item *item, const Unit *user) {
    IES_check_ret(item, 0);
    IES_check_ret(user, 0);

    /* -- skips: Anyone can use -- */
    if (item->users.class == NULL) {
        return (1);
    }

    int num_class = DARR_NUM(item->users.class);
    if (num_class <= 0)
        return (1);

    /* -- Check if unit class is in list of classes -- */
    for (size_t i = 0; i < num_class; i++) {
        if (Unit_Class(user) == item->users.class[i]) {
            return (1);
        }
    }

    return (0);
}

void Item_Use(const Item *item, Unit *user,
              Unit **targets, int num) {
    IES_check(item);
    IES_check(user);
    IES_check(targets);

    /* --- Note: Game takes charge of depletion --- */
    SDL_assert(item != NULL);
    if ((item->effect.active <= ITEM_EFFECT_NULL) ||
        (item->effect.active >= ITEM_EFFECT_NUM)) {
        SDL_assert(false);
        return;
    }
    use_function_t usef = item_effect_funcs[item->effect.active];

    if (!usef) {
        SDL_assert(false);
        return;
    }

    for (i32 i = 0; i < num; i++) {
        usef(item, user, targets[i]);
    }
}

/* --- I/O --- */
s8 Item_Filename(s8 filename, i32 id) {
    /* --- Not only Pure items --- */
    SDL_assert(global_itemNames != NULL);

    char buffer[DEFAULT_BUFFER_SIZE] = {0};
    char *token;

    /* - add item type subfolder to filename - */
    i32 typecode    = _Item_Type(id);
    s8 *types       = Names_wpnType(typecode);
    SDL_assert(types);
    filename = s8cat(filename, types[0]);
    filename = s8cat(filename, s8_var(PHYSFS_SEPARATOR));
    Names_wpnType_Free(types);

    /* - add Item name to filename - */
    s8 name = Item_Name(id);
    memcpy(buffer, name.data, name.num);
    token = strtok(buffer, " \t");
    while (token != NULL) {
        filename = s8cat(filename, s8_var(token));
        token = strtok(NULL, " \t");
    }

    /* - add .json to filename - */
    filename = s8cat(filename, s8_literal(".json"));
    return (filename);
}

void Item_Reload(i32 id) {
    IES_check(gl_weapons_dtab);
    IES_check(gl_items_dtab);

    /* Overwrite item ONLY if it already exists */
    if (DTAB_GET(gl_items_dtab, id) != NULL) {
        Item_Free(DTAB_GET(gl_items_dtab, id));
        DTAB_DEL(gl_items_dtab, id);
    }
    if (DTAB_GET(gl_weapons_dtab, id) != NULL) {
        Weapon_Free(DTAB_GET(gl_weapons_dtab, id));
        DTAB_DEL(gl_weapons_dtab, id);
    }

    Item_Load(id);
}

void Item_All_Load(void) {
    for (size_t o = ITEM_ORDER_START; o < ITEM_NUM; o++) {
        i32 id = Item_Order2ID(o);
        Item_Load(id);
    }
}

void Item_All_Reload(void) {
    for (size_t o = ITEM_ORDER_START; o < ITEM_NUM; o++) {
        i32 id = Item_Order2ID(o);
        Item_Reload(id);
    }
}


void Item_All_Free(struct dtab *items_dtab) {
    IES_check(items_dtab);
    // TODO
}

void Item_Load(i32 id) {
    IES_check(gl_weapons_dtab);
    IES_check(gl_items_dtab);

    /* -- Skip if already loaded -- */
    if ((DTAB_GET(gl_weapons_dtab, id) != NULL) ||
        (DTAB_GET(gl_items_dtab, id)   != NULL)) {
        // SDL_Log("%d is already loaded", id);
        return;
    }

    // SDL_Log("Item_Load %d", id);
    s8 filename = s8_mut("items"PHYSFS_SEPARATOR);
    filename    = Item_Filename(filename, id);

    struct dtab *dtab_put   = NULL;
    void *itemorwpn         = NULL;
    Weapon wpn  = Weapon_default;
    Item item   = Item_default;

    /* - Picking weapon or item/staff - */
    if (Weapon_ID_isValid(id)) {
        // SDL_Log("Loading weapon %d", id);
        SDL_assert(wpn.jsonio_header.json_element == JSON_WEAPON);
        dtab_put    = gl_weapons_dtab;
        itemorwpn   = &wpn;
    } else if ( Item_Pure_ID_isValid(id) ||
                Staff_ID_isValid(id)) {
        // SDL_Log("Loading item %d", id);
        SDL_assert(item.jsonio_header.json_element == JSON_ITEM);
        dtab_put    = gl_items_dtab;
        itemorwpn   = &item;
    } else {
        // Invalid id, do nothing
        SDL_assert(0);
        return;
    }

    /* - Reading JSON - */
    jsonio_readJSON(filename, itemorwpn);
    /* Setting id to correct value */
    /* TODO: move to readjson funcs */
    wpn.item.ids.id     = id;
    item.ids.id         = id;

#ifndef NDEBUG
    if (Weapon_ID_isValid(id)) {
        SDL_assert(wpn.jsonio_header.json_element == JSON_WEAPON);
    } else {
        SDL_assert(item.jsonio_header.json_element == JSON_ITEM);
    }
#endif /* NDEBUG */

    /* - Putting in correct dtab - */
    DTAB_ADD(dtab_put, itemorwpn, id);

    s8_free(&filename);
}

void Item_writeJSON(const void *_input, cJSON *jitem) {
    IES_check(_input);
    IES_check(jitem);

    /* - Preliminaries - */
    const struct Item *_item = _input;
    SDL_assert(jitem != NULL);
    cJSON *jid        = cJSON_CreateNumber(_item->ids.id);
    cJSON *jaura      = cJSON_CreateObject();
    cJSON *jcanSell   = cJSON_CreateBool(_item->flags.canSell);
    cJSON *jusers     = cJSON_CreateObject();
    cJSON *jeffects   = cJSON_CreateObject();
    cJSON *jusers_ids = cJSON_CreateArray();
    cJSON *jusers_id  = NULL;
    cJSON *jclass     = cJSON_CreateObject();
    cJSON *jclass_ids = cJSON_CreateArray();
    cJSON *jclass_id  = NULL;

    cJSON *jcanUse_Full = cJSON_CreateNumber(_item->flags.canUse_Full);
    cJSON_AddItemToObject(jitem, "canUse_Full", jcanUse_Full);
    cJSON *jhandedness  = cJSON_CreateNumber(_item->flags.handedness);
    cJSON_AddItemToObject(jitem, "Handedness",   jhandedness);

    /* - Users - */
    if (_item->users.id != NULL) {
        for (i32 i = 0; i < DARR_NUM(_item->users.id); i++) {
            jusers_id = cJSON_CreateNumber(_item->users.id[i]);
            cJSON_AddItemToArray(jusers_ids, jusers_id);
        }
    }
    cJSON_AddItemToObject(jusers, "id", jusers_ids);

    /* - Classes - */
    if (_item->users.class != NULL) {
        for (i32 i = 0; i < DARR_NUM(_item->users.class); i++) {
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
    i32 type = _Item_Type(_item->ids.id);
    jtype2 = cJSON_CreateNumber(type);
    cJSON_AddItemToObject(jtypes, "id", jtype2);
    s8 *types = Names_wpnType(type);
    for (i32 i = 0; i < DARR_NUM(types); i++) {
        jtype2 = cJSON_CreateString(types[i].data);
        cJSON_AddItemToObject(jtypes, "Type", jtype2);
        s8_free(&types[i]);
    }
    DARR_FREE(types);

    /* - Aura - */
    Aura_writeJSON(&(_item->aura), jaura);

    /* -- Adding to JSON -- */
    cJSON_AddStringToObject(jitem, "Description", _item->description);
    cJSON_AddItemToObject(jitem,   "id",          jid);
    cJSON_AddItemToObject(jitem,   "Aura",        jaura);
    cJSON_AddItemToObject(jitem,   "Users",       jusers);
    cJSON_AddItemToObject(jitem,   "Class",       jclass);
    cJSON_AddItemToObject(jitem,   "canSell",     jcanSell);
    cJSON_AddItemToObject(jitem,   "Effects",     jeffects);
    cJSON_AddItemToObject(jitem,   "Types",       jtypes);

    /* - Writing stats - */
    cJSON *jstats = cJSON_CreateObject();
    const struct Item_stats *_stats = &(_item->stats);
    Item_stats_writeJSON(_stats, jstats);
    cJSON_AddItemToObject(jitem, "Stats", jstats);
}

void Item_readJSON(void *input, const cJSON *_jitem) {
    IES_check(input);
    IES_check(_jitem);

    /* - Preliminaries - */
    struct Item *item = (struct Item *)input;
    Item_Free(item);
    cJSON *jid          = cJSON_GetObjectItemCaseSensitive(_jitem,      "id");
    cJSON *jdescription = cJSON_GetObjectItemCaseSensitive(_jitem,      "Description");
    cJSON *jaura        = cJSON_GetObjectItemCaseSensitive(_jitem,      "Aura");
    cJSON *jcanSell     = cJSON_GetObjectItemCaseSensitive(_jitem,      "canSell");
    cJSON *jcanUse_Full = cJSON_GetObjectItemCaseSensitive(_jitem,      "canUse_Full");
    cJSON *jcanRepair   = cJSON_GetObjectItemCaseSensitive(_jitem,      "canRepair");
    cJSON *jusers       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Users");
    cJSON *jstats       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Stats");
    cJSON *jeffects     = cJSON_GetObjectItemCaseSensitive(_jitem,      "Effects");
    cJSON *jpassive     = cJSON_GetObjectItemCaseSensitive(jeffects,    "passive");
    cJSON *jactive      = cJSON_GetObjectItemCaseSensitive(jeffects,    "active");
    cJSON *jtarget      = cJSON_GetObjectItemCaseSensitive(_jitem,      "Target");
    cJSON *jhandedness  = cJSON_GetObjectItemCaseSensitive(_jitem, "Handedness");
    // cJSON *jprice       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Price");
    cJSON *jtypes       = cJSON_GetObjectItemCaseSensitive(_jitem,      "Types");
    cJSON *jtypeid      = cJSON_GetObjectItemCaseSensitive(jtypes,      "id");

    item->ids.id            = cJSON_GetNumberValue(jid); /* returns 0 if junit is NULL */

    if (jhandedness != NULL)
        item->flags.handedness  = cJSON_GetNumberValue(jhandedness);

    /* - Users - */
    cJSON *jusers_ids = cJSON_GetObjectItem(jusers, "id");
    struct cJSON *jusers_id;
    SDL_assert(item->ids.id > 0);
    SDL_assert(global_itemOrders != NULL);
    SDL_assert(item->users.id == NULL);
    item->users.id = DARR_INIT(item->users.id, i32, 16);
    if (jusers_ids  != NULL) {
        cJSON_ArrayForEach(jusers_id, jusers_ids) {
            DARR_PUT(item->users.id, cJSON_GetNumberValue(jusers_id));
        }
    }

    /* - Item order - */
    SDL_assert(global_itemOrders != NULL);
    i32 item_order = *(i32 *)DTAB_GET(global_itemOrders, item->ids.id);
    // i32 max_order  = DARR_NUM(global_itemNames);
    // SDL_Log("%d %d", item_order, max_order);
    // SDL_assert((item_order > 0));
    SDL_assert(item_order < ITEM_NUM);

    SDL_assert(global_itemNames[item_order].data != NULL);

    /* - Description - */
    char *string = cJSON_GetStringValue(jdescription);
    if (string != NULL) {
        size_t len = strlen(string);
        SDL_assert(len < ITEM_DESCRIPTION_LEN);
        memcpy(item->description, string, len);
    }

    /* - Bonus/Malus - */
    Aura_readJSON(&item->aura, jaura);

    /* - Effects - */
    if (jpassive != NULL) {
        item->effect.passive = cJSON_GetNumberValue(jpassive);
    }

    if (jactive != NULL) {
        i32 active_order    = 0;
        i32 active_id       = cJSON_GetNumberValue(jactive);
        for (i32 i = 0; i < ITEM_EFFECT_NUM; i++) {
            if (item_effect_ids[i] == active_id) {
                active_order = i;
                break;
            }
        }
        item->effect.active = active_order;
    }

    /* - canUse_Full - */
    if (jcanUse_Full != NULL) {
        item->flags.canUse_Full = cJSON_GetNumberValue(jcanUse_Full);
    }

    /* - Target - */
    if (Item_Pure_ID_isValid(item->ids.id)) {
        if (Staff_ID_isValid(item->ids.id)) {
            item->ids.target = TARGET_FRIENDLY;
        }
    }
    if (jtarget != NULL) {
        item->ids.target = cJSON_GetNumberValue(jtarget);
    }

    /* - Stats - */
    if (jstats != NULL)
        Item_stats_readJSON(&(item->stats), jstats);

    /* - Sellable - */
    if (jcanSell != NULL)
        item->flags.canSell = cJSON_IsTrue(jcanSell);

    /* - Repairable - */
    if (jcanRepair != NULL)
        item->flags.canRepair = cJSON_IsTrue(jcanRepair);
}

void Item_Free(struct Item *item) {
    IES_check(item);

    if (item->users.id != NULL) {
        DARR_FREE(item->users.id);
        item->users.id = NULL;
    }

    s8_free(&item->jsonio_header.json_filename);
}

/* --- Is --- */
u64 Item_Archetype(i32 id) {
    if (Item_Pure_ID_isValid(id)) {
        return (ITEM_ARCHETYPE_ITEM);
    }

    if (Staff_ID_isValid(id)) {
        return (ITEM_ARCHETYPE_STAFF);
    }
    if (Item_isShield(id)) {
        return (ITEM_ARCHETYPE_SHIELD);
    }

    SDL_assert(Weapon_ID_isValid(id));
    return (ITEM_ARCHETYPE_WEAPON);
}

u64 Item_Type(const struct Item *const item) {
    IES_check_ret(item, 0ULL);
    return (_Item_Type(item->ids.id));
}

u64 _Item_Type(i32 id) {

    if ((id <= ITEM_NULL) || (id >= ITEM_ID_END)) {
        return (0);
    }

    if (Item_Pure_ID_isValid(id) || (id == ITEM_ID_BROKEN)) {
        return (ITEM_TYPE_ITEM);
    }

    i32 type_exp = _Item_Type_Exp(id);
    IES_assert(type_exp >= 0);
    IES_assert(type_exp <= ITEM_TYPE_NUM);
    u64 typecode = (1ULL << (type_exp - 1));

    return (typecode);
}

i32  Item_Type_Exp( const struct Item *const item) {
    IES_check_ret(item, 0);
    return (_Item_Type_Exp(item->ids.id));
}

i32 _Item_Type_Exp(i32 id) {
    if ((id <= ITEM_NULL) || (id >= ITEM_ID_END)) {
        return (0);
    }

    if (Item_Pure_ID_isValid(id) || (id == ITEM_ID_BROKEN)) {
        return (ITEM_TYPE_EXP_ITEM);
    }

    // Works for staves too
    int type_exp = id / SOTA_WPN_ID_FACTOR + 1;
    return (type_exp);
}


u64  Item_SubType(  const struct Item *const item) {
    IES_check_ret(item, 0ULL);

    return (0ULL);
}

u64 _Item_SubType(  i32 id) {
    // TODO
    return (0ULL);
}

b32 Item_hasType(const struct Item *const item, u64 type) {
    IES_check_ret(item, 0);
    i32 item_type = _Item_Type(item->ids.id);
    return (flagsum_isIn(type, item_type));
}

b32 Item_isOffhand(i32  id) {
    return (Weapon_isOffhand(id));
}

b32 Item_isShield(i32  id) {
    /* Must be equivalent to using shield item archetype */
    return (Weapon_isShield(id));
}

b32 Item_isWeapon(i32 id) {
    /* Must be equivalent to using weapon item archetype */
    return (Item_Archetype(id) == ITEM_ARCHETYPE_WEAPON);
}

i32 Item_Stat(const Item *item, i32 stattype)  {
    IES_check_ret(item, 0);
    SDL_assert(stattype > ITEM_STAT_START);
    SDL_assert(stattype < ITEM_STAT_END);
    const i32 *const arr = Item_Stat_Arr(item);
    i32 stat = arr[stattype - 1];
    return (stat);
}

const i32 *Item_Stat_Arr(const Item *item) {
    IES_check_ret(item, NULL);
    return (&item->stats.price);
}

/* --- Handing --- */
/* Is item only wieldable with two hands? */
b32 Item_TwoHand_Only(const Item *item) {
    IES_check_ret(item, 0);
    return (Item_Handedness(item) == WEAPON_HAND_TWO);
}

/* Is item only wieldable with one hand? */
b32 Item_OneHand_Only(const Item *item) {
    IES_check_ret(item, 0);
    b32 left_hand   = (Item_Handedness(item) == WEAPON_HAND_LEFT);
    b32 right_hand  = (Item_Handedness(item) == WEAPON_HAND_RIGHT);
    b32 one_hand    = (Item_Handedness(item) == WEAPON_HAND_ONE);
    return (one_hand || left_hand || right_hand);
}

i32 Item_Handedness(const Item *item) {
    IES_check_ret(item, 0);
    return (item->flags.handedness);
}

void Item_Handedness_Set(Item *item, i32 set) {
    IES_check(item);
    item->flags.handedness = set;
}

/* --- Range of item, for using --- */
struct Range Item_Range(const Item *const item) {
    IES_check_ret(item, Range_default);
    return (item->range);
}

/* --- Remaining uses --- */
i32 Pure_Item_remUses(const Item    *item,
                      const InvItem *invitem) {
    IES_check_ret(item,     0);
    IES_check_ret(invitem,  0);
    if (invitem->id == ITEM_ID_BROKEN) {
        return (-1);
    }
    SDL_assert(item->stats.uses > 0);
    SDL_assert(invitem->used    >= 0);
    SDL_assert(invitem->used    <= item->stats.uses);
    return (item->stats.uses - invitem->used);
}

i32 Item_remUses(const InvItem *invitem) {
    /* Get item uses left. # used is in invitem.
    **  - Returns -1 if item is invalid.
    **  - Does not load pure item or weapon . */
    IES_check_ret(invitem, 0);
    if (!Item_ID_isValid(invitem->id)) {
        return (-1);
    }
    const Weapon    *weapon = DTAB_GET_CONST(gl_weapons_dtab,   invitem->id);
    const Item      *item   = DTAB_GET_CONST(gl_items_dtab,     invitem->id);
    SDL_assert(weapon || item);

    i32 rem = item ? Pure_Item_remUses(item, invitem) :
              Weapon_remUses(weapon, invitem);
    return (rem);
}

/* --- Getters --- */
Item *Item_Get(InvItem *invitem) {
    IES_check_ret(invitem, NULL);
    return (_Item_Get(invitem->id));
}

Item *_Item_Get(i32 id) {
    if (Item_Pure_ID_isValid(id) || Staff_ID_isValid(id)) {
        /* id for pure item or staff */
        // SDL_Log("Pure Item or Staff");
        return (DTAB_GET(gl_items_dtab, id));
    }

    if (Weapon_ID_isValid(id)) {
        /* id for weapon (including shields) */
        // SDL_Log("Weapon");
        Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);
        return (&weapon->item);
    }

    /* All item ids are either pure items, staves, or weapons. */
    IES_assert(0);
    return (NULL);
}

/* --- CanUse_Full --- */
b32 Item_CanUse_Full_HP_LT( Game *IES,      Unit *user,
                            Unit *target,   Item *item) {
    IES_check_ret(target,   0);
    /* If target HP is Less Than (LT) item IS usable */
    return (!Unit_HP_isFull(target));
}

/* Getter for canuse_Full function */
item_CanUse_full_t Item_CanUse_Func(i32 id) {
    if ((id > ITEM_CanUse_Full_NULL) &&
        (id < ITEM_CanUse_Full_NUM)) {
        return (item_CanUse_full[id]);
    }
    return (NULL);
}

void InvItem_Repair(InvItem *item, i32 repair) {
    IES_check(item);
    item->used = repair > item->used ? 0 : item->used - repair;
}

s8 InvItem_Name(InvItem *invitem) {
    IES_check_ret(invitem, (s8) {
        0
    });
    return (Item_Name(invitem->id));
}

s8 Item_Name(i32 id) {
    /* --- Not only Pure items --- */
    SDL_assert(global_itemOrders    != NULL);
    SDL_assert(global_itemNames     != NULL);
    int *order = DTAB_GET(global_itemOrders, id);
    SDL_assert(order != NULL);
    return (global_itemNames[*order]);
}

