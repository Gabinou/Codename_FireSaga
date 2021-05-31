#include "minctest.h"
#include "tile.h"
#include "jsonio.h"
#include "platform.h"
#include "utilities.h"
#include "narrative.h"

// void baseTiles(std_unordered_map<short int, Tile> * in_tiles, const std_vector<short int> toload) {
//     SDL_Log("Loading base tiles\n");
//     Tile temp_tile;
//     Movement_cost temp_cost;
//     Tile_stats temp_stats;
//     int index;

//     for (short unsigned int i = 0; i < toload.size(); i++) {
//         if (toload[i] > DEFAULT_TILE_MAX) {
//             index = toload[i] / DEFAULT_TILE_DIVISOR;
//         } else {
//             index = toload[i];
//         }

//         switch (index) {
//             case TILE_PLAIN: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {10, 0, 1, 0};
//                 temp_tile = Tile(TILE_PLAIN, "Plain", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_BUSH: {
//                 temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_BUSH, "Bush", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_FOREST: {
//                 temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_FOREST, "Forest", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_THICKET: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {20, 0, 3, 0};
//                 temp_tile = Tile(TILE_THICKET, "Thicket", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_SNAG: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_SNAG, "Snag", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_BRIDGE: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_BRIDGE, "Bridge", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_HOLE: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_HOLE, "Hole", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_SEA: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_SEA, "Sea", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_LAKE: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_LAKE, "Lake", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_RIVER: {
//                 temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_RIVER, "River", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_WATERFALL: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_WATERFALL, "Waterfall", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_CLIFF: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_CLIFF, "cliff", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_HILL: {
//                 temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_HILL, "Hill", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_MOUNTAIN: {
//                 temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
//                 temp_stats = {25, 0, 3, 0};
//                 temp_tile = Tile(TILE_MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_PEAK: {
//                 temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
//                 temp_stats = {30, 0, 4, 0};
//                 temp_tile = Tile(TILE_PEAK, "Peak", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_CAVE: {
//                 temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
//                 temp_stats = {20, 0, 2, 0};
//                 temp_tile = Tile(TILE_CAVE, "Cave", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_SAND: {
//                 temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_SAND, "Sand", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_WASTELAND: {
//                 temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_WASTELAND, "Wasteland", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_ROCK: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
//                 temp_stats = {5, 0, 2, 0};
//                 temp_tile = Tile(TILE_ROCK, "Rock", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_BONES: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_BONES, "Bones", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_FLOOR: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_FLOOR, "Floor", temp_cost, temp_stats, true);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_PILLAR: {
//                 temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
//                 temp_stats = {10, 1, 10, 0};
//                 temp_tile = Tile(TILE_PILLAR, "Pillar", temp_cost, temp_stats, true);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_WALL: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_WALL, "Wall", temp_cost, temp_stats, true);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_DOOR: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_DOOR, "Door", temp_cost, temp_stats, true);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_THRONE: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {20, 2, 15, 0};
//                 temp_tile = Tile(TILE_THRONE, "Throne", temp_cost, temp_stats, true);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_GATE: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {10, 1, 10, 0};
//                 temp_tile = Tile(TILE_GATE, "Gate", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_FENCE: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_FENCE, "Fence", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_SHOP: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {10, 1, 10, 0};
//                 temp_tile = Tile(TILE_SHOP, "Shop", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_ARMORY: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_ARMORY, "Armory", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_VILLAGE: {
//                 temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//                 temp_stats = {15, 0, 2, 0};
//                 temp_tile = Tile(TILE_VILLAGE, "Village", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_FORT: {
//                 temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
//                 temp_stats = {10, 2, 10, 0};
//                 temp_tile = Tile(TILE_FORT, "Fort", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_CASTLE: {
//                 temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_CASTLE, "Castle", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_SNOW: {
//                 temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_SNOW, "Snow", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             case TILE_GLACIER: {
//                 temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
//                 temp_stats = {0, 0, 0, 0};
//                 temp_tile = Tile(TILE_GLACIER, "Glacier", temp_cost, temp_stats, false);
//                 in_tiles->emplace(index, temp_tile);
//                 break;
//             }

//             default: {
//                 break;
//             }
//         }

//         SDL_Log("loaded tile: %s, %d", temp_tile.name.c_str(), temp_tile.id);
//     }
// }


// void writeTilesJSON() {
//     std_unordered_map<short int, Tile> temp_tiles;
//     std_vector<short int> temp_toload = {0};
//     std_string filename;

//     for (short int i = 10; i < TILE_ICE; i++) {
//         temp_toload[0] = i;
//         baseTiles(&temp_tiles, temp_toload);

