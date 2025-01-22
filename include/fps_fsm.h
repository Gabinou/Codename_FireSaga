#ifndef RENDER_FSM_H
#define RENDER_FSM_H

#include "enums.h"

/* --- FINITE-STATE MACHINE FOR CONTROL --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<combo_states>_<state prefix>
    - Array keys are states for last prefix
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m`
        - Prefixes: m for menu, s for state, ss for substate

*   Type:       fsm_<trigger>_<combo_states>_<state prefix>_t
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m_t`

*   Function:   fsm_<trigger>_<combo_states>...

    - <trigger>  : r for render, c for control + Frame
    - <combo_states> -> <state1>_<state2>_... -> <prefix><Event>_...
        - Prefixes: s for state, ss for substate
        - Example: `fsm_rFrame_sCmbt`

*   Hierarchy:
        Trigger: Frame
        States:  state -> substate -> menu -> menu option

*/

/* --- FORWARD DECLARATIONS --- */
struct Game;

typedef void (*fsm_main_t)(struct Game *);

extern const fsm_main_t fsm_cFrame_s[GAME_STATE_NUM];
extern const fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern const fsm_main_t fsm_rFrame_s[GAME_STATE_NUM];
extern const fsm_main_t fsm_rFrame_sGmpMap_ss[GAME_SUBSTATE_NUM];

/* --- Control FSM --- */
/* - Control FSM standalone state - */
void fsm_cFrame_sCmbt(   struct Game *sota);
void fsm_cFrame_sPrep(   struct Game *sota);
void fsm_cFrame_sCutScn( struct Game *sota);
void fsm_cFrame_sGmpMap( struct Game *sota);
void fsm_cFrame_sScnTalk(struct Game *sota);
void fsm_cFrame_sTtlScrn(struct Game *sota);
void fsm_cFrame_sGmpCamp(struct Game *sota);

/* - Control FSM combo Gameplay_Map state + substate - */
void fsm_cFrame_sGmpMap_ssMenu(     struct Game *sota);
void fsm_cFrame_sGmpMap_ssSave(     struct Game *sota);
void fsm_cFrame_sGmpMap_ssStby(     struct Game *sota);
void fsm_cFrame_sGmpMap_ssAnim(     struct Game *sota);
void fsm_cFrame_sGmpMap_ssPause(    struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapNPC(   struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapCmbt(  struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapMini(  struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapAnim(  struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapCndt(  struct Game *sota);
void fsm_cFrame_sGmpMap_ssMapUnitMv(struct Game *sota);

/* - Control FSM combo Prep state + substate - */
void fsm_cFrame_sPrep_ssMapCndt(    struct Game *sota);

/* --- render FSM --- */
/* - Render FSM standalone state - */
void fsm_rFrame_sCmbt(   struct Game *sota);
void fsm_rFrame_sPrep(   struct Game *sota);
void fsm_rFrame_sCutScn( struct Game *sota);
void fsm_rFrame_sGmpMap( struct Game *sota);
void fsm_rFrame_sGmpCamp(struct Game *sota);
void fsm_rFrame_sScnTalk(struct Game *sota);
void fsm_rFrame_sTtlScrn(struct Game *sota);

/* Render FSM combo Gameplay_Map state + substate */
void fsm_rFrame_sGmpMap_ssMenu(     struct Game *sota);
void fsm_rFrame_sGmpMap_ssSave(     struct Game *sota);
void fsm_rFrame_sGmpMap_ssStby(     struct Game *sota);
void fsm_rFrame_sGmpMap_ssAnim(     struct Game *sota);
void fsm_rFrame_sGmpMap_ssPause(    struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapNPC(   struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapCmbt(  struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapMini(  struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapAnim(  struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapCndt(  struct Game *sota);
void fsm_rFrame_sGmpMap_ssMapUnitMv(struct Game *sota);

#endif /* RENDER_FSM_H */
