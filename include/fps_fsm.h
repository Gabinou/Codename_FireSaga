#ifndef RENDER_FSM_H
#define RENDER_FSM_H

#include "enums.h"
#include "game/game.h"
#include "game/cursor.h"
#include "globals.h"
#include "n9patch.h"

typedef void (*fsm_main_t)(struct Game *);

extern fsm_main_t fsm_control_state[GAME_STATE_NUM];
extern fsm_main_t fsm_control_substate[GAME_SUBSTATE_NUM];
extern fsm_main_t fsm_render_state[GAME_STATE_NUM];
extern fsm_main_t fsm_render_substate[GAME_SUBSTATE_NUM];

void fsm_control_state_Combat(       struct Game *sota);
void fsm_control_state_Scene_FMV(    struct Game *sota);
void fsm_control_state_Scene_Talk(   struct Game *sota);
void fsm_control_state_Preparation(  struct Game *sota);
void fsm_control_state_Title_Screen( struct Game *sota);
void fsm_control_state_Gameplay_Map( struct Game *sota);
void fsm_control_state_Gameplay_Camp(struct Game *sota);

void fsm_control_substate_Menu(          struct Game *sota);
void fsm_control_substate_Saving(        struct Game *sota);
void fsm_control_substate_Paused(        struct Game *sota);
void fsm_control_substate_Standby(       struct Game *sota);
void fsm_control_substate_Animation(     struct Game *sota);
void fsm_control_substate_Map_Combat(    struct Game *sota);
void fsm_control_substate_Map_Minimap(   struct Game *sota);
void fsm_control_substate_Map_NPCTurn(   struct Game *sota);
void fsm_control_substate_Map_Animation( struct Game *sota);
void fsm_control_substate_Map_Candidates(struct Game *sota);
void fsm_control_substate_Map_Unit_Moves(struct Game *sota);

void fsm_render_state_Combat(       struct Game *sota);
void fsm_render_state_Scene_FMV(    struct Game *sota);
void fsm_render_state_Scene_Talk(   struct Game *sota);
void fsm_render_state_Preparation(  struct Game *sota);
void fsm_render_state_Title_Screen( struct Game *sota);
void fsm_render_state_Gameplay_Map( struct Game *sota);
void fsm_render_state_Gameplay_Camp(struct Game *sota);

void fsm_render_substate_Menu(          struct Game *sota);
void fsm_render_substate_Paused(        struct Game *sota);
void fsm_render_substate_Saving(        struct Game *sota);
void fsm_render_substate_Standby(       struct Game *sota);
void fsm_render_substate_Animation(     struct Game *sota);
void fsm_render_substate_Map_Combat(    struct Game *sota);
void fsm_render_substate_Map_Minimap(   struct Game *sota);
void fsm_render_substate_Map_NPCTurn(   struct Game *sota);
void fsm_render_substate_Map_Animation( struct Game *sota);
void fsm_render_substate_Map_Unit_Moves(struct Game *sota);
void fsm_render_substate_Map_Defendants(struct Game *sota);

#endif /* RENDER_FSM_H */
