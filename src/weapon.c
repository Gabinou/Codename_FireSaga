
#include "weapon.h"

/* Note: magic weapons EXPLODE if infused */
struct Weapon Weapon_default = {
    .jsonio_header.json_element   = JSON_WEAPON,

    .handedness   = WEAPON_HAND_ANY,
    .subtype      = WEAPON_SUBTYPE_NONE,
    .canAttack    = 1,
};

/* --- Constructors/Destructors --- */
void Weapon_Init(struct Weapon *weapon) {
    *weapon = Weapon_default;

    SDL_assert(weapon       != NULL);
    SDL_assert(weapon->item == NULL);

    weapon->item    = SDL_malloc(sizeof(struct Item));
    *(weapon->item) = Item_default;
}

void Weapon_Free(struct Weapon *weapon) {
    if (weapon == NULL) {
        return;
    }

    s8_free(&weapon->jsonio_header.json_filename);

    if (weapon->item == NULL) {
        return;
    }

    Item_Free(weapon->item);
    SDL_free(weapon->item);
    weapon->item = NULL;
}

/* --- isCan? --- */
b32 Weapon_canInfuse(struct Weapon *weapon, struct Inventory_item *item) {
    SDL_assert(weapon);
    b32 out = (item->infusion <= SOTA_INFUSEABLE) && !weapon->isMagic;
    return (out);
}

b32 Weapon_canAttack(struct Weapon *weapon) {
    weapon->canAttack  = Weapon_canAttackfromType(weapon);
    weapon->canAttack *= Weapon_canAttackfromID(weapon);
    return (weapon->canAttack);
}

b32 Weapon_canAttackfromType(struct Weapon *weapon) {
    SDL_assert(weapon);
    SDL_assert(weapon->item != NULL);
    b32 iscan = flagsum_isIn(weapon->item->type, ITEM_ARCHETYPE_WEAPON);
    return (iscan);
}

b32 Weapon_canAttackfromID(struct Weapon *weapon) {
    SDL_assert(weapon);
    SDL_assert(weapon->item != NULL);
    return ((weapon->item->id == ITEM_NULL) || (weapon->item->id == ITEM_ID_BROKEN) ? 0 : 1);
}

/* --- I/O --- */
void Weapon_readJSON(void *input, cJSON *jwpn) {
    struct Weapon *weapon = (struct Weapon *) input;
    SDL_assert(weapon != NULL);
    SDL_assert(jwpn != NULL);
    Weapon_Init(weapon);
    SDL_assert(weapon->item != NULL);
    Item_readJSON(weapon->item, jwpn);
    cJSON *jstats           = cJSON_GetObjectItemCaseSensitive(jwpn, "Stats");
    cJSON *jsubtype         = cJSON_GetObjectItemCaseSensitive(jwpn, "Subtype");
    cJSON *jhandedness      = cJSON_GetObjectItemCaseSensitive(jwpn, "Handedness");
    cJSON *jeffective       = cJSON_GetObjectItemCaseSensitive(jwpn, "Effective");

    if (jhandedness != NULL)
        weapon->handedness  = cJSON_GetNumberValue(jhandedness);
    if (jsubtype != NULL)
        weapon->subtype     = cJSON_GetNumberValue(jsubtype);
    if (jstats != NULL)
        Weapon_stats_readJSON(&(weapon->stats), jstats);

    /* Set item range to weapon */
    weapon->item->range.min = weapon->stats.range.min;
    weapon->item->range.max = weapon->stats.range.max;
    if (jeffective != NULL)
        weapon->effective   = cJSON_GetNumberValue(jeffective);

    weapon->canAttack       = Weapon_canAttack(weapon);
}

