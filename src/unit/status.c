
#include "unit/status.h"
#include "nmath.h"

struct Unit_status Unit_status_default = {.turns = 3};

/* --- Statuses --- */
void Unit_Status_Add(struct Unit *unit, struct Unit_status status) {
    i16 i = Unit_Status_Find(unit, status.status);
    if (unit->status_queue == NULL)
        unit->status_queue = DARR_INIT(unit->status_queue, struct Unit_status, 2);

    if (i < 0) {
        // Status not in queue, putting it
        DARR_PUT(unit->status_queue, status);
    } else {
        // Refresh status number of turns.
        unit->status_queue[i].turns = STATUS_DEFAULT_TURNS;
    }
}

void Unit_Status_Decrement(struct Unit *unit) {
    if (unit->status_queue == NULL) {
        return;
    }
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        // increment counter AFTER :
        //      - 1 turn left -> 0 turn left -> remove
        if (unit->status_queue[i].turns-- <= 0)
            DARR_DEL(unit->status_queue, i);
    }
}

/* -- Find -- */
i16 Unit_Status_Find(struct Unit *unit, i16 status) {
    SDL_assert(unit->status_queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        if (unit->status_queue[i].status == status)
            return (i);
    }

    return (-1);
}
