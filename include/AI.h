
#ifndef AI_H
#define AI_H

#include <stdint.h>
#include "SDL2/SDL.h"
#include "types.h"
#include "enums.h"
#include "unit.h"
#include "narrative.h"
#include "equations.h"
#include "nmath.h"
#include "cJSON.h"

// --- FORWARD DECLARATIONS ---
struct Game;

struct AI_PushPull_Out {
    struct nmath_point_int32_t pos;
    int_fast8_t rating;
};

enum AI_RATINGS {
    // Ratings for AI decision making in [-128, 128]
    AI_RATING_AGGRESSOR_DEATH_CERTAIN = 40,
    AI_RATING_AGGRESSOR_DEATH_POSSIBLE = 20,
    AI_RATING_DEFENDANT_DEATH_POSSIBLE = 10,
    AI_RATING_DEFENDANT_DEATH_CERTAIN = 30,
    AI_RATING_AGGRESSOR_MULTIPLIER = 2,
    AI_RATING_aggressor_brave = 10,
    AI_RATING_aggressor_doubles = 10,
    AI_RATING_defendant_brave = 10,
    AI_RATING_defendant_doubles = 10,
    AI_RATING_defendant_retaliates = 20,

    AI_RATING_STAFF_SILENCE = 50,
    AI_RATING_STAFF_PUSH = 20,
    AI_RATING_STAFF_PULL = 20,
    AI_RATING_STAFF_HEAL = 10,
};

enum AI_PRIORITIES {
    // staffWielders should prioritize using staffs
    // -> UNLESS there is a killable target close by or something
    // staffWielders that have AI_PRIORITY_KILL should act first
    // staffWielders that have AI_PRIORITY_PROTECT should act last
    // -> pullers should act before healers
    AI_PRIORITY_START = 0,
    AI_PRIORITY_KILL = 1,       // Always attacks, ignores other actions.
    AI_PRIORITY_PROTECT = 2,    // Rescues friendlies, heal friendlies. Otherwise attacks.
    AI_PRIORITY_SEIZE = 3,      // Runs to objective. Attacks only if it doesn't slow down or kills you.
    AI_PRIORITY_LOOT = 4,       // Goes for chests. Does not attack.
    AI_PRIORITY_SURVIVE = 5,    // Attacks. Runs away when injured. Take healing items from friendlies to heal.
    AI_PRIORITY_FLEE = 6,       // Does not attack.
    AI_PRIORITY_END = 7,
};

enum AI_MOVEMENT_TYPE {
    AI_MOVE_START = 0,
    AI_MOVE_ONCHAPTER = 1ULL << 0,
    AI_MOVE_ENEMYINRANGE = 1ULL << 1,
    AI_MOVE_NEVER = 1ULL << 2,
    AI_MOVE_OPTIONS = 3,
};

#define AI_PULL_FRIENDLY_MISSINGHP 0.25f
#define AI_PUSH_FRIENDLY_MISSINGHP 0.75f

typedef struct AI {
    tnecs_entity_t target_protect;
    tnecs_entity_t target_kill;
    struct nmath_point_int32_t target_seize;
    struct nmath_point_int32_t target_open;
    struct nmath_point_int32_t target_defend;
    struct nmath_point_int32_t target_interim;
    uf8 priority;
    uf8 move_chapter;
    uf8 move_type;
} AI;
extern struct AI AI_default;

extern struct nmath_point_int32_t *Target_Assailable_Positions(
        tnecs_entity_t in_attacker,
        tnecs_entity_t in_defender, i32 *in_movemap);

extern uf8 AI_menuOption_Choose(tnecs_world_t *in_world,
                                tnecs_entity_t in_attacker);

extern tnecs_entity_t AI_Target_Attack(tnecs_world_t *in_world,
                                       tnecs_entity_t in_attacker,
                                       tnecs_entity_t *in_possibleDefenders, struct Combat_Forecast *in_forecasts,
                                       uf8 num_defender);
extern tnecs_entity_t AI_Target_Heal(tnecs_world_t *in_world,
                                     tnecs_entity_t in_staffWielder,
                                     tnecs_entity_t *in_possiblePatients, uf8 num_patients);
extern tnecs_entity_t AI_Target_magicShield(tnecs_world_t *in_world,
                                            tnecs_entity_t in_staffWielder, tnecs_entity_t *in_possiblePatients,
                                            uf8 num_patients);
extern tnecs_entity_t AI_Target_Silence(tnecs_world_t *in_world,
                                        tnecs_entity_t in_staffWielder,
                                        uf8 *in_hit_rates, tnecs_entity_t *in_possibleDefenders,
                                        uf8 num_defender);
extern tnecs_entity_t AI_Target_Pull(tnecs_world_t *in_world,
                                     tnecs_entity_t in_staffWielder,
                                     tnecs_entity_t *in_friendlies, uf8 num_friendly, tnecs_entity_t *in_enemies,
                                     uf8 num_enemy);
extern tnecs_entity_t AI_Target_Push(tnecs_world_t *in_world,
                                     tnecs_entity_t in_staffWielder,
                                     tnecs_entity_t *in_friendlies, uf8 num_friendly, tnecs_entity_t *in_enemies,
                                     uf8 num_enemy);

extern tnecs_entity_t AI_interimTarget_Compute(struct Map *in_map,
                                               tnecs_entity_t in_actor);
extern tnecs_entity_t AI_MovetoTarget(struct Map *in_map, tnecs_entity_t in_actor);

extern int_fast8_t AI_Forecast_Rating(struct Combat_Forecast in_forecast);
extern int_fast8_t AI_Silence_Rating(tnecs_world_t *in_world, uf8 in_hit_rate,
                                     tnecs_entity_t in_enemy_ent);

extern struct AI_PushPull_Out AI_PushPull_bestPosition(i32 *in_gradientmap,
                                                       size_t row_len,
                                                       size_t col_len, i32 pushpull_distance, struct nmath_point_int32_t victim_pos,
                                                       int_fast8_t sign);

extern struct AI_PushPull_Out AI_PushPull_Friendly_Offensively_Rating(
        tnecs_world_t *in_world,
        struct Map *in_map, tnecs_entity_t in_friendly_ent);
extern struct AI_PushPull_Out AI_PushPull_Friendly_Defensively_Rating(
        tnecs_world_t *in_world,
        struct Map *in_map, tnecs_entity_t in_friendly_ent);

extern struct AI_PushPull_Out AI_PushPull_Enemy_Offensively_Rating(
        tnecs_world_t *in_world,
        struct Map *in_map, tnecs_entity_t in_enemy_ent);
extern struct AI_PushPull_Out AI_PushPull_Enemy_Defensively_Rating(
        tnecs_world_t *in_world,
        struct Map *in_map, tnecs_entity_t in_enemy_ent);

extern struct nmath_point_int32_t AI_Boss_Arc3_Target_Leg_Right(tnecs_entity_t *unitmap);
extern struct nmath_point_int32_t AI_Boss_Arc3_Target_Leg_Left(tnecs_entity_t *unitmap);
extern struct nmath_point_int32_t AI_Boss_Arc3_Target_Arm_Right(tnecs_entity_t *unitmap);
extern struct nmath_point_int32_t AI_Boss_Arc3_Target_Arm_Left(tnecs_entity_t *unitmap);


#endif /* AI_H */