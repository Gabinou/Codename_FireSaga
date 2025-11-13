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

#include "macros.h"
#include "platform.h"

#include "unit/status.h"

const Unit_status Unit_status_default        = {0};
const Unit_statuses Unit_statuses_default    = {0};

i32 Unit_Statuses_Num(Unit_Statuses *statuses) {
    IES_check_ret(statuses, 0);
    IES_check_ret(  statuses->num < UNIT_STATUS_MAX, 
                    UNIT_STATUS_MAX);
    return(statuses->num);
}

/* --- Statuses --- */
void Unit_Status_Push(  Unit_Statuses   *statuses, 
                        Unit_Status      status) {
    IES_check(statuses);
    IES_check(status.turns > 0);

    /* -- No statuses, add at queue start -- */
    if (statuses->num == 0) {
        statuses->queue[statuses->num++] = status;
        return;
    }

    /* -- Already afflicted, update turns -- */
    i32 order = Unit_Status_Order(statuses, status.type);
    if ((order >= 0) && (order < UNIT_STATUS_MAX)) {
        statuses->queue[order].turns = status.turns;
        return;
    }

    /* -- Too many statuses --- */
    IES_check(num < UNIT_STATUS_MAX);

    /* --- Find where to add status -- */
    i32 insert = statuses->num;
    for (int i = 0; i < statuses->num; ++i) {
        if (status.turns <= statuses->queue[i].turns) {
            insert = i;
            break;
        }
    }
    IES_check(insert < UNIT_STATUS_MAX);
    IES_check(insert >= 0);

    /* -- Status has longest turns, add at queue end -- */
    if (insert == statuses->num) {
        statuses->queue[statuses->num++] = status;
        return;
    }

    /* -- Status has mid turns, moving statuses over -- */
    memmove(statuses->queue[insert + 1], 
            statuses->queue[insert], 
            num - insert);
    statuses->queue[insert] = status;
    statuses->num++;
}

void Unit_Status_Decrement(Unit_Statuses *statuses) {
    for (int i = 0; i < statuses->num; ++i) {
        statuses->queue[i].turns--;
    }
}

void Unit_Status_Restore(Unit_Statuses *statuses, i32 type) {
}

void Unit_Status_RestoreAll(Unit_Statuses *statuses) {
    statuses->num = 0;
    memset(statuses->queue, 0, UNIT_STATUS_MAX);
}

i32 Unit_Status_Order(Unit_Statuses *statuses, i32 type) {
    return(-1);
}

i32 Unit_Status_Turns(Unit_Statuses *statuses, i32 type) {
    /* -- Find if afflicted by status of type -- */
    Unit_Status_Order(statuses, type);
    /* -- output remaining turn -- */

}
