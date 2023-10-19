#ifndef RENDER_FSM_H
#define RENDER_FSM_H

#include "enums.h"
#include "game/game.h"
#include "game/cursor.h"
#include "globals.h"
#include "n9patch.h"

/* --- FINITE-STATE MACHINE FOR CONTROL --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<combo_states>_<state prefix>
    - Array keys are states for last prefix 
    - Ex: `fsm_eAcpt_sGmpMap_m`

*   Type:       fsm_<trigger>_<combo_states>_<state prefix>_t
    - Ex: `fsm_eAcpt_sGmpMap_m_t`

*   Function:   fsm_<trigger>_<state1>_<state2>...

    - <trigger>  :     R for render, C for control (every frame)
    - <stateN>      -> <prefix><Event>
        - Prefixes: s for state, ss for substate
        - Example: `fsm_R_sCmbt`

*   Hierarchy:
        Trigger: Frame
        States:  state -> substate -> menu -> menu option

*/

typedef void (*fsm_main_t)(struct Game *);

extern fsm_main_t fsm_C_s[GAME_STATE_NUM];
extern fsm_main_t fsm_C_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern fsm_main_t fsm_R_s[GAME_STATE_NUM];
extern fsm_main_t fsm_R_sGmpMap_ss[GAME_SUBSTATE_NUM];

/* --- Control FSM --- */
/* - Control FSM standalone state - */
void fsm_C_sCmbt(   struct Game *sota);
void fsm_C_sPrep(   struct Game *sota);
void fsm_C_sScnFMV( struct Game *sota);
void fsm_C_sGmpMap( struct Game *sota);
void fsm_C_sScnTalk(struct Game *sota);
void fsm_C_sTtlScrn(struct Game *sota);
void fsm_C_sGmpCamp(struct Game *sota);

/* - Control FSM combo Gameplay_Map state + substate - */
void fsm_C_sGmpMap_ssMenu(          struct Game *sota);
void fsm_C_sGmpMap_ssSaving(        struct Game *sota);
void fsm_C_sGmpMap_ssPaused(        struct Game *sota);
void fsm_C_sGmpMap_ssStandby(       struct Game *sota);
void fsm_C_sGmpMap_ssAnimation(     struct Game *sota);
void fsm_C_sGmpMap_ssMap_Combat(    struct Game *sota);
void fsm_C_sGmpMap_ssMap_Minimap(   struct Game *sota);
void fsm_C_sGmpMap_ssMap_NPCTurn(   struct Game *sota);
void fsm_C_sGmpMap_ssMap_Animation( struct Game *sota);
void fsm_C_sGmpMap_ssMap_Candidates(struct Game *sota);
void fsm_C_sGmpMap_ssMap_Unit_Moves(struct Game *sota);

/* --- render FSM --- */
/* - Render FSM standalone state - */
void fsm_R_sCmbt(   struct Game *sota);
void fsm_R_sPrep(   struct Game *sota);
void fsm_R_sScnFMV( struct Game *sota);
void fsm_R_sGmpMap( struct Game *sota);
void fsm_R_sGmpCamp(struct Game *sota);
void fsm_R_sScnTalk(struct Game *sota);
void fsm_R_sTtlScrn(struct Game *sota);

/* Render FSM combo Gameplay_Map state + substate */
void fsm_R_sGmpMap_ssMenu(          struct Game *sota);
void fsm_R_sGmpMap_ssPaused(        struct Game *sota);
void fsm_R_sGmpMap_ssSaving(        struct Game *sota);
void fsm_R_sGmpMap_ssStandby(       struct Game *sota);
void fsm_R_sGmpMap_ssAnimation(     struct Game *sota);
void fsm_R_sGmpMap_ssMap_Combat(    struct Game *sota);
void fsm_R_sGmpMap_ssMap_Minimap(   struct Game *sota);
void fsm_R_sGmpMap_ssMap_NPCTurn(   struct Game *sota);
void fsm_R_sGmpMap_ssMap_Animation( struct Game *sota);
void fsm_R_sGmpMap_ssMap_Unit_Moves(struct Game *sota);
void fsm_R_sGmpMap_ssMap_Defendants(struct Game *sota);

#endif /* RENDER_FSM_H */
