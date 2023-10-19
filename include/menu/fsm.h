#ifndef MENU_FPS_H
#define MENU_FPS_H

#include "enums.h"
#include "menu/menu.h"
#include "menu/staff_select.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- TYPEDEFS --- */
typedef void (*fsm_menu_t)(struct Game *, struct MenuComponent *);

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
extern fsm_menu_t fsm_eCrsMvs_m             [MENU_TYPE_END];
extern fsm_menu_t fsm_eCncl_sGmpMap_ssMenu_m[MENU_TYPE_END];
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_m[MENU_TYPE_END];

/* event_Input_Accept */
void fsm_eAcpt_sGmpMap_ssMenu_mTM( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSM( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPCM(struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mISM(struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSSM(struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM(struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mWSM(struct Game *sota, struct MenuComponent *mc);

/* event_Input_Cancel */
void fsm_eCncl_sGmpMap_ssMenu_mSM( struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mTM( struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mPCM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mISM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mSSM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mPSM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMenu_mWSM(struct Game *sota, struct MenuComponent *mc);

/* event_Cursor_Moves */
void fsm_eCrsMvs_mISM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCrsMvs_mSSM(struct Game *sota, struct MenuComponent *mc);
void fsm_eCrsMvs_mWSM(struct Game *sota, struct MenuComponent *mc);

/* -- PSM menu-option FSMs -- */
// Only posible state pair [Gameplay_Map, MENU]
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo[MENU_OPTION_NUM];

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_quit(     struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_wait(     struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_staff(    struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_dance(    struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_items(    struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_trade(    struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_attack(   struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_end_turn( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo_debug_map(struct Game *sota, struct MenuComponent *mc);

/* -- Map candidates FSMs -- */
/* event_Input_Accept */
extern fsm_menu_t fsm_eAcpt_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eAcpt_sGmpMap_ssMapCndt_mo_staff( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_mo_dance( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_mo_trade( struct Game *sota, struct MenuComponent *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_mo_attack(struct Game *sota, struct MenuComponent *mc);

/* event_Input_Cancel */
extern fsm_menu_t fsm_eCncl_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eCncl_sGmpMap_ssMapCndt_mo_dance( struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_mo_staff( struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_mo_trade( struct Game *sota, struct MenuComponent *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_mo_attack(struct Game *sota, struct MenuComponent *mc);

#endif /* MENU_FPS_H */