void Weapon_writeJSON(void *input, cJSON *jwpn) {
    struct Weapon *weapon = (struct Weapon *) input;
    SDL_assert(jwpn         != NULL);
    SDL_assert(weapon       != NULL);
    SDL_assert(weapon->item != NULL);

    weapon->item->write_stats = false;
    Item_writeJSON(weapon->item, jwpn);
    cJSON *jitemstats   = cJSON_CreateObject();
    Weapon_stats_writeJSON(&(weapon->stats), jitemstats);
    Item_stats_writeJSON(&(weapon->item->stats), jitemstats);
    cJSON *jsubtype     = cJSON_CreateNumber(weapon->subtype);
    cJSON *jeffective   = cJSON_CreateNumber(weapon->effective);
    cJSON *jhandedness  = cJSON_CreateNumber(weapon->handedness);
    cJSON_AddItemToObject(jwpn, "Stats",        jitemstats);
    cJSON_AddItemToObject(jwpn, "Subtype",      jsubtype);
    cJSON_AddItemToObject(jwpn, "Effective",    jeffective);
    cJSON_AddItemToObject(jwpn, "Handedness",   jhandedness);
}

void Weapon_Reload(struct dtab *weapons_dtab, i16 id) {
    /* Overwrite weapon ONLY if it already exists */
    if (DTAB_GET(weapons_dtab, id) != NULL) {
        Weapon_Free(DTAB_GET(weapons_dtab, id));
        DTAB_DEL(weapons_dtab, id);
        Weapon_Load(weapons_dtab, id);
    }
}

void Weapon_Load(struct dtab *weapons_dtab, i16 id) {
    SDL_assert(weapons_dtab != NULL);
    if (!Weapon_ID_isValid(id)) {
        SDL_LogError(SOTA_LOG_SYSTEM, "Weapon ID '%d' invalid", id);
        SDL_assert(Weapon_ID_isValid(id));
        exit(1);
    }

    SDL_assert(weapons_dtab != NULL);

    /* -- Skip is already loaded -- */
    if (DTAB_GET(weapons_dtab, id) != NULL) {
        return;
    }

    s8 filename = s8_mut("items"PHYSFS_SEPARATOR);
    filename    = Weapon_Filename(filename, id);

    struct Weapon temp_weapon = Weapon_default;
    SDL_assert(temp_weapon.jsonio_header.json_element == JSON_WEAPON);

    /* - read weapon - */
    SDL_assert(temp_weapon.jsonio_header.json_filename.data == NULL);
    jsonio_readJSON(filename, &temp_weapon);
    SDL_assert(temp_weapon.jsonio_header.json_filename.data != NULL);
    SDL_assert(temp_weapon.item != NULL);

    temp_weapon.item->type = 1 << (id / ITEM_DIVISOR);
    temp_weapon.item->id = id;

    /* - Add weapon to dtab - */
    DTAB_ADD(weapons_dtab, &temp_weapon, id);
    s8_free(&filename);
}

s8 Weapon_Filename(s8 filename, i16 id) {
    SDL_assert(global_itemNames != NULL);

    char buffer[DEFAULT_BUFFER_SIZE] = {0};
    char *token;

    /* - add weapon type subfolder to filename - */
    int type_exp = id / SOTA_WPN_ID_FACTOR;
    i16 typecode = (1 << type_exp);
    s8 *types = Names_wpnType(typecode);
    filename = s8cat(filename, types[0]);
    filename = s8cat(filename, s8_var(PHYSFS_SEPARATOR));
    Names_wpnType_Free(types);

    /* - add weapon name to filename - */
    size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);
    SDL_assert(item_order != 0);
    memcpy(buffer, global_itemNames[item_order].data, global_itemNames[item_order].num);
    token = strtok(buffer, " \t");
    while (token != NULL) {
        filename = s8cat(filename, s8_var(token));
        token = strtok(NULL, " \t");
    }

    /* - add .json to filename - */
    filename = s8cat(filename, s8_literal(".json"));
    return (filename);
}

