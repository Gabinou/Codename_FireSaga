
#include "conditions.h"
#include "unit.h"

struct Conditions Conditions_Line_default = {
    .alive      = {0},
    .dead       = {0},
    .recruited  = {0},
};

struct Conditions Conditions_Game_default = {
    .alive      = {0xFFFFFFFF}, /* Everyone alive at start      */
    .dead       = {0},          /* No one dead at start         */
    .recruited  = {0},          /* No one recruited at start    */
};


void Conditions_Dead_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Dead(cond, order);
}

void Conditions_Alive_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Alive(cond, order);
}

void Conditions_Recruited_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Recruited(cond, order);
}

void Conditions_Dead(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->dead, unit_order);
    Bitfield_Off(cond->alive, unit_order);
}

void Conditions_Alive(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->alive, unit_order);
    Bitfield_Off(cond->dead, unit_order);
}

void Conditions_Recruited(struct Conditions *cond, size_t unit_order) {
    Bitfield_On(cond->recruited, unit_order);
}

/* --  Are all conditions in conds1 satisfied in conds2? -- */
b32 Conditions_Compare(struct Conditions *line_cond, struct Conditions *game_cond) {
    size_t len_alive    = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_dead     = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_rec      = BITFIELD_LEN(UNIT_ORDER_PC_END);

    /* --  Are all conditions met? -- */
    b32 isalive = Bitfield_isIn(line_cond->alive,     game_cond->alive,     len_alive);
    b32 isdead  = Bitfield_isIn(line_cond->dead,      game_cond->dead,      len_dead);
    b32 isrec   = Bitfield_isIn(line_cond->recruited, game_cond->recruited, len_rec);
    return (isdead && isalive && isrec);
}
