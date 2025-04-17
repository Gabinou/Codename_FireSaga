
#include "unit/mount.h"

const Mount Mount_default = {
    .jsonio_header.json_element = JSON_MOUNT,
};

/* --- MOUNTS --- */
/* TODO read mounts from json */

const struct Mount NIBAL = {
    .jsonio_header.json_element = JSON_MOUNT,
    .bond       = UNIT_ID_HAMILCAR,
    .sex        = true,
    .type       = MOUNT_TYPE_SALAMANDER,
    .price      = -1,
    .carry      = 40,
    .attached   = 10,
    .promoted   = true,
    .skill      = UNIT_SKILL_EXP_VENOMOUS_SPIT,
};

const struct Mount GOITEIA = {
    .jsonio_header.json_element = JSON_MOUNT,
    .sex        = true,
    .type       = MOUNT_TYPE_PEGASUS,
    .price      = 5000,
    .carry      = 7,
    .attached   = 3,
};

const struct Mount MANWE = {
    .jsonio_header.json_element = JSON_MOUNT,
    .type       = MOUNT_TYPE_EAGLE,
    .sex        = true,
    .price      = -1,
    .carry      = 13,
    .promoted   = true,
};

const struct Mount DIVINE   = {
    .jsonio_header.json_element = JSON_MOUNT,

};
const struct Mount REMI     = {
    .jsonio_header.json_element = JSON_MOUNT,

};
const struct Mount RAIA     = {
    .jsonio_header.json_element = JSON_MOUNT,

};

// TODO change to array of mounts when reading from JSON
// Array of pointers, cause array of structs can't be
// initialized by non-constants.
// Structs are never constants.
const Mount *const gl_mounts[MOUNT_NUM] = {
#define REGISTER_ENUM(x) &x,
#include "names/mounts.h"
#undef REGISTER_ENUM
};

void Mount_readJSON(    void *input, const cJSON *jmount) {
    Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);

    cJSON *jsex      = cJSON_GetObjectItem(jmount, "sex");
    cJSON *jtype     = cJSON_GetObjectItem(jmount, "type");
    cJSON *jbond     = cJSON_GetObjectItem(jmount, "bond");
    cJSON *jmove     = cJSON_GetObjectItem(jmount, "move");
    cJSON *jmages    = cJSON_GetObjectItem(jmount, "mages");
    cJSON *jskill    = cJSON_GetObjectItem(jmount, "skill");
    cJSON *jprice    = cJSON_GetObjectItem(jmount, "price");
    cJSON *jcarry    = cJSON_GetObjectItem(jmount, "carry");
    cJSON *jpromoted = cJSON_GetObjectItem(jmount, "promoted");
    cJSON *jattached = cJSON_GetObjectItem(jmount, "attached");

    mount->sex      = cJSON_GetNumberValue(jsex);
    mount->type     = cJSON_GetNumberValue(jtype);
    mount->bond     = cJSON_GetNumberValue(jbond);
    mount->move     = cJSON_GetNumberValue(jmove);
    mount->mages    = cJSON_GetNumberValue(jmages);
    mount->skill    = cJSON_GetNumberValue(jskill);
    mount->price    = cJSON_GetNumberValue(jprice);
    mount->carry    = cJSON_GetNumberValue(jcarry);
    mount->promoted = cJSON_GetNumberValue(jpromoted);
    mount->attached = cJSON_GetNumberValue(jattached);
}

void Mount_writeJSON(   const void *input, cJSON *jmount) {
    const Mount *mount = input;
    SDL_assert(jmount);
    SDL_assert(mount);
    cJSON *jsex      = cJSON_CreateNumber(mount->sex);
    cJSON *jtype     = cJSON_CreateNumber(mount->type);
    cJSON *jbond     = cJSON_CreateNumber(mount->bond);
    cJSON *jmove     = cJSON_CreateNumber(mount->move);
    cJSON *jmages    = cJSON_CreateNumber(mount->mages);
    cJSON *jskill    = cJSON_CreateNumber(mount->skill);
    cJSON *jprice    = cJSON_CreateNumber(mount->price);
    cJSON *jcarry    = cJSON_CreateNumber(mount->carry);
    cJSON *jpromoted = cJSON_CreateNumber(mount->promoted);
    cJSON *jattached = cJSON_CreateNumber(mount->attached);

    cJSON_AddItemToObject(jmount,   "sex",      jsex);
    cJSON_AddItemToObject(jmount,   "type",     jtype);
    cJSON_AddItemToObject(jmount,   "bond",     jbond);
    cJSON_AddItemToObject(jmount,   "move",     jmove);
    cJSON_AddItemToObject(jmount,   "mages",    jmages);
    cJSON_AddItemToObject(jmount,   "skill",    jskill);
    cJSON_AddItemToObject(jmount,   "price",    jprice);
    cJSON_AddItemToObject(jmount,   "carry",    jcarry);
    cJSON_AddItemToObject(jmount,   "promoted", jpromoted);
    cJSON_AddItemToObject(jmount,   "attached", jattached);
}
