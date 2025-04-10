
#include "nourstest.h"
#include "platform.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/find.h"
#include "menu/menu.h"
#include "pathfinding.h"
#include "macros.h"
#include "structs.h"
#include "map/map.h"
#include "bars/map_hp.h"
#include "systems/render.h"
#include "filesystem.h"
#include "noursclock.h"
#include "log.h"
#include "macros.h"
#include "RNG.h"
#include "platform.h"
#include "position.h"
#include "popup/popup.h"
#include "menu/menu.h"
#include "unit/boss.h"
#include "unit/anim.h"
#include "unit/flags.h"
#include "unit/equipment.h"
#include "menu/loadout_select.h"
#include "menu/deployment.h"
#include "menu/stats.h"
#include "map/animation.h"
#include "unit/party.h"
#include "map/render.h"
#include "scene.h"
#include "cutscene.h"
#include "slider.h"
#include "pixelfonts.h"
#include "sprite.h"
#include "input.h"
#include "tile.h"
#include "hover.h"
#include "combat.h"
#include "music.h"
#include "controller/mouse.h"
#include "controller/touchpad.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "systems/time_system.h"
#include "systems/slide.h"
#include "AI.h"
#include "events.h"
#include "fps_fsm.h"
#include "fsm.h"
#include "names.h"
#include "text.h"
#include "convoy.h"
#include "utilities.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_map_simple(void) {
    i32 x = 6, y = 6;
    size_t row_len = TEST_ROW_LEN, col_len = TEST_COL_LEN;
    struct Range range = {2, 4};
    i32 *list           = DARR_INIT(list, i32, 16);
    i32 *test_matrix    = SDL_calloc(row_len * col_len, sizeof(*test_matrix));

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
        SDL_free(perimeter_edges);
        perimeter_edges = NULL;
    }
}

void test_map_usable(void) {
    /* Does the loadout make sense for unit/class/selection on MAP. */

    /* Tnecs init */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);

