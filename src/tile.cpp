#include "tile.hpp"

Tile::~Tile() {
}

Tile::Tile(void) { 
}

Tile::Tile(const short int in_id, const char* in_name, const Movement_cost in_cost, const Tile_stats in_stats, const bool in_inside) {
	name = in_name;
    id = in_id;
	cost_struct = in_cost;
    cost[UNIT::MVT::FOOT_SLOW] = in_cost.foot_slow;
    cost[UNIT::MVT::FOOT_FAST] = in_cost.foot_fast;
    cost[UNIT::MVT::MAGES] = in_cost.mages;
    cost[UNIT::MVT::RIDERS_SLOW] = in_cost.riders_slow;
    cost[UNIT::MVT::RIDERS_FAST] = in_cost.riders_fast;
    cost[UNIT::MVT::FLIERS] = in_cost.fliers;
    cost[UNIT::MVT::ARMORS] = in_cost.armors;
    cost[UNIT::MVT::PIRATES] = in_cost.pirates;
    cost[UNIT::MVT::BANDITS] = in_cost.bandits;
    stats = in_stats;
    inside = in_inside;
}

unsigned char * Tile::getCost() {
	return(cost);
}

Movement_cost Tile::getCoststruct() {
	return(cost_struct);
}

std::string Tile::getName() {
	return(name);
}

Tile_stats Tile::getStats() {
	return(stats);
}

bool Tile::isInside() {
    return(inside);
}

bool Tile::isOutside() {
    return(!inside);
}
short int Tile::getid() {
    return(id);
}

void Tile::setInside(const bool in_inside) {
    inside = in_inside;
}

void Tile::write(const char * filename, const char * mode) {
    FILE * fp;
    fp = fopen(filename, mode);
    fprintf(fp, "%s \n", name.c_str());
    if (inside) {
        fprintf(fp, "%s \n", "Inside");
    } else {
        fprintf(fp, "%s \n", "Outside");
    }
    fprintf(fp, "Stats, Dodge, PProtection, MProtection, Heal \n");
    fprintf(fp, "Stats,\t\t%d,\t%d,\t%d,\t%d\n", stats.dodge, stats.Pprot, stats.Mprot, stats.heal);
    fprintf(fp, "\n");
    fclose(fp);
}

std::unordered_map<short int, Tile> all_tiles;
std::unordered_map<short int, Tile> loaded_tiles;

