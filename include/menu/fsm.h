#ifndef MENU_FPS_H
#define MENU_FPS_H

#include "enums.h"

#include "menu/first.h"
#include "menu/unit_action.h"
#include "menu/item_action.h"

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
extern const menu_elem_move_t menu_elem_move[MENU_TYPE_END];
i32 DeploymentMenu_Elem_Move(struct Menu *mc, i32 d);

/* event_Input_Accept */
extern const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_m[MENU_TYPE_END];
void fsm_eAcpt_sGmpMap_ssMenu_mDM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mTM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSM( struct Game *sota, struct Menu *mc);
// void fsm_eAcpt_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mUAM_mo[UAM_OPTION_NUM];
void fsm_eAcpt_sGmpMap_ssMenu_mUAM( struct Game *sota,
                                    struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moItem(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTrade( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moSeize( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTalk(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moAtk(   struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moStaff( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moDance( struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moRescue(struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moOpen(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moWait(  struct Game *s, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mIAM_mo[IAM_OPTION_NUM];
void fsm_eAcpt_sGmpMap_ssMenu_mIAM( struct Game *sota,
                                    struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moEquip(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moUse(    struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moDrop(   struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moTrade(  struct Game *s, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mMAM_mo[UAM_OPTION_NUM];
void fsm_eAcpt_sGmpMap_ssMenu_mMAM( struct Game *sota,
                                    struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moUnit(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moCnvy(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moStts(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moQuit(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moEndT(  struct Game *s, struct Menu *mc);

/* event_Input_Cancel */
extern const fsm_menu_t fsm_eCncl_sGmpMap_ssMenu_m[MENU_TYPE_END];
void fsm_eCncl_sGmpMap_ssMenu_mSM( struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mTM( struct Game *sota, struct Menu *mc);
// void fsm_eCncl_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mUAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mMAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mIAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eCncl_sPrep_ssMenu_m[MENU_TYPE_END];
void fsm_eCncl_sPrep_ssMenu_mSM( struct Game *sota, struct Menu *mc);
void fsm_eCncl_sPrep_ssMenu_mDM( struct Game *sota, struct Menu *mc);

// extern const fsm_menu_t fsm_eCncl_sPrep_ssMapCndt_m[MENU_TYPE_END];
// void fsm_eCncl_sPrep_ssMapCndt_mSM( struct Game *sota, struct Menu *mc);
// void fsm_eCncl_sPrep_ssMapCndt_mDM( struct Game *sota, struct Menu *mc);

/* event_Cursor_Moves */
extern const fsm_menu_t fsm_eCrsMvs_sGmpMap_ssMenu_m      [MENU_TYPE_END];
void fsm_eCrsMvs_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eCrsMvs_sGmpMap_mo[MENU_OPTION_NUM];
void fsm_eCrsMvs_sGmpMap_moAtk(struct Game *sota, struct Menu *mc);

/* This name doesn't make sense. It should be:
    - sGmpMap -> sTtlScrn
 */
extern const fsm_menu_t fsm_eAcpt_sTtlScrn_ssMenu_m[MENU_TYPE_END];
extern const fsm_menu_t fsm_eAcpt_sTtlScrn_ssMenu_mFM_mo[FM_OPTION_NUM];
void fsm_eAcpt_sTtlScrn_ssMenu_mFM(struct Game *sota, struct Menu *mc);

void fsm_eAcpt_sTtlScrn_ssMenu_mFM_moDbgMap(   struct Game *s,
                                               struct Menu *mc);

/* -- Map candidates FSMs -- */
/* TODO:
**  2. New name convention -> fsm_eAcpt_ssMapCndt_moAtk
**      - Menu does not determine behavior, option does
**      - Previous menu is always on top of menu_stack
**      - + fact that its candidate choice time i.e. ssMapCndt */
/* event_Input_Accept */
extern const fsm_menu_t fsm_eAcpt_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eAcpt_sGmpMap_ssMapCndt_moAtk(  struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_sGmpMap_ssMapCndt_moUse(struct Game *sota, struct Menu *mc);

/* event_Input_Cancel */
extern const fsm_menu_t fsm_eCncl_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM];

void fsm_eCncl_sGmpMap_ssMapCndt_moAtk(  struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *mc);
void fsm_eCncl_sGmpMap_ssMapCndt_moUse(struct Game *sota, struct Menu *mc);

/* event_Input_Start */
extern const fsm_menu_t fsm_eStart_sPrep_ssMenu_m[MENU_TYPE_END];
void fsm_eStart_sPrep_ssMenu_mSM(struct Game *sota, struct Menu *mc);
void fsm_eStart_sPrep_ssMenu_mDM(struct Game *sota, struct Menu *mc);

/* event_Input_Stats */
/* -- Stats  -- */
extern const fsm_menu_t fsm_eStats_sPrep_ssMenu_m[MENU_TYPE_END];
void fsm_eStats_sPrep_ssMenu_mSM( struct Game *sota, struct Menu *mc);
void fsm_eStats_sPrep_ssMenu_mDM( struct Game *sota, struct Menu *mc);

// extern const fsm_menu_t fsm_eStats_sPrep_ssMapCndt_m[MENU_TYPE_END];
// void fsm_eStats_sPrep_ssMapCndt_mSM( struct Game *sota, struct Menu *mc);
// void fsm_eStats_sPrep_ssMapCndt_mDM( struct Game *sota, struct Menu *mc);


#endif /* MENU_FPS_H */
