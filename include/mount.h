#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "structs.h"

/* -- Mount -- */
struct Mount {
    u8      json_element;   /* JSON_ELEM_bOFFSET = 0 */
    char   *json_filename; /* JSON_ELEM_bOFFSET = 1 */

    i8     gets_attached;  /* How many lvlups till rider becomes owner. */
    i8     carry_capacity;
    i8     type;
    i8     skill;

    i16    unit_bond;      /* get +1hit/+1 avoid bonus */
    i16    owner_lvlups;
    /* 0 is upon riding, -1 is never */
    i16    owner;          /* Only one that can ride */
    i16    price;

    char *name;
    /* TOO BUFF TO RIDE */
    /* con only? con + weapon weight? */
    /* ridable_classes: only if necessary: */
    /* i8 * ridable_classes; // 0 means all classes. */
    bool sex            : 1; // 0:F, 1:M. eg. hasPenis.
    /* Most mounts are scared of magic. */
    /* Healing magic doesn't count, not scary at all. */
    bool promoted       : 1;
    bool mages_can_ride : 1;
};

/* --- WALKING MOUNTS --- */
extern struct Mount Mount_default_horse;
extern struct Mount Mount_default_salamander;

/* --- FLYING MOUNTS --- */
extern struct Mount Mount_default_pegasus;
extern struct Mount Mount_default_eagle;

#endif /* MOUNT_H */
