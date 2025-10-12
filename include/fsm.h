#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Unit;
struct Input_Arguments;
struct PlayerSelectMenu;
struct LoadoutSelectMenu;
struct PopUp_Loadout_Stats;

/* --- FINITE-STATE MACHINE FOR EVENTS --- */
/* -- NAMING CONVENTION -- */
/*  fsm_e<trigger>_s<state>_ss<substate>
**  - Example:
**      - `fsm_eCncl_sGMap` calls `fsm_eCncl_sGMap_ssMenu`
**      - `fsm_eUnitDsel_ssMenu` called for every state
**  Hierarchy:
**  event   -> /       fsm         \ -> menu_fsm -> mo_fsm
**            / state  ->  substate \
*/

/* --- TYPEDEFS --- */
typedef void (*fsm_eCncl_s_t)(     struct Game *, tnecs_E);
typedef void (*fsm_eGlbRng_s_t)(   struct Game *);
typedef void (*fsm_eCrsMvd_s_t)(   struct Game *, tnecs_E, struct Point *);
typedef void (*fsm_eGmp2Stby_s_t)( struct Game *, tnecs_E);
typedef void (*fsm_eMenuRight_s_t)(struct Game *, i32);
typedef fsm_eCncl_s_t       fsm_eStart_s_t;
typedef fsm_eCncl_s_t       fsm_eAcpt_s_t;
typedef fsm_eCncl_s_t       fsm_eStats_s_t;
typedef fsm_eCncl_s_t       fsm_eCncl_sGmpMap_ss_t;
typedef fsm_eMenuRight_s_t  fsm_eMenuLeft_s_t;
typedef fsm_eMenuRight_s_t  fsm_eMenuLeft_sGmpMap_ss_t;
typedef fsm_eMenuRight_s_t  fsm_eMenuRight_sGmpMap_ss_t;
typedef fsm_eCrsMvd_s_t     fsm_eCrsMvs_s_t;
typedef fsm_eGlbRng_s_t     fsm_eMenuLeft_sPrep_ss_t;
typedef fsm_eGlbRng_s_t     fsm_eCmbtEnd_ss_t;
typedef fsm_eGmp2Stby_s_t   fsm_eUnitDsel_s_t;
typedef fsm_eGmp2Stby_s_t   fsm_eUnitSel_s_t ;
typedef fsm_eGmp2Stby_s_t   fsm_eUnitDng_s_t;
typedef fsm_eGmp2Stby_s_t   fsm_eCrsDeHvUnit_s_t;
typedef fsm_eGmp2Stby_s_t   fsm_eCrsHvUnit_s_t;
typedef fsm_eGmp2Stby_s_t   fsm_Unit_Wait_state_t;

/* -- FSM: Cursor_Moves EVENT --  */
extern const fsm_eCrsMvs_s_t fsm_eCrsMvs_s[GAME_STATE_NUM];
extern const fsm_eCrsMvs_s_t fsm_eCrsMvs_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern const fsm_eCrsMvs_s_t fsm_eCrsMvs_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_eCrsMvs_s_t fsm_eCrsMvs_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsMvs_sPrep(  struct Game *s, tnecs_E t, struct Point *);
void fsm_eCrsMvs_sGmpMap(struct Game *s, tnecs_E t, struct Point *);

void fsm_eCrsMvs_sGmpMap_ssStby(    struct Game *sota, tnecs_E t, struct Point *);
void fsm_eCrsMvs_sGmpMap_ssMapCndt( struct Game *sota, tnecs_E t, struct Point *);
void fsm_eCrsMvs_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_E t, struct Point *);

/* -- Substate only -> don't care about state -- */
void fsm_eCrsMvs_ssMenu(    struct Game *sota, tnecs_E t, struct Point *);

/* -- FSM: Cursor_Moved EVENT -- */
extern const fsm_eCrsMvd_s_t fsm_eCrsMvd_s[GAME_STATE_NUM];
extern const fsm_eCrsMvd_s_t fsm_eCrsMvd_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsMvd_sGmpMap(struct Game *, tnecs_E t, struct Point *);

