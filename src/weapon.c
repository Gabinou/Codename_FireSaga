
#include "weapon.h"

struct Weapon Weapon_default = {
    .json_element = JSON_WEAPON,
    .item = NULL,
    .stats = {
        .Pmight = 0,
        .Mmight = 0,
        .combat = {
            .attack = {0, 0},
            .defense = {0, 0},
            .hit = 0,
            .crit = 0,
            .dodge = 0,
            .favor = 0,
            .speed = 0
        },
        .wgt = 0,
        .uses = 0,
        .prof =  0,
        .range = {0, 0},
        .hand = {1, 2},
        .dmg_type = 0,
    },
    .effective = 0,
    .infused = {0, 0},
    .infuseable = 0,
    .attackable = 1,
};

void Weapon_Infuse(struct Weapon * in_weapon, uint8_t in_mag, uint16_t in_type) {
    SDL_assert(in_weapon);
    SDL_assert((in_weapon->infused.power <= 0) || (in_weapon->stats.Mmight > 0));
    SDL_assert(flagsum_isIn(in_type, ITEM_TYPE_ELEMENTAL) || flagsum_isIn(in_type, ITEM_TYPE_ANGELIC) || flagsum_isIn(in_type, ITEM_TYPE_DEMONIC));
    in_weapon->infused.power = in_mag;
    in_weapon->infused.type = in_type;
}

void Weapon_attackablefromType(struct Weapon * in_weapon) {
    SDL_assert(in_weapon);
    SDL_assert(in_weapon->item != NULL);
    if (flagsum_isIn(in_weapon->item->type, ITEM_TYPE_SWORD) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_LANCE) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_AXE) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_ELEMENTAL) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_DEMONIC) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_ANGELIC) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_OFFHAND) ||
            flagsum_isIn(in_weapon->item->type, ITEM_TYPE_BOW)) {
        in_weapon->attackable = true;
    }
}


bool Weapon_canInfuse(struct Weapon * in_weapon) {
    SDL_assert(in_weapon);
    bool out = (in_weapon->infused.power < 0) ? true & in_weapon->infuseable : false & in_weapon->infuseable;
    return (out);
}

void Weapon_readJSON(struct Weapon * in_weapon, cJSON * in_jwpn) {
    SDL_Log("Weapon_readJSON");
    if (in_weapon->item != NULL) {
        free(in_weapon->item);
    }
    in_weapon->item = (struct Item *)malloc(sizeof(*in_weapon->item));

    SDL_assert(in_jwpn != NULL);

    Item_readJSON(in_weapon->item, in_jwpn);
    cJSON * jstats = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Stats");
    cJSON * jinfused = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Infused");
    cJSON * jpower = cJSON_GetObjectItemCaseSensitive(jinfused, "Power");
    cJSON * jitype = cJSON_GetObjectItemCaseSensitive(jinfused, "Type");
    cJSON * jeffective = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Effective");
    readJSON_Wpnstats(jstats, &(in_weapon->stats));
    in_weapon->infused.power = cJSON_GetNumberValue(jpower);
    in_weapon->infused.type = cJSON_GetNumberValue(jitype);
    in_weapon->effective = cJSON_GetNumberValue(jeffective);
    Weapon_attackablefromType(in_weapon);
}

void Weapon_writeJSON(struct Weapon * in_weapon, cJSON * in_jwpn) {
    SDL_Log("Weapon_writeJSON");

    SDL_assert(in_jwpn != NULL);
    SDL_assert(in_weapon->item != NULL);
    Item_writeJSON(in_weapon->item, in_jwpn);
    cJSON * jitemstats = cJSON_CreateObject();
    cJSON * jinfused = cJSON_CreateObject();
    cJSON * jpower = cJSON_CreateNumber(in_weapon->infused.power);
    cJSON * jtype = cJSON_CreateNumber(in_weapon->infused.type);
    cJSON * jeffective = cJSON_CreateNumber(in_weapon->effective);
    writeJSON_Wpnstats(jitemstats, &(in_weapon->stats));
    cJSON_AddItemToObject(jinfused, "Power", jpower);
    cJSON_AddItemToObject(jinfused, "Type", jtype);
    cJSON_AddItemToObject(in_jwpn, "Stats", jitemstats);
    cJSON_AddItemToObject(in_jwpn, "Infused", jinfused);
}
