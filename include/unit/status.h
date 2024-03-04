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
struct Unit_status {
    /* Number of turns to be in this state. */
    /* <0 means forever. */
    i8 status;
    i8 turns;
};
extern struct Unit_status Unit_status_default;

/* --- Unit status --- */
void Unit_Status_Add(      struct Unit *u, struct Unit_status);
i16  Unit_Status_Find(      struct Unit *u, i16 status);
void Unit_Status_Remove(   struct Unit *u, i16 i);
void Unit_Status_Decrement(struct Unit *u);
i16  Unit_Status_Find_Turns(struct Unit *unit, i16 turns);

#endif /* UNIT_STATUS_H */
