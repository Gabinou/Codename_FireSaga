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
    // Queue:
    //  + number of statuses known, no parsing
    //  - need to memcpy statuses to put in turn orden 
    Unit_Status queue[UNIT_STATUS_MAX];
    // Push:    New statuses added at turn
    // Pop:     Old statuses taken from the start
    i32 num;

    // "bit array":
    //  + no memcpy 
    //  - parse EVERY STATUS EVERYTIME to find status num 
    i32 turns[UNIT_STATUS_NUM];

    // "Each status is a component":
    //  - a system for every component??? forget it

} Unit_Statuses;

extern const Unit_Status Unit_Status_default;
extern const Unit_Statuses Unit_Statuses_default;

/* --- Unit status --- */
i32 Unit_Statuses_Num(         Unit_Statuses *s);

void Unit_Statuses_Push(        Unit_Statuses *ss, Unit_Status s);
void Unit_Statuses_Restore(     Unit_Statuses *s, i32 i);
void Unit_Statuses_RestoreAll(  Unit_Statuses *s);
void Unit_Statuses_Decrement(   Unit_Statuses *s, i32 i);

i32 Unit_Status_Turns(Unit_Statuses *s, i32 i);

#endif /* UNIT_STATUS_H */