void fsm_eCrsMvd_sGmpMap_ssStby(     struct Game *sota, tnecs_E t, struct Point *);
void fsm_eCrsMvd_sGmpMap_ssMapCndt(  struct Game *sota, tnecs_E t, struct Point *);
void fsm_eCrsMvd_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_E t, struct Point *);

void fsm_eCrsMvs_sPrep_ssMapCndt(  struct Game *sota, tnecs_E t, struct Point *);

/* -- FSM: Cursor_Hovers_Unit EVENT -- */
extern const fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_s[GAME_STATE_NUM];
extern const fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsHvUnit_sGmpMap(struct Game *sota, tnecs_E ent);

void fsm_eCrsHvUnit_ssStby(   struct Game *sota, tnecs_E ent);
void fsm_eCrsHvUnit_ssMapCndt(struct Game *sota, tnecs_E ent);

/* -- FSM: Cursor_Dehovers_Unit EVENT -- */
extern const fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_s[GAME_STATE_NUM];
extern const fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsDeHvUnit_sGmpMap(struct Game *sota, tnecs_E ent);

void fsm_eCrsDeHvUnit_ssStby(    struct Game *sota, tnecs_E ent);
void fsm_eCrsDeHvUnit_ssMapCndt( struct Game *sota, tnecs_E ent);
void fsm_eCrsDeHvUnit_ssMapUnitMv(struct Game *sota, tnecs_E ent);

/* -- FSM: Input_Start EVENT -- */
// e.g. PAUSE -> Remove later
extern const fsm_eStart_s_t fsm_eStart_s[GAME_STATE_NUM];
extern const fsm_eAcpt_s_t  fsm_eStart_sPrep_ss[GAME_SUBSTATE_NUM];
void fsm_eStart_sPrep(      struct Game *sota, tnecs_E ent);
void fsm_eStart_sCutScn(    struct Game *sota, tnecs_E ent);
void fsm_eStart_sScnTalk(   struct Game *sota, tnecs_E ent);

void fsm_eStart_sPrep_ssMenu(   struct Game *sota, tnecs_E ent);
void fsm_eStart_sPrep_ssMapCndt(struct Game *sota, tnecs_E ent);

