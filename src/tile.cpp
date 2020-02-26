#include "shared.hpp"
#include "tile.hpp"

Tile::~Tile() {
}

Tile::Tile(void) { 
}

Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[4]) {
	name = in_name;
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
	stats.dodge = (unsigned int)in_stats[0];
	stats.Pprot = (unsigned int)in_stats[1];
    stats.Mprot = (unsigned int)in_stats[2];
	stats.heal = (unsigned int)in_stats[3];
}
Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3], const bool in_inside) : Tile(in_name, in_cost, in_stats) {
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
    printf("Loading certain base tiles\n");
    unsigned int * temp_stats[3];// dodge, def, heal
    std::unordered_map<short int, Tile> temp_tiles;
    Movement_cost temp_cost;
    for (short unsigned int i = 0; i < toload.size(); i++) {
        switch (toload[i]/10){
            case TILE::PLAIN: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile plain("Plain", temp_cost, temp_stats);
                temp_tiles[TILE::PLAIN] = plain;
                break;
            }
            case TILE::BUSH: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)1;
                temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
                Tile bush("Bush", temp_cost, temp_stats);
                temp_tiles[TILE::BUSH] = bush;
                break;
            }
            case TILE::FOREST: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile forest("Forest", temp_cost, temp_stats);
                temp_tiles[TILE::FOREST] = forest;
                break; 
            }
            case TILE::THICKET: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile thicket("Thicket", temp_cost, temp_stats);
                temp_tiles[TILE::THICKET] = thicket;
                break; 
            }
            case TILE::SNAG: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile snag("Snag", temp_cost, temp_stats);
                temp_tiles[TILE::SNAG] = snag;
                break; 
            }
            case TILE::BRIDGE: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile bridge("Bridge", temp_cost, temp_stats);
                temp_tiles[TILE::BRIDGE] = bridge;
                break; 
            }
            case TILE::HOLE: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile hole("Hole", temp_cost, temp_stats);
                temp_tiles[TILE::HOLE] = hole;
                break; 
            }
            case TILE::SEA: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
                Tile sea("Sea", temp_cost, temp_stats);
                temp_tiles[TILE::SEA] = sea;
                break; 
            }
            case TILE::LAKE: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
                Tile lake("Lake", temp_cost, temp_stats);
                temp_tiles[TILE::LAKE] = lake;
                break; 
            }
            case TILE::RIVER: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
                Tile river("River", temp_cost, temp_stats);
                temp_tiles[TILE::RIVER] = river;
                break; 
            }
            case TILE::WATERFALL: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile waterfall("Waterfall", temp_cost, temp_stats);
                temp_tiles[TILE::WATERFALL] = waterfall;
                break; 
            }
            case TILE::CLIFF: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile cliff("cliff", temp_cost, temp_stats);
                temp_tiles[TILE::CLIFF] = cliff;
                break; 
            }
            case TILE::HILL: {
                temp_stats[0] = (unsigned int *)15;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
                Tile hill("hill", temp_cost, temp_stats);
                temp_tiles[TILE::HILL] = hill;
                break; 
            }
            case TILE::MOUNTAIN: {
                temp_stats[0] = (unsigned int *)25;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)3;
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                Tile mountain("mountain", temp_cost, temp_stats);
                temp_tiles[TILE::MOUNTAIN] = mountain;
                break;
            }
            case TILE::PEAK: {
                temp_stats[0] = (unsigned int *)30;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)4;
                temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
                Tile peak("Peak", temp_cost, temp_stats);
                temp_tiles[TILE::PEAK] = peak;    
                break; 
            }
            case TILE::CAVE: {
                temp_stats[0] = (unsigned int *)20;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                Tile cave("Cave", temp_cost, temp_stats);
                temp_tiles[TILE::CAVE] = cave;
                break; 
            }
            case TILE::SAND: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                Tile sand("Sand", temp_cost, temp_stats);
                temp_tiles[TILE::SAND] = sand;
                break; 
            }
            case TILE::WASTELAND: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                Tile wasteland("Wasteland", temp_cost, temp_stats);
                temp_tiles[TILE::WASTELAND] = wasteland;
                break; 
            }
            case TILE::ROCK: {
                temp_stats[0] = (unsigned int *)5;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
                Tile rock("Rock", temp_cost, temp_stats);
                temp_tiles[TILE::ROCK] = rock;
                break; 
            }
            case TILE::BONES: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile bones("Bones", temp_cost, temp_stats);
                temp_tiles[TILE::BONES] = bones;
                break; 
            }
            case TILE::FLOOR: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile floor("Floor", temp_cost, temp_stats);
                temp_tiles[TILE::FLOOR] = floor;
                break;
                }
            case TILE::PILLAR: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
                Tile pillar("Pillar", temp_cost, temp_stats);
                temp_tiles[TILE::PILLAR] = pillar;
                break; 
            }
            case TILE::WALL: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile wall("Wall", temp_cost, temp_stats);
                temp_tiles[TILE::WALL] = wall;
                break; 
            }
            case TILE::DOOR: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile door("Door", temp_cost, temp_stats);
                temp_tiles[TILE::DOOR] = door;
                break; 
            }
            case TILE::THRONE: {
                temp_stats[0] = (unsigned int *)20;
                temp_stats[1] = (unsigned int *)2;
                temp_stats[2] = (unsigned int *)15;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile throne("Throne", temp_cost, temp_stats);
                temp_tiles[TILE::THRONE] = throne;
                break; 
            }
            case TILE::GATE: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile gate("Gate", temp_cost, temp_stats);
                temp_tiles[TILE::GATE] = gate;
                break; 
            }
            case TILE::FENCE: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile fence("Fence", temp_cost, temp_stats);
                temp_tiles[TILE::FENCE] = fence;
                break; 
            }
            case TILE::SHOP: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile shop("Shop", temp_cost, temp_stats);
                temp_tiles[TILE::SHOP] = shop;
                break; 
            }
            case TILE::ARMORY: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile armory("Armory", temp_cost, temp_stats);
                temp_tiles[TILE::ARMORY] = armory;
                break; 
            }
            case TILE::VILLAGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile village("Village", temp_cost, temp_stats);
                temp_tiles[TILE::VILLAGE] = village;
                break; 
            }
            case TILE::FORT: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)2;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
                Tile fort("Fort", temp_cost, temp_stats);
                temp_tiles[TILE::FORT] = fort;
                break; 
            }
            case TILE::CASTLE: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile castle("Castle", temp_cost, temp_stats);
                temp_tiles[TILE::CASTLE] = castle;
                break; 
            }
            case TILE::SNOW: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
                Tile snow("Snow", temp_cost, temp_stats);
                temp_tiles[TILE::SNOW] = snow;
                break; 
            }            
            case TILE::GLACIER: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
                Tile glacier("Glacier", temp_cost, temp_stats);
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
	printf("Loading all base tiles\n");
	unsigned int * temp_stats[3] = {0, 0, 0};// dodge, def, heal
    // Fliers always have 1 movement because they dismount inside.
    
    // Mvt_types: foot_slow, foot_fast, mages, riders_slow, riders_fast, fliers, armors, pirates, bandits;
    Movement_cost temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile plain("Plain", temp_cost, temp_stats);
    all_tiles[TILE::PLAIN] = plain;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)1;
    temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
    Tile bush("Bush", temp_cost, temp_stats);
    all_tiles[TILE::BUSH] = bush;
    temp_stats[0] = (unsigned int *)15;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
    Tile forest("Forest", temp_cost, temp_stats);
    all_tiles[TILE::FOREST] = forest;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile thicket("Thicket", temp_cost, temp_stats);
    all_tiles[TILE::THICKET] = thicket;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile snag("Snag", temp_cost, temp_stats);
    all_tiles[TILE::SNAG] = snag;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile bridge("Bridge", temp_cost, temp_stats);
    all_tiles[TILE::BRIDGE] = bridge;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile hole("Hole", temp_cost, temp_stats);
    all_tiles[TILE::HOLE] = hole;

    temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
    Tile sea("Sea", temp_cost, temp_stats);
    all_tiles[TILE::SEA] = sea;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
    Tile lake("Lake", temp_cost, temp_stats);
    all_tiles[TILE::LAKE] = lake;
    temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
    Tile river("River", temp_cost, temp_stats);
    all_tiles[TILE::RIVER] = river;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile waterfall("Waterfall", temp_cost, temp_stats);
    all_tiles[TILE::WATERFALL] = waterfall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile cliff("cliff", temp_cost, temp_stats);
    all_tiles[TILE::CLIFF] = cliff;

    temp_stats[0] = (unsigned int *)15;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
    Tile hill("hill", temp_cost, temp_stats);
    all_tiles[TILE::HILL] = hill;
    temp_stats[0] = (unsigned int *)25;
    temp_stats[2] = (unsigned int *)3;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    Tile mountain("mountain", temp_cost, temp_stats);
    all_tiles[TILE::MOUNTAIN] = mountain;
    temp_stats[0] = (unsigned int *)30;
    temp_stats[2] = (unsigned int *)4;
    temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
    Tile peak("Peak", temp_cost, temp_stats);
    all_tiles[TILE::PEAK] = peak;    
    temp_stats[0] = (unsigned int *)20;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    Tile cave("Cave", temp_cost, temp_stats);
    all_tiles[TILE::CAVE] = cave;

    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile sand("Sand", temp_cost, temp_stats);
    all_tiles[TILE::SAND] = sand;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile wasteland("Wasteland", temp_cost, temp_stats);
    all_tiles[TILE::WASTELAND] = wasteland;
    temp_stats[0] = (unsigned int *)5;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
    Tile rock("Rock", temp_cost, temp_stats);
    all_tiles[TILE::ROCK] = rock;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile bones("Bones", temp_cost, temp_stats);
    all_tiles[TILE::BONES] = bones;

    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile floor("Floor", temp_cost, temp_stats);
    all_tiles[TILE::FLOOR] = floor;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    Tile pillar("Pillar", temp_cost, temp_stats);
    all_tiles[TILE::PILLAR] = pillar;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile wall("Wall", temp_cost, temp_stats);
    all_tiles[TILE::WALL] = wall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile door("Door", temp_cost, temp_stats);
    all_tiles[TILE::DOOR] = door;
    temp_stats[0] = (unsigned int *)20;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)15;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile throne("Throne", temp_cost, temp_stats);
    all_tiles[TILE::THRONE] = throne;

    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile gate("Gate", temp_cost, temp_stats);
    all_tiles[TILE::GATE] = gate;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile fence("Fence", temp_cost, temp_stats);
    all_tiles[TILE::FENCE] = fence;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile shop("Shop", temp_cost, temp_stats);
    all_tiles[TILE::SHOP] = shop;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile armory("Armory", temp_cost, temp_stats);
    all_tiles[TILE::ARMORY] = armory;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile village("Village", temp_cost, temp_stats);
    all_tiles[TILE::VILLAGE] = village;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
    Tile fort("Fort", temp_cost, temp_stats);
    all_tiles[TILE::FORT] = fort;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile castle("Castle", temp_cost, temp_stats);
    all_tiles[TILE::CASTLE] = castle;

    temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
    Tile snow("Snow", temp_cost, temp_stats);
    all_tiles[TILE::SNOW] = snow;
    temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
    Tile glacier("Glacier", temp_cost, temp_stats);
    all_tiles[TILE::GLACIER] = glacier;
}

std::vector<short int> testTiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap2Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap3Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap4Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap5Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap6Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap7Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap8Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap9Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap10Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap11Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap12Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap13Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap14Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap15Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap16Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap17Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap18Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap19Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap20Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap21Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap22Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap23Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap24Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> chap25Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> gaiden1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * 10 + 0, TILE::FOREST * 10 + 0, TILE::SEA * 10 + 0, TILE::HILL * 10 + 0};
    return(temp_vector);
}

std::vector<short int> (*chapTiles[40])() = {testTiles, chap2Tiles, chap3Tiles, chap4Tiles,
    chap5Tiles, chap6Tiles, chap7Tiles, chap8Tiles, chap9Tiles, chap10Tiles,
    chap11Tiles, chap12Tiles, chap13Tiles, chap14Tiles, chap15Tiles, chap16Tiles,
    chap17Tiles, chap18Tiles, chap19Tiles, chap20Tiles, chap21Tiles, chap22Tiles,
    chap23Tiles, chap24Tiles, chap25Tiles, gaiden1Tiles};
