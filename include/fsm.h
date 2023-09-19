#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include "globals.h"
#include "enums.h"
#include "game/game.h"
#include "game/popup.h"
#include "game/cursor.h"
#include "game/menu.h"
#include "game/unit.h"
#include "menu/fsm.h"
#include "tnecs.h"
#include "unit.h"
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
struct PlayerSelectMenu;
struct LoadoutSelectMenu;
struct PopUp_Loadout_Stats;

/* --- FINITE-STATE MACHINE FOR EVENTS --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<type>_<event>
*   Type:       fsm_<type>_<event>_t
*   Function:   fsm_<type>_<event>_<INDEX>
*   <fsm_array>[<index>] -> returns <fsm_function>
*   Ex: fsm_substate_IC_MAP_UNIT_MOVES
*/
// Two types of substate fsm:
// Called by event -> fsm_sub_event
// Called by state -> fsm_substate
//    If need one fsm_substate per state -> fsm_sub_<state_name>

/* --- TYPEDEFS --- */
typedef void (*fsm_state_IGR_t)( struct Game *);
typedef void (*fsm_state_IC_t)(  struct Game *, tnecs_entity_t);
typedef void (*fsm_state_CMd_t)( struct Game *, tnecs_entity_t, struct Point *);
typedef void (*fsm_state_GR2S_t)(struct Game *, tnecs_entity_t);
typedef void (*fsm_state_ImR_t)( struct Game *, i32);
typedef fsm_state_IC_t   fsm_state_IA_t;
typedef fsm_state_IC_t   fsm_state_IS_t;
typedef fsm_state_IC_t   fsm_substate_IC_t;
typedef fsm_state_ImR_t  fsm_state_ImL_t;
typedef fsm_state_ImR_t  fsm_sub_event_ImL_t;
typedef fsm_state_ImR_t  fsm_sub_event_ImR_t;
typedef fsm_state_CMd_t  fsm_state_CMs_t;
typedef fsm_state_GR2S_t fsm_state_UD_t;
typedef fsm_state_GR2S_t fsm_state_US_t ;
typedef fsm_state_GR2S_t fsm_state_UDgr_t;
typedef fsm_state_GR2S_t fsm_state_CDU_t;
typedef fsm_state_GR2S_t fsm_state_CHU_t;
typedef fsm_state_GR2S_t fsm_Unit_Wait_state_t;
typedef fsm_state_GR2S_t fsm_state_UD_t;

/* -- FSM: Cursor_Moves EVENT --  */
extern fsm_state_CMs_t fsm_state_CMs[GAME_STATE_NUM];
extern fsm_state_CMs_t fsm_sub_event_CMs[GAME_SUBSTATE_NUM];

void fsm_state_CMs_GAMEPLAY_MAP(struct Game *s, tnecs_entity_t t, struct Point *);

void fsm_sub_event_CMs_MENU(          struct Game *sota, tnecs_entity_t t, struct Point *);
void fsm_sub_event_CMs_STANDBY(       struct Game *sota, tnecs_entity_t t, struct Point *);
void fsm_sub_event_CMs_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t t, struct Point *);
void fsm_sub_event_CMs_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t t, struct Point *);

/* -- FSM: Cursor_Moved EVENT -- */
extern fsm_state_CMd_t fsm_state_CMd[GAME_STATE_NUM];
extern fsm_state_CMd_t fsm_sub_event_CMd[GAME_SUBSTATE_NUM];

void fsm_state_CMd_GAMEPLAY_MAP(struct Game *, tnecs_entity_t t, struct Point *);

void fsm_sub_event_CMd_STANDBY(       struct Game *sota, tnecs_entity_t t, struct Point *);
void fsm_sub_event_CMd_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t t, struct Point *);
void fsm_sub_event_CMd_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t t, struct Point *);

/* -- FSM: Cursor_Hovers_Unit EVENT -- */
extern fsm_state_CHU_t fsm_state_CHU[GAME_STATE_NUM];
extern fsm_state_CHU_t fsm_substate_CHU[GAME_SUBSTATE_NUM];

void fsm_state_CHU_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t ent);

void fsm_substate_CHU_STANDBY(       struct Game *sota, tnecs_entity_t ent);
void fsm_substate_CHU_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Cursor_Dehovers_Unit EVENT -- */
extern fsm_state_CDU_t fsm_state_CDU[GAME_STATE_NUM];
extern fsm_state_CDU_t fsm_substate_CDU[GAME_SUBSTATE_NUM];

void fsm_state_CDU_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t ent);

