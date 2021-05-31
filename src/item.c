
#include "item.h"
// #include "unit.h"

struct Item Item_default = {
    .json_element = JSON_ITEM,
    .bonus_stats = {.hp = 0, .str = 0, .mag = 0, .dex = 0, .agi = 0, .luck = 0, .def = 0, .res = 0, .con = 0, .move = 0, .prof = 0},
    .malus_stats = {.hp = 0, .str = 0, .mag = 0, .dex = 0, .agi = 0, .luck = 0, .def = 0, .res = 0, .con = 0, .move = 0, .prof = 0},
    .sellable = true,
    .equippable = false, // unused by weapons
    .id = 0, // 0 means empty.
    .type = 0,
    .passive_effect = 0,
    .use_effectp = NULL,
    .users = NULL, // item only usable by users. empty = everyone
    .users_num = 0,
    .name = "",
    .description = "",
};

int8_t useEffect_heal(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {
    // HEALING ITEMS CAN BE USED ON OTHER UNITS/PEGASUSES/ENEMIES.
    uint8_t healing = (in_target->current_stats.hp * in_item->use_power) / 100 ;
    Unit_getsHealed(in_target, healing);
}

int8_t useEffect_divineShield(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {
    in_target->divine_shield = true;
}

int8_t useEffect_blowHorn(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {

}

int8_t useEffect_promote(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {

}

int8_t useEffect_lvlUp(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {
    Unit_lvlUp(in_user);
}

int8_t useEffect_gainSkill(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {
    in_target->skills += in_item->use_power;
}

int8_t useEffect_gainStats(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {
    switch (in_item->id) {
        case ITEM_NAME_TALISMAN_HP:
            in_target->current_stats.hp += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_STR:
            in_target->current_stats.str += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_MAG:
            in_target->current_stats.mag += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_DEX:
            in_target->current_stats.dex += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_AGI:
            in_target->current_stats.agi += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_LUCK:
            in_target->current_stats.luck += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_DEF:
            in_target->current_stats.def += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_RES:
            in_target->current_stats.res += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_CON:
            in_target->current_stats.con += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_PROF:
            in_target->current_stats.prof += in_item->use_power;
            break;
        case ITEM_NAME_TALISMAN_MOVE:
            in_target->current_stats.move += in_item->use_power;
            break;
    }
}

int8_t useEffect_callPegasus(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {

}

int8_t useEffect_callHorse(struct Item * in_item, struct Unit * in_user, struct Unit * in_target) {

}


void Item_writeJSON(struct Item * in_item, cJSON * in_jitem) {
    SDL_Log("Item_writeJSON");
    SDL_assert(in_jitem != NULL);
    cJSON * jname = cJSON_CreateString(in_item->name);
    cJSON * jid = cJSON_CreateNumber(in_item->id);
    cJSON * jbonus = cJSON_CreateObject();
    writeJSON_Unitstats(jbonus, &(in_item->bonus_stats));
    cJSON * jmalus = cJSON_CreateObject();
    writeJSON_Unitstats(jmalus, &(in_item->malus_stats));
    SDL_Log("Wrote maluses");

    cJSON * jsellable = cJSON_CreateBool(in_item->sellable);
    cJSON * jusers = cJSON_CreateObject();
    cJSON * jusers_num = cJSON_CreateNumber(in_item->users_num);
    cJSON_AddItemToObject(jusers, "num", jusers_num);

    cJSON * jusers_ids = cJSON_CreateArray();
    cJSON * jusers_id = NULL;
    for (int16_t i = 0; i < arrlen(in_item->users); i++) {
        jusers_id = cJSON_CreateNumber(in_item->users[i]);
        cJSON_AddItemToArray(jusers_ids, jusers_id);
    }
    cJSON_AddItemToObject(jusers, "id", jusers_ids);

    cJSON * jpassive_effects = cJSON_CreateObject();
    cJSON * jpassive_effect = NULL;
    jpassive_effect = cJSON_CreateNumber(in_item->passive_effect);
    cJSON_AddItemToObject(jpassive_effects, "id", jpassive_effect);
    SDL_Log("before utilities_wpnEffects");
    char ** effects = utilities_wpnEffects(in_item->passive_effect);
    SDL_Log("after utilities_wpnEffects");
    for (int16_t i = 0; i < DARR_NUM(effects); i++) {
        jpassive_effect = cJSON_CreateString(effects[i]);
        cJSON_AddItemToObject(jpassive_effects, "Effect", jpassive_effect);
    }
    cJSON * jtypes = cJSON_CreateObject();
    cJSON * jtype2 = NULL;
    jtype2 = cJSON_CreateNumber(in_item->type);
    cJSON_AddItemToObject(jtypes, "id", jtype2);
    char ** types = utilities_wpnType(in_item->type);
    for (int16_t i = 0; i < DARR_NUM(types); i++) {
        jtype2 = cJSON_CreateString(types[i]);
        cJSON_AddItemToObject(jtypes, "Type", jtype2);
    }
    cJSON_AddItemToObject(in_jitem, "Name", jname);
    cJSON_AddItemToObject(in_jitem, "id", jid);
    cJSON_AddStringToObject(in_jitem, "Description", in_item->description);
    cJSON_AddItemToObject(in_jitem, "Bonus", jbonus);
    cJSON_AddItemToObject(in_jitem, "Users", jusers);
    cJSON_AddItemToObject(in_jitem, "Malus", jmalus);
    cJSON_AddItemToObject(in_jitem, "Sellable", jsellable);
    cJSON_AddItemToObject(in_jitem, "Effects", jpassive_effects);
    cJSON_AddItemToObject(in_jitem, "Types", jtypes);
}

void Item_readJSON(struct Item * in_item, cJSON * in_jitem) {
    SDL_Log("Item_readJSON");
    SDL_assert(in_jitem != NULL);
    cJSON * jname = cJSON_GetObjectItemCaseSensitive(in_jitem, "Name");
    cJSON * jid = cJSON_GetObjectItemCaseSensitive(in_jitem, "id");
    cJSON * jdescription = cJSON_GetObjectItemCaseSensitive(in_jitem, "Description");
    cJSON * jbonus_stats = cJSON_GetObjectItemCaseSensitive(in_jitem, "Bonus");
    cJSON * jmalus_stats = cJSON_GetObjectItemCaseSensitive(in_jitem, "Malus");
    cJSON * jsellable = cJSON_GetObjectItemCaseSensitive(in_jitem, "Sellable");
    cJSON * jusers = cJSON_GetObjectItemCaseSensitive(in_jitem, "Users");
    cJSON * jusers_num = cJSON_GetObjectItemCaseSensitive(jusers, "num");
    cJSON * jpassive_effects = cJSON_GetObjectItemCaseSensitive(in_jitem, "Effects");
    cJSON * jpassive_effect = cJSON_GetObjectItemCaseSensitive(jpassive_effects, "id");
    cJSON * jtypes = cJSON_GetObjectItemCaseSensitive(in_jitem, "Types");
    cJSON * jtypeid = cJSON_GetObjectItemCaseSensitive(jtypes, "id");
    in_item->id = cJSON_GetNumberValue(jid); //returns 0 if junit is NULL
    in_item->name = cJSON_GetStringValue(jname);
    in_item->users_num = cJSON_GetNumberValue(jusers_num);
    cJSON * jusers_ids = cJSON_GetObjectItem(jusers, "id");
    struct cJSON * jusers_id;

    SDL_assert(in_item->id > 0);
    SDL_Log("Read name: %s", hmget(itemNames, in_item->id));
    strcpy(in_item->name, hmget(itemNames, in_item->id));

    if (in_item->users != NULL) {
        in_item->users = NULL;
    }

    cJSON_ArrayForEach(jusers_id, jusers_ids) {
        arrput(in_item->users, cJSON_GetNumberValue(jusers_id));
    }

    in_item->description = cJSON_GetStringValue(jdescription);
    readJSON_Unitstats(jbonus_stats, &(in_item->bonus_stats));
    readJSON_Unitstats(jmalus_stats, &(in_item->malus_stats));
    in_item->passive_effect = cJSON_GetNumberValue(jpassive_effect);
    in_item->type = cJSON_GetNumberValue(jtypeid);
    in_item->sellable = cJSON_IsTrue(jsellable);
}
