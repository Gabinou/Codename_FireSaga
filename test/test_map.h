#include "minctest.h"
#include "map.h"
#include "platform.h"
#include "sprite.h"
#include "position.h"
#include "enums.h"
#include "platform.h"
#include "flecs.h"
#include "tnecs.h"
#include "stb_ds.h"

int16_t * testTiles() {
    SDL_Log("testTiles");
    int_tile_t * temp_arr = NULL;
    arrput(temp_arr, TILE_PLAIN * DEFAULT_TILE_DIVISOR + 0);
    arrput(temp_arr, TILE_FOREST * DEFAULT_TILE_DIVISOR + 0);
    arrput(temp_arr,  TILE_HILL * DEFAULT_TILE_DIVISOR + 0);
    return (temp_arr);
}

struct Map_arrival * chapTestArrivals(struct Map_arrival * out) {
    SDL_Log("chapTestArrivals");
    arrfree(out);
    struct Map_arrival temp;
    temp.id = UNIT_NAME_BANDIT;
    temp.turn = 0;
    temp.position.x = 6;
    temp.position.y = 10;
    temp.levelups = 0;
    arrput(out, temp);
    temp.id = UNIT_NAME_BANDIT;
    temp.turn = 0;
    temp.position.x = 12;
    temp.position.y = 4;
    temp.levelups = 0;

    arrput(out, temp);
    temp.id = UNIT_NAME_SEBASTIAN;
    temp.turn = 0;
    temp.position.x = 20;
    temp.position.y = 12;
    temp.levelups = 0;
    arrput(out, temp);
    return (out);
}

struct Inventory_item * chapTestEquipments(struct Inventory_item * out) {
    SDL_Log("chapTestEquipments");
    arrfree(out);
    struct Inventory_item temp = Inventory_item_default;
    arrsetlen(out, DEFAULT_EQUIPMENT_SIZE * 3);
    temp.id = ITEM_NAME_IRON_SWORD;
    out[DEFAULT_EQUIPMENT_SIZE * 0 + 0] = temp;
    temp.id = ITEM_NAME_IRON_SWORD;
    out[DEFAULT_EQUIPMENT_SIZE * 0 + 1] = temp;

    temp.id = ITEM_NAME_IRON_AXE;
    out[DEFAULT_EQUIPMENT_SIZE * 1 + 0] = temp;
    temp.id = ITEM_NAME_LEATHER_SHIELD;
    out[DEFAULT_EQUIPMENT_SIZE * 1 + 1] = temp;

    temp.id = ITEM_NAME_IRON_LANCE;
    out[DEFAULT_EQUIPMENT_SIZE * 2 + 0] = temp;
    temp.id = ITEM_NAME_KITE_SHIELD;
    out[DEFAULT_EQUIPMENT_SIZE * 2 + 1] = temp;
    return (out);
}

enum MAP_LENS {
    MAP_ROW_LEN = 21,
    MAP_COL_LEN = 25,
};

int_tile_t * testTilemap(int_tile_t * in_tilemap) {
    SDL_Log("testTilemap");
    if (in_tilemap != NULL) {
        arrfree(in_tilemap);
    }
    arrsetlen(in_tilemap, MAP_ROW_LEN * MAP_COL_LEN);

    int16_t tilemap[MAP_ROW_LEN * MAP_COL_LEN] = {
        100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 100, 300, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 120, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
    };
    for (uint8_t row = 0; row < MAP_ROW_LEN; row++) {
        for (uint8_t col = 0; col < MAP_COL_LEN; col++) {
            in_tilemap[(row * MAP_COL_LEN + col)] = tilemap[(row * MAP_COL_LEN + col)];
        }
    }
    return (in_tilemap);
}

