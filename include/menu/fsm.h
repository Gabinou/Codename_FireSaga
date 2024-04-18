#ifndef MENU_FPS_H
#define MENU_FPS_H

#include "enums.h"
#include "menu/menu.h"
#include "menu/staff_select.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Menu;

/* --- TYPEDEFS --- */
typedef void (*fsm_menu_t)(struct Game *, struct Menu *);
typedef i32 (* menu_elem_move_t)(struct Menu *, i32);

/* --- FINITE-STATE MACHINE FOR EVENTS --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<combo_states>_<state prefix>
    - Array keys are states for last prefix
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m`

*   Type:       fsm_<trigger>_<combo_states>_<state prefix>_t
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m_t`

*   Function:   fsm_<trigger>_<state1>_<state2>...

    - <trigger>  -> <prefix><abbreviation> OR <Exit/Entry>
        - Prefix: e for event
        - See abbreviations in names/game_states.h ...

    - <stateN>      -> <prefix><abbreviation>
        - Prefixes: m for menu, mo for menu option

    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_moStats`
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_mPreCmbt`

*   Hierarchy:
        Trigger: Event
        States:  state -> substate -> menu -> menu option
*/

/* -- Menu-type FSMs -- */
/* menu_elem_move */
extern menu_elem_move_t menu_elem_move[MENU_TYPE_END];
i32 DeploymentMenu_Elem_Move(struct Menu *mc, i32 d);

/* event_Input_Accept */
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_m[MENU_TYPE_END];
void fsm_eAcpt_sGmpMap_ssMenu_mDM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mTM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSM( struct Game *sota, struct Menu *mc);
// void fsm_eAcpt_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

/* event_Input_Cancel */
extern fsm_menu_t fsm_eCncl_sGmpMap_ssMenu_m[MENU_TYPE_END];
void fsm_eCncl_sGmpMap_ssMenu_mSM( struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mTM( struct Game *sota, struct Menu *mc);
// void fsm_eCncl_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

/* event_Cursor_Moves */
extern fsm_menu_t fsm_eCrsMvs_sGmpMap_ssMenu_m      [MENU_TYPE_END];
void fsm_eCrsMvs_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

extern fsm_menu_t fsm_eCrsMvs_sGmpMap_mo[MENU_OPTION_NUM];
void fsm_eCrsMvs_sGmpMap_moAtk(struct Game *sota, struct Menu *mc);

/* -- PSM menu-option FSMs -- */
// Only posible state pair [Gameplay_Map, MENU]
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo[MENU_OPTION_NUM];

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moAtk(   struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moQuit(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moWait(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moItem(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moEndT(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moStaff( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDance( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moSeize( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moTrade( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDbgMap(struct Game *s, struct Menu *mc);

/* -- Map candidates FSMs -- */
/* event_Input_Accept */
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eAcpt_sGmpMap_ssMapCndt_moAtk(  struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *mc);

/* event_Input_Cancel */
extern fsm_menu_t fsm_eCncl_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eCncl_sGmpMap_ssMapCndt_moAtk(  struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *mc);

/* event_Input_Start */
extern fsm_menu_t fsm_eStart_sPrep_ssMenu_m[MENU_TYPE_END];
void fsm_eStart_sPrep_ssMenu_mSM(struct Game *sota, struct Menu *mc);
void fsm_eStart_sPrep_ssMenu_mDM(struct Game *sota, struct Menu *mc);

/* -- Menu Pop FSM -- */
/* Need Popped Last / Any popped menu fsm ? */
extern fsm_menu_t fsm_Pop_sGmpMap_ssMenu_m[MENU_TYPE_END];

void fsm_Pop_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc);

#endif /* MENU_FPS_H */
