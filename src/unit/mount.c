
#include "unit/mount.h"

const Mount Mount_default = {
    .jsonio_header.json_element = JSON_MOUNT,
};

/* --- MOUNTS --- */
/* TODO read mounts from json */
const struct Mount HORSIE = {
    .jsonio_header.json_element = JSON_MOUNT,
    .type       = MOUNT_TYPE_HORSE,
    .price      = 2000,
    .sex        = true,
    .carry      = 10,
    .attached   = 1,
    .promoted   = false,
};

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
    SDL_assert(mount);

}
