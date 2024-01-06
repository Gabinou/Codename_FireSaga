
#include "unit/status.h"

struct Unit_status Unit_status_default = {.status = 0, .turns = 3};

/* --- Statuses --- */
void Unit_Status_Add(struct Unit *unit, struct Unit_status status) {
    i16 i = Unit_Status_Find(unit, status.status);
    if (unit->status_queue == NULL)
        unit->status_queue = DARR_INIT(unit->status_queue, struct Unit_status, 2);

    if (i < 0)
        DARR_PUT(unit->status_queue, status);
    else {
        unit->status_queue[i].turns = STATUS_DEFAULT_TURNS;
        i = Unit_Status_Find_Turns(unit, unit->status_queue[i].turns);
        DARR_INSERT(unit->status_queue, status, i);
    }
}

void Unit_Status_Decrement(struct Unit *unit) {
    if (unit->status_queue == NULL) {
        return;
    }
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        unit->status_queue[i].turns--;
        if (unit->status_queue[i].turns <= 0)
            DARR_DEL(unit->status_queue, i);
    }

}

i16 Unit_Status_Find(struct Unit *unit, i16 status) {
    SDL_assert(unit->status_queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        if (unit->status_queue[i].status == status) {
            return (i);
        }
    }

    return (-1);
}

// What is that for?
i16 Unit_Status_Find_Turns(struct Unit *unit, i16 turns) {
    i16 out = -1;
    SDL_assert(unit->status_queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        if (unit->status_queue[i].turns == turns) {
            out = i;
            break;
        }
    }

    return (out);
}