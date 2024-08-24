#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include "enums.h"
#include "unit/unit.h"
#include "game/popup.h"
#include "game/cursor.h"
#include "game/menu.h"
#include "game/unit.h"
#include "menu/menu.h"
#include "menu/fsm.h"
#include "types.h"
#include "tnecs.h"
#include "unit/unit.h"
#include "slider.h"
#include "nmath.h"
#include "popup/loadout_stats.h"
#include "menu/player_select.h"
#include "menu/loadout_select.h"
#include "arrow.h"
#include "structs.h"
#include "map/ontile.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Unit;
struct Input_Arguments;
struct PlayerSelectMenu;
struct LoadoutSelectMenu;
struct PopUp_Loadout_Stats;

/* --- FINITE-STATE MACHINE FOR EVENTS --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<trigger>_<combo_states>_<state prefix>
    - Array keys are states for last prefix
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m`

*   Type:       fsm_<trigger>_<combo_states>_<state prefix>_t
    - Ex: `fsm_eAcpt_sGmpMap_ssMenu_m_t`

*   Function:   fsm_<trigger>_<combo_states>_...

    - <trigger>  -> <prefix><abbreviation> OR <Exit/Entry>
        - event OR Exit/Entry
        - Prefix: e for event
        - See abbreviations in names/game_states.h ...

    - <combo_states> -> <state1>_<state2>_... -> <prefix><abbreviation>_...
        - Prefixes: s for state, ss for substate, m for menu

    - Combo example: `fsm_eCncl_sGMap_ssMenu_mPlSlct`
        - Event calls `fsm_eCncl_sGMap`
        - Which calls `fsm_eCncl_sGMap_ssMenu`
        - Which calls `fsm_eCncl_sGMap_ssMenu_mPlSlct`

*   Hierarchy:
        Trigger: Event
        States:  state -> substate -> menu -> menu option
*/

/* --- TYPEDEFS --- */
typedef void (*fsm_eCncl_s_t)(     struct Game *, tnecs_entity);
typedef void (*fsm_eGlbRng_s_t)(   struct Game *);
typedef void (*fsm_eCrsMvd_s_t)(   struct Game *, tnecs_entity, struct Point *);
typedef void (*fsm_eGmp2Stby_s_t)( struct Game *, tnecs_entity);
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
typedef fsm_eGmp2Stby_s_t   fsm_eUnitDsel_s_t;

/* -- FSM: Cursor_Moves EVENT --  */
extern fsm_eCrsMvs_s_t fsm_eCrsMvs_s[GAME_STATE_NUM];
extern fsm_eCrsMvs_s_t fsm_eCrsMvs_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern fsm_eCrsMvs_s_t fsm_eCrsMvs_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eCrsMvs_s_t fsm_eCrsMvs_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsMvs_sPrep(  struct Game *s, tnecs_entity t, struct Point *);
void fsm_eCrsMvs_sGmpMap(struct Game *s, tnecs_entity t, struct Point *);

void fsm_eCrsMvs_sGmpMap_ssStby(    struct Game *sota, tnecs_entity t, struct Point *);
void fsm_eCrsMvs_sGmpMap_ssMapCndt( struct Game *sota, tnecs_entity t, struct Point *);
void fsm_eCrsMvs_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity t, struct Point *);

/* -- Substate only -> don't care about state -- */
void fsm_eCrsMvs_ssMenu(    struct Game *sota, tnecs_entity t, struct Point *);

/* -- FSM: Cursor_Moved EVENT -- */
extern fsm_eCrsMvd_s_t fsm_eCrsMvd_s[GAME_STATE_NUM];
extern fsm_eCrsMvd_s_t fsm_eCrsMvd_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eCrsMvd_s_t fsm_eCrsMvd_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsMvd_sGmpMap(struct Game *, tnecs_entity t, struct Point *);

