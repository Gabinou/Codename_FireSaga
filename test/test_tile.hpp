#include "minctest.h"
#include "tile.hpp"
#include "utilities.hpp"

void baseTiles(std::unordered_map<short int, Tile> * in_tiles, const std::vector<short int> toload) {
    SDL_Log("Loading base tiles\n");
    Tile temp_tile;
    Movement_cost temp_cost;
    Tile_stats temp_stats;
    int index;

    for (short unsigned int i = 0; i < toload.size(); i++) {
        if (toload[i] > DEFAULT::TILE_MAX) {
            index = toload[i] / DEFAULT::TILE_DIVISOR;
        } else {
            index = toload[i];
        }

        switch (index) {
            case TILE::PLAIN: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 0, 1, 0};
                temp_tile = Tile(TILE::PLAIN, "Plain", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BUSH: {
                temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::BUSH, "Bush", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FOREST: {
                temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::FOREST, "Forest", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::THICKET: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 0, 3, 0};
                temp_tile = Tile(TILE::THICKET, "Thicket", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SNAG: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::SNAG, "Snag", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BRIDGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::BRIDGE, "Bridge", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::HOLE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::HOLE, "Hole", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SEA: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::SEA, "Sea", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::LAKE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::LAKE, "Lake", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::RIVER: {
                temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::RIVER, "River", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WATERFALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::WATERFALL, "Waterfall", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CLIFF: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::CLIFF, "cliff", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::HILL: {
                temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::HILL, "Hill", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::MOUNTAIN: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {25, 0, 3, 0};
                temp_tile = Tile(TILE::MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::PEAK: {
                temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
                temp_stats = {30, 0, 4, 0};
                temp_tile = Tile(TILE::PEAK, "Peak", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CAVE: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {20, 0, 2, 0};
                temp_tile = Tile(TILE::CAVE, "Cave", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::SAND, "Sand", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WASTELAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::WASTELAND, "Wasteland", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::ROCK: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
                temp_stats = {5, 0, 2, 0};
                temp_tile = Tile(TILE::ROCK, "Rock", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BONES: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::BONES, "Bones", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FLOOR: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::FLOOR, "Floor", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::PILLAR: {
                temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::PILLAR, "Pillar", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::WALL, "Wall", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::DOOR: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::DOOR, "Door", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::THRONE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 2, 15, 0};
                temp_tile = Tile(TILE::THRONE, "Throne", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::GATE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::GATE, "Gate", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FENCE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::FENCE, "Fence", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SHOP: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::SHOP, "Shop", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::ARMORY: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::ARMORY, "Armory", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::VILLAGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::VILLAGE, "Village", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FORT: {
                temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
                temp_stats = {10, 2, 10, 0};
                temp_tile = Tile(TILE::FORT, "Fort", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CASTLE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::CASTLE, "Castle", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SNOW: {
                temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::SNOW, "Snow", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::GLACIER: {
                temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::GLACIER, "Glacier", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            default: {
                break;
            }
        }

        SDL_Log("loaded tile: %s, %d", temp_tile.getName().c_str(), temp_tile.getid());
    }
}


void writeTilesJSON() {
    std::unordered_map<short int, Tile> temp_tiles;
    std::vector<short int> temp_toload = {0};
    std::string filename;

    for (short int i = 10; i < TILE::ICE; i++) {
        temp_toload[0] = i;
        baseTiles(&temp_tiles, temp_toload);

        if (temp_tiles[i].getid() == i) {
            SDL_Log("True %d, %d", i, temp_toload[0]);
            filename = temp_tiles[i].getName() + ".json";
            temp_tiles[i].writeJSON(filename.c_str(), false);
        } else {
            SDL_Log("False %d, %d", i, temp_toload[0]);
        }
    }
}

void test_tile() {
    SDL_Log("test_tile");

    if (PHYSFS_stat("saves", nullptr) == 0) {
        PHYSFS_mkdir("saves");
    }

    Tile tile1, tile2, tile3;
    Tile_stats in_tile_stats, out_tile_stats;
    Movement_cost in_cost, out_cost;
    unsigned char * cost_array;
    in_tile_stats = {3, 0, 80, 0, };
    in_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    tile1 = Tile(TILE::THRONE, "Throne", in_cost, in_tile_stats, true);
    lok(tile1.getName() == "Throne");
    lok(tile1.getid() == TILE::THRONE);
    cost_array = tile1.getCost();
    out_cost = tile1.getCoststruct();
    out_tile_stats = tile1.getStats();
    lok(in_cost.foot_slow == out_cost.foot_slow);
    lok(in_cost.foot_fast == out_cost.foot_fast);
    lok(in_cost.mages == out_cost.mages);
    lok(in_cost.riders_slow == out_cost.riders_slow);
    lok(in_cost.riders_fast == out_cost.riders_fast);
    lok(in_cost.fliers == out_cost.fliers);
    lok(in_cost.armors == out_cost.armors);
    lok(in_cost.pirates == out_cost.pirates);
    lok(in_cost.bandits == out_cost.bandits);
    lok(cost_array[UNIT::MVT::FOOT_SLOW] == out_cost.foot_slow);
    lok(cost_array[UNIT::MVT::FOOT_FAST] == out_cost.foot_fast);
    lok(cost_array[UNIT::MVT::MAGES] == out_cost.mages);
    lok(cost_array[UNIT::MVT::RIDERS_SLOW] == out_cost.riders_slow);
    lok(cost_array[UNIT::MVT::RIDERS_FAST] == out_cost.riders_fast);
    lok(cost_array[UNIT::MVT::FLIERS] == out_cost.fliers);
    lok(cost_array[UNIT::MVT::ARMORS] == out_cost.armors);
    lok(cost_array[UNIT::MVT::PIRATES] == out_cost.pirates);
    lok(cost_array[UNIT::MVT::BANDITS] == out_cost.bandits);
    lok(out_tile_stats.dodge == in_tile_stats.dodge);
    lok(out_tile_stats.Pprot == in_tile_stats.Pprot);
    lok(out_tile_stats.Mprot == in_tile_stats.Mprot);
    lok(out_tile_stats.heal == in_tile_stats.heal);
    lok(tile1.isInside() == true);
    tile1.writeXML("saves//tile_test.xml");
    tile1.writeJSON("saves//tile_test.json");

    tile2 = Tile();
    tile2.readXML("saves//tile_test.xml");
    tile2.writeXML("saves//tile_rewrite.xml");

    tile3 = Tile();
    tile3.readJSON("saves//tile_test.json");
    tile3.writeJSON("saves//tile_rewrite.json");

    lok(fequal("saves//tile_test.xml", "saves//tile_rewrite.xml"));
    lok(fequal("saves//tile_test.json", "saves//tile_rewrite.json"));
    lok(tile1.getName() == tile2.getName());
    lok(tile1.getid() == tile2.getid());
    lok(tile1.getName() == tile3.getName());
    lok(tile1.getid() == tile3.getid());
    cost_array = tile1.getCost();
    out_cost = tile1.getCoststruct();
    in_cost = tile2.getCoststruct();
    out_tile_stats = tile1.getStats();
    in_tile_stats = tile2.getStats();
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

    in_cost = tile3.getCoststruct();
    in_tile_stats = tile3.getStats();
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