std::unordered_map<short int, Tile> baseTiles(const std::vector<short int> toload) {
    SDL_Log("Loading base tiles\n");
//     unsigned int * temp_stats[3];// dodge, def, heal
//     Tile_stats temp_stats;
    std::unordered_map<short int, Tile> temp_tiles;
    Movement_cost temp_cost;
    Tile_stats temp_stats;
    for (short unsigned int i = 0; i < toload.size(); i++) {
        switch (toload[i]/10){
            case TILE::PLAIN: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 0, 1, 0};
                Tile plain(TILE::PLAIN, "Plain", temp_cost, temp_stats, false);
                temp_tiles[TILE::PLAIN] = plain;
                break;
            }
            case TILE::BUSH: {
                temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
                temp_stats = {15, 0, 2, 0};
                Tile bush(TILE::BUSH, "Bush", temp_cost, temp_stats, false);
                temp_tiles[TILE::BUSH] = bush;
                break;
             }
             case TILE::FOREST: {
                temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
                temp_stats = {15, 0, 2, 0};
                Tile forest(TILE::FOREST, "Forest", temp_cost, temp_stats, false);
                temp_tiles[TILE::FOREST] = forest;
                break; 
             }
             case TILE::THICKET: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 0, 3, 0};
                Tile thicket(TILE::THICKET, "Thicket", temp_cost, temp_stats, false);
                temp_tiles[TILE::THICKET] = thicket;
                break; 
             }
             case TILE::SNAG: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                Tile snag(TILE::SNAG, "Snag", temp_cost, temp_stats, false);
                temp_tiles[TILE::SNAG] = snag;
                break; 
             }
             case TILE::BRIDGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                Tile bridge(TILE::BRIDGE, "Bridge", temp_cost, temp_stats, false);
                temp_tiles[TILE::BRIDGE] = bridge;
                break; 
             }
             case TILE::HOLE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                Tile hole(TILE::HOLE, "Hole", temp_cost, temp_stats, false);
                temp_tiles[TILE::HOLE] = hole;
                break; 
             }
             case TILE::SEA: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
                temp_stats = {15, 0, 2, 0};
                Tile sea(TILE::SEA, "Sea", temp_cost, temp_stats, false);
                temp_tiles[TILE::SEA] = sea;
                break; 
             }
             case TILE::LAKE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
                temp_stats = {15, 0, 2, 0};
                Tile lake(TILE::LAKE, "Lake", temp_cost, temp_stats, false);
                temp_tiles[TILE::LAKE] = lake;
                break; 
             }
             case TILE::RIVER: {
                temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
                temp_stats = {15, 0, 2, 0};
                Tile river(TILE::RIVER, "River", temp_cost, temp_stats, false);
                temp_tiles[TILE::RIVER] = river;
                break; 
             }
             case TILE::WATERFALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                Tile waterfall(TILE::WATERFALL, "Waterfall", temp_cost, temp_stats, false);
                temp_tiles[TILE::WATERFALL] = waterfall;
                break; 
             }
             case TILE::CLIFF: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                Tile cliff(TILE::CLIFF, "cliff", temp_cost, temp_stats, false);
                temp_tiles[TILE::CLIFF] = cliff;
                break; 
             }
             case TILE::HILL: {
                temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
                temp_stats = {15, 0, 2, 0};
                Tile hill(TILE::HILL, "Hill", temp_cost, temp_stats, false);
                temp_tiles[TILE::HILL] = hill;
                break; 
             }
             case TILE::MOUNTAIN: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {25, 0, 3, 0};
                Tile mountain(TILE::MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
                temp_tiles[TILE::MOUNTAIN] = mountain;
                break;
             }
             case TILE::PEAK: {
                temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
                temp_stats = {30, 0, 4, 0};
                Tile peak(TILE::PEAK, "Peak", temp_cost, temp_stats, false);
                temp_tiles[TILE::PEAK] = peak;    
                break; 
             }
             case TILE::CAVE: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {20, 0, 2, 0};
                Tile cave(TILE::CAVE, "Cave", temp_cost, temp_stats, false);
                temp_tiles[TILE::CAVE] = cave;
                break; 
             }
             case TILE::SAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                Tile sand(TILE::SAND, "Sand", temp_cost, temp_stats, false);
                temp_tiles[TILE::SAND] = sand;
                break; 
            }
            case TILE::WASTELAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                Tile wasteland(TILE::WASTELAND, "Wasteland", temp_cost, temp_stats, false);
                temp_tiles[TILE::WASTELAND] = wasteland;
                break; 
            }
            case TILE::ROCK: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
                temp_stats = {5, 0, 2, 0};
                Tile rock(TILE::ROCK, "Rock", temp_cost, temp_stats, false);
                temp_tiles[TILE::ROCK] = rock;
                break; 
            }
            case TILE::BONES: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                Tile bones(TILE::BONES, "Bones", temp_cost, temp_stats, false);
                temp_tiles[TILE::BONES] = bones;
                break; 
            }
            case TILE::FLOOR: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {0, 0, 0, 0};
                Tile floor(TILE::FLOOR, "Floor", temp_cost, temp_stats, true);
                temp_tiles[TILE::FLOOR] = floor;
                break;
                }
            case TILE::PILLAR: {
                temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
                temp_stats = {10, 1, 10, 0};
                Tile pillar(TILE::PILLAR, "Pillar", temp_cost, temp_stats, true);
                temp_tiles[TILE::PILLAR] = pillar;
                break; 
            }
            case TILE::WALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                Tile wall(TILE::WALL, "Wall", temp_cost, temp_stats, true);
                temp_tiles[TILE::WALL] = wall;
                break; 
            }
            case TILE::DOOR: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                Tile door(TILE::DOOR, "Door", temp_cost, temp_stats, true);
                temp_tiles[TILE::DOOR] = door;
                break; 
            }
            case TILE::THRONE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 2, 15, 0};
                Tile throne(TILE::THRONE, "Throne", temp_cost, temp_stats, true);
                temp_tiles[TILE::THRONE] = throne;
                break; 
            }
            case TILE::GATE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {10, 1, 10, 0};
                Tile gate(TILE::GATE, "Gate", temp_cost, temp_stats, false);
                temp_tiles[TILE::GATE] = gate;
                break; 
            }
            case TILE::FENCE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                Tile fence(TILE::FENCE, "Fence", temp_cost, temp_stats, false);
                temp_tiles[TILE::FENCE] = fence;
                break; 
            }
            case TILE::SHOP: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 1, 10, 0};
                Tile shop(TILE::SHOP, "Shop", temp_cost, temp_stats, false);
                temp_tiles[TILE::SHOP] = shop;
                break; 
            }
            case TILE::ARMORY: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                Tile armory(TILE::ARMORY, "Armory", temp_cost, temp_stats, false);
                temp_tiles[TILE::ARMORY] = armory;
                break; 
            }
            case TILE::VILLAGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                Tile village(TILE::VILLAGE, "Village", temp_cost, temp_stats, false);
                temp_tiles[TILE::VILLAGE] = village;
                break; 
            }
            case TILE::FORT: {
                temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
                temp_stats = {10, 2, 10, 0};
                Tile fort(TILE::FORT, "Fort", temp_cost, temp_stats, false);
                temp_tiles[TILE::FORT] = fort;
                break; 
            }
            case TILE::CASTLE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                Tile castle(TILE::CASTLE, "Castle", temp_cost, temp_stats, false);
                temp_tiles[TILE::CASTLE] = castle;
                break; 
            }
            case TILE::SNOW: {
                temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
                temp_stats = {0, 0, 0, 0};
                Tile snow(TILE::SNOW, "Snow", temp_cost, temp_stats, false);
                temp_tiles[TILE::SNOW] = snow;
                break; 
            }            
            case TILE::GLACIER: {
                temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
                temp_stats = {0, 0, 0, 0};
                Tile glacier(TILE::GLACIER, "Glacier", temp_cost, temp_stats, false);
                temp_tiles[TILE::GLACIER] = glacier;
                break; 
            }
            default: {
                break; 
            }
        }
    }
    return(temp_tiles);
}

