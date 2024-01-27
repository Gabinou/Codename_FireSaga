#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "nstr.h"
#include "structs.h"
#include "unit/unit.h"

/* -- Mount -- */
struct Mount {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

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

/* --- Mount --- */
void Unit_Mount_Mount(   struct Unit *u, i16 id);
void Unit_Mount_Bonus(   struct Unit *u, i16 id);
void Unit_Mount_Assign(  struct Unit *u, i16 id);
void Unit_Mount_Unmount( struct Unit *u, i16 id);
void Unit_Mount_Deassign(struct Unit *u, i16 id);

#endif /* MOUNT_H */
