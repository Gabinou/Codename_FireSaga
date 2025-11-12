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
    STATUS_DEFAULT_TURNS = 5,
};

typedef struct Unit_Status {
    i32 type;
    i32 turns
} Unit_Status;

typedef struct Unit_Statuses {
    // If its a queue, we always know how many statuses. and where to get them (pop)
    // If its a "bit array", need to parse EVERY STATUS EVERYTIME to find if status is applicable
    Unit_Status queue[UNIT_STATUS_NUM];

    // Push:    New statuses added at the end
    // Pop:     Old statuses taken from the start
    i32 start;
    i32 end;
} Unit_Statuses;

extern const Unit_Status Unit_Status_default;
extern const Unit_Status Unit_Status_default;

/* --- Unit status --- */
void Unit_Statuses_Add(       Unit_Status *s, i32 i, i32 t);
void Unit_Statuses_Restore(   Unit_Status *s, i32 i);
void Unit_Statuses_RestoreAll(Unit_Status *s);
void Unit_Statuses_Decrement( Unit_Status *s, i32 i);

/* -- Turns left -- */
i32  Unit_Statuses_Left(      Unit_Status *s, i32 i);

/* -- Any status affecting unit? -- */
i32  Unit_Statuses_Any(       Unit_Status *s);

#endif /* UNIT_STATUS_H */
