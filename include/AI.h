
#ifndef AI_H
#define AI_H

#include <stdint.h>
#include "SDL.h"
#include "types.h"
#include "enums.h"
#include "unit.h"
#include "narrative.h"
#include "equations.h"
#include "nmath.h"
#include "cJSON.h"

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
//  - AI Component
//      - Master and slave priorities
//          - Use master priority by default: AI_PRIORITY_KILL
//          - Condition met to use slave condition
//          - Condition depends on master
//              - fsm_slave called by fsm_master
//          - If HP is below 50% use slave priority: AI_PRIORITY_SURVIVE
//      - What are criteria to switch priorities?
//          - low HP    AI_PRIORITY_KILL -> AI_PRIORITY_SURVIVE
//          - no chests AI_PRIORITY_LOOT -> AI_PRIORITY_FLEE
//  - AI Command: two parts
//      - AI Move
//          - Needs: costmap, target pos, self pos
//      - AI Act
//          - Needs: target pos, self pos AFTER move
//
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
//              - Outputs tile to move and action to take
//


/* --- FORWARD DECLARATIONS --- */
struct Game;

struct AI_PushPull_Out {
    struct Point pos;
    i8 rating;
};

struct AI_Action {
    struct Point move;          /* {-1, -1} if none */
    struct Point target_move;   /* {-1, -1} if none */
    struct Point target_action; /* {-1, -1} if none */
    int action;
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
    // staffWielders should prioritize using staffs
    // -> UNLESS there is a killable target close by or something
    // staffWielders that have AI_PRIORITY_KILL should act first
    // staffWielders that have AI_PRIORITY_PROTECT should act last
    // -> pullers should act before healers

    AI_PRIORITY_START = 0,
    /* -- AI_PRIORITY_KILL -- */
    // Always attacks, ignores other actions
    // Goes aggressively towards enemy units
    //  - Can specify target unit, or not
    //
    // TWO BIRDS: Killing includes walls and snags,
    // so they will naturally clear path to units
    AI_PRIORITY_KILL,

    /* -- AI_PRIORITY_PROTECT -- */
    /* Rescues friendlies, heal friendlies. Otherwise attacks. */
    AI_PRIORITY_PROTECT,

    /* -- AI_PRIORITY_SEIZE -- */
    /* Runs to objective. Attacks only if it doesn't slow down or kills you. */
    AI_PRIORITY_SEIZE,

    /* -- AI_PRIORITY_LOOT -- */
    /* Goes for chests. Does not attack. */
    AI_PRIORITY_LOOT,

    /* -- AI_PRIORITY_STAFF -- */
    /* Tries to use staves. */
    AI_PRIORITY_STAFF,

    /* -- AI_PRIORITY_SURVIVE -- */
    /* AAI switches  */
    /* Runs away when injured. Take healing items from friendlies to heal. */
    AI_PRIORITY_SURVIVE,

    /* -- AI_PRIORITY_FLEE -- */
    /* Runs away from PCs. Does not attack.*/
    AI_PRIORITY_FLEE,

    /* -- AI_PRIORITY_TRIGGER -- */
    /* Waits for condition to be met to do something. */
    /* -> I don't like it that much, but it might be useful */
    AI_PRIORITY_TRIGGER,

    /* -- AI_PRIORITY_SKILL -- */
    /* Unit tries to use active skill. */
    /* Skill to use and target are stored elsewhere. */
    /* Ex: Fortunée's telekinesis, Hamilcar's cleave. */
    AI_PRIORITY_SKILL,

    /* -- AI_PRIORITY_DO_NOTHING -- */
    /* Does not attack */
    AI_PRIORITY_DO_NOTHING,

    /* -- AI_PRIORITY_MOVE_TO -- */
    /* - Can specify target unit, tile */
    AI_PRIORITY_MOVE_TO,

    /* -- AI_PRIORITY_PATROL -- */
    /* - Move between two tiles */
    AI_PRIORITY_PATROL,

