
#include "conditions.h"
#include "unit.h"

/* No requirements */
struct Conditions Conditions_default = {0};

/* Everyone alive at start      */
/* No one dead at start         */
/* No one recruited at start    */
struct Conditions Conditions_Game_start = {
    .alive      = {0xFFFFFFFF},
};

/// @brief  Are all conditions in cond satisfied in game_cond?
/// @return  true if all bits in cond are set in game_cond
b32 Conditions_Match(struct Conditions *cond, struct Conditions *game_cond) {
    size_t len_alive    = BITFIELD_LEN(UNIT_NUM);
    size_t len_dead     = BITFIELD_LEN(UNIT_NUM);
    size_t len_rec      = BITFIELD_LEN(UNIT_ORDER_PC_END);

    /* Raw are the conditions the same? */
    b32 isalive = Bitfield_isIn(cond->alive,     game_cond->alive,     len_alive);
    b32 isdead  = Bitfield_isIn(cond->dead,      game_cond->dead,      len_dead);
    b32 isrec   = Bitfield_isIn(cond->recruited, game_cond->recruited, len_rec);
    b32 raw_match = (isdead && isalive && isrec);

    /* Filter for impossible conditions: Units can't be both alive and dead. */
    b32 isalive_and_dead_cond = Bitfield_Any(cond->alive,      cond->dead,         len_alive);
    b32 isalive_and_dead_game = Bitfield_Any(game_cond->alive, game_cond->dead,    len_alive);

    // SDL_Log("%d %d %d", isdead, isalive, isrec);
    SDL_Log("%d %d", isalive_and_dead_cond, isalive_and_dead_game);
    return (raw_match && !isalive_and_dead_cond && !isalive_and_dead_game);
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
    SDL_assert(cond         != NULL);
    SDL_assert(cond->dead   != NULL);
    SDL_assert(cond->alive  != NULL);
    Bitfield_On( cond->dead,    unit_order);
    Bitfield_Off(cond->alive,   unit_order);
}

void Conditions_Alive_Order(struct Conditions *cond, size_t unit_order) {
    SDL_assert(cond         != NULL);
    SDL_assert(cond->dead   != NULL);
    SDL_assert(cond->alive  != NULL);
    Bitfield_On( cond->alive,   unit_order);
    Bitfield_Off(cond->dead,    unit_order);
}

void Conditions_Recruited_Order(struct Conditions *cond, size_t unit_order) {
    SDL_assert(cond             != NULL);
    SDL_assert(cond->recruited  != NULL);
    Bitfield_On(cond->recruited, unit_order);
}