//         if (temp_tiles[i].id == i) {
//             SDL_Log("True %d, %d", i, temp_toload[0]);
//             filename = temp_tiles[i].name + ".json";
//             temp_tiles[i].writeJSON(filename.c_str(), false);
//         } else {
//             SDL_Log("False %d, %d", i, temp_toload[0]);
//         }
//     }
// }

void test_tile() {
    SDL_Log("test_tile");
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    struct Tile tile2 = Tile_default;
    struct Tile tile3 = Tile_default;
    struct Tile_stats in_tile_stats = {0, 30, 0, 8};
    struct Tile_stats out_tile_stats = Tile_stats_default;
    struct Movement_cost in_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    struct Tile tile1 =  {
        .json_element = JSON_TILE,
        .cost_struct = in_cost,
        .inside = true,
        .name = "Throne",
        .id = TILE_THRONE,
        .stats = in_tile_stats
    };
    Tile_makeMvtCostarray(&tile1);
    struct Movement_cost out_cost = Movement_cost_default;
    uint8_t cost_array[UNIT_MVT_END];
    lok(strcmp(tile1.name, "Throne") == 0);
    lok(tile1.id == TILE_THRONE);
    memcpy(cost_array, tile1.cost_array, sizeof(tile1.cost_array[0]) * (UNIT_MVT_END));
    out_cost = tile1.cost_struct;
    out_tile_stats = tile1.stats;
    lok(in_cost.foot_slow == out_cost.foot_slow);
    lok(in_cost.foot_fast == out_cost.foot_fast);
    lok(in_cost.mages == out_cost.mages);
    lok(in_cost.riders_slow == out_cost.riders_slow);
    lok(in_cost.riders_fast == out_cost.riders_fast);
    lok(in_cost.fliers == out_cost.fliers);
    lok(in_cost.armors == out_cost.armors);
    lok(in_cost.pirates == out_cost.pirates);
    lok(in_cost.bandits == out_cost.bandits);
    lok(cost_array[UNIT_MVT_FOOT_SLOW] == out_cost.foot_slow);
    lok(cost_array[UNIT_MVT_FOOT_FAST] == out_cost.foot_fast);
    lok(cost_array[UNIT_MVT_MAGES] == out_cost.mages);
    lok(cost_array[UNIT_MVT_RIDERS_SLOW] == out_cost.riders_slow);
    lok(cost_array[UNIT_MVT_RIDERS_FAST] == out_cost.riders_fast);
    lok(cost_array[UNIT_MVT_FLIERS] == out_cost.fliers);
    lok(cost_array[UNIT_MVT_ARMORS] == out_cost.armors);
    lok(cost_array[UNIT_MVT_PIRATES] == out_cost.pirates);
    lok(cost_array[UNIT_MVT_BANDITS] == out_cost.bandits);
    lok(out_tile_stats.dodge == in_tile_stats.dodge);
    lok(out_tile_stats.Pprot == in_tile_stats.Pprot);
    lok(out_tile_stats.Mprot == in_tile_stats.Mprot);
    lok(out_tile_stats.heal == in_tile_stats.heal);
    lok(tile1.inside == true);
    writeJSON("saves"PHYSFS_SEPARATOR"tile_test.json", &tile1, false);
    readJSON("saves"PHYSFS_SEPARATOR"tile_test.json", &tile3);
    writeJSON("saves"PHYSFS_SEPARATOR"tile_rewrite.json", &tile3, false);
    lok(fequal("saves"PHYSFS_SEPARATOR"tile_test.json", "saves"PHYSFS_SEPARATOR"tile_rewrite.json"));
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"tile_rewrite.json"));
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"tile_test.json"));
    lok(strcmp(tile1.name, tile3.name) == 0);
    lok(tile1.id == tile3.id);
    memcpy(cost_array, tile1.cost_array, sizeof(tile1.cost_array[0]) * (UNIT_MVT_END));
    out_cost = tile1.cost_struct;
    out_tile_stats = tile1.stats;
    in_cost = tile3.cost_struct;
    in_tile_stats = tile3.stats;
    lok(in_cost.foot_slow == out_cost.foot_slow);
    lok(in_cost.foot_fast == out_cost.foot_fast);
    lok(in_cost.mages == out_cost.mages);
    lok(in_cost.riders_slow == out_cost.riders_slow);
    lok(in_cost.riders_fast == out_cost.riders_fast);
    lok(in_cost.fliers == out_cost.fliers);
    lok(in_cost.armors == out_cost.armors);
    lok(in_cost.pirates == out_cost.pirates);
    lok(in_cost.bandits == out_cost.bandits);
    lok(out_tile_stats.dodge == in_tile_stats.dodge);
    lok(out_tile_stats.Pprot == in_tile_stats.Pprot);
    lok(out_tile_stats.Mprot == in_tile_stats.Mprot);
    lok(out_tile_stats.heal == in_tile_stats.heal);
}
