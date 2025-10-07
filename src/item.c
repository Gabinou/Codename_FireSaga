
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
    .flags.handedness   = WEAPON_HAND_ANY,
    .ids.target         = TARGET_ENEMY,
    .range              = {0, 1},
    .flags.canUse       = true,
    .flags.canSell      = true,
};

#define REGISTER_ENUM(x, y) ITEM_EFFECT_ID_##x,
const i16 item_effect_ids[ITEM_EFFECT_NUM] = {
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
i32 useEffect_STAFF_HEAL(const struct Item *const item,
                         struct Unit *user,
                         struct Unit *target) {
    // HEALING ITEMS CAN BE USED ON OTHER UNITS/PEGASUSES/ENEMIES.
    u8 healing = Eq_Staff_Healing(item->stats.AP, user->stats.current.mag);
    Unit_getsHealed(target, healing);
    return (1);
}

i32 useEffect_USE_DIVINE_SHIELD(const struct Item *const item,
                                struct Unit *user,
                                struct Unit *target) {
    Unit_DivineShield_set(target, true);
    return (1);
}

i32 useEffect_BLOW_HORN(const struct Item *const item,
                        struct Unit *user,
                        struct Unit *target) {
    return (1);
}

i32 useEffect_USE_PROMOTE(const struct Item *const item,
                          struct Unit *user,
                          struct Unit *target) {
    // Unit_Promote(unit, i8 new_class_index);
    return (1);
}

i32 useEffect_USE_LVL_UP(const struct Item *const item,
                         struct Unit *user,
                         struct Unit *target) {
    Unit_lvlUp(user);
    return (1);
}

i32 useEffect_USE_GAIN_SKILL(const struct Item *const item,
                             struct Unit *user,
                             struct Unit *target) {
    Unit_Skill_Add(user, item->stats.AP);
    // why return -1
    return (1);
}

i32 useEffect_USE_GAIN_STATS(const struct Item *const item,
                             struct Unit *user,
                             struct Unit *target) {
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

i32 useEffect_CALL_PEGASUS(const struct Item *const item,
                           struct Unit *user,
                           struct Unit *target) {
    return (1);
}

i32 useEffect_CALL_HORSE(const struct Item *const item,
                         struct Unit *user,
                         struct Unit *target) {
    return (1);
}

/* --- ITEM --- */
void Inventory_item_Swap(Inventory_item *items, u8 i1, u8 i2) {
    Inventory_item buffer = items[i1];
    items[i1] = items[i2];
    items[i2] = buffer;
}

void Inventory_item_Deplete(Inventory_item  *invitem,
                            Item            *item) {
    /* Decrease Durability */
    invitem->used++;
    if (invitem->used >= item->stats.uses)
        Inventory_item_Break(invitem);
}

void Inventory_item_Break(Inventory_item *inventory_item) {
    // TODO: Game animation/notification of some kind.
    *inventory_item = Inventory_item_broken;
}

b32 Item_ID_isValid(u16 id) {
    b32 valid = false;
    valid |= ((id > ITEM_ID_ITEM_START) && (id < ITEM_ID_ITEM_END));
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
b32 Item_canUse(i32 id) {
    if (Item_isStaff(id)) {
        // Staves have STAFF menu option, not USE.
        return (0);
    }
    const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
    if (item == NULL) {
        SDL_assert(0);  /* debug    */
        return (0);     /* release  */
    }
    SDL_assert(item->ids.id == id);
    return (_Item_canUse(item));
}

b32 _Item_canUse(const Item *item) {
    if (Item_isStaff(item->ids.id)) {
        // Staves have STAFF menu option, not USE.
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


b32 Unit_isItemUser(const Item *item, const Unit *user) {
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

b32 Unit_isItemClass(const Item *item, const Unit *user) {
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
    /* --- Note: Game takes charge of depletion --- */
    SDL_assert(item != NULL);
    if ((item->effect.active > ITEM_EFFECT_NULL) ||
        (item->effect.active < ITEM_EFFECT_NUM)) {
        SDL_assert(false);
        return;
    }
    use_function_t active_func = item_effect_funcs[item->effect.active];

    if (!active_func) {
        SDL_assert(false);
        return;
    }
    for (i16 i = 0; i < num; i++) {
        active_func(item, user, targets[i]);
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

    cJSON *jhandedness  = cJSON_CreateNumber(_item->flags.handedness);
    cJSON_AddItemToObject(jitem, "Handedness",   jhandedness);

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
    jtype2 = cJSON_CreateNumber(_item->type.top);
    cJSON_AddItemToObject(jtypes, "id", jtype2);
    s8 *types = Names_wpnType(_item->type.top);
    for (i16 i = 0; i < DARR_NUM(types); i++) {
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
    item->users.id = DARR_INIT(item->users.id, u16, 16);
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
    size_t len = strlen(string);
    SDL_assert(len < ITEM_DESCRIPTION_LEN);
    memcpy(item->description, string, len);

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

    /* - Target - */

    if (Item_ID_isValid(item->ids.id)) {
        if (Item_isStaff(item->ids.id)) {
            item->ids.target = TARGET_FRIENDLY;
        }
    }
    if (jtarget != NULL) {
        item->ids.target = cJSON_GetNumberValue(jtarget);
    }

    /* - Stats - */
    if (jstats != NULL)
        Item_stats_readJSON(&(item->stats), jstats);

    /* - Type - */
    if (jtypeid != NULL)
        item->type.top = cJSON_GetNumberValue(jtypeid);

    /* - Sellable - */
    if (jcanSell != NULL)
        item->flags.canSell = cJSON_IsTrue(jcanSell);

    /* - Repairable - */
    if (jcanRepair != NULL)
        item->flags.canRepair = cJSON_IsTrue(jcanRepair);
}

void Item_Free(struct Item *item) {
    if (item == NULL) {
        return;
    }

    if (item->users.id != NULL) {
        DARR_FREE(item->users.id);
        item->users.id = NULL;
    }

    s8_free(&item->jsonio_header.json_filename);
}

/* --- Is --- */
u64 Item_Archetype(i32 id) {
    // TODO : wpn_is funcs vs As: redundant?
    u64 archetype = ITEM_ARCHETYPE_ITEM;
    if (!Item_ID_isValid(id)) {
        if (Item_isStaff(id)) {
            archetype = ITEM_ARCHETYPE_STAFF;
        } else if (Item_isShield(id)) {
            archetype = ITEM_ARCHETYPE_SHIELD;
        } else {
            archetype = ITEM_ARCHETYPE_WEAPON;
        }
    }
    return (archetype);
}

u16 Item_ID2Type(i32 id) {
    u64 archetype = Item_Archetype(id);
    u16 type = 0;
    if (archetype == ITEM_ARCHETYPE_ITEM) {
        const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
        SDL_assert(item);
        type = Item_Typecode(item);
    } else {
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        SDL_assert(weapon);
        type = Item_Typecode(&weapon->item);
    }
    return (type);
}

u16 Item_Typecode(const struct Item *const item) {
    SDL_assert(item);
    return (item->type.top);
}

b32 Item_hasType(const struct Item *const item, u64 type) {
    // TODO: use flag isin macro
    return ((type & item->type.top) > 0);
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
    return (Staff_ID_isValid(id));
}

b32 Item_isWeapon(i16 id) {
    /* Must be equivalent to using weapon item archetype */
    return (Item_Archetype(id) == ITEM_ARCHETYPE_WEAPON);
}

i32 Item_Stat(const struct Item *const item, i16 stattype)  {
    SDL_assert((stattype > ITEM_STAT_START) && (stattype < ITEM_STAT_END));
    const i32 *item_stats_arr = &item->stats.price;
    i32 stat = item_stats_arr[stattype - 1];
    return (stat);
}

/* --- Handing --- */
/* Can item be onehanded? */
b32 Item_TwoHand_Only(const Item *item) {
    return (Item_Handedness(item) == WEAPON_HAND_TWO);
}

/* Can item be twohanded? */
b32 Item_OneHand_Only(const Item *item) {
    b32 left_hand   = (Item_Handedness(item) == WEAPON_HAND_LEFT);
    b32 right_hand  = (Item_Handedness(item) == WEAPON_HAND_RIGHT);
    b32 one_hand    = (Item_Handedness(item) == WEAPON_HAND_ONE);
    return (one_hand || left_hand || right_hand);
}

i32 Item_Handedness(const Item *item) {
    SDL_assert(item != NULL);
    return (item->flags.handedness);
}

void Item_Handedness_Set(Item *item, i32 set) {
    item->flags.handedness =  set;
}

struct Range Item_Range(const Item *const item) {
    return (item->range);
}

i32 Pure_Item_Uses(const Item *item,
                   const Inventory_item *invitem) {
    SDL_assert(item->stats.uses >  0);
    SDL_assert(invitem->used    >= 0);
    SDL_assert(invitem->used    <= item->stats.uses);
    return (item->stats.uses - invitem->used);
}

i32 Item_Uses(i32 id, const Inventory_item *invitem) {
    /* Get item uses left. # used is in invitem.
    **  - Returns -1 if item is invalid.
    **  - Does not load pure item or weapon . */
    if (!Item_ID_isValid(id) && !Weapon_ID_isValid(id)) {
        return (-1);
    }
    const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
    const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
    SDL_assert(weapon || item);

    i32 leftover = item ?   Pure_Item_Uses(item, invitem) :
                   Weapon_Uses(weapon, invitem);
    return (leftover);
}

/* --- Getter --- */
struct Item *Item_Get(struct Inventory_item *invitem) {
    return (_Item_Get(invitem->id));
}

struct Item *_Item_Get(i32 id) {
    if (Item_ID_isValid(id)) {
        /* id for pure item */
        return (DTAB_GET(gl_items_dtab, id));
    }

    if (Weapon_ID_isValid(id)) {
        /* id for weapon */
        Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);
        return (&weapon->item);
    }

    /* All item ids are either pure items, or weapons. */
    IES_assert(0);
    return (NULL);
}

/* --- CanUse_Full --- */
b32 Item_CanUse_Full_HP_LT( struct Game *IES,
                            Unit        *user,
                            Unit        *target,
                            Item        *item) {

    /* If target HP is Less Than (LT) item IS usable */
    return (!Unit_HP_isFull(target));
}

item_CanUse_full_t Item_CanUse_Func(i32 id) {
    if ((canUse_Full > ITEM_CanUse_Full_NULL) &&
        (canUse_Full < ITEM_CanUse_Full_NUM)) {
        return(item_CanUse_full[canUse_Full]);
    }
    return(NULL);
}
