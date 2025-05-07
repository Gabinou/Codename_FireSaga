#ifndef MOUNT_H
#define MOUNT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

/* --- STRUCT DECLARATION --- */
typedef struct _Mount_Rider {
    /* Determines who can ride */

    /* --- ID: Rider id. --- */
    /* - If attached <= 0 and
     *  - rider.id == 0, anyone can ride
     *  - rider.id != 0, rider is owner
     * Note: owner might not be bonded unit */
    i32 id;
    /* --- ATTACHMENT --- */
    /* Mounts can get attached.
    *   Once attached reaches 0, only owner can ride
    *   0 is upon riding, -1 is never
    */
    i32 attached;   /* lvlups to rider becoming owner.  */
    /* --- CARRY: too buff to ride --- */
    /* carry > con? or carry > con + equipped weight?   */
    i32 carry;

    /* --- MAGES: Many mounts are scared of mages. --- */
    /* Healing magic doesn't count, not scary at all.   */
    b32 mages;
    /* --- PROMOTED: Promoted units only. --- */
    b32 promoted;
    /* --- MOUNTED: Is rider currently mounted? --- */
    /* In mount struct because:
        - No unit.flags.mounted == true && unit.id.mount == 0
    */
    b32 mounted;
    // i32 * ridable_classes; /* 0 means all classes.   */

} _Mount_Rider;

typedef struct _Mount_Bonus {
    i32 bond;       /* id of bonus for bonded unit      */
    i32 bond_unit;  /* id of bonded unit                */
    i32 skill;      /* Given to rider when mounted      */
} _Mount_Bonus;

typedef struct _Mount_Stats {
    i32 price;
    b32 sex; /* 0:F, 1:M. eg. hasPenis. */
} _Mount_Flags;

/* -- Mount -- */
typedef struct Mount {
    struct jsonIO_Header jsonio_header;

    i32 id;     /* MOUNT_HORSIE... */
    i32 type;   /* MOUNT_HORSE... */
    struct _Mount_Bonus bonus;
    struct _Mount_Stats stats;
    struct _Mount_Rider rider;
} Mount;
extern const Mount Mount_default;
extern Mount gl_mounts[MOUNT_NUM];

/* --- Mount bonuses --- */
typedef struct Mount_Bonus {
    struct jsonIO_Header jsonio_header;
    struct Unit_stats unit_stats;
} Mount_Bonus;
extern const Mount_Bonus Mount_Bonus_default;

extern Mount_Bonus mount_bonuses[MOUNT_NUM];

/* --- Mount --- */
void Mounts_Load(void);
void Mount_readJSON(    void *input, const cJSON *jmount);
void Mount_writeJSON(   const void *input, cJSON *jmount);
Mount_Bonus Mount_Bonus_Compute(struct Mount *mount);

/* -- Valid -- */
void Mount_Type_isValid(i32 type);
void Mount_ID_isValid(i32 id);

/* --- TODO --- */
void Mount_Bonus_readJSON(  void *input, const cJSON *jmount);
void Mount_Bonus_writeJSON( const void *input, cJSON *jmount);
void Mount_Can_Ride(struct Mount *mount, i32 unit_id);

#endif /* MOUNT_H */
