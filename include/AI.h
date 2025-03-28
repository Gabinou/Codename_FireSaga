
#ifndef AI_H
#define AI_H

#include <stdint.h>
#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Unit;
struct Timer;
struct UnitMoveAnimation;

struct cJSON;
typedef struct cJSON cJSON;

/* --- AI DESIGN --- */
// AI has two parts
//  - AI Act
//      - Decides what action to take depending on AI_PRIORITY.
//  - AI Move
//      - Decides when to move depending on AI_MOVE, where depending on AI act.
//      - Needs: costmap, target pos, self pos
//
// Decision-making:
//  - AI Act: Decides WHAT to do
//      - Master priority decides action/move target
//      - Slave  priority decides if not in the way of master action/target
//  - AI Move:
//      - Move to target chosen in AI Act
//      - Move when AI_MOVE lets you
//
// Call order:
//      AI_Decide_Action -> AI_Decide_Move -> AI_Move -> AI_Act

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
    AI_ACTION_START = -1,
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
    /* Runs away when injured. Take healing items from friendlies to heal. */
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
    AI_MOVE_TRIGGER = 3,  /* TODO */

    /* -- AI_MOVE_NEVER -- */
    /* Unit does not move. */
    AI_MOVE_NEVER = 4,

    AI_MOVE_NUM,
};

/* AI COMPONENT */
typedef struct AI {
    struct jsonIO_Header jsonio_header;

    int priority_master;
    int priority_slave;
    int move;
    struct Point target_move;
    int turn_move;
} AI;
extern const struct AI AI_default;

/* --- Decider FSM --- */
typedef void (*AI_Decider)(struct Game *s, tnecs_entity e, struct AI_Action *a);
/* -- Master Deciders -- */
extern const AI_Decider AI_Decider_master[AI_PRIORITY_NUM];

/* -- Slave Deciders -- */
extern const AI_Decider AI_Decider_slave[AI_PRIORITY_NUM];

/* -- Decider Move FSM -- */
// Note: These functions return whether character moves or not
typedef b32  (*AI_Decider_Move)(struct Game *s, tnecs_entity e);
extern const AI_Decider_Move AI_Decider_move[AI_MOVE_NUM];

/* --- Doer FSM --- */
typedef AI_Decider AI_Doer;
extern const AI_Doer AI_Act_action[AI_ACTION_NUM];

void _AI_Doer_Wait(  struct Game *s, tnecs_entity e, struct AI_Action *a);
void _AI_Doer_Attack(struct Game *s, tnecs_entity e, struct AI_Action *a);

void Unit_Move_onMap_Animate(struct Game  *s, tnecs_entity e,
                             struct Timer *t, struct UnitMoveAnimation *a);

/* --- I/O --- */
void AI_readJSON( void *ai, cJSON *jai);
void AI_writeJSON(void *ai, cJSON *jai);

s8 AI_filename(i32 ai_id);
s8 AI_ID_isvalid(i32 ai_id);

/* --- AI_State --- */
void AI_State_Pop( struct AI_State *ais, tnecs_world *world);
void AI_State_Init(struct AI_State *ais, tnecs_world *world, struct Map *map);
void AI_State_Free(struct AI_State *ais);

void AI_State_Turn_Start( struct AI_State *ais);
void AI_State_Turn_Finish(struct AI_State *ais);

/* --- AI Unit control --- */
/* Call order:
*       AI_Decide_Action -> AI_Decide_Move -> AI_Move -> AI_Act
*/

/* -- AI Deciders -- */
tnecs_entity AI_Decide_Next(struct Game *s);

void  AI_Decide_Move(struct Game *s, tnecs_entity e, struct AI_Action *a);
void _AI_Decide_Move(struct Game *s, tnecs_entity e, struct AI_Action *a);

void AI_Decide_Action(struct Game *s, tnecs_entity e, struct AI_Action *a);

/* -- AI Doers -- */
void AI_Move(struct Game *s, tnecs_entity e, struct AI_Action *a);
void AI_Act( struct Game *s, tnecs_entity e, struct AI_Action *a);

#endif /* AI_H */