void fsm_eCrsMvd_sGmpMap_ssStby(     struct Game *sota, tnecs_entity t, struct Point *);
void fsm_eCrsMvd_sGmpMap_ssMapCndt(  struct Game *sota, tnecs_entity t, struct Point *);
void fsm_eCrsMvd_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity t, struct Point *);

void fsm_eCrsMvs_sPrep_ssMapCndt(  struct Game *sota, tnecs_entity t, struct Point *);

/* -- FSM: Cursor_Hovers_Unit EVENT -- */
extern fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_s[GAME_STATE_NUM];
extern fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsHvUnit_sGmpMap(struct Game *sota, tnecs_entity ent);

void fsm_eCrsHvUnit_ssStby(   struct Game *sota, tnecs_entity ent);
void fsm_eCrsHvUnit_ssMapCndt(struct Game *sota, tnecs_entity ent);

/* -- FSM: Cursor_Dehovers_Unit EVENT -- */
extern fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_s[GAME_STATE_NUM];
extern fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_ss[GAME_SUBSTATE_NUM];

void fsm_eCrsDeHvUnit_sGmpMap(struct Game *sota, tnecs_entity ent);

void fsm_eCrsDeHvUnit_ssStby(    struct Game *sota, tnecs_entity ent);
void fsm_eCrsDeHvUnit_ssMapCndt( struct Game *sota, tnecs_entity ent);
void fsm_eCrsDeHvUnit_ssMapUnitMv(struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_Start EVENT -- */
// e.g. PAUSE -> Remove later
extern fsm_eStart_s_t fsm_eStart_s[GAME_STATE_NUM];
extern fsm_eAcpt_s_t  fsm_eStart_sPrep_ss[GAME_SUBSTATE_NUM];
void fsm_eStart_sPrep(      struct Game *sota, tnecs_entity ent);
void fsm_eStart_sCutScn(    struct Game *sota, tnecs_entity ent);
void fsm_eStart_sScnTalk(   struct Game *sota, tnecs_entity ent);

void fsm_eStart_sPrep_ssMenu(   struct Game *sota, tnecs_entity ent);
void fsm_eStart_sPrep_ssMapCndt(struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_Accept EVENT -- */
extern fsm_eAcpt_s_t fsm_eAcpt_s[GAME_STATE_NUM];
extern fsm_eAcpt_s_t fsm_eAcpt_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eAcpt_s_t fsm_eAcpt_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eAcpt_sPrep(   struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap( struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sCutScn( struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sScnTalk(struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sTtlScrn(struct Game *sota, tnecs_entity ent);

void fsm_eAcpt_sPrep_ssMapCndt( struct Game *sota, tnecs_entity ent);

void fsm_eAcpt_sGmpMap_ssMenu(    struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssStby(    struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssAnim(    struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssMapNPC(  struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssMapMini( struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssMapCndt( struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssMapCmbt( struct Game *sota, tnecs_entity ent);
void fsm_eAcpt_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_Cancel EVENT -- */
extern fsm_eCncl_s_t  fsm_eCncl_s[GAME_STATE_NUM];
extern fsm_eCncl_sGmpMap_ss_t fsm_eCncl_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eCncl_sGmpMap_ss_t fsm_eCncl_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eCncl_sPrep(   struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap( struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sScnTalk(struct Game *sota, tnecs_entity ent);

void fsm_eCncl_sPrep_ssMenu(    struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sPrep_ssMapCndt( struct Game *sota, tnecs_entity ent);

void fsm_eCncl_sGmpMap_ssMenu(    struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssStby(    struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssAnim(    struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssMapNPC(  struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssMapCndt( struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssMapCmbt( struct Game *sota, tnecs_entity ent);
void fsm_eCncl_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_Stats EVENT -- */
extern fsm_eStats_s_t fsm_eStats_s[GAME_STATE_NUM];
extern fsm_eStats_s_t fsm_eStats_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eStats_s_t fsm_eStats_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eStats_sPrep(  struct Game *sota, tnecs_entity ent);
void fsm_eStats_sGmpMap(struct Game *sota, tnecs_entity ent);

void fsm_eStats_sGmpMap_ssStby(struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_menuRight EVENT -- */
extern fsm_eMenuRight_s_t  fsm_eMenuRight_s[GAME_STATE_NUM];
extern fsm_eMenuRight_sGmpMap_ss_t fsm_eMenuRight_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eMenuRight_sGmpMap(    struct Game *sota, i32 c);
void fsm_eMenuRight_sScnTalk(   struct Game *sota, i32 c);

void fsm_eMenuRight_sGmpMap_ssMenu(struct Game *sota, i32 c);
void fsm_eMenuRight_sGmpMap_ssStby(struct Game *sota, i32 c);

/* -- FSM: Input_menuLeft EVENT -- */
extern fsm_eMenuLeft_s_t fsm_eMenuLeft_s[GAME_STATE_NUM];
extern fsm_eMenuLeft_sPrep_ss_t     fsm_eMenuLeft_sPrep_ss[GAME_SUBSTATE_NUM];
extern fsm_eMenuLeft_sGmpMap_ss_t   fsm_eMenuLeft_sGmpMap_ss[GAME_SUBSTATE_NUM];

void fsm_eMenuLeft_sPrep(   struct Game *sota, i32 c);
void fsm_eMenuLeft_sGmpMap( struct Game *sota, i32 c);

void fsm_eMenuLeft_sPrep_ssMenu(    struct Game *sota);
void fsm_eMenuLeft_sPrep_ssMapCndt( struct Game *sota);

void fsm_eMenuLeft_sGmpMap_ssMenu(struct Game *sota, i32 c);
void fsm_eMenuLeft_sGmpMap_ssStby(struct Game *sota, i32 c);

/* -- FSM: Unit_Danger EVENT -- */
extern fsm_eUnitDng_s_t fsm_eUnitDng_s[GAME_STATE_NUM];
extern fsm_eUnitDng_s_t fsm_eUnitDng_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitDng_ssStby(struct Game *sota, tnecs_entity ent);

/* -- FSM: Gameplay_Return2Standby EVENT -- */
extern fsm_eGmp2Stby_s_t fsm_eGmp2Stby_s[GAME_STATE_NUM];
extern fsm_eGmp2Stby_s_t fsm_eGmp2Stby_ss[GAME_SUBSTATE_NUM];

void fsm_eGmp2Stby_sGmpMap(   struct Game *sota, tnecs_entity ent);
void fsm_eGmp2Stby_ssMapUnitMv(struct Game *sota, tnecs_entity ent);

/* -- FSM: Unit_Select EVENT -- */
extern fsm_eUnitSel_s_t fsm_eUnitSel_s[GAME_STATE_NUM];
extern fsm_eUnitSel_s_t fsm_eUnitSel_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitSel_ssStby(struct Game *sota, tnecs_entity ent);

/* -- FSM: Unit_Deselect EVENT -- */
extern fsm_eUnitDsel_s_t fsm_eUnitDsel_s[GAME_STATE_NUM];
extern fsm_eUnitDsel_s_t fsm_eUnitDsel_ss[GAME_SUBSTATE_NUM];

void fsm_eUnitDsel_ssMenu(      struct Game *sota, tnecs_entity ent);
void fsm_eUnitDsel_ssMapUnitMv( struct Game *sota, tnecs_entity ent);

/* -- FSM: Input_globalRange EVENT -- */
extern fsm_eGlbRng_s_t fsm_eGlbRng_ss[GAME_SUBSTATE_NUM];

void fsm_eGlbRng_ssStby(struct Game *sota);

/* -- FSM: Combat_End EVENT -- */
extern fsm_eCmbtEnd_ss_t fsm_eCmbtEnd_ss[GAME_SUBSTATE_NUM];

void fsm_eCmbtEnd_ssMapNPC(  struct Game *sota);
void fsm_eCmbtEnd_ssMapCndt( struct Game *sota);

#endif /* FSM_H */
