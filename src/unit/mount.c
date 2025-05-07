
#include "unit/mount.h"
#include "unit/stats.h"
#include "jsonio.h"

const Mount Mount_default = {
    .jsonio_header.json_element = JSON_MOUNT,
};

const Mount_Bonus Mount_Bonus_default = {
    .jsonio_header.json_element = JSON_MOUNT_BONUS,
};

/* --- MOUNTS --- */
Mount       gl_mounts[MOUNT_NUM]        = {0};
Mount_Bonus mount_bonuses[MOUNT_NUM]    = {0};

void Mounts_Load(void) {
    s8 filename;
#define REGISTER_ENUM(x) if (MOUNT_##x != MOUNT_TYPE_END) {\
        gl_mounts[MOUNT_##x] = Mount_default;\
        filename = s8_mut("mounts/");\
        filename = s8cat(filename, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
        filename = s8cat(filename, s8_literal(".json"));\
        jsonio_readJSON(filename, &gl_mounts[MOUNT_##x]);\
        SDL_Log("%d %d", MOUNT_##x, gl_mounts[MOUNT_##x].id);\
        SDL_assert(MOUNT_##x == gl_mounts[MOUNT_##x].id);\
        s8_free(&filename);\
    }
#include "names/mounts.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x) mount_bonuses[MOUNT_##x] = Mount_Bonus_default;\
    filename = s8_mut("mounts/bonus/");\
    filename = s8cat(filename, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    filename = s8cat(filename, s8_literal(".json"));\
    jsonio_readJSON(filename, &mount_bonuses[MOUNT_##x]);\
    s8_free(&filename);
#include "names/mounts.h"
}
#undef REGISTER_ENUM

void Mount_readJSON(    void *input, const cJSON *jmount) {
    Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);

    cJSON *jid          = cJSON_GetObjectItem(jmount, "id");
    cJSON *jsex         = cJSON_GetObjectItem(jmount, "sex");
    cJSON *jtype        = cJSON_GetObjectItem(jmount, "type");
    cJSON *jrider       = cJSON_GetObjectItem(jmount, "Rider");
    cJSON *jprice       = cJSON_GetObjectItem(jmount, "price");
    cJSON *jskill       = cJSON_GetObjectItem(jmount, "skill");
    cJSON *jbond        = cJSON_GetObjectItem(jmount, "bond");
    cJSON *jbond_unit   = cJSON_GetObjectItem(jmount, "bond_unit");
    cJSON *jcarry       = cJSON_GetObjectItem(jrider, "carry");
    cJSON *jrider_id    = cJSON_GetObjectItem(jrider, "promoted");
    cJSON *jpromoted    = cJSON_GetObjectItem(jrider, "promoted");
    cJSON *jattached    = cJSON_GetObjectItem(jrider, "attached");
    cJSON *jmages       = cJSON_GetObjectItem(jrider, "mages");

    JSONIO_READ_NUMBER(mount->id,               jid);
    JSONIO_READ_NUMBER(mount->type,             jtype);

    JSONIO_READ_NUMBER(mount->bonus.bond,       jbond);
    JSONIO_READ_NUMBER(mount->bonus.bond_unit,  jbond_unit);
    JSONIO_READ_NUMBER(mount->bonus.skill,      jskill);

    JSONIO_READ_FLAG(mount->rider.mages,        jmages);
    JSONIO_READ_FLAG(mount->rider.promoted,     jpromoted);
    JSONIO_READ_NUMBER(mount->rider.id,         jrider_id);
    JSONIO_READ_NUMBER(mount->rider.carry,      jcarry);
    JSONIO_READ_NUMBER(mount->rider.attached,   jattached);

    JSONIO_READ_FLAG(mount->stats.sex,          jsex);
    JSONIO_READ_NUMBER(mount->stats.price,     jprice);
}

void Mount_writeJSON(   const void *input, cJSON *jmount) {
    const Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);
    cJSON *jid          = cJSON_CreateNumber(mount->id);
    cJSON *jsex         = cJSON_CreateNumber(mount->stats.sex);
    cJSON *jtype        = cJSON_CreateNumber(mount->type);
    cJSON *jbond        = cJSON_CreateNumber(mount->bonus.bond);
    cJSON *jbond_unit   = cJSON_CreateNumber(mount->bonus.bond_unit);
    cJSON *jmages       = cJSON_CreateNumber(mount->rider.mages);
    cJSON *jskill       = cJSON_CreateNumber(mount->bonus.skill);
    cJSON *jprice       = cJSON_CreateNumber(mount->stats.price);
    cJSON *jcarry       = cJSON_CreateNumber(mount->rider.carry);
    cJSON *jpromoted    = cJSON_CreateNumber(mount->rider.promoted);
    cJSON *jattached    = cJSON_CreateNumber(mount->rider.attached);
    cJSON *jrider       = cJSON_CreateObject();

    cJSON_AddItemToObject(jmount,   "id",       jid);
    cJSON_AddItemToObject(jmount,   "sex",      jsex);
    cJSON_AddItemToObject(jmount,   "type",     jtype);
    cJSON_AddItemToObject(jmount,   "bond",     jbond);
    cJSON_AddItemToObject(jmount,   "price",    jprice);

    cJSON_AddItemToObject(jrider,   "mages",    jmages);
    cJSON_AddItemToObject(jrider,   "carry",    jcarry);
    cJSON_AddItemToObject(jrider,   "promoted", jpromoted);
    cJSON_AddItemToObject(jrider,   "attached", jattached);
    cJSON_AddItemToObject(jmount,   "Rider",    jrider);

    cJSON_AddItemToObject(jmount,   "skill",    jskill);
    cJSON_AddItemToObject(jmount,   "bond",     jbond);
    cJSON_AddItemToObject(jmount,   "bond_unit", jbond_unit);
}

void Mount_Bonus_readJSON(  void *input, const cJSON *jmount_bonus) {
    Mount_Bonus *mount_bonus = input;
    cJSON *junit_stats = cJSON_GetObjectItem(jmount_bonus, "Unit_Stats");

    Unit_stats_readJSON(&mount_bonus->unit_stats, junit_stats);
}

b32 Mount_Type_isValid(i32 type) {
    return ((type >= MOUNT_NULL) && (type < MOUNT_TYPE_END));
}

b32 Mount_ID_isValid(i32 id) {
    return ((id > MOUNT_TYPE_END) && (id < MOUNT_NUM));
}

Mount_Bonus Mount_Bonus_Compute(struct Mount *mount) {
    // Note: bonus stats when riding come from 3 sources:
    //  1. Mount type bonus
    //  2. Unique mount bonus
    //  3. Bond bonus
    //  mount_bonus is
    //      - max(type_bonus, unique_bonus) + bond_bonus
    Mount_Bonus out = Mount_Bonus_default;

    Unit_stats type_bonus   = {0};
    Unit_stats unique_bonus = {0};
    if (Mount_Type_isValid(mount->type)) {
        type_bonus = mount_bonuses[mount->type].unit_stats;
    }
    if (Mount_ID_isValid(mount->id)) {
        unique_bonus = mount_bonuses[mount->id].unit_stats;
    }
    out.unit_stats = Unit_stats_plus(type_bonus,
                                     unique_bonus);
    return (out);
}
