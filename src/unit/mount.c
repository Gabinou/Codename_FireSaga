
#include "unit/mount.h"

/* --- MOUNTS --- */
/* TODO read mounts from json */
const struct Mount HORSIE = {
    .unit_bond       = 0,
    .type            = MOUNT_TYPE_HORSE,
    .price           = 2000,
    .sex             = true,
    .carry_capacity  = 10,
    .mages_can_ride  = 0,
    .gets_attached   = 1,
    .promoted        = false,
    .owner           = 0,
    .owner_lvlups    = 0,
    .skill           = 0,
};

const struct Mount NIBAL = {
    .unit_bond       = UNIT_ID_HAMILCAR,
    .sex             = true,
    .type            = MOUNT_TYPE_SALAMANDER,
    .price           = -1,
    .carry_capacity  = 40,
    .gets_attached   = 10,
    .promoted        = true,
    .owner           = 0,
    .owner_lvlups    = 0,
    .skill           = UNIT_SKILL_EXP_VENOMOUS_SPIT,
};

const struct Mount GOITEIA = {
    .sex             = true,
    .type            = MOUNT_TYPE_PEGASUS,
    .unit_bond       = 0,
    .price           = 5000,
    .carry_capacity  = 7,
    .mages_can_ride  = 0,
    .gets_attached   = 3,
    .promoted        = false,
    .owner           = 0,
    .owner_lvlups    = 0,
    .skill           = 0,
};

const struct Mount MANWE = {
    .type            = MOUNT_TYPE_EAGLE,
    .sex             = true,
    .unit_bond       = 0,
    .price           = -1,
    .carry_capacity  = 13,
    .mages_can_ride  = 0,
    .gets_attached   = 0,
    .promoted        = true,
    .owner           = 0,
    .owner_lvlups    = 0,
    .skill           = 0,
};

const struct Mount DIVINE   = {0};
const struct Mount REMI     = {0};
const struct Mount RAIA     = {0};

// TODO change to array of mounts when reading from JSON
// Array of pointers, cause array of structs can't be
// initialized by non-constants.
// Structs are never constants.
const Mount *const gl_mounts[MOUNT_NUM] = {
#define REGISTER_ENUM(x) &x,
#include "names/mounts.h"
#undef REGISTER_ENUM
};
