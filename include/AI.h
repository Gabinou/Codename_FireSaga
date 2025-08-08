
#ifndef AI_H
#define AI_H
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
** AI: Control any unit not controlled by player
**
*/

#include <stdint.h>
#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Unit;
struct Map;
struct Timer;
struct UnitMoveAnimation;
struct cJSON;
struct Position;

/* --- AI DESIGN --- */
// AI has two parts
//  - AI Act
//      - Decides what to do based on AI_PRIORITY.
//      - Only decides action if it is doable:
//          - AI_ACTION_ATTACK only if can attack
//  - AI Move
//      - Decides:
//          - If to move based on AI_Mo,
//          - where depending on AI_Act.
//      - Needs: costmap, target pos, self pos
//
// Decision-making:
//  - AI Act: Decides WHAT to do
//      - Master priority decides action/move target
//      - Slave  priority decides IF
//        not in the way of master action/target
//  - AI Move: Decides WHERE to go
//      - Move to target chosen in AI Act
//      - Move when AI_MOVE lets you
//
// Call order:
//  - Decide:
//      - AI_Decide_Action -> AI_Decide_Move -> AI_Decide_Equipment
//  - Do:
//      - AI_Move -> AI_Act

enum AI_NEXT_ENTITY {
    AI_NEXT_ENTITY_NULL = -1,
};

enum AI_RATINGS {
    // Ratings for AI decision making in [-128, 128]
    AI_RATING_AGGRESSOR_DEATH_CERTAIN   = 40,
    AI_RATING_AGGRESSOR_DEATH_POSSIBLE  = 20,
    AI_RATING_DEFENDANT_DEATH_POSSIBLE  = 10,
    AI_RATING_DEFENDANT_DEATH_CERTAIN   = 30,
    AI_RATING_AGGRESSOR_MULTIPLIER      =  2,
    AI_RATING_AGGRESSOR_BRAVE           = 10,
    AI_RATING_AGGRESSOR_DOUBLES         = 10,
    AI_RATING_DEFENDANT_BRAVE           = 10,
    AI_RATING_DEFENDANT_DOUBLES         = 10,
    AI_RATING_DEFENDANT_RETALIATES      = 20,

    AI_RATING_STAFF_SILENCE             = 50,
    AI_RATING_STAFF_PUSH                = 20,
    AI_RATING_STAFF_PULL                = 20,
    AI_RATING_STAFF_HEAL                = 10,
};

/* -- AI_ACTIONS -- */
/* What action can the AI take.
** Mostly same as player, so menu_options */
/* AI */
enum AI_ACTIONS {
    AI_ACTION_START = -1,
#define REGISTER_ENUM(x, y) AI_ACTION_##x,
#include "names/menu/options.h"
#undef REGISTER_ENUM
    AI_ACTION_NUM,
};

/* -- AI_PRIORITIES -- */
/* How does the AI decide what action to take,
** where to move */
enum AI_PRIORITIES {
    AI_PRIORITY_START = -1,
    /* -- AI_PRIORITY_KILL -- */
    // Attacks to kill.
    //  - PARAMETER: Target unit
    //
    // TWO BIRDS: Killing includes walls and snags,
    // so they will naturally clear path to units
    AI_PRIORITY_KILL = 0,

    /* -- AI_PRIORITY_PATROL -- */
    //  - Move between two tiles
    //  - PARAMETER: Target tiles
    AI_PRIORITY_PATROL = 1,

    /* -- AI_PRIORITY_LOOT -- */
    /* Goes for chests. */
    //  - PARAMETER: List of chests?
    AI_PRIORITY_LOOT = 2,

    /* -- AI_PRIORITY_STAFF -- */
    /* Use staves. */
    AI_PRIORITY_STAFF = 3,

    /* -- AI_PRIORITY_SURVIVE -- */
    /* Runs away when injured.  */
    /* Use healing items. */
    /* Runs away if none left. */
    AI_PRIORITY_SURVIVE = 4,

    /* -- AI_PRIORITY_FLEE -- */
    /* Runs away from opposing army units. */
    AI_PRIORITY_FLEE = 5,

    /* -- AI_PRIORITY_SKILL -- */
    /* Tries to use active skills. */
    /* Skill to use and target are stored elsewhere. */
    /* Ex: Fortunée's telekinesis, Hamilcar's cleave. */
    AI_PRIORITY_SKILL = 6,

    /* -- AI_PRIORITY_DO_NOTHING -- */
    /* Does not attack. Does not move. */
    AI_PRIORITY_DO_NOTHING = 7,

    /* -- AI_PRIORITY_MOVE_TO -- */
    //  - PARAMETER: Target unit, target tile
    //  - PARAMETER: Action to take
    AI_PRIORITY_MOVE_TO = 8, // Needs AfterMove decider

    AI_PRIORITY_NUM,
};

/* -- AI_MOVE -- */
/* When can the AI start moving */
enum AI_MOVE_ENUM {
    AI_MOVE_START = -1,

    /* -- AI_MOVE_ONTURN -- */
    /* Unit can't start moving until turn.
    **  - Always move:  turn <= 0,
    **  - Never move:   turn >= 255, */
    AI_MOVE_ONTURN  = 0,

    /* -- AI_MOVE_INRANGE -- */
    /* Waits for enemy to go in attackmap range to move.
    ** Range can be move, attack, or any random value */
    AI_MOVE_INRANGE = 1,

    /* -- AI_MOVE_TRIGGER -- */
    /* Waits for something to happen to move.
    ** Extra parameters needed... */
    AI_MOVE_TRIGGER = 2,  /* TODO */

