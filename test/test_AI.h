#include "AI.h"
#include "pathfinding.h"
#include "linalg.h"
#include "enums.h"
#include "flecs.h"
#include "unit.h"
#include "position.h"

enum ENUM_TEST_AI {
    TEST_AI_ROW_LEN = 21,
    TEST_AI_COL_LEN = 25,
};

void test_AI() {
    SDL_Log("test_AI");
    uint32_t move;
    struct Point start = {10, 6};
    struct Point end = {15, 1};
    struct HexPoint hexstart = {10, -4, 6};
    int_tile_t range[2];
    int_tile_t * position;

    move = 5;
    range[0] = 1;
    range[1] = 2;

    struct AI temp_AI1 = AI_default;
    struct AI temp_AI2 = AI_default;
    struct AI temp_AI3 = AI_default;

    // ecs_world_t * world_test_ai = ecs_init();
    // ECS_COMPONENT(world_test_ai, Position);
    // ECS_COMPONENT(world_test_ai, Unit);
    // ecs_entity_t friendly_ent_1 = ecs_new(world_test_ai, 0);
    // ecs_entity_t friendly_ent_2 = ecs_new(world_test_ai, 0);
    // ecs_entity_t friendly_ent_3 = ecs_new(world_test_ai, 0);
    // ecs_entity_t enemy_ent_1 = ecs_new(world_test_ai, 0);
    // ecs_entity_t enemy_ent_2 = ecs_new(world_test_ai, 0);
    // ecs_entity_t enemy_ent_3 = ecs_new(world_test_ai, 0);

    // ecs_add(world_test_ai, friendly_ent_1, Position);
    // ecs_add(world_test_ai, friendly_ent_2, Position);
    // ecs_add(world_test_ai, friendly_ent_3, Position);
    // ecs_add(world_test_ai, friendly_ent_1, Unit);
    // ecs_add(world_test_ai, friendly_ent_2, Unit);
    // ecs_add(world_test_ai, friendly_ent_3, Unit);

    // ecs_add(world_test_ai, enemy_ent_1, Unit);
    // ecs_add(world_test_ai, enemy_ent_2, Unit);
    // ecs_add(world_test_ai, enemy_ent_3, Unit);
    // ecs_add(world_test_ai, enemy_ent_1, Position);
    // ecs_add(world_test_ai, enemy_ent_2, Position);
    // ecs_add(world_test_ai, enemy_ent_3, Position);

    // int_tile_t temp_costmapp[TEST_AI_ROW_LEN * TEST_AI_COL_LEN] = {
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    // };




}
