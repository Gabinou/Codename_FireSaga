#ifndef UNIT_STATUS_H
#define UNIT_STATUS_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"

/* --- Enums --- */
enum STATUSES {
    STATUS_DEFAULT_TURNS = 5,
};

/* --- UNIT STATUS --- */
typedef struct Unit_status {
    /* Number of turns to be in this state. */
    /* <0 means forever. */
    i8 status;
    i8 turns;
} Unit_status;
extern struct Unit_status Unit_status_default;

/* --- Unit status --- */
void Unit_Status_Add(      Unit *u, Unit_status s);
void Unit_Status_Decrement(Unit *u);

/* -- Find -- */
i16  Unit_Status_Find(      Unit *u, i16 status);

#endif /* UNIT_STATUS_H */
