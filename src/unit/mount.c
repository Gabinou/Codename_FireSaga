
#include "unit/mount.h"
#include "jsonio.h"

const Mount Mount_default = {
    .jsonio_header.json_element = JSON_MOUNT,
};

/* --- MOUNTS --- */
Mount gl_mounts[MOUNT_NUM] = {0};

void Mounts_Load(void) {
    s8 filename;
#define REGISTER_ENUM(x) gl_mounts[MOUNT_##x] = Mount_default;\
    filename = s8cat(s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2), s8_literal(".json"));\
    jsonio_readJSON(filename, &gl_mounts[MOUNT_##x]);\
    s8_free(&filename);
#include "names/mounts.h"
#undef REGISTER_ENUM
}

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