void Weapon_Save(struct dtab *weapons_dtab, i16 id) {
    SDL_assert(Weapon_ID_isValid(id));
    SDL_assert(weapons_dtab != NULL);
    char *token;
    char buffer[DEFAULT_BUFFER_SIZE];
    if (DTAB_GET(weapons_dtab, id) != NULL) {
        s8 filename = s8_mut("items"PHYSFS_SEPARATOR);
        filename    = Weapon_Filename(filename, id);
        b32 append = false;
        struct Weapon *weapon = (struct Weapon *)DTAB_GET(weapons_dtab, id);
        jsonio_writeJSON(filename, weapon, false);
        s8_free(&filename);
    }
}

void Weapons_All_Load(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_SLING_END; i++) {
        if (Weapon_ID_isValid(i))
            Weapon_Load(weapons_dtab, i);
    }
}

void Weapons_All_Reload(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_SLING_END; i++) {
        if (Weapon_ID_isValid(i))
            Weapon_Reload(weapons_dtab, i);
    }
}

void Weapons_All_Save(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_SLING_END; i++) {
        if (!Weapon_ID_isValid(i))
            continue;

        if (DTAB_GET(weapons_dtab, i) != NULL)
            Weapon_Save(weapons_dtab, i);
    }

}

void Weapons_All_Free(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_SLING_END; i++) {
        if (DTAB_GET(weapons_dtab, i) != NULL)
            Weapon_Free(DTAB_GET(weapons_dtab, i));
    }
}

u16 Weapon_TypeExp(struct Weapon *weapon) {
    u64 wpntypecode = weapon->item->type;

    SDL_assert(wpntypecode > ITEM_NULL);
    /* Double type: SwordOffhand*/

    if ((wpntypecode & ITEM_TYPE_SWORD) && (wpntypecode & ITEM_TYPE_OFFHAND)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDOFFHAND);
    }

    /* Double type: SwordAxe*/
    if ((wpntypecode & ITEM_TYPE_SWORD) && (wpntypecode & ITEM_TYPE_AXE)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDAXE);
    }

    /* Double type: LanceAxe*/
    if ((wpntypecode & ITEM_TYPE_LANCE) && (wpntypecode & ITEM_TYPE_AXE)) {
        return (ITEM_TYPE_EXP_DOUBLE_LANCEAXE);
    }

    /* Single type loop*/
    u16 type_exp = 1;
    while (type_exp < ITEM_TYPE_EXP_END) {
        if (((1UL << (type_exp - 1)) & wpntypecode) > 0)
            break;
        type_exp++;
    }

    return (type_exp);
}

b32 Weapon_isOffhand(i32 id) {
    b32 is = ((id > ITEM_ID_OFFHAND_START) && (id < ITEM_ID_OFFHAND_END));
    return (is);
}

b32 Weapon_isShield(i32 id) {
    b32 is = ((id > ITEM_ID_SHIELD_START) && (id < ITEM_ID_SHIELD_END));
    return (is);
}

b32 Weapon_isTrinket(i32 id) {
    b32 is = ((id > ITEM_ID_SHIELD_START) && (id < ITEM_ID_SHIELD_END));
    return (is);
}

b32 Weapon_canWeakhand(i32 id) {
    /* Weapon can be carried in weakhand without penalty */
    b32 is = Weapon_isShield(id) || Weapon_isOffhand(id) || Weapon_isTrinket(id);
    return (is);
}

b32 Weapon_isStaff(i32 id) {
    b32 is = ((id > ITEM_ID_STAFF_START) && (id < ITEM_ID_STAFF_END));
    return (is);
}

