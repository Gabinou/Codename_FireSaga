
#include "unit/status.h"
#include "nmath.h"

const struct Unit_status Unit_status_default = {.turns = 3};

/* --- Statuses --- */
void Unit_Status_Add(struct Unit *unit, struct Unit_status status) {
    i16 i = Unit_Status_Find(unit, status.status);
    if (unit->statuses.queue == NULL)
        unit->statuses.queue = DARR_INIT(unit->statuses.queue, struct Unit_status, 2);

    if (i < 0) {
        // Status not in queue, putting it
        DARR_PUT(unit->statuses.queue, status);
    } else {
        // Refresh status number of turns.
        unit->statuses.queue[i].turns = STATUS_DEFAULT_TURNS;
    }
}

void Unit_Status_Decrement(struct Unit *unit) {
    if (unit->statuses.queue == NULL) {
        return;
    }
    for (size_t i = 0; i < DARR_NUM(unit->statuses.queue); i++) {
        // increment counter AFTER :
        //      - 1 turn left -> 0 turn left -> remove
        if (unit->statuses.queue[i].turns-- <= 0)
            DARR_DEL(unit->statuses.queue, i);
    }
}

/* -- Find -- */
i16 Unit_Status_Find(struct Unit *unit, i16 status) {
    SDL_assert(unit->statuses.queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->statuses.queue); i++) {
        if (unit->statuses.queue[i].status == status)
            return (i);
    }

    return (-1);
}