void test_map() {
    SDL_Log("test_map");
    utilities_allNames_Load();
    tnecs_world_t * tnecs_world = tnecs_world_genesis();
    ecs_world_t * temp_world = ecs_init();
    TNECS_REGISTER_COMPONENT(tnecs_world, Unit);
    TNECS_REGISTER_COMPONENT(tnecs_world, Position);
    lok(strcmp("NULL", tnecs_world->component_names[0]) == 0);
    lok(strcmp("Unit", tnecs_world->component_names[1]) == 0);
    lok(strcmp("Position", tnecs_world->component_names[2]) == 0);

    ECS_IMPORT(temp_world, UnitModule);
    ECS_IMPORT(temp_world, PositionModule);

    short int tilesize = 32;
    struct Map map1;
    struct Map map2;
    struct Map * map3 = malloc(sizeof(struct Map));
    map1 = Map_default;
    map2 = Map_default;
    *map3 = Map_default;

    map1.items_num = NULL;
    arrsetcap(map1.items_num, 8);
    map2.items_num = NULL;
    arrsetcap(map2.items_num, 8);
    map3->items_num = NULL;
    arrsetcap(map3->items_num, 8);

    struct Tiles_hash * tiles1 = NULL;
    hmdefault(tiles1, &Tile_default);
    map1.tiles = tiles1;
    struct Tiles_hash * tiles2 = NULL;
    hmdefault(tiles2, &Tile_default);
    map2.tiles = tiles2;
    struct Tiles_hash * tiles3 = NULL;
    hmdefault(tiles3, &Tile_default);
    map3->tiles = tiles3;

    map1.starting_positions = NULL;
    map2.starting_positions = NULL;
    map3->starting_positions = NULL;

    map1.tilesize[0] = tilesize;
    map1.tilesize[1] = tilesize;

    struct Point in_start;
    struct Point out_start;
    struct Point temp = {0, 1};
    arrput(map1.tilesindex, 100);
    arrput(map1.tilesindex, 120);
    arrput(map1.tilesindex, 300);
    arrput(map2.tilesindex, 100);
    arrput(map2.tilesindex, 120);
    arrput(map2.tilesindex, 300);
    size_t in_len = arrlen(map1.tilesindex);
    lok(arrlen(map1.tilesindex) == 3);
    if (in_len == 3) {
        for (uint16_t i = 0; i < in_len; i++) {
            //SDL_Log("i %d", i);
            lok(map2.tilesindex[i] == map1.tilesindex[i]);
        }
    }
    map1.tiles_num = 3;
    Map_Tiles_Load(& map1);

    hmget(map1.tiles, 10);
    // hmget(map1->tiles, 100);

    map1.tilemap = testTilemap(map1.tilemap);
    map2.tilemap = testTilemap(map2.tilemap);

    map1.row_len = MAP_ROW_LEN;
    map2.row_len = MAP_ROW_LEN;
    map1.col_len = MAP_COL_LEN;
    map2.col_len = MAP_COL_LEN;

    arrsetcap(map1.units_onfield, 10);
    arrsetcap(map2.units_onfield, 10);
    arrsetcap(map1.armies_onfield, 10);
    arrsetcap(map2.armies_onfield, 10);

    map1.unitmap = NULL;
    map2.unitmap = NULL;
    arrsetlen(map1.unitmap, MAP_ROW_LEN * MAP_COL_LEN);
    arrsetlen(map2.unitmap, MAP_ROW_LEN * MAP_COL_LEN);
    for (uint8_t row = 0; row < MAP_ROW_LEN; row++) {
        for (uint8_t col = 0; col < MAP_COL_LEN; col++) {
            map1.unitmap[row * MAP_ROW_LEN + col] = 0;
            map2.unitmap[row * MAP_ROW_LEN + col] = 0;
        }
    }

    map1.tnecs_unitmap = NULL;
    map2.tnecs_unitmap = NULL;
    arrsetlen(map1.tnecs_unitmap, MAP_ROW_LEN * MAP_COL_LEN);
    arrsetlen(map2.tnecs_unitmap, MAP_ROW_LEN * MAP_COL_LEN);
    for (uint8_t row = 0; row < MAP_ROW_LEN; row++) {
        for (uint8_t col = 0; col < MAP_COL_LEN; col++) {
            map1.tnecs_unitmap[row * MAP_ROW_LEN + col] = 0;
            map2.tnecs_unitmap[row * MAP_ROW_LEN + col] = 0;
        }
    }

    Map_Bounds_Compute(&map1);

    lok(map1.offset_px.x == DEFAULT_TILEMAP_XOFFSET);
    lok(map1.offset_px.y == DEFAULT_TILEMAP_YOFFSET);

    uint8_t arrivals_num = 3;
    map1.arrivals = NULL;
    map1.arrivals = chapTestArrivals(map1.arrivals);
    map1.arrivals_num = 3;
    map2.arrivals = NULL;

    map1.arrival_equipments = NULL;
    map2.arrival_equipments = NULL;
    arrsetlen(map2.arrival_equipments, arrivals_num * DEFAULT_EQUIPMENT_SIZE);
    map1.arrival_equipments = chapTestEquipments(map1.arrival_equipments);
    arrput(map1.items_num, 2);
    arrput(map1.items_num, 2);
    arrput(map1.items_num, 2);
    arrput(map2.items_num, 2);
    arrput(map2.items_num, 2);
    arrput(map2.items_num, 2);

    lok(map1.arrivals[0].id == UNIT_NAME_BANDIT);
    lok(map1.arrivals[0].turn == 0);
    lok(map1.arrivals[0].position.x == 6);
    lok(map1.arrivals[0].position.y == 10);
    lok(map1.arrivals[0].levelups == 0);

    lok(map1.arrivals[1].id == UNIT_NAME_BANDIT);
    lok(map1.arrivals[1].turn == 0);
    lok(map1.arrivals[1].position.x == 12);
    lok(map1.arrivals[1].position.y == 4);
    lok(map1.arrivals[1].levelups == 0);

    lok(map1.arrivals[2].id == UNIT_NAME_SEBASTIAN);
    lok(map1.arrivals[2].turn == 0);
    lok(map1.arrivals[2].position.x == 20);
    lok(map1.arrivals[2].position.y == 12);
    lok(map1.arrivals[2].levelups == 0);

    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 0 + 0].id == ITEM_NAME_IRON_SWORD);
    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 0 + 1].id == ITEM_NAME_IRON_SWORD);
    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 1 + 0].id == ITEM_NAME_IRON_AXE);
    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 1 + 1].id == ITEM_NAME_LEATHER_SHIELD);
    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 2 + 0].id == ITEM_NAME_IRON_LANCE);
    lok(map1.arrival_equipments[DEFAULT_EQUIPMENT_SIZE * 2 + 1].id == ITEM_NAME_KITE_SHIELD);

    struct Unit unit1 = Unit_default;
    struct Unit unit2 = Unit_default;
    struct Unit unit3 = Unit_default;
    struct Unit unit4 = Unit_default;
    ecs_entity_t temp_entity;
    tnecs_entity_t tnecs_entity;
    temp_entity = ecs_new(temp_world, 0);
    tnecs_entity = TNECS_ENTITY_CREATE_WCOMPONENTS(tnecs_world, Position, Unit);

    ecs_add(temp_world, temp_entity, Position);
    ecs_add(temp_world, temp_entity, Unit);
    struct Unit * temp_entity_unit_mptr, * tnecs_unit_ptr;
    temp_entity_unit_mptr = ecs_get_mut(temp_world, temp_entity, Unit, NULL);
    tnecs_unit_ptr = TNECS_GET_COMPONENT(tnecs_world, tnecs_entity, Unit);
    lok(tnecs_unit_ptr != NULL);

    struct Unit_stats in_stats = {15,  4,  5,  7,  6,  8,  4,  6,  5,  5,  6};
    struct Unit_stats out_stats = Unit_stats_default;
    struct Unit_stats in_caps = {48, 14, 25, 32, 34, 28, 19, 40, 15};
    struct Unit_stats out_caps = Unit_stats_default;
    struct Unit_stats in_growths = {60, 50, 20, 60, 70, 40, 30, 20, 10, 0};
    struct Unit_stats out_growths = Unit_stats_default;
    struct Inventory_item in_wpn = Inventory_item_default;
    struct Inventory_item out_wpn = Inventory_item_default;
    int16_t in_exp = 0;
    int16_t out_exp = 0;
    Unit_setid(temp_entity_unit_mptr, UNIT_NAME_SILOU);
    Unit_setClassind(temp_entity_unit_mptr, UNIT_CLASS_MAGE);
    Unit_setSex(temp_entity_unit_mptr, UNIT_SEX_F);
    tnecs_unit_ptr->base_stats = in_stats;
    tnecs_unit_ptr->current_stats = in_stats;
    tnecs_unit_ptr->caps_stats = in_stats;
    tnecs_unit_ptr->growths = in_growths;
    in_exp = 400;
    tnecs_unit_ptr->exp = in_exp;
    tnecs_unit_ptr->base_exp = in_exp;
    tnecs_unit_ptr->army = ARMY_ERWIN;
    in_wpn.id = ITEM_NAME_BALL_LIGHTNING;

    Unit_Item_Add(tnecs_unit_ptr, in_wpn);

    // Map_Unit_Put(&map1, temp_world, 6, 6, temp_entity);
    Map_Unit_Put_tnecs(&map1, tnecs_world, 6, 6, tnecs_entity);

    struct Point position1 = {1, 2};
    struct Point position2 = {3, 4};
    struct Point position3 = {5, 6};
    map1.starting_positions_num = 0;
    Map_startingPos_Add(&map1, position1);
    Map_startingPos_Add(&map1, position2);
    Map_startingPos_Add(&map1, position3);

    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }

    writeJSON("saves"PHYSFS_SEPARATOR"map_test.json", &map1, false);
    readJSON("saves"PHYSFS_SEPARATOR"map_test.json", &map2);
    readJSON("saves"PHYSFS_SEPARATOR"map_test.json", map3);
    writeJSON("saves"PHYSFS_SEPARATOR"map_rewrite.json", &map2, false);
    writeJSON("saves"PHYSFS_SEPARATOR"map_rewrite3.json", map3, false);
    lok(fequal("saves"PHYSFS_SEPARATOR"map_rewrite3.json", "saves"PHYSFS_SEPARATOR"map_rewrite.json")); //->fc map_rewrite3.json map_rewrite.json ARE THE SAME
    lok(fequal("saves"PHYSFS_SEPARATOR"map_test.json", "saves"PHYSFS_SEPARATOR"map_rewrite.json"));
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"map_rewrite.json"));
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"map_test.json"));
}
