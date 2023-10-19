#ifndef RENDER_FSM_H
#define RENDER_FSM_H

#include "enums.h"
#include "game/game.h"
#include "game/cursor.h"
#include "globals.h"
#include "n9patch.h"

/* --- FINITE-STATE MACHINE FOR CONTROL --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<type>_<event>
        - Example: `fsm_R_s`
*   Type:       fsm_<type>_<event>_t
        - Example: `fsm_R_s_t`
*   Function:   fsm_<type>_<state1>_<state2>...
    - <type>  :     R for render, C for control
    - <stateN>      -> <prefix><Event>
        - Prefixes: s for state, ss for substate
        - Example: `fsm_R_sCombat`

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
void fsm_C_sCombat(       struct Game *sota);
void fsm_C_sScene_FMV(    struct Game *sota);
void fsm_C_sScene_Talk(   struct Game *sota);
void fsm_C_sPreparation(  struct Game *sota);
void fsm_C_sTitle_Screen( struct Game *sota);
void fsm_C_sGameplay_Map( struct Game *sota);
void fsm_C_sGameplay_Camp(struct Game *sota);

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
void fsm_R_sCombat(       struct Game *sota);
void fsm_R_sScene_FMV(    struct Game *sota);
void fsm_R_sScene_Talk(   struct Game *sota);
void fsm_R_sPreparation(  struct Game *sota);
void fsm_R_sTitle_Screen( struct Game *sota);
void fsm_R_sGameplay_Map( struct Game *sota);
void fsm_R_sGameplay_Camp(struct Game *sota);

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
