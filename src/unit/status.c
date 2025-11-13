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
**  ss affect units during gameplay.
**
*/

#include "macros.h"
#include "platform.h"

#include "unit/status.h"

const Unit_Status   Unit_Status_default      = {0};
const Unit_Statuses Unit_Statuses_default    = {0};

i32 Unit_Statuses_Num(Unit_Statuses *ss) {
    IES_check_ret(ss, 0);
    IES_check_ret(  ss->num < UNIT_STATUS_MAX,
                    UNIT_STATUS_MAX);
    return (ss->num);
}

Unit_Status Unit_Statuses_Pop(Unit_Statuses *ss, i32 order) {
    IES_check_ret(ss,                       Unit_Status_default);
    IES_check_ret(order >= 0,               Unit_Status_default);
    IES_check_ret(order < UNIT_STATUS_MAX,  Unit_Status_default);

    /* -- Skip if no ss in queue -- */
    if (ss->num <= 0) {
        ss->num = 0;
        return (Unit_Status_default);
    }

    /* -- Status is last in queue -- */
    if (order == (ss->num - 1)) {
        ss->num--;
        return (ss->queue[order]);
    }

    /* -- Move over ss behind order -- */
    Unit_Status out = ss->queue[order];
    i32 tomove      = ss->num-- - order;
    size_t bytesize = tomove * sizeof(*ss->queue);
    memmove(ss->queue + order, ss->queue + order + 1, bytesize);
    return (out);
}

void Unit_Status_Push(  Unit_Statuses   *ss,
                        Unit_Status      status) {
    IES_check(ss);
    IES_check(status.turns > 0);

    /* -- No ss, add at queue start -- */
    if (ss->num == 0) {
        ss->queue[ss->num++] = status;
        return;
    }

    /* -- Already afflicted, update turns -- */
    i32 order = Unit_Status_Order(ss, status.type);
    if ((order >= 0) && (order < UNIT_STATUS_MAX)) {
        ss->queue[order].turns = status.turns;
        return;
    }

    /* -- Too many ss --- */
    IES_check(ss->num < UNIT_STATUS_MAX);

    /* --- Find where to add status -- */
    i32 insert = ss->num;
    for (int i = 0; i < ss->num; ++i) {
        if (status.turns <= ss->queue[i].turns) {
            insert = i;
            break;
        }
    }
    IES_check(insert < UNIT_STATUS_MAX);
    IES_check(insert >= 0);

    /* -- Status has longest turns, add at queue end -- */
    if (insert == ss->num) {
        ss->queue[ss->num++] = status;
        return;
    }

    /* -- Status has mid turns, moving ss over -- */
    size_t bytesize = (ss->num - insert) * sizeof(*ss->queue);
    memmove(ss->queue + insert + 1,
            ss->queue + insert,
            bytesize);
    ss->queue[insert] = status;
    ss->num++;
}

void Unit_Status_Decrement(Unit_Statuses *ss) {
    IES_check(ss);
    for (int i = 0; i < ss->num; ++i) {
        if (ss->queue[i].turns > 0) {
            ss->queue[i].turns--;
        }
    }
}

void Unit_Status_Restore(Unit_Statuses *ss, i32 type) {
    IES_check(ss);
    int order = Unit_Status_Order(ss, type);
    Unit_Statuses_Pop(ss, order);
}

void Unit_Status_RestoreAll(Unit_Statuses *ss) {
    IES_check(ss);
    ss->num = 0;
    size_t bytesize = UNIT_STATUS_MAX * sizeof(*ss->queue);
    memset(ss->queue, 0, bytesize);
}

i32 Unit_Status_Order(Unit_Statuses *ss, i32 type) {
    IES_check_ret(ss, -1);
    for (int i = 0; i < ss->num; ++i) {
        if (ss->queue[i].type == type) {
            return (i);
        }
    }

    return (-1);
}

i32 Unit_Status_Turns(Unit_Statuses *ss, i32 type) {
    IES_check_ret(ss, 0);
    /* -- Find if afflicted by status of type -- */
    i32 order = Unit_Status_Order(ss, type);

    if ((order < 0) || (order >= UNIT_STATUS_MAX)) {
        /* -- Status not found -- */
        return (0);
    }

    return (ss->queue[order].turns);
}
