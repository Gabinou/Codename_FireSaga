#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <string.h>
#include "enums.h"
#include "bitfields.h"
#include "nstr.h"

/* -- Game Narrative Conditions -- */
// 1- To load and play Scenes,
// 2- To choose between lines in a Scene dynamically
// NOTE: Conditions are naturally AND
struct Conditions {
    u32 dead     [BITFIELD_LEN(UNIT_NUM)];
    u32 alive    [BITFIELD_LEN(UNIT_NUM)];
    u32 recruited[BITFIELD_LEN(UNIT_ORDER_PC_END)];
};
// NOTE: Alive and dead are separate for those conditions:
//      - line REQUIRES unit to be alive        -> alive bit 1
//      - line does not care if unit is alive   -> alive bit 0
//      - line REQUIRES unit to be dead         -> dead bit  1
//      - line does not care if unit is dead    -> dead bit  0
extern struct Conditions Conditions_Game_default;
extern struct Conditions Conditions_Line_default;


/* --- Conditions --- */
b32 Conditions_Compare(struct Conditions *line_cond, struct Conditions *game_cond);

void Conditions_Dead_char(     struct Conditions *cond, char *u);
void Conditions_Alive_char(    struct Conditions *cond, char *u);
void Conditions_Recruited_char(struct Conditions *cond, char *u);

void Conditions_Dead(     struct Conditions *cond, size_t uo);
void Conditions_Alive(    struct Conditions *cond, size_t uo);
void Conditions_Recruited(struct Conditions *cond, size_t uo);

#endif /* CONDITIONS_H */