    AI_PRIORITY_END,
};

/* -- AI_MOVE -- */
/* When can the AI start moving */
enum AI_MOVE {
    AI_MOVE_START = 0,

    /* -- AI_MOVE_ALWAYS -- */
    /* Unit can always move. */
    AI_MOVE_ALWAYS,

    /* -- AI_MOVE_ONCHAPTER -- */
    /* Unit can't start moving until certain chapter. */
    AI_MOVE_ONCHAPTER,

    /* -- AI_MOVE_STANDBY -- */
    /* Waits for enemy to go in attackmap range to move. */
    /* Range can be move, attack, or any random value */
    /* DESIGN LIMIT: Don't make range SMALLER than attackmap! SUCKS */
    AI_MOVE_STANDBY,

    /* -- AI_MOVE_NEVER -- */
    /* Unit does not move. */
    AI_MOVE_NEVER,

    AI_MOVE_NUM,
};

#define AI_PULL_FRIENDLY_MISSINGHP 0.25f
#define AI_PUSH_FRIENDLY_MISSINGHP 0.75f

typedef struct AI {
    entity target_protect;
    entity target_kill;
    struct Point target_seize;
    struct Point target_open;
    struct Point target_defend;
    struct Point target_interim;
    u8 priority;
    u8 move_chapter;
    u8 move_type;
} AI;
struct AI AI_default;

struct Point *Target_Assailable_Positions(entity att, entity dfd, i32 *movemap);

u8 AI_menuOption_Choose(world *world, entity att);

entity AI_Target_Attack(world *world, entity att, entity *possibleDefenders,
                        struct Combat_Forecast *forecasts, u8 num_defender);
entity AI_Target_Heal(world *world, entity staffWielder,
                      entity *possiblePatients, u8 num_patients);
entity AI_Target_magicShield(world *world, entity staffWielder,
                             entity *possiblePatients, u8 num_patients);
entity AI_Target_Silence(world *world, entity staffWielder, u8 *hit_rates,
                         entity *possibleDefenders, u8 num_defender);
entity AI_Target_Pull(world *world, entity staffWielder, entity *friendlies,
                      u8 num_friendly, entity *enemies, u8 num_enemy);
entity AI_Target_Push(world *world, entity staffWielder, entity *friendlies,
                      u8 num_friendly, entity *enemies, u8 num_enemy);

entity AI_interimTarget_Compute(struct Map *map, entity actor);
entity AI_MovetoTarget(struct Map *map, entity actor);

i8 AI_Forecast_Rating(struct Combat_Forecast forecast);
i8 AI_Silence_Rating(world *world, u8 hit_rate, entity enemy_ent);

struct AI_PushPull_Out AI_PushPull_bestPosition(i32 *gradientmap,
                                                size_t row_len, size_t col_len,
                                                i32 pushpull_distance,
                                                struct Point victim_pos, i8 sign);

struct AI_PushPull_Out AI_PushPull_Friendly_Offensively_Rating(
        world *world, struct Map *map, entity friendly_ent);
struct AI_PushPull_Out AI_PushPull_Friendly_Defensively_Rating(
        world *world, struct Map *map, entity friendly_ent);

struct AI_PushPull_Out AI_PushPull_Enemy_Offensively_Rating(
        world *world, struct Map *map, entity enemy_ent);
struct AI_PushPull_Out AI_PushPull_Enemy_Defensively_Rating(
        world *world, struct Map *map, entity enemy_ent);

struct Point AI_Boss_Arc3_Target_Leg_Right(entity *unitmap);
struct Point AI_Boss_Arc3_Target_Leg_Left(entity *unitmap);
struct Point AI_Boss_Arc3_Target_Arm_Right(entity *unitmap);
struct Point AI_Boss_Arc3_Target_Arm_Left(entity *unitmap);


#endif /* AI_H */