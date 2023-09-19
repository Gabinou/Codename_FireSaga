
#include "mount.h"

/* --- MOUNTS --- */
struct Mount Mount_default_horse = {
    .name =              "Horsie",
    .unit_bond =         0,
    .type =              MOUNT_TYPE_HORSE,
    .price =             2000,
    .sex =               true,
    // .ridable_classes =   NULL,
    .carry_capacity =    10,
    .mages_can_ride =    0,
    .gets_attached =     1,
    .promoted =          false,
    .owner =             0,
    .owner_lvlups =      0,
    .skill =             0,
};

struct Mount Mount_default_salamander = {
    .name =             "Nibal",
    .unit_bond =        UNIT_ID_HAMILCAR,
    .sex =              true,
    .type =             MOUNT_TYPE_SALAMANDER,
    .price =            -1,
    // .ridable_classes =  NULL,
    .carry_capacity =   40,
    .gets_attached =    10,
    .promoted =         true,
    .owner =            0,
    .owner_lvlups =     0,
    // .skill =            UNIT_SKILL_VENOMOUS_SPIT, // const int don't work
};

struct Mount Mount_default_pegasus = {
    .name =             "FlyieHorsie",
    .sex =              true,
    .type =             MOUNT_TYPE_PEGASUS,
    .unit_bond =        0,
    .price =            5000,
    // .ridable_classes =  NULL,
    .carry_capacity =   7,
    .mages_can_ride =   0,
    .gets_attached =    3,
    .promoted =         false,
    .owner =            0,
    .owner_lvlups =     0,
    .skill =            0,
};

struct Mount Mount_default_eagle = {
    .name =             "Manwe",
    .type =             MOUNT_TYPE_EAGLE,
    .sex =              true,
    .unit_bond =        0,
    .price =            -1,
    // .ridable_classes =  NULL,
    .carry_capacity =   13,
    .mages_can_ride =   0,
    .gets_attached =    0,
    .promoted =         true,
    .owner =            0,
    .owner_lvlups =     0,
    .skill =            0,
};
