#ifndef MENU_FSM_H
#define MENU_FSM_H

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

/* --- FINITE-STATE MACHINE FOR MENUS --- */
// - Menus don't care about game state

/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<menu>_<menu_option>
    - Ex: `fsm_eAcpt_mUAM_moWait`
    - Ex: `fsm_eAcpt_mPreCmbt`
*   Hierarchy:
        Trigger: Event
        States:  state -> substate -> menu -> menu option
*/

/* -- Menu-type FSMs -- */
/* menu_elem_move */
extern const menu_elem_move_t menu_elem_move[MENU_TYPE_END];

i32 DeploymentMenu_Elem_Move(struct Menu *mc, i32 d);

/* event_Input_Accept */
extern const fsm_menu_t fsm_eAcpt_m[MENU_TYPE_END];

void fsm_eAcpt_mDM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_mTM( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_mSM( struct Game *sota, struct Menu *mc);
// void fsm_eAcpt_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_mISM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eAcpt_mLSM(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_mUAM_mo[UAM_OPTION_NUM];

void fsm_eAcpt_mUAM(            struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moItem(     struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moTrade(    struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moSeize(    struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moTalk(     struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moAtk(      struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moStaff(    struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moDance(    struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moRescue(   struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moOpen(     struct Game *s, struct Menu *mc);
void fsm_eAcpt_mUAM_moWait(     struct Game *s, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_mIAM_mo[IAM_OPTION_NUM];

void fsm_eAcpt_mIAM(        struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moEquip(struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moUse(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moDrop( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moTrade(struct Game *s, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_mMAM_mo[UAM_OPTION_NUM];

void fsm_eAcpt_mMAM(        struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moUnit( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moCnvy( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moStts( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moQuit( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moEndT( struct Game *s, struct Menu *mc);

/* event_Input_Cancel */
extern const fsm_menu_t fsm_eCncl_m[MENU_TYPE_END];

void fsm_eCncl_mSM( struct Game *sota, struct Menu *mc);
void fsm_eCncl_mTM( struct Game *sota, struct Menu *mc);
// void fsm_eCncl_mPCP(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mUAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mMAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mIAM(struct Game *sota, struct Menu *mc);
void fsm_eCncl_mLSM(struct Game *sota, struct Menu *mc);

/* event_Cursor_Moves */
extern const fsm_menu_t fsm_eCrsMvs_m      [MENU_TYPE_END];

void fsm_eCrsMvs_mISM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_mSSM(struct Game *sota, struct Menu *mc);
void fsm_eCrsMvs_mLSM(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eCrsMvs_mo[MENU_OPTION_NUM];
void fsm_eCrsMvs_moAtk(struct Game *sota, struct Menu *mc);

extern const fsm_menu_t fsm_eAcpt_m[MENU_TYPE_END];
extern const fsm_menu_t fsm_eAcpt_mFM_mo[FM_OPTION_NUM];
void fsm_eAcpt_mFM(struct Game *sota, struct Menu *mc);

void fsm_eAcpt_mFM_moDbgMap(   struct Game *s,
                               struct Menu *mc);

/* -- Map candidates FSMs -- */
extern const fsm_menu_t fsm_eAcpt_mo[MENU_OPTION_NUM];

void fsm_eAcpt_moAtk(   struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moStaff( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moDance( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moTrade( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moUse(   struct Game *sota, struct Menu *mc);

/* event_Input_Cancel */
extern const fsm_menu_t fsm_eCncl_mo[MENU_OPTION_NUM];

void fsm_eCncl_moAtk(   struct Game *sota, struct Menu *mc);
void fsm_eCncl_moDance( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moStaff( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moTrade( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moUse(   struct Game *sota, struct Menu *mc);

/* event_Input_Stats */
/* -- Stats  -- */
extern const fsm_menu_t fsm_eStats_m[MENU_TYPE_END];

void fsm_eStats_mSM( struct Game *sota, struct Menu *mc);
void fsm_eStats_mDM( struct Game *sota, struct Menu *mc);

#endif /* MENU_FSM_H */
