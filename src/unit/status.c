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

Unit_Status Unit_Statuses_Pop(Unit_Statuses *ss, int order) {
    IES_check_ret(ss,                       Unit_Status_default);
    IES_check_ret(order >= 0,               Unit_Status_default);
    IES_check_ret(order < UNIT_STATUS_MAX,  Unit_Status_default);

    /* -- Skip if no statuses in queue -- */
    if (ss->num <= 0) {
        ss->num = 0;
        return(Unit_Status_default);
    } 

    /* -- Status is last in queue -- */
    if (order == (ss->num - 1)) {
        ss->num--;
        return(ss->queue[order]);
    }

    /* -- Move over statuses behind order -- */
    Unit_Status out = ss->queue[order]; 
    i32 tomove      = ss->num-- - order;
    size_t bytesize = tomove * sizeof(*ss->queue);
    memmove(ss->queue[order], ss->queue[order + 1], bytesize);
    return(out);
}

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
    size_t bytesize = (num - insert) * sizeof(*ss->queue);
    memmove(statuses->queue[insert + 1], 
            statuses->queue[insert], 
            bytesize);
    statuses->queue[insert] = status;
    statuses->num++;
}

void Unit_Status_Decrement(Unit_Statuses *statuses) {
    IES_check(statuses);
    for (int i = 0; i < statuses->num; ++i) {
        if (statuses->queue[i].turns > 0) {
            statuses->queue[i].turns--;
        }
    }
}

void Unit_Status_Restore(Unit_Statuses *statuses, i32 type) {
    IES_check(statuses);
    int order = Unit_Status_Order(statuses, type);
    Unit_Statuses_Pop(statuses, order);    
}

void Unit_Status_RestoreAll(Unit_Statuses *ss) {
    IES_check(ss);
    ss->num = 0;
    size_t bytesize = UNIT_STATUS_MAX * sizeof(*ss->queue);
    memset(ss->queue, 0, bytesize);
}

i32 Unit_Status_Order(Unit_Statuses *statuses, i32 type) {
    IES_check(statuses);
    int order = -1;
    for (int i = 0; i < statuses->num; ++i) {
        if (statuses->queue[i].type == type) {
            order = i;
            break;
        }
    }

    return(order);
}

i32 Unit_Status_Turns(Unit_Statuses *statuses, i32 type) {
    IES_check(statuses);
    /* -- Find if afflicted by status of type -- */
    Unit_Status_Order(statuses, type);
    /* -- output remaining turn -- */
}