#include "register_components.h"

    tnecs_entity Silou  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit_ID, Position_ID);
    tnecs_entity Erwin  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit_ID, Position_ID);
    tnecs_entity Enemy  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit_ID, Position_ID);
    Unit *silou         = IES_GET_COMPONENT(world, Silou, Unit);
    Unit *erwin         = IES_GET_COMPONENT(world, Erwin, Unit);
    Unit *enemy         = IES_GET_COMPONENT(world, Enemy, Unit);

    Position *silou_pos = IES_GET_COMPONENT(world, Silou, Position);
    Position *erwin_pos = IES_GET_COMPONENT(world, Erwin, Position);
    Position *enemy_pos = IES_GET_COMPONENT(world, Enemy, Position);

    /* Units init */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(silou, weapons_dtab);

    /* --- Testing 1 range only --- */
    Unit_Equippable_set(silou, ITEM_TYPE_SWORD);
    Inventory_item *silou_eq = Unit_Equipment(silou);

    silou_eq[0].id              = ITEM_ID_FLEURET;
    silou_eq[1].id              = ITEM_ID_IRON_AXE;
    silou_eq[2].id              = ITEM_ID_IRON_LANCE;
    silou_eq[3].id              = ITEM_ID_COMPOSITE_BOW;
    silou_eq[4].id              = ITEM_ID_HEAL;
    Unit_Army_set(silou, ARMY_FRIENDLY);
    Unit_Army_set(erwin, ARMY_FRIENDLY);
    Unit_Army_set(enemy, ARMY_ENEMY);
    silou->stats.current.move   = 2;
    silou_pos->tilemap_pos.x    = 0;
    silou_pos->tilemap_pos.y    = 0;
    erwin_pos->tilemap_pos.x    = 0;
    erwin_pos->tilemap_pos.y    = 1;
    enemy_pos->tilemap_pos.x    = 0;
    enemy_pos->tilemap_pos.y    = 2;

    Unit_Equip(silou, UNIT_HAND_LEFT, ITEM1);
    Unit_Unequip(silou, UNIT_HAND_RIGHT);

    Unit_stats eff_stats = Unit_effectiveStats(silou);
    SDL_assert(silou->stats.current.move == eff_stats.move);

    /* Map init */
    NewMap new_map  = NewMap_default;
    new_map.row_len = TEST_ROW_LEN;
    new_map.col_len = TEST_COL_LEN;
    new_map.world   = world;

    Map *map = Map_New(new_map);
    SDL_assert(map->attacktolist    != NULL);
    SDL_assert(map->tilemap         != NULL);

    DARR_PUT(map->tilesindex, TILE_PLAIN);
    DARR_PUT(map->tilesindex, TILE_LAKE);
    Map_Tiles_Load(map);

    for (int i = 0; i < (map->row_len * map->col_len); i++) {
        map->tilemap[i] = TILE_PLAIN * TILE_DIVISOR + 1;
    }

    SDL_assert(DARR_NUM(map->tilesindex)    == 2);
    SDL_assert(DARR_NUM(map->tiles)         == 2);
    SDL_assert(DARR_NUM(map->tilesindex)    == 2);

    /* --- NO enemy in range --- */
    _Map_Unit_Put(map, silou_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Silou);
    canEquip can_equip  = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = false;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0);

    /* -- Testing 1 range, 0 move -- */
    _Map_Unit_Put(map, erwin_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Erwin);
    _Map_Unit_Put(map, enemy_pos->tilemap_pos.x, enemy_pos->tilemap_pos.y, Enemy);

    /* -- Testing 1 range, 1 move, Erwin blocks -- */
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = false;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0)
    ;
    nourstest_true(_Map_Unit_Remove_Map(map,
                                        erwin_pos->tilemap_pos.x,
                                        erwin_pos->tilemap_pos.y) == Erwin);
    nourstest_true(_Map_Unit_Remove_Map(map,
                                        enemy_pos->tilemap_pos.x,
                                        enemy_pos->tilemap_pos.y) == Enemy);

    erwin_pos->tilemap_pos.x    = 0;
    erwin_pos->tilemap_pos.y    = 1;
    enemy_pos->tilemap_pos.x    = 0;
    enemy_pos->tilemap_pos.y    = 2;
    _Map_Unit_Put(map, erwin_pos->tilemap_pos.x,    erwin_pos->tilemap_pos.y,   Erwin);
    _Map_Unit_Put(map, enemy_pos->tilemap_pos.x,   enemy_pos->tilemap_pos.y,  Enemy);

    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0);

    silou->stats.current.move           = 4;
    eff_stats = Unit_effectiveStats(silou);
    SDL_assert(silou->stats.current.move == eff_stats.move);
    _Map_Unit_Put(map, 1, 2,  Erwin);

    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0);

    /* --- 1 enemy in range --- */
    nourstest_true(_Map_Unit_Remove_Map(map, 1, 2) == Erwin);
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 1);
    nourstest_true(silou->can_equip.arr[0]    == ITEM1);

    /* --- TODO: Range types, blocked by unit --- */
    Unit_Equippable_set(silou, ITEM_TYPE_BOW);
    memset(map->unitmap, 0, sizeof(*map->unitmap) * map->col_len * map->row_len);
    silou->stats.current.move           = 1;

    silou_pos->tilemap_pos.x    = 0;
    silou_pos->tilemap_pos.y    = 0;
    erwin_pos->tilemap_pos.x    = 0;
    erwin_pos->tilemap_pos.y    = 1;
    enemy_pos->tilemap_pos.x    = 0;
    enemy_pos->tilemap_pos.y    = 3;
    _Map_Unit_Put(map, silou_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Silou);
    _Map_Unit_Put(map, erwin_pos->tilemap_pos.x, erwin_pos->tilemap_pos.y, Erwin);
    _Map_Unit_Put(map, enemy_pos->tilemap_pos.x, enemy_pos->tilemap_pos.y, Enemy);
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0);

    // Can't attack, Erwin is preventing move to tile Silou can attack from
    silou->stats.current.move           = 2;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num == 0);

    silou->stats.current.move           = 3;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 1);
    nourstest_true(silou->can_equip.arr[0]    == ITEM4);

    /* --- TODO: multiple types, blocked by unit --- */
    Unit_Equippable_set(silou, ITEM_TYPE_BOW | ITEM_TYPE_SWORD);
    silou->stats.current.move           = 1;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 0);

    // printf("UNITMAP\n");
    // entity_print(map->unitmap, map->row_len, map->col_len);

    silou->stats.current.move           = 2;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 1);
    nourstest_true(silou->can_equip.arr[0]    == ITEM1);

    silou->stats.current.move           = 3;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 2);
    nourstest_true(silou->can_equip.arr[0]    == ITEM1);
    nourstest_true(silou->can_equip.arr[1]    == ITEM4);

    /* --- Testing staff --- */
    Unit_Equippable_set(silou, ITEM_TYPE_STAFF);
    memset(map->unitmap, 0, sizeof(*map->unitmap) * map->col_len * map->row_len);
    silou->stats.current.move = 1;

    silou_pos->tilemap_pos.x    = 0;
    silou_pos->tilemap_pos.y    = 0;
    enemy_pos->tilemap_pos.x    = 0;
    enemy_pos->tilemap_pos.y    = 1;
    erwin_pos->tilemap_pos.x    = 0;
    erwin_pos->tilemap_pos.y    = 2;
    _Map_Unit_Put(map, silou_pos->tilemap_pos.x, silou_pos->tilemap_pos.y, Silou);
    _Map_Unit_Put(map, erwin_pos->tilemap_pos.x, erwin_pos->tilemap_pos.y, Erwin);
    _Map_Unit_Put(map, enemy_pos->tilemap_pos.x, enemy_pos->tilemap_pos.y, Enemy);

    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 0);

    silou->stats.current.move = 2;
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 0);

    silou->stats.current.move = 3;
    // Can't equip staff when patient is full health
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_STAFF;
    can_equip.move      = true;
    Map_canEquip(map, Silou, can_equip);
    // printf("silou->can_equip.num %d \n", silou->can_equip.num);
    nourstest_true(silou->can_equip.num      == 0);

    erwin->counters.hp       =  1;
    erwin->stats.current.hp = 19;

    // Can equip staff when patient is NOT full health
    can_equip                   = canEquip_default;
    can_equip.archetype         = ITEM_ARCHETYPE_STAFF;
    can_equip.move              = true;
    can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 1);
    nourstest_true(silou->can_equip.arr[0]    == ITEM5);

    // Can't equip staff, can't reach patient
    can_equip           = canEquip_default;
    can_equip.archetype = ITEM_ARCHETYPE_STAFF;
    can_equip.move      = false;
    Map_canEquip(map, Silou, can_equip);
    nourstest_true(silou->can_equip.num      == 0);

    /* --- Staff with blocked tiles next to SELF --- */
    /* -- Place staff user on, next to blocked tiles -- */
    for (int i = 0; i < (map->row_len * map->col_len); i++) {
        map->tilemap[i] = TILE_LAKE * TILE_DIVISOR + 1;
    }
    Map_Costmap_Wipe(map);

    MapAct map_to       = MapAct_default;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = Silou;
    map_to.move         = true;

    silou_pos->tilemap_pos.x    = 1;
    silou_pos->tilemap_pos.y    = 1;
    i32 index;
    index = sota_2D_index((silou_pos->tilemap_pos.x), (silou_pos->tilemap_pos.y), map->col_len);
    map->tilemap[index] = TILE_PLAIN * TILE_DIVISOR + 1;

    /* - healtopmap - */
    Map_Act_To(map, map_to);

    // printf("COST\n");
    // matrix_print(map->costmap, map->row_len, map->col_len);
    // printf("MOVE\n");
    // matrix_print(map->movemap, map->row_len, map->col_len);
    // printf("HEAL\n");
    // matrix_print(map->healtomap, map->row_len, map->col_len);

    index = sota_2D_index((silou_pos->tilemap_pos.x + 1), (silou_pos->tilemap_pos.y), map->col_len);
    nourstest_true(map->healtomap[index] > 0);
    index = sota_2D_index((silou_pos->tilemap_pos.x), (silou_pos->tilemap_pos.y + 1), map->col_len);
    nourstest_true(map->healtomap[index] > 0);
    index = sota_2D_index((silou_pos->tilemap_pos.x - 1), (silou_pos->tilemap_pos.y), map->col_len);
    nourstest_true(map->healtomap[index] > 0);
    index = sota_2D_index((silou_pos->tilemap_pos.x), (silou_pos->tilemap_pos.y - 1), map->col_len);
    nourstest_true(map->healtomap[index] > 0);

    // TODO: Staff with no target
    // TODO: Staff with enemy target
    // TODO: Ranged staff

    Map_Free(map);
    tnecs_world_destroy(&world);
    SDL_free(map);
#undef Unit_ID
#undef Position_ID
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

void test_map(void) {
    test_map_simple();
    test_map_perimeter();
    test_map_usable();
}