#ifndef UNIT_STATUS_H
#define UNIT_STATUS_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
**  Statuses affect units during gameplay.
**  Ex: damage every turn, can't move, can't cast magic
**
**  Requirements:
**      1. Multiple Statuses can affect you
**      2. Each status has its own timer
**  Design:
**      - Turns left is array
**      - Component with all bitflags? -> YES
**          + Systems gets all units w/status
**      - Inside unit? -> NO
**          + Only unit get affected by status
**          - Wasted space when no status.
**          - Need to check EVERY unit for statuses.
**      - Individual component per status? -> NO
**          - too many components
*/

#include "types.h"
#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

/* --- Enums --- */
enum STATUSES {
    STATUS_DEFAULT_TURNS    = 5,
    UNIT_STATUS_MAX         = 3,
};

typedef struct Unit_Status {
    i32 type;
    i32 turns;
} Unit_Status;

typedef struct Unit_Statuses {
    Unit_Status queue[UNIT_STATUS_MAX];
    i32 num;
} Unit_Statuses;

extern const Unit_Status    Unit_Status_default;
extern const Unit_Statuses  Unit_Statuses_default;

/* --- Unit status --- */
i32 Unit_Status_Add(Unit *u, Unit_Status s);
i32 Unit_Statuses_Num(Unit_Statuses *ss);
i32 Unit_Status_Order(Unit_Statuses *ss, i32 t);

void        Unit_Statuses_Push( Unit_Statuses *ss, Unit_Status s);
Unit_Status Unit_Statuses_Pop(  Unit_Statuses *ss, i32 o);

void Unit_Statuses_Decrement(   Unit_Statuses *ss);
void Unit_Statuses_Restore(     Unit_Statuses *ss, i32 i);
void Unit_Statuses_RestoreAll(  Unit_Statuses *ss);

i32 Unit_Status_Turns(Unit_Statuses *ss, i32 i);

#endif /* UNIT_STATUS_H */