    /* issue: how to make unit AI_MOVE_INRANGE, but start charging after X turns? */
    /* 1- Map changes unit AI. */
    /*      + Clean clear AI move types */
    /*      + Clearer AI json */
    /*      - More map complexity */
    /* 2- All AI_move have a turn value */
    /*      - Muddies the AI types? */
    /*          * rm AI_MOVE_ALWAYS, AI_MOVE_NEVER */
    /*          * use move.turn = -1 for infinite move */

    AI_MOVE_NUM,
};

/* AI COMPONENT */
typedef struct AI_Priority {
    int master;
    int slave;
} AI_Priority;

typedef struct AI_Move {
    int mode;
    Point target;
    int turn;
} AI_Move;

typedef struct Unit_AI {
    struct jsonIO_Header jsonio_header;
    AI_Move     move;
    AI_Priority priority;
} Unit_AI;
extern const struct Unit_AI Unit_AI_default;

void Unit_AI_Init(Unit_AI *uai);
void Unit_AI_Init_tnecs(void *voiduai);

b32  Unit_AI_Move_Never(const Unit_AI *uai);

/* -- AI Equip -- */
void AI_Decide_Equipment_Kill(  struct Unit     *agg,
                                struct Unit_AI  *agg_ai,
                                struct Point    *pos_agg,
                                struct Unit     *dft,
                                struct Point    *pos_dft);

/* --- Decider FSM --- */
typedef void (*AI_Decider)( struct Game *s,
                            tnecs_entity e,
                            struct AI_Action *a);
/* -- Master Deciders -- */
extern const AI_Decider AI_Decider_master[AI_PRIORITY_NUM];

/* -- Slave Deciders -- */
extern const AI_Decider AI_Decider_slave[AI_PRIORITY_NUM];

/* -- Decider Move FSM -- */
// Note: These functions return whether character moves or not
typedef b32  (*AI_Move_Can)(struct Game *s, tnecs_entity e);
extern const AI_Move_Can ai_move_can[AI_MOVE_NUM];
typedef b32  (*AI_Move_Decide)( struct Game *s,
                                tnecs_entity e,
                                AI_Action *a);

/* --- Doer FSM --- */
typedef AI_Decider AI_Doer;
extern const AI_Doer AI_Act_action[AI_ACTION_NUM];
extern const AI_Move_Decide ai_move_decide[AI_ACTION_NUM];

void _AI_Doer_Wait(  struct Game *s,
                     tnecs_entity e,
                     struct AI_Action *a);
void _AI_Doer_Attack(struct Game *s,
                     tnecs_entity e,
                     struct AI_Action *a);

void Unit_Move_onMap_Animate(struct Game  *s,
                             tnecs_entity e,
                             struct Timer *t,
                             struct UnitMoveAnimation *a);

/* --- I/O --- */
void AI_readJSON( void *ai, const cJSON *jai);
void AI_writeJSON(const void *ai, cJSON *jai);

i32 AI_ID_isvalid(i32 ai_id);

/* --- AI_State --- */
void Game_AI_Pop( struct Game_AI *ais);
void Game_AI_Init(struct Game_AI *ais, struct Map *map);
void Game_AI_Free(struct Game_AI *ais);

void Game_AI_Turn_Start( struct Game_AI *ais);
void Game_AI_Turn_Finish(struct Game_AI *ais);

/* --- AI Unit control --- */
/* Call order:
*       AI_Decide_Action -> AI_Decide_Move -> AI_Move -> AI_Act
*/

/* -- AI Deciders -- */
tnecs_entity AI_Decide_Next(struct Game *s);
void  AI_Decide_Equipment(struct Game *s,
                          tnecs_entity e,
                          struct AI_Action *a);

void  AI_Decide_Move(struct Game *s,
                     tnecs_entity e,
                     struct AI_Action *a);
void _AI_Decide_Move(struct Game *s,
                     tnecs_entity e,
                     struct AI_Action *a);

void AI_Decide_Action(struct Game *s,
                      tnecs_entity e,
                      struct AI_Action *a);

void  AI_Decide_Equipment(  struct Game *s,
                            tnecs_entity e,
                            struct AI_Action *a);

void _AI_Decider_Master_Kill(struct Game *sota,
                             tnecs_entity aggressor,
                             struct AI_Action *action);

void _AI_Decider_Master_Staff(struct Game *sota,
                              tnecs_entity npc_ent,
                              struct AI_Action *action);

void _AI_Decider_Master_Nothing(struct Game *sota,
                                tnecs_entity npc_ent,
                                struct AI_Action *action);

void _AI_Decider_Master_Move_To(struct Game *sota,
                                tnecs_entity npc_ent,
                                struct AI_Action *action);

void _AI_Decider_Slave_Kill(struct Game *sota,
                            tnecs_entity npc_ent,
                            struct AI_Action *action);

/* -- AI Move Can -- */

b32 _AI_Move_Can_onTurn(struct Game *sota,
                        tnecs_entity npc_ent);

b32 _AI_Move_Can_inRange(   struct Game *sota,
                            tnecs_entity npc_ent);

b32 _AI_Move_Can_Trigger(   struct Game *sota,
                            tnecs_entity npc_ent);
/* -- AI Doers -- */
void AI_Doer_Move(struct Game *s,
                  tnecs_entity e,
                  struct AI_Action *a);
void AI_Doer_Act(struct Game *s,
                 tnecs_entity e,
                 struct AI_Action *a);

/* -- Game -- */
void Game_AI_Enemy_Turn(struct Game *IES);

#endif /* AI_H */
