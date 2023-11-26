
#ifndef AI_H
#define AI_H

#include <stdint.h>
#include "types.h"
#include "enums.h"
#include "structs.h"
#include "unit.h"
#include "narrative.h"
#include "equations.h"
#include "nmath.h"
#include "cJSON.h"
#include "game/game.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Timer;

typedef tnecs_world  world;
typedef tnecs_entity entity;

/* --- AI DESIGN --- */
/* -- High-level design -- */
//  1. Make a decision pair: Who does What?
//  2. Send a command
//      - Move friendly unit
//      - Heal friendly unit
//      - Attack enemy unit
//      - Open chest
//      - ...
//  3. Wait for execution to animate
//  4. If a friendly can still act, back to 1.
//  5. Finish

/* -- Implementation details -- */
//  - AI Command: two parts
//      - AI Move
//          - Only moves unit.
//          - Needs: costmap, target pos, self pos
//      - AI Act
//          - Decides what action to take.
//              - If ultimate target is not in range, go for attack on the way...
//         - Master and slave priorities
//              - Use master priority by default: AI_PRIORITY_KILL
//              - Condition met to use slave condition
//              - Condition depends on master
//                  - fsm_slave called by fsm_master
//              - Ex: If HP is below 50% use slave priority: AI_PRIORITY_SURVIVE
//          - What are criteria to switch priorities?
//              - low HP    AI_PRIORITY_KILL -> AI_PRIORITY_SURVIVE
//              - no chests AI_PRIORITY_LOOT -> AI_PRIORITY_FLEE
//              - Needs: target pos, self pos AFTER move
//
//  1. Make a decision pair: Who does What?
//     - How does AI decide what to do?
//          1.1 Go through all units with certain priorities, for a priority order
//              - Order is whatever brings GameOver faster
//              - Example: Kill should be high on the list
//              - A bit more predictable
//              - Each map should have a priority order list?
//          1.2 Go through units in random order
//              - Simple
//              - Unpredictable
//          2. Use Unit master/slave AI_Priority to decide -> fsm
//              - action.target_action needs to be decided FIRST.
//                  - Includes checking if in range or not.
//              - action.target_move decided after what has been decided.
//                  - movement is a sub-goal, in service of ultimate goal
//

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
/* What action can the AI take. Mostly same as player, so menu_options */
enum AI_ACTIONS {
    AI_ACTION_START = 0,
#define REGISTER_ENUM(x, y) AI_ACTION_##x,
#include "names/menu/options.h"
#undef REGISTER_ENUM
    AI_ACTION_NUM,
};

/* -- AI_PRIORITIES -- */
/* How does the AI decide what action to take, where to move */
enum AI_PRIORITIES {
    AI_PRIORITY_START = -1,
    /* -- AI_PRIORITY_KILL -- */
    // Attacks to kill.
    //  - PARAMETER: Target unit
    //
    // TWO BIRDS: Killing includes walls and snags,
    // so they will naturally clear path to units
    AI_PRIORITY_KILL = 0,

    /* -- AI_PRIORITY_SEIZE -- */
    /* Runs to objective. May attack on the wayé */
    //  - PARAMETER: Tile to seize
    AI_PRIORITY_SEIZE = 1,

    /* -- AI_PRIORITY_LOOT -- */
    /* Goes for chests. */
    //  - PARAMETER: List of chests?
    AI_PRIORITY_LOOT = 2,

    /* -- AI_PRIORITY_STAFF -- */
    /* Use staves. */
    AI_PRIORITY_STAFF = 3,

    /* -- AI_PRIORITY_SURVIVE -- */
    /* Runs away when injured. Take healing items from friendlies to heal. */
    /* Runs away if none left. */
    AI_PRIORITY_SURVIVE = 4,

    /* -- AI_PRIORITY_FLEE -- */
    /* Runs away from s. */
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
    AI_PRIORITY_MOVE_TO = 8,

    /* -- AI_PRIORITY_PATROL -- */
    //  - Move between two tiles
    //  - PARAMETER: Target unit, target tiles
    AI_PRIORITY_PATROL = 9,

