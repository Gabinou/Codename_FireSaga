#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

#define REGISTER_ENUM(x) MOUNT_BONUS_##x,
enum MOUNT_BONUSES {
    MOUNT_BONUS_NULL  = 0,
#include "names/mounts_types.h"
    // TODO: unique bonuses here
    MOUNT_BONUS_END,
};
#undef REGISTER_ENUM

/* --- FORWARD DECLARATIONS --- */
typedef struct Mount_Can_Ride {
    /* Determines who can ride */
    /* --- CARRY: too buff to ride --- */
    /* carry > con? or carry > con + equipped weight? */
    i32 carry;
    /* Many mounts are scared of mages. */
    b32 mages;
    /* Promoted units only. */
    b32 promoted_only;

    // i32 * ridable_classes; /* 0 means all classes. */
} Mount_Can_Ride;

typedef struct Mount_Bonus {
    i32 skill;      /* Given to rider when mounted      */
    i32 id;         /* id of bonus to rider             */
    i32 bond;       /* id of bonus to bonded unit       */
    i32 bond_unit;  /* id of bonded unit                */
    /* Mounts can get attached.
    /* Once attached reaches 0, only owner can ride     */
    /* 0 is upon riding, -1 is never                    */
    i32 attached;   /* lvlups to rider becoming owner.  */
    /* If attached <= 0 and owner == 0, anyone can ride */
    i32 owner;      
    /* Note: rider != bonded unit */
} Mount_Bonus;

typedef struct Mount_Flags {
    b32 sex; /* 0:F, 1:M. eg. hasPenis. */
    /* Mounted in mount struct because:
        - No unit.flags.mounted == true && unit.id.mount == 0
    */
    b32 mounted;    /* Is unit currently mounted */
    /* Healing magic doesn't count, not scary at all. */
} Mount_Flags;

/* -- Mount -- */
typedef struct Mount {
    struct jsonIO_Header jsonio_header;

    i32 type;   /* MOUNT_TYPE_HORSE... */
    i32 price;

    struct Mount_Bonus      bonus;
    struct Mount_Flags      flags;
    struct Mount_Can_Ride   can_ride;
} Mount;
extern const Mount Mount_default;
extern Mount gl_mounts[MOUNT_NUM];

/* --- Mount bonuses --- */
extern struct Unit_stats       unit_stats[MOUNT_BONUS_END];
// extern struct Computed_Stats   computed_stats[MOUNT_BONUS_END];

/* --- Mount --- */
void Mounts_Load(void);
void Mount_readJSON(    void *input, const cJSON *jmount);
void Mount_writeJSON(   const void *input, cJSON *jmount);
/* --- TODO --- */
void Mount_Bonus_readJSON(  void *input, const cJSON *jmount);
void Mount_Bonus_writeJSON( const void *input, cJSON *jmount);

#endif /* MOUNT_H */
