#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

/* -- Mount -- */
struct Mount {
    struct jsonIO_Header jsonio_header;

    i8     gets_attached;  /* How many lvlups till rider becomes owner. */
    i8     carry_capacity;
    i8     type;
    i8     skill;

    i32 move;

    i16    unit_bond;      /* get +1hit/+1 avoid bonus */
    i16    owner_lvlups;
    /* 0 is upon riding, -1 is never */
    i16    owner;          /* Only one that can ride */
    i16    price;

    char *name;
    /* --- TOO BUFF TO RIDE --- */
    /* con only? con + weapon weight? */
    /* ridable_classes: only if necessary: */
    /* i8 * ridable_classes; // 0 means all classes. */
    b32 sex; // 0:F, 1:M. eg. hasPenis.
    /* Most mounts are scared of magic. */
    /* Healing magic doesn't count, not scary at all. */
    b32 promoted;
    b32 mages_can_ride;
};

/* --- WALKING MOUNTS --- */
extern const struct Mount Mount_default_horse;
extern const struct Mount Mount_default_salamander;

/* --- FLYING MOUNTS --- */
extern const struct Mount Mount_default_pegasus;
extern const struct Mount Mount_default_eagle;

/* --- Mount --- */
void Unit_Mount_Mount(   struct Unit *u, i16 id);
void Unit_Mount_Bonus(   struct Unit *u, i16 id);
void Unit_Mount_Assign(  struct Unit *u, i16 id);
void Unit_Mount_Unmount( struct Unit *u, i16 id);
void Unit_Mount_Deassign(struct Unit *u, i16 id);

#endif /* MOUNT_H */