    AI_PRIORITY_NUM,
};

/* -- AI_MOVE -- */
/* When can the AI start moving */
enum AI_MOVE {
    AI_MOVE_START = -1,

    /* -- AI_MOVE_ALWAYS -- */
    /* Unit can always move. */
    AI_MOVE_ALWAYS = 0,

    /* -- AI_MOVE_ONCHAPTER -- */
    /* Unit can't start moving until certain chapter. */
    AI_MOVE_ONCHAPTER = 1,

    /* -- AI_MOVE_INRANGE -- */
    /* Waits for enemy to go in attackmap range to move. */
    /* Range can be move, attack, or any random value */
    /* DESIGN LIMIT: Don't make range SMALLER than attackmap! SUCKS */
    AI_MOVE_INRANGE = 2,

    /* -- AI_MOVE_TRIGGER -- */
    /* Waits for something to happen to move. */
    /* Extra parameters needed... */
    AI_MOVE_TRIGGER = 3,

    /* -- AI_MOVE_NEVER -- */
    /* Unit does not move. */
    AI_MOVE_NEVER = 4,

    AI_MOVE_NUM,
};

/* AI COMPONENT */
typedef struct AI {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    int priority_master;
    int priority_slave;
    int move;
    struct Point target_move;
} AI;
struct AI AI_default;

/* --- Decider FSM --- */
typedef void (*AI_Decider)(struct Game *s, tnecs_entity e, struct AI_Action *a);
extern AI_Decider AI_Decider_master[AI_PRIORITY_NUM];
extern AI_Decider AI_Decider_slave[AI_PRIORITY_NUM];
static void _AI_Decider_Do_Kill(   struct Game *s, tnecs_entity e, struct AI_Action *a);
static void _AI_Decider_Do_Kill(   struct Game *s, tnecs_entity e, struct AI_Action *a);
static void _AI_Decider_Do_Nothing(struct Game *s, tnecs_entity e, struct AI_Action *a);
static void _AI_Decider_Do_Move_To(struct Game *s, tnecs_entity e, struct AI_Action *a);

/* Dedider Utilities after moving */
extern AI_Decider AI_Decider_AfterMove[AI_PRIORITY_NUM];
static void _AI_Decider_Kill_AfterMove( struct Game *s, tnecs_entity e, struct AI_Action *a);
static void _AI_Decider_Staff_AfterMove(struct Game *s, tnecs_entity e, struct AI_Action *a);

/* --- Doer FSM --- */
typedef AI_Decider AI_Doer;
extern AI_Doer AI_Act_action[AI_ACTION_NUM];
static void _AI_Doer_Wait(struct Game *s, tnecs_entity e, struct AI_Action *a);

void Unit_Move_onMap_Animate(struct Game *s, tnecs_entity e,
                             struct Timer *t, struct UnitMoveAnimation *a);

/* --- I/O --- */
void AI_readJSON( void *ai, cJSON *jai);
void AI_writeJSON(void *ai, cJSON *jai);

/* --- AI_State --- */
void AI_State_Pop( struct AI_State *ais, tnecs_world *world);
void AI_State_Init(struct AI_State *ais, tnecs_world *world, struct Map *map);

void AI_State_Turn_Start( struct AI_State *ais);
void AI_State_Turn_Finish(struct AI_State *ais);

/* --- AI Unit control --- */
/* Call order: AI_Decide_Action -> AI_Decide_Move -> AI_Move -> AI_Act */

/* -- AI Deciders -- */
entity AI_Decide_Next(struct Game *s);
void AI_Decide_Action(struct Game *s, tnecs_entity e, struct AI_Action *a);
void AI_Decide_Move(  struct Game *s, tnecs_entity e, struct AI_Action *a);

/* -- AI Doers -- */
void AI_Move(struct Game *s, tnecs_entity e, struct AI_Action *a);
void AI_Act( struct Game *s, tnecs_entity e, struct AI_Action *a);

#endif /* AI_H */
