#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

/* -- Mount -- */
typedef struct Mount {
    struct jsonIO_Header jsonio_header;

    /* 0 is upon riding, -1 is never */
    i8      attached;  /* How many lvlups till rider becomes owner. */
    i8      carry;
    i8      type;
    i8      skill;

    i32     move;

    i16     bond;      /* get +1hit/+1 avoid bonus */

    i16     price;

    /* --- TOO BUFF TO RIDE --- */
    /* con only? con + weapon weight? */
    /* ridable_classes: only if necessary: */
    /* i8 * ridable_classes; // 0 means all classes. */
    b32 sex; /* 0:F, 1:M. eg. hasPenis. */
    /* Most mounts are scared of magic. */
    /* Healing magic doesn't count, not scary at all. */
    b32 promoted;
    b32 mages;
} Mount;

extern const Mount Mount_default;
extern const Mount *const gl_mounts[MOUNT_NUM];

#define REGISTER_ENUM(x) extern const Mount x;
#include "names/mounts.h"
#undef REGISTER_ENUM

/* --- Mount --- */
void Unit_Mount_Mount(   struct Unit *u, i16 id);
void Unit_Mount_Bonus(   struct Unit *u, i16 id);
void Unit_Mount_Assign(  struct Unit *u, i16 id);
void Unit_Mount_Unmount( struct Unit *u, i16 id);
void Unit_Mount_Deassign(struct Unit *u, i16 id);
void Mount_readJSON(    void *input, const cJSON *jmount);
void Mount_writeJSON(   const void *input, cJSON *jmount);

#endif /* MOUNT_H */
