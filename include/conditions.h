#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <string.h>
#include "enums.h"
#include "bitfields.h"

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
//      - REQUIRES unit to be alive        -> alive bit 1
//      - does not care if unit is alive   -> alive bit 0
//      - REQUIRES unit to be dead         -> dead bit  1
//      - does not care if unit is dead    -> dead bit  0
extern struct Conditions Conditions_Game_start;
extern struct Conditions Conditions_default;

/* --- Conditions --- */
b32 Conditions_Match(struct Conditions *line_cond, struct Conditions *game_cond);

/* Set condition from Unit Name */
void Conditions_Dead_Name(     struct Conditions *cond, char *u);
void Conditions_Alive_Name(    struct Conditions *cond, char *u);
void Conditions_Recruited_Name(struct Conditions *cond, char *u);

/* Set condition from Unit ID */
void Conditions_Dead_ID(     struct Conditions *cond, u16 uid);
void Conditions_Alive_ID(    struct Conditions *cond, u16 uid);
void Conditions_Recruited_ID(struct Conditions *cond, u16 uid);

/* Set condition from Unit Order */
void Conditions_Dead_Order(     struct Conditions *cond, size_t uo);
void Conditions_Alive_Order(    struct Conditions *cond, size_t uo);
void Conditions_Recruited_Order(struct Conditions *cond, size_t uo);

#endif /* CONDITIONS_H */
