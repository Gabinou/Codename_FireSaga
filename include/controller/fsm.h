#ifndef FSM_CONTROLLER_H
#define FSM_CONTROLLER_H

#include <stdint.h>
#include "enums.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* --- FINITE-STATE MACHINE FOR INPUTS --- */
/* Contextual inputs according to game state! */

/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<combo_states>_<state prefix>
    - Array keys are states for last prefix
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m`

*   Type:       fsm_<trigger>_<combo_states>_<state prefix>_t
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m_t`

*   Function:   fsm_<trigger>_<state1>_<state2>...

    - <trigger>  -> <prefix><abbreviation> OR <Exit/Entry>
        - event OR Exit/Entry
        - Prefix: e for event
        - See abbreviations in names/game_states.h ...

    - <stateN>      -> <prefix><abbreviation>
        - Prefixes: s for state, ss for substate, m for menu

    - Combo example: `fsm_eCncl_sGMap_ssMenu_mPlSlct`
        - Event calls `fsm_eCncl_sGMap`
        - Which calls `fsm_eCncl_sGMap_ssMenu`
        - Which calls `fsm_eCncl_sGMap_ssMenu_mPlSlct`

*   Hierarchy:
        Trigger: Input
        States:  state -> substate -> menu -> menu option
*/
/* --- TYPEDEFS --- */
typedef void (*fsm_Input_s_t)(struct Game *);

/* -- FSM: Input --  */
extern const fsm_Input_s_t fsm_Input_s[GAME_STATE_NUM];
extern const fsm_Input_s_t fsm_Input_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_Input_sCUTSCENE(       struct Game *sota);
void fsm_Input_sTITLE_SCREEN(   struct Game *sota);
void fsm_Input_sGAMEPLAY_MAP(   struct Game *sota);

void fsm_Input_sGmpMap_ssMenu(struct Game *sota);
void fsm_Input_sGmpMap_ssStby(struct Game *sota);

#endif /* FSM_CONTROLLER_H */
