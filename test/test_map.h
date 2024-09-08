#include "nourstest.h"
#include "platform.h"
#include "pathfinding.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_map_simple(void) {
    i32 x = 6, y = 6;
    size_t row_len = TEST_ROW_LEN, col_len = TEST_COL_LEN;
    struct Range range = {2, 4};
    i32 *list           = DARR_INIT(list, i32, 16);
    i32 *test_matrix    = calloc(row_len * col_len, sizeof(*test_matrix));

    list        = Taxicab_Circle_List(list, test_matrix, 1, x,  y, row_len, col_len, &range);
    memset(test_matrix, 0, row_len * col_len * sizeof(*test_matrix));
    Taxicab_Circle(test_matrix, 1, x, y, row_len, col_len, &range);

    i32 matrix[TEST_COL_LEN * TEST_ROW_LEN] = {
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 01, 01, 01, 00, 00, 00, 01, 01,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00
    };

    for (i32 i = 0; i < (DARR_NUM(list) / 2); i++) {
        nourstest_true(matrix[list[TWO_D * i] * col_len + list[TWO_D * i + 1]] == 1);
    }
    for (i32 i = 0; i < col_len * row_len; i++) {
        nourstest_true(matrix[i] == test_matrix[i]);
    }
    DARR_FREE(list);
    SDL_free(test_matrix);
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

#define TEST_ROW_LEN 11
#define TEST_COL_LEN 10

void test_map_perimeter(void) {
    i32 dangermap[TEST_COL_LEN * TEST_ROW_LEN] = {
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 01, 01, 01, 00, 00, 00, 01, 01,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00,
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00
    };

    struct Padding *perimeter_edges;
    perimeter_edges = Map_PerimeterM(dangermap, TEST_ROW_LEN, TEST_COL_LEN);

    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].bottom == 0);

    /* Leftward point */
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].right  == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].top    == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].left   == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].bottom == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].top    == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].left   == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].bottom == 0);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].right  == 0);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].top    == 1);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].left   == 1);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].bottom == 0);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].right  == 0);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].top    == 1);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].left   == 1);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].bottom == 0);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].right  == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].top    == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].left   == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].bottom == 0);

    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].top    == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].left   == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].bottom == 0);

    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].right  == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].top    == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].left   == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].bottom == 0);

    if (perimeter_edges != NULL) {
        free(perimeter_edges);
        perimeter_edges = NULL;
    }
}

void test_map_usable(void) {
    /* Does the loadout make sense for unit/class/selection on MAP. */

    /* Tnecs init */
    tnecs_world *world = tnecs_world_genesis();
    TNECS_REGISTER_COMPONENT(world, Unit);
    TNECS_REGISTER_COMPONENT(world, Position);
    tnecs_entity Silou  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    tnecs_entity Erwin  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    tnecs_entity Enemy1 = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    tnecs_entity Enemy2 = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    tnecs_entity Enemy3 = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);

    Unit *silou     = TNECS_GET_COMPONENT(world, Silou,     Unit);
    Unit *erwin     = TNECS_GET_COMPONENT(world, Erwin,     Unit);
    Unit *enemy1    = TNECS_GET_COMPONENT(world, Enemy1,    Unit);
    Unit *enemy2    = TNECS_GET_COMPONENT(world, Enemy2,    Unit);
    Unit *enemy3    = TNECS_GET_COMPONENT(world, Enemy3,    Unit);

    Position *silou_pos     = TNECS_GET_COMPONENT(world, Silou,     Position);
    Position *erwin_pos     = TNECS_GET_COMPONENT(world, Erwin,     Position);
    Position *enemy1_pos    = TNECS_GET_COMPONENT(world, Enemy1,    Position);
    Position *enemy2_pos    = TNECS_GET_COMPONENT(world, Enemy2,    Position);
    Position *enemy3_pos    = TNECS_GET_COMPONENT(world, Enemy3,    Position);

    /* Units init */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(silou, weapons_dtab);

    /* --- Testing 1 range only --- */
    silou->equippable = ITEM_TYPE_SWORD;
    silou->_equipped[UNIT_HAND_LEFT]    =  0;
    silou->_equipped[UNIT_HAND_RIGHT]   = -1;
    silou->_equipment[0].id             = ITEM_ID_FLEURET;
    silou->_equipment[1].id             = ITEM_ID_IRON_AXE;
    silou->_equipment[2].id             = ITEM_ID_IRON_LANCE;
    silou->_equipment[3].id             = ITEM_ID_SHORT_BOW;
    silou->current_stats.move           = 1;
    silou_pos->tilemap_pos.x    = 0;
    silou_pos->tilemap_pos.y    = 0;
    erwin_pos->tilemap_pos.x    = 0;
    erwin_pos->tilemap_pos.y    = 1;
    enemy1_pos->tilemap_pos.x   = 1;
    enemy1_pos->tilemap_pos.y   = 1;
    enemy2_pos->tilemap_pos.x   = 2;
    enemy2_pos->tilemap_pos.y   = 1;
    enemy3_pos->tilemap_pos.x   = 3;
    enemy3_pos->tilemap_pos.y   = 1;

    /* Map init */
    Map *map = Map_Init(map, TEST_COL_LEN, TEST_ROW_LEN);
    map->row_len = TEST_ROW_LEN;
    map->col_len = TEST_COL_LEN;
    map->world = world;
    Map_dArrays_Init(map);

    /* -- Testing 1 range, 0 move -- */
    _Map_Unit_Put(map, silou_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Silou);
    _Map_Unit_Put(map, erwin_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Erwin);
    _Map_Unit_Put(map, enemy1_pos->tilemap_pos.x, enemy1_pos->tilemap_pos.y, Enemy1);
    _Map_Unit_Put(map, enemy2_pos->tilemap_pos.x, enemy2_pos->tilemap_pos.y, Enemy2);
    _Map_Unit_Put(map, enemy3_pos->tilemap_pos.x, enemy3_pos->tilemap_pos.y, Enemy3);

    Map_canEquip(map, Silou, false, ITEM_ARCHETYPE_WEAPON);
    nourstest_true(silou->num_canEquip == 0);

    Map_Free(map);
    tnecs_world_destroy(world);
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

void test_map(void) {
    test_map_simple();
    test_map_perimeter();
    test_map_usable();
}