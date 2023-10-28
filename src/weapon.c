
#include "weapon.h"

struct Weapon Weapon_default = {
    .json_element   = JSON_WEAPON,
    .json_filename  = NULL,

    .item         = NULL,
    .handedness   = WEAPON_HAND_ANY,
    .subtype      = WEAPON_SUBTYPE_NONE,
    .stats        = {0},
    .effective    = 0,
    .isMagic      = 0, /* magic weapons EXPLODE if infused */
    .canAttack    = 1,
};

struct WpnorItem WpnorItem_default = {
    .dtab   = SOTA_DTAB_NULL,
    .wpn    = NULL,
    .item   = NULL,
};

/* --- Constructors/Destructors --- */
void Weapon_Init(struct Weapon *weapon) {
    Weapon_Free(weapon);

    SDL_assert(weapon       != NULL);
    SDL_assert(weapon->item == NULL);

    weapon->item    = malloc(sizeof(struct Item));
    *(weapon->item) = Item_default;
}

void Weapon_Free(struct Weapon *weapon) {
    if (weapon == NULL) {
        return;
    }

    if (weapon->json_filename != NULL) {
        free(weapon->json_filename);
        weapon->json_filename = NULL;
    }

    if (weapon->item == NULL) {
        return;
    }

    Item_Free(weapon->item);
    free(weapon->item);
    weapon->item = NULL;
}

/* --- isCan? --- */
bool Weapon_canInfuse(const struct Weapon         *weapon, const struct Inventory_item *item) {
    SDL_assert(weapon);
    bool out = (item->infusion <= SOTA_INFUSEABLE) && !weapon->isMagic;
    return (out);
}

bool Weapon_canAttack(struct Weapon *weapon) {
    weapon->canAttack  = Weapon_canAttackfromType(weapon);
    weapon->canAttack *= Weapon_canAttackfromID(weapon);
    return (weapon->canAttack);
}

bool Weapon_canAttackfromType(const struct Weapon *weapon) {
    SDL_Log("%ld", weapon->item->type);
    SDL_assert(weapon);
    SDL_assert(weapon->item != NULL);
    bool iscan = flagsum_isIn(weapon->item->type, ITEM_TYPE_canATTACK);
    return (iscan);
}

bool Weapon_canAttackfromID(const struct Weapon *weapon) {
    SDL_assert(weapon);
    SDL_assert(weapon->item != NULL);
    return ((weapon->item->id == ITEM_NULL) || (weapon->item->id == ITEM_ID_BROKEN) ? 0 : 1);
}

/* --- I/O --- */
void Weapon_readJSON(void *input, const cJSON *const jwpn) {
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
    if (jhandedness)
        weapon->handedness  = cJSON_GetNumberValue(jhandedness);
    if (jsubtype)
        weapon->subtype     = cJSON_GetNumberValue(jsubtype);
    Filesystem_readJSON_Wpnstats(jstats, &(weapon->stats));
    /* Set item range to weapon */
    weapon->item->range.min = weapon->stats.range.min;
    weapon->item->range.max = weapon->stats.range.max;
    weapon->effective       = cJSON_GetNumberValue(jeffective);
    weapon->canAttack       = Weapon_canAttack(weapon);
}

void Weapon_writeJSON(const void *input, cJSON *jwpn) {
    struct Weapon *weapon = (struct Weapon *) input;
    SDL_assert(jwpn         != NULL);
    SDL_assert(weapon       != NULL);
    SDL_assert(weapon->item != NULL);

    weapon->item->write_stats = false;
    Item_writeJSON(weapon->item, jwpn);
    cJSON *jitemstats   = cJSON_CreateObject();
    Filesystem_writeJSON_Wpnstats(jitemstats,   &(weapon->stats));
    Filesystem_writeJSON_Itemstats(jitemstats,  &(weapon->item->stats));
    cJSON *jsubtype     = cJSON_CreateNumber(weapon->subtype);
    cJSON *jeffective   = cJSON_CreateNumber(weapon->effective);
    cJSON *jhandedness  = cJSON_CreateNumber(weapon->handedness);
    cJSON_AddItemToObject(jwpn, "Stats",      jitemstats);
    cJSON_AddItemToObject(jwpn, "Subtype",    jsubtype);
    cJSON_AddItemToObject(jwpn, "Effective",  jeffective);
    cJSON_AddItemToObject(jwpn, "Handedness", jhandedness);
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
    SDL_assert(Weapon_ID_isValid(id));
    SDL_assert(weapons_dtab != NULL);

    /* -- Skip is already loaded -- */
    if (DTAB_GET(weapons_dtab, id) != NULL) {
        return;
    }


    char filename[DEFAULT_BUFFER_SIZE] = "items"PHYSFS_SEPARATOR;
    Weapon_Filename(filename, id);

    struct Weapon temp_weapon = Weapon_default;
    SDL_Log("Loading weapon %ld %s", id, filename);
    SDL_assert(temp_weapon.json_element == JSON_WEAPON);

    /* - read weapon - */
    jsonio_readJSON(filename, &temp_weapon);
    SDL_assert(temp_weapon.item != NULL);
    temp_weapon.item->type = 1 << (id / ITEM_DIVISOR);
    if (temp_weapon.item->id != id) {
        SDL_Log("Read id %d from file %s, expected %d", temp_weapon.item->id, filename, id);
        exit(ERROR_Generic);
    }

    /* - Add weapon to dtab - */
    DTAB_ADD(weapons_dtab, &temp_weapon, id);

}

