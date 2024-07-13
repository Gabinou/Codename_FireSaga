
#include "conditions.h"
#include "unit.h"

struct Conditions Conditions_default = {
    /* No requirements */
    .alive      = {0},
    .dead       = {0},
    .recruited  = {0},
};

struct Conditions Conditions_Game_start = {
    .alive      = {0xFFFFFFFF}, /* Everyone alive at start      */
    .dead       = {0},          /* No one dead at start         */
    .recruited  = {0},          /* No one recruited at start    */
};

/// @brief  Are all conditions in cond1 satisfied in cond2?
/// @return  true if all bits in cond1 are set in cond2
b32 Conditions_Compare(struct Conditions *cond1, struct Conditions *cond2) {
    size_t len_alive    = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_dead     = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_rec      = BITFIELD_LEN(UNIT_ORDER_PC_END);

    b32 isalive = Bitfield_isIn(cond1->alive,     cond2->alive,     len_alive);
    b32 isdead  = Bitfield_isIn(cond1->dead,      cond2->dead,      len_dead);
    b32 isrec   = Bitfield_isIn(cond1->recruited, cond2->recruited, len_rec);
    return (isdead && isalive && isrec);
}

/* Set condition from Unit Name */
void Conditions_Dead_Name(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Dead_Order(cond, order);
}

void Conditions_Alive_Name(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Alive_Order(cond, order);
}

void Conditions_Recruited_Name(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Recruited_Order(cond, order);
}

/* Set condition from Unit ID */
void Conditions_Dead_ID(     struct Conditions *cond, u16 _id) {
    SDL_assert(global_unitOrders != NULL);
    u64 order = *(u64 *)DTAB_GET(global_unitOrders, _id);
    Conditions_Dead_Order(cond, order);
}

void Conditions_Alive_ID(    struct Conditions *cond, u16 _id) {
    SDL_assert(global_unitOrders != NULL);
    u64 order = *(u64 *)DTAB_GET(global_unitOrders, _id);
    Conditions_Alive_Order(cond, order);
}

void Conditions_Recruited_ID(struct Conditions *cond, u16 _id) {
    SDL_assert(global_unitOrders != NULL);
    u64 order = *(u64 *)DTAB_GET(global_unitOrders, _id);
    Conditions_Alive_Order(cond, order);
}

/* Set condition from Unit order */
void Conditions_Dead_Order(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->dead, unit_order);
}

void Conditions_Alive_Order(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->alive, unit_order);
}

void Conditions_Recruited_Order(struct Conditions *cond, size_t unit_order) {
    Bitfield_On(cond->recruited, unit_order);
}