/* -- FSM: Input_Accept EVENT -- */
extern const fsm_eAcpt_s_t fsm_eAcpt_s[GAME_STATE_NUM];
extern const fsm_eAcpt_s_t fsm_eAcpt_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_eAcpt_s_t fsm_eAcpt_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eAcpt_sPrep(   struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap( struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sCutScn( struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sScnTalk(struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sTtlScrn(struct Game *sota, tnecs_E ent);

void fsm_eAcpt_sPrep_ssMapCndt( struct Game *sota, tnecs_E ent);

void fsm_eAcpt_sGmpMap_ssMenu(      struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssStby(      struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssAnim(      struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssMapNPC(    struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssMapMini(   struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssMapCndt(   struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssMapCmbt(   struct Game *sota, tnecs_E ent);
void fsm_eAcpt_sGmpMap_ssMapUnitMv( struct Game *sota, tnecs_E ent);

void fsm_eAcpt_sTtlScrn_ssMenu(     struct Game *sota, tnecs_E ent);

/* -- FSM: Input_Cancel EVENT -- */
extern const fsm_eCncl_s_t  fsm_eCncl_s[GAME_STATE_NUM];
extern const fsm_eCncl_sGmpMap_ss_t fsm_eCncl_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_eCncl_sGmpMap_ss_t fsm_eCncl_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eCncl_sPrep(   struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap( struct Game *sota, tnecs_E ent);
void fsm_eCncl_sScnTalk(struct Game *sota, tnecs_E ent);

void fsm_eCncl_sPrep_ssMenu(    struct Game *sota, tnecs_E ent);
void fsm_eCncl_sPrep_ssMapCndt( struct Game *sota, tnecs_E ent);

void fsm_eCncl_sGmpMap_ssMenu(    struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssStby(    struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssAnim(    struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssMapNPC(  struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssMapCndt( struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssMapCmbt( struct Game *sota, tnecs_E ent);
void fsm_eCncl_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_E ent);

/* -- FSM: Input_Stats EVENT -- */
extern const fsm_eStats_s_t fsm_eStats_s[GAME_STATE_NUM];
extern const fsm_eStats_s_t fsm_eStats_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_eStats_s_t fsm_eStats_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eStats_sPrep(  struct Game *sota, tnecs_E ent);

void fsm_eStats_sPrep_ssMenu(   struct Game *sota, tnecs_E ent);
void fsm_eStats_sPrep_ssMapCndt(struct Game *sota, tnecs_E ent);

void fsm_eStats_sGmpMap(struct Game *sota, tnecs_E ent);

void fsm_eStats_sGmpMap_ssStby(  struct Game *sota, tnecs_E ent);


/* -- FSM: Input_menuRight EVENT -- */
extern const fsm_eMenuRight_s_t  fsm_eMenuRight_s[GAME_STATE_NUM];
extern const fsm_eMenuRight_sGmpMap_ss_t fsm_eMenuRight_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eMenuRight_sGmpMap(    struct Game *sota, i32 c);
void fsm_eMenuRight_sScnTalk(   struct Game *sota, i32 c);

void fsm_eMenuRight_sGmpMap_ssMenu(struct Game *sota, i32 c);
void fsm_eMenuRight_sGmpMap_ssStby(struct Game *sota, i32 c);

/* -- FSM: Input_menuLeft EVENT -- */
extern const fsm_eMenuLeft_s_t fsm_eMenuLeft_s[GAME_STATE_NUM];
extern const fsm_eMenuLeft_sPrep_ss_t     fsm_eMenuLeft_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_eMenuLeft_sGmpMap_ss_t   fsm_eMenuLeft_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eMenuLeft_sPrep(   struct Game *sota, i32 c);
void fsm_eMenuLeft_sGmpMap( struct Game *sota, i32 c);

void fsm_eMenuLeft_sPrep_ssMenu(    struct Game *sota);
void fsm_eMenuLeft_sPrep_ssMapCndt( struct Game *sota);

void fsm_eMenuLeft_sGmpMap_ssMenu(struct Game *sota, i32 c);
void fsm_eMenuLeft_sGmpMap_ssStby(struct Game *sota, i32 c);

/* -- FSM: Unit_Danger EVENT -- */
extern const fsm_eUnitDng_s_t fsm_eUnitDng_s[GAME_STATE_NUM];
extern const fsm_eUnitDng_s_t fsm_eUnitDng_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitDng_ssStby(struct Game *sota, tnecs_E ent);

/* -- FSM: Gameplay_Return2Standby EVENT -- */
extern const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_s[GAME_STATE_NUM];
extern const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_ss[GAME_SUBSTATE_NUM];

void fsm_eGmp2Stby_sGmpMap(   struct Game *sota, tnecs_E ent);
void fsm_eGmp2Stby_ssMapUnitMv(struct Game *sota, tnecs_E ent);

/* -- FSM: Unit_Select EVENT -- */
extern const fsm_eUnitSel_s_t fsm_eUnitSel_s[GAME_STATE_NUM];
extern const fsm_eUnitSel_s_t fsm_eUnitSel_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitSel_ssStby(struct Game *sota, tnecs_E ent);

/* -- FSM: Unit_Deselect EVENT -- */
extern const fsm_eUnitDsel_s_t fsm_eUnitDsel_s[GAME_STATE_NUM];
extern const fsm_eUnitDsel_s_t fsm_eUnitDsel_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitDsel_ssMenu(      struct Game *sota, tnecs_E ent);
void fsm_eUnitDsel_ssMapUnitMv( struct Game *sota, tnecs_E ent);

/* -- FSM: Input_globalRange EVENT -- */
extern const fsm_eGlbRng_s_t fsm_eGlbRng_ss[GAME_SUBSTATE_NUM];

void fsm_eGlbRng_ssStby(struct Game *sota);

/* -- FSM: Combat_End EVENT -- */
extern const fsm_eCmbtEnd_ss_t fsm_eCmbtEnd_ss[GAME_SUBSTATE_NUM];

void fsm_eCmbtEnd_ssMapNPC(  struct Game *sota);
void fsm_eCmbtEnd_ssMapCndt( struct Game *sota);

#endif /* FSM_H */
