#ifndef MENU_OPTION_FSM_H
#define MENU_OPTION_FSM_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** mo_fsm: Finite State Machine for game.menu.option
**  - Different behavior according to menu & game state
**
*/

/* --- FINITE-STATE MACHINE FOR MENU OPTIONS --- */
// - Menu options care about parent menu

/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<menu>_<menu_option>
**  - Ex: `fsm_eAcpt_mUAM_moWait`
**  Hierarchy:
**  event   -> /       fsm         \ -> menu_fsm -> mo_fsm
**            / state  ->  substate \
*/

#include "enums.h"

#include "menu/first.h"
#include "menu/map_action.h"
#include "menu/unit_action.h"
#include "menu/item_action.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Menu;

/* --- TYPEDEFS --- */
typedef void (*fsm_menu_t)(struct Game *, struct Menu *);
typedef i32 (* menu_elem_move_t)(struct Menu *, i32);

void fsm_Item_Use(  Game *IES,
                    tnecs_E user_E, tnecs_E patient_E);

/* --- Parent menu DEPENDENT mo_fsm --- */
/* -- UnitActionMenu -- */
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
void fsm_eAcpt_mUAM_moUse(      struct Game *s, struct Menu *mc);

/* -- ItemActionMenu -- */
extern const fsm_menu_t fsm_eAcpt_mIAM_mo[IAM_OPTION_NUM];

void fsm_eAcpt_mIAM(        struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moEquip(struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moUse(  struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moDrop( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mIAM_moTrade(struct Game *s, struct Menu *mc);

/* -- MapActionMenu -- */
extern const fsm_menu_t fsm_eAcpt_mMAM_mo[MAM_OPTION_NUM];

void fsm_eAcpt_mMAM(        struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moUnit( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moCnvy( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moStts( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moQuit( struct Game *s, struct Menu *mc);
void fsm_eAcpt_mMAM_moEndT( struct Game *s, struct Menu *mc);

/* -- FirstMenu -- */
extern const fsm_menu_t fsm_eAcpt_mFM_mo[FM_OPTION_NUM];
void fsm_eAcpt_mFM(struct Game *sota, struct Menu *mc);

void fsm_eAcpt_mFM_moDbgMap(   struct Game *s,
                               struct Menu *mc);

/* --- Parent menu INDEPENDENT mo_fsm --- */

/* -- CursorMoves -- */
extern const fsm_menu_t fsm_eCrsMvs_mo[MENU_OPTION_NUM];

void fsm_eCrsMvs_moAtk(struct Game *sota, struct Menu *mc);

/* -- InputAccept -- */
extern const fsm_menu_t fsm_eAcpt_mo[MENU_OPTION_NUM];

void fsm_eAcpt_moAtk(   struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moStaff( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moDance( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moTrade( struct Game *sota, struct Menu *mc);
void fsm_eAcpt_moUse(   struct Game *sota, struct Menu *mc);

/* -- InputCancel -- */
extern const fsm_menu_t fsm_eCncl_mo[MENU_OPTION_NUM];

void fsm_eCncl_moAtk(   struct Game *sota, struct Menu *mc);
void fsm_eCncl_moDance( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moStaff( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moTrade( struct Game *sota, struct Menu *mc);
void fsm_eCncl_moUse(   struct Game *sota, struct Menu *mc);

#endif /* MENU_OPTION_FSM_H */
