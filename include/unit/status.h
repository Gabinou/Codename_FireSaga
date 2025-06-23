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

/* --- UNIT STATUS --- */
typedef struct Unit_status {
    // Number of turns to be in this state.
    // < 0 is forever.
    i32 turns[UNIT_STATUS_NUM];
} Unit_Status;
extern const struct Unit_status Unit_status_default;

/* --- Unit status --- */
void Unit_Status_Add(       Unit_Status *s, i32 i, i32 t);
void Unit_Status_Decrement( Unit_Status *s, i32 i);
void Unit_Status_Restore(   Unit_Status *s, i32 i);

/* -- Turns left -- */
i32  Unit_Status_Left(      Unit_Status *s, i32 i);

/* -- Any status affecting unit? -- */
i32  Unit_Status_Any(       Unit_Status *s);

#endif /* UNIT_STATUS_H */