b32 Weapon_ID_isValid(i32 id) {
    if (id == ITEM_NULL) {
        return (false);
    }
    b32 valid = false;
    valid |= ((id > ITEM_ID_SWORD_START)     && (id < ITEM_ID_SWORD_END));
    valid |= ((id > ITEM_ID_LANCE_START)     && (id < ITEM_ID_LANCE_END));
    valid |= ((id > ITEM_ID_AXE_START)       && (id < ITEM_ID_AXE_END));
    valid |= ((id > ITEM_ID_BOW_START)       && (id < ITEM_ID_BOW_END));
    valid |= ((id > ITEM_ID_SHIELD_START)    && (id < ITEM_ID_SHIELD_END));
    valid |= ((id > ITEM_ID_OFFHAND_START)   && (id < ITEM_ID_OFFHAND_END));
    valid |= ((id > ITEM_ID_ELEMENTAL_START) && (id < ITEM_ID_ELEMENTAL_END));
    valid |= ((id > ITEM_ID_ANGELIC_START)   && (id < ITEM_ID_ANGELIC_END));
    valid |= ((id > ITEM_ID_DEMONIC_START)   && (id < ITEM_ID_DEMONIC_END));
    valid |= ((id > ITEM_ID_STAFF_START)     && (id < ITEM_ID_STAFF_END));
    valid |= ((id > ITEM_ID_CLAW_START)      && (id < ITEM_ID_CLAW_END));
    valid |= ((id > ITEM_ID_TRINKET_START)   && (id < ITEM_ID_TRINKET_END));
    valid |= ((id > ITEM_ID_STANDARD_START)  && (id < ITEM_ID_STANDARD_END));
    valid |= ((id > ITEM_ID_SLING_START)     && (id < ITEM_ID_SLING_END));
    valid |= (id == ITEM_ID_GBE);
    return (valid);
}

/* --- Repair --- */
void Weapon_Repair(struct Weapon *wpn, struct Inventory_item *item, u8 AP) {
    /* Repair scaled by item STRENGTH.*/
    /* TODO: hardness equation */
    u8 hardness = Equation_Weapon_Attackvar(5,
                                            wpn->stats.attack[DAMAGE_TYPE_PHYSICAL],
                                            wpn->stats.attack[DAMAGE_TYPE_MAGICAL],
                                            wpn->stats.attack[DAMAGE_TYPE_TRUE],
                                            wpn->stats.protection[DAMAGE_TYPE_PHYSICAL],
                                            wpn->stats.protection[DAMAGE_TYPE_MAGICAL]);

    u8 repaired_uses = AP / hardness;
    item->used = repaired_uses > item->used ? 0 : item->used - repaired_uses;
}

/* --- Stats --- */
int Weapon_Stat(struct Weapon *weapon, i16 stattype) {
    SDL_assert((stattype > ITEM_STAT_START) && (stattype < WEAPON_STAT_END));

    if ((stattype > ITEM_STAT_START) && (stattype < ITEM_STAT_END)) {
        return (Item_Stat(weapon->item, stattype));
    }

    i32 *wpn_stats_arr = weapon->stats.attack;
    int stat = wpn_stats_arr[stattype - WEAPON_STAT_START - 1];
    return (stat);
}

int Weapon_Stat_inRange(struct Weapon *weapon, i16 stattype, int distance) {
    /* Gives weapon stat if distance is in range.
    *  Shields and offhands are always in range.
    *    DEBUG: input -1 to always be in_range
    */
    struct Range range = weapon->stats.range;
    b32 in_range = ((distance < 0) || (range.min <= distance) && (distance <= range.max));
    b32 isshield  = Weapon_isShield(weapon->item->id);
    b32 isoffhand = Weapon_isOffhand(weapon->item->id);
    return ((in_range || isshield || isoffhand) ? Weapon_Stat(weapon, stattype) : 0);
}

/* --- Handing --- */
// Can weapon be onehanded?
b32 Weapon_TwoHand_Only(Weapon *wpn) {
    return (wpn->handedness == WEAPON_HAND_TWO);
}

// Can weapon be twohanded?
b32 Weapon_OneHand_Only(Weapon *wpn) {
    b32 left_hand   = (wpn->handedness == WEAPON_HAND_LEFT);
    b32 right_hand  = (wpn->handedness == WEAPON_HAND_RIGHT);
    b32 one_hand    = (wpn->handedness == WEAPON_HAND_ONE);
    return (one_hand || left_hand || right_hand);
}