void fsm_substate_CDU_STANDBY(       struct Game *sota, tnecs_entity_t ent);
void fsm_substate_CDU_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t ent);
void fsm_substate_CDU_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Input_Accept EVENT -- */
extern fsm_state_IA_t fsm_state_IA[GAME_STATE_NUM];
extern fsm_state_IA_t fsm_substate_IA[GAME_SUBSTATE_NUM];

void fsm_state_IA_TITLE_SCREEN(struct Game *sota, tnecs_entity_t ent);
void fsm_state_IA_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t ent);

void fsm_substate_IA_MENU(            struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_PAUSED(          struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_STANDBY(         struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_ANIMATION(       struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_COMBAT(      struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_NPCTURN(     struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_MINIMAP(     struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_UNIT_MOVES(  struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_CANDIDATES(  struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IA_MAP_GLOBAL_RANGE(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Input_Cancel EVENT -- */
extern fsm_state_IC_t fsm_state_IC[GAME_STATE_NUM];
extern fsm_substate_IC_t fsm_substate_IC[GAME_SUBSTATE_NUM];

void fsm_state_IC_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t ent);

void fsm_substate_IC_MENU(            struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_STANDBY(         struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_ANIMATION(       struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_MAP_COMBAT(      struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_STATS_MENU(      struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_MAP_NPCTURN(     struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_MAP_UNIT_MOVES(  struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_MAP_CANDIDATES(  struct Game *sota, tnecs_entity_t ent);
void fsm_substate_IC_MAP_GLOBAL_RANGE(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Input_Stats EVENT -- */
extern fsm_state_IS_t fsm_state_IS[GAME_STATE_NUM];
extern fsm_state_IS_t fsm_sub_event_IS[GAME_SUBSTATE_NUM];

void fsm_sub_event_IS_STANDBY(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Input_menuRight EVENT -- */
extern fsm_state_ImR_t fsm_state_ImR[GAME_STATE_NUM];
extern fsm_sub_event_ImR_t fsm_sub_event_ImR[GAME_SUBSTATE_NUM];

void fsm_state_ImR_GAMEPLAY_MAP(struct Game *sota, int32_t c);

void fsm_sub_event_ImR_MENU(   struct Game *sota, int32_t c);
void fsm_sub_event_ImR_STANDBY(struct Game *sota, int32_t c);

/* -- FSM: Input_menuLeft EVENT -- */
extern fsm_state_ImL_t fsm_state_ImL[GAME_STATE_NUM];
extern fsm_sub_event_ImL_t fsm_sub_event_ImL[GAME_SUBSTATE_NUM];

void fsm_state_ImL_GAMEPLAY_MAP(struct Game *sota, int32_t c);

void fsm_sub_event_ImL_MENU(   struct Game *sota, int32_t c);
void fsm_sub_event_ImL_STANDBY(struct Game *sota, int32_t c);

/* -- FSM: Unit_Danger EVENT -- */
extern fsm_state_UDgr_t fsm_state_UDgr[GAME_STATE_NUM];
extern fsm_state_UDgr_t fsm_substate_UDgr[GAME_SUBSTATE_NUM];

void fsm_substate_UDgr_STANDBY(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Gameplay_Return2Standby EVENT -- */
extern fsm_state_GR2S_t fsm_state_GR2S[GAME_STATE_NUM];
extern fsm_state_GR2S_t fsm_substate_GR2S[GAME_SUBSTATE_NUM];

void fsm_state_GR2S_GAMEPLAY_MAP(     struct Game *sota, tnecs_entity_t ent);
void fsm_substate_GR2S_GAMEPLAY_CAMP( struct Game *sota, tnecs_entity_t ent);
void fsm_substate_GR2S_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Unit_Select EVENT -- */
extern fsm_state_US_t fsm_state_US[GAME_STATE_NUM];
extern fsm_state_US_t fsm_substate_US[GAME_SUBSTATE_NUM];

void fsm_substate_US_STANDBY(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Unit_Deselect EVENT -- */
extern fsm_state_UD_t fsm_state_UD[GAME_STATE_NUM];
extern fsm_state_UD_t fsm_substate_UD[GAME_SUBSTATE_NUM];

void fsm_substate_UD_MENU(          struct Game *sota, tnecs_entity_t ent);
void fsm_substate_UD_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t ent);

/* -- FSM: Input_globalRange EVENT -- */
extern fsm_state_IGR_t fsm_sub_event_IGR[GAME_SUBSTATE_NUM];

void fsm_sub_event_IGR_STANDBY(struct Game *sota);

#endif /* FSM_H */
