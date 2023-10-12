#ifndef FSM_CONTROLLER_H
#define FSM_CONTROLLER_H

#include <stdint.h>
#include "globals.h"
#include "enums.h"
#include "events.h"
#include "game/game.h"

/* --- FINITE-STATE MACHINE FOR INPUTS --- */
/* Contextual inputs according to game state! */

/* --- TYPEDEFS --- */
typedef void (*fsm_state_inputs_t)(struct Game *);

/* -- FSM: Cursor_Moves EVENT --  */
extern fsm_state_inputs_t fsm_state_inputs[GAME_STATE_NUM];
extern fsm_state_inputs_t fsm_substate_inputs[GAME_SUBSTATE_NUM];

void fsm_state_inputs_TITLE_SCREEN(struct Game *sota);
void fsm_state_inputs_GAMEPLAY_MAP(struct Game *sota);

void fsm_substate_inputs_MENU(struct Game *sota);

#endif /* FSM_CONTROLLER_H */
