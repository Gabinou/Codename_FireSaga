
#include "unit/mount.h"
#include "jsonio.h"

const Mount Mount_default = {
    .jsonio_header.json_element = JSON_MOUNT,
};

const Mount_Bonus Mount_Bonus_default = {
    .jsonio_header.json_element = JSON_MOUNT_BONUS,
};

struct Unit_stats mount_bonuses[MOUNT_BONUS_END] = {
    // Defaults for basic mounts.
    /* NULL */ 
    {0},
    /* MOUNT_BONUS_HORSE */ 
    {
        .move = 8
    },
    /* MOUNT_BONUS_SALAMANDER */ 
    {
        .move = 7
    },
    /* MOUNT_BONUS_PEGASUS */
    {
        .move = 8
    },
    /* MOUNT_BONUS_EAGLE */
    {
        .move = 8
    },
    /* MOUNT_BONUS_GOOSE */
    {
        .move = 9
    },
    /* MOUNT_BONUS_BEAVER */
    {
        .move = 7
    },
}

/* --- MOUNTS --- */
Mount gl_mounts[MOUNT_NUM] = {0};

void Mounts_Load(void) {
    s8 filename;
#define REGISTER_ENUM(x) gl_mounts[MOUNT_##x] = Mount_default;\
    filename = s8_mut("mounts/");\
    filename = s8cat(filename, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    filename = s8cat(filename, s8_literal(".json"));\
    jsonio_readJSON(filename, &gl_mounts[MOUNT_##x]);\
    s8_free(&filename);
#include "names/mount_types.h"
#include "names/mounts.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x) 
    mounts_bonuses[MOUNT_##x].jsonio_header.json_element = JSON_MOUNT_BONUS;\
    filename = s8_mut("mounts/bonus/");\
    filename = s8cat(filename, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    filename = s8cat(filename, s8_literal(".json"));\
    jsonio_readJSON(filename, &mounts_bonuses[MOUNT_##x]);\
    s8_free(&filename);
#include "names/mount_types.h"
#include "names/mounts.h"
}

void Mount_readJSON(    void *input, const cJSON *jmount) {
    Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);

    cJSON *jid          = cJSON_GetObjectItem(jmount, "id");
    cJSON *jsex         = cJSON_GetObjectItem(jmount, "sex");
    cJSON *jbond        = cJSON_GetObjectItem(jmount, "bond");
    cJSON *jmove        = cJSON_GetObjectItem(jmount, "move");
    cJSON *jmages       = cJSON_GetObjectItem(jmount, "mages");
    cJSON *jskill       = cJSON_GetObjectItem(jmount, "skill");
    cJSON *jprice       = cJSON_GetObjectItem(jmount, "price");
    cJSON *jcarry       = cJSON_GetObjectItem(jmount, "carry");
    cJSON *jpromoted    = cJSON_GetObjectItem(jmount, "promoted");
    cJSON *jattached    = cJSON_GetObjectItem(jmount, "attached");

    mount->type     = cJSON_GetNumberValue(jtype);
    mount->bond     = cJSON_GetNumberValue(jbond);
    mount->move     = cJSON_GetNumberValue(jmove);
    mount->skill    = cJSON_GetNumberValue(jskill);
    mount->price    = cJSON_GetNumberValue(jprice);
    mount->carry    = cJSON_GetNumberValue(jcarry);
    mount->attached = cJSON_GetNumberValue(jattached);

    JSONIO_READ_FLAG(mount->sex,        jsex);
    JSONIO_READ_FLAG(mount->mages,      jmages);
    JSONIO_READ_FLAG(mount->promoted,   jpromoted);
}

void Mount_writeJSON(   const void *input, cJSON *jmount) {
    const Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);
    cJSON *jsex      = cJSON_CreateNumber(mount->sex);
    cJSON *jtype     = cJSON_CreateNumber(mount->type);
    cJSON *jbond     = cJSON_CreateNumber(mount->bond);
    cJSON *jmages    = cJSON_CreateNumber(mount->mages);
    cJSON *jskill    = cJSON_CreateNumber(mount->skill);
    cJSON *jprice    = cJSON_CreateNumber(mount->price);
    cJSON *jcarry    = cJSON_CreateNumber(mount->carry);
    cJSON *jpromoted = cJSON_CreateNumber(mount->promoted);
    cJSON *jattached = cJSON_CreateNumber(mount->attached);

    cJSON_AddItemToObject(jmount,   "sex",      jsex);
    cJSON_AddItemToObject(jmount,   "type",     jtype);
    cJSON_AddItemToObject(jmount,   "bond",     jbond);
    cJSON_AddItemToObject(jmount,   "mages",    jmages);
    cJSON_AddItemToObject(jmount,   "skill",    jskill);
    cJSON_AddItemToObject(jmount,   "price",    jprice);
    cJSON_AddItemToObject(jmount,   "carry",    jcarry);
    cJSON_AddItemToObject(jmount,   "promoted", jpromoted);
    cJSON_AddItemToObject(jmount,   "attached", jattached);
}

void Mount_Bonus_readJSON(  void *input, const cJSON *jmount_bonus) {
    const Mount_Bonus *mount_bonus = input;
    cJSON *junit_stats = cJSON_GetObjectItem(jmount_bonus, "id");
  
    Unit_stats_readJSON(mount_bonus->unit_stat, junit_stats);
}