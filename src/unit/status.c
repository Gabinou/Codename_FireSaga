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
**
*/

#include "unit/status.h"
#include "nmath.h"

const struct Unit_status Unit_status_default = {0};

/* --- Statuses --- */
void Unit_Status_Add(Unit_Status *status, i32 s, i32 turns) {
    SDL_assert(status != NULL);
    SDL_assert(s > UNIT_STATUS_START);
    SDL_assert(s < UNIT_STATUS_NUM);
    status->turns[s] = turns;
}

void Unit_Status_Decrement(Unit_Status *status,
                           i32 s) {
    SDL_assert(status != NULL);
    SDL_assert(s > UNIT_STATUS_START);
    SDL_assert(s < UNIT_STATUS_NUM);
    if (status->turns[s] <= 0) {
        // if == 0 -> status restored
        // if <  0 -> status infinite
        return;
    }
    status->turns[s]--;
}

void Unit_Status_Restore(Unit_Status *status,
                         i32 s) {
    SDL_assert(status != NULL);
    SDL_assert(s > UNIT_STATUS_START);
    SDL_assert(s < UNIT_STATUS_NUM);
    status->turns[s] = 0;
}

/* -- Find -- */
i32 Unit_Status_Left(Unit_Status *status,
                     i32 s) {
    SDL_assert(status != NULL);
    SDL_assert(s > UNIT_STATUS_START);
    SDL_assert(s < UNIT_STATUS_NUM);
    return (status->turns[s]);
}

i32 Unit_Status_Any(Unit_Status *status) {
    for (size_t s = 0; s < UNIT_STATUS_NUM; s++) {
        if (status->turns[s] != 0) {
            return (1);
        }
    }
    return (0);
}