void baseTiles() {
	SDL_Log("Loading all base tiles\n");
    // Fliers always have 1 movement because they dismount inside.
    
    // Mvt_types: foot_slow, foot_fast, mages, riders_slow, riders_fast, fliers, armors, pirates, bandits;
    Movement_cost temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    // Tile_stats:  dodge, Pprot, Mprot, heal
    Tile_stats temp_stats = {10, 0, 1, 0};
    Tile plain(TILE::PLAIN, "Plain", temp_cost, temp_stats, false);
    all_tiles[TILE::PLAIN] = plain;
    temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
    temp_stats = {15, 0, 2, 0};
    Tile bush(TILE::BUSH, "Bush", temp_cost, temp_stats, false);
    all_tiles[TILE::BUSH] = bush;

    temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
    temp_stats = {15, 0, 2, 0};
    Tile forest(TILE::FOREST, "Forest", temp_cost, temp_stats, false);
    all_tiles[TILE::FOREST] = forest;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {20, 0, 3, 0};
    Tile thicket(TILE::THICKET, "Thicket", temp_cost, temp_stats, false);
    all_tiles[TILE::THICKET] = thicket;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {15, 0, 2, 0};
    Tile snag(TILE::SNAG, "Snag", temp_cost, temp_stats, false);
    all_tiles[TILE::SNAG] = snag;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {15, 0, 2, 0};
    Tile bridge(TILE::BRIDGE, "Bridge", temp_cost, temp_stats, false);
    all_tiles[TILE::BRIDGE] = bridge;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {15, 0, 2, 0};
    Tile hole(TILE::HOLE, "Hole", temp_cost, temp_stats, false);
    all_tiles[TILE::HOLE] = hole;

    temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
    temp_stats = {15, 0, 2, 0};
    Tile sea(TILE::SEA, "Sea", temp_cost, temp_stats, false);
    all_tiles[TILE::SEA] = sea;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
    temp_stats = {15, 0, 2, 0};
    Tile lake(TILE::LAKE, "Lake", temp_cost, temp_stats, false);
    all_tiles[TILE::LAKE] = lake;
    temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
    temp_stats = {15, 0, 2, 0};
    Tile river(TILE::RIVER, "River", temp_cost, temp_stats, false);
    all_tiles[TILE::RIVER] = river;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {15, 0, 2, 0};
    Tile waterfall(TILE::WATERFALL, "Waterfall", temp_cost, temp_stats, false);
    all_tiles[TILE::WATERFALL] = waterfall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {15, 0, 2, 0};
    Tile cliff(TILE::CLIFF, "Cliff", temp_cost, temp_stats, false);
    all_tiles[TILE::CLIFF] = cliff;

    temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
    temp_stats = {15, 0, 2, 0};
    Tile hill(TILE::HILL, "Hill", temp_cost, temp_stats, false);
    all_tiles[TILE::HILL] = hill;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    temp_stats = {25, 0, 3, 0};
    Tile mountain(TILE::MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
    all_tiles[TILE::MOUNTAIN] = mountain;
    temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
    temp_stats = {30, 0, 4, 0};
    Tile peak(TILE::PEAK, "Peak", temp_cost, temp_stats, false);
    all_tiles[TILE::PEAK] = peak;    
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    temp_stats = {20, 0, 2, 0};
    Tile cave(TILE::CAVE, "Cave", temp_cost, temp_stats, false);
    all_tiles[TILE::CAVE] = cave;

    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    temp_stats = {0, 0, 0, 0};
    Tile sand(TILE::SAND, "Sand", temp_cost, temp_stats, false);
    all_tiles[TILE::SAND] = sand;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    temp_stats = {0, 0, 0, 0};
    Tile wasteland(TILE::WASTELAND, "Wasteland", temp_cost, temp_stats, false);
    all_tiles[TILE::WASTELAND] = wasteland;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
    temp_stats = {5, 0, 2, 0};
    Tile rock(TILE::ROCK, "Rock", temp_cost, temp_stats, false);
    all_tiles[TILE::ROCK] = rock;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {0, 0, 0, 0};
    Tile bones(TILE::BONES, "Bones", temp_cost, temp_stats, false);
    all_tiles[TILE::BONES] = bones;

    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {0, 0, 0, 0};
    Tile floor(TILE::FLOOR, "Floor", temp_cost, temp_stats, true);
    all_tiles[TILE::FLOOR] = floor;
    temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    temp_stats = {10, 1, 10, 0};
    Tile pillar(TILE::PILLAR, "Pillar", temp_cost, temp_stats, true);
    all_tiles[TILE::PILLAR] = pillar;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {0, 0, 0, 0};
    Tile wall(TILE::WALL, "Wall", temp_cost, temp_stats, true);
    all_tiles[TILE::WALL] = wall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {0, 0, 0, 0};
    Tile door(TILE::DOOR, "Door", temp_cost, temp_stats, true);
    all_tiles[TILE::DOOR] = door;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {20, 2, 15, 0};
    Tile throne(TILE::THRONE, "Throne", temp_cost, temp_stats, true);
    all_tiles[TILE::THRONE] = throne;

    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {10, 1, 10, 0};
    Tile gate(TILE::GATE, "Gate", temp_cost, temp_stats, false);
    all_tiles[TILE::GATE] = gate;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {0, 0, 0, 0};
    Tile fence(TILE::FENCE, "Fence", temp_cost, temp_stats, false);
    all_tiles[TILE::FENCE] = fence;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {10, 1, 10, 0};
    Tile shop(TILE::SHOP, "Shop", temp_cost, temp_stats, false);
    all_tiles[TILE::SHOP] = shop;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {15, 0, 2, 0};
    Tile armory(TILE::ARMORY, "Armory", temp_cost, temp_stats, false);
    all_tiles[TILE::ARMORY] = armory;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    temp_stats = {15, 0, 2, 0};
    Tile village(TILE::VILLAGE, "Village", temp_cost, temp_stats, false);
    all_tiles[TILE::VILLAGE] = village;
    temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
    temp_stats = {10, 2, 10, 0};
    Tile fort(TILE::FORT, "Fort", temp_cost, temp_stats, false);
    all_tiles[TILE::FORT] = fort;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    temp_stats = {0, 0, 0, 0};
    Tile castle(TILE::CASTLE, "Castle", temp_cost, temp_stats, false);
    all_tiles[TILE::CASTLE] = castle;

    temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
    temp_stats = {0, 0, 0, 0};
    Tile snow(TILE::SNOW, "Snow", temp_cost, temp_stats, false);
    all_tiles[TILE::SNOW] = snow;
    temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
    temp_stats = {0, 0, 0, 0};
    Tile glacier(TILE::GLACIER, "Glacier", temp_cost, temp_stats, false);
    all_tiles[TILE::GLACIER] = glacier;
}

std::vector<short int> testTiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap2Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap3Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap4Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap5Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap6Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap7Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap8Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap9Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap10Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap11Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap12Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap13Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap14Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap15Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap16Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap17Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap18Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap19Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap20Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap21Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap22Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap23Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap24Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> chap25Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> gaiden1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return(temp_vector);
}

std::vector<short int> (*chapTiles[40])() = {testTiles, chap1Tiles, chap2Tiles, chap3Tiles, chap4Tiles,
    chap5Tiles, chap6Tiles, chap7Tiles, chap8Tiles, chap9Tiles, chap10Tiles,
    chap11Tiles, chap12Tiles, chap13Tiles, chap14Tiles, chap15Tiles, chap16Tiles,
    chap17Tiles, chap18Tiles, chap19Tiles, chap20Tiles, chap21Tiles, chap22Tiles,
    chap23Tiles, chap24Tiles, chap25Tiles, gaiden1Tiles};