void Weapon_Filename(char *filename, i16 id) {
    char buffer[DEFAULT_BUFFER_SIZE];
    char *token;

    /* - add weapon type subfolder to filename - */
    int type_exp = id / SOTA_WPN_ID_FACTOR;
    i16 typecode = (1 << type_exp);
    char **types = Names_wpnType(typecode);
    strncat(filename, types[0], strlen(types[0]));
    strncat(filename, PHYSFS_SEPARATOR, 2);
    Names_wpnType_Free(types);

    /* - add weapon name to filename - */
    size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);
    SDL_assert(item_order != 0);
    strncpy(buffer, global_itemNames[item_order], DEFAULT_BUFFER_SIZE);
    token = strtok(buffer, " \t");
    while (token != NULL) {
        strcat(filename, token);
        token = strtok(NULL, " \t");
    }

    /* - add .json to filename - */
    strcat(filename, ".json");

}


void Weapon_Save(struct dtab *weapons_dtab, i16 id) {
    SDL_assert(Weapon_ID_isValid(id));
    SDL_assert(weapons_dtab != NULL);
    char *token;
    char buffer[DEFAULT_BUFFER_SIZE];
    if (DTAB_GET(weapons_dtab, id) != NULL) {
        SDL_Log("Saving Weapon id %ld", id);
        char filename[DEFAULT_BUFFER_SIZE] = "items"PHYSFS_SEPARATOR;
        size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);
        SDL_assert(item_order != 0);
        SDL_Log("%s", global_itemNames[item_order]);
        strncpy(buffer, global_itemNames[item_order], DEFAULT_BUFFER_SIZE);
        token = strtok(buffer, " \t");
        while (token != NULL) {
            strcat(filename, token);
            token = strtok(NULL, " \t");
        }
        strcat(filename, ".json");
        SDL_Log("Saving weapon %ld %s", id, filename);
        bool append = false;
        struct Weapon *weapon = (struct Weapon *)DTAB_GET(weapons_dtab, id);
        jsonio_writeJSON(filename, weapon, false);
    }
}

void Weapons_All_Load(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_CLAW_END; i++) {
        SDL_Log("Loading Weapon %zu", i);
        if (Weapon_ID_isValid(i))
            Weapon_Load(weapons_dtab, i);
    }
}

void Weapons_All_Reload(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_CLAW_END; i++) {
        SDL_Log("Reloading Weapon %zu", i);
        if (Weapon_ID_isValid(i))
            Weapon_Reload(weapons_dtab, i);
    }
}

void Weapons_All_Save(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_CLAW_END; i++) {
        SDL_Log("%zu", i);
        if (!Weapon_ID_isValid(i))
            continue;

        if (DTAB_GET(weapons_dtab, i) != NULL)
            Weapon_Save(weapons_dtab, i);
    }

}

void Weapons_All_Free(struct dtab *weapons_dtab) {
    for (size_t i = ITEM_NULL; i < ITEM_ID_CLAW_END; i++) {
        if (DTAB_GET(weapons_dtab, i) != NULL)
            Weapon_Free(DTAB_GET(weapons_dtab, i));
    }

}

u16 Weapon_TypeExp(const struct Weapon *weapon) {
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

bool Weapon_isOffhand(u16 id) {
    bool is = ((id > ITEM_ID_OFFHAND_START) && (id < ITEM_ID_OFFHAND_END));
    return (is);
}

bool Weapon_isShield(u16 id) {
    bool is = ((id > ITEM_ID_SHIELD_START) && (id < ITEM_ID_SHIELD_END));
    return (is);
}

bool Weapon_isStaff(u16 id) {
    bool is = ((id > ITEM_ID_STAFF_START) && (id < ITEM_ID_STAFF_END));
    return (is);
}

bool Weapon_ID_isValid(u16 id) {
    bool valid = false;
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
int Weapon_Stat(const struct Weapon *weapon, i16 stattype) {
    SDL_assert((stattype > ITEM_STAT_START) && (stattype < WEAPON_STAT_END));

    if ((stattype > ITEM_STAT_START) && (stattype < ITEM_STAT_END)) {
        return (Item_Stat(weapon->item, stattype));
    }

    u8 *wpn_stats_arr  = (u8 *)&weapon->stats;
    int stat = wpn_stats_arr[stattype - WEAPON_STAT_START - 1];
    return (stat);
}

int Weapon_Stat_inRange(const struct Weapon *weapon, i16 stattype, int distance) {
    /* Gives weapon stat if distance is in range.
    *  Shields and offhands are always in range.
    *    DEBUG: input -1 to always be in_range
    */
    struct Range range = weapon->stats.range;
    bool in_range = ((distance < 0) || (range.min <= distance) && (distance <= range.min));
    bool isshield  = Weapon_isShield(weapon->item->id);
    bool isoffhand = Weapon_isOffhand(weapon->item->id);
    return ((in_range || isshield || isoffhand) ? Weapon_Stat(weapon, stattype) : 0);
}
