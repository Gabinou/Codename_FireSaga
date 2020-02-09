#include "shared.hpp"
#include "tile.hpp"

Tile::~Tile() {
}

Tile::Tile(void) { 
}

Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[4]) {
	name = in_name;
	cost_struct = in_cost;
    cost_map["foot_slow"] = in_cost.foot_slow;
    cost_map["foot_fast"] = in_cost.foot_fast;
    cost_map["mages"] = in_cost.mages;
    cost_map["riders_slow"] = in_cost.riders_slow;
    cost_map["riders_fast"] = in_cost.riders_fast;
    cost_map["fliers"] = in_cost.fliers;
    cost_map["armors"] = in_cost.armors;
    cost_map["pirates"] = in_cost.pirates;
    cost_map["bandits"] = in_cost.bandits;
	stats.dodge = (unsigned int)in_stats[0];
	stats.Pprot = (unsigned int)in_stats[1];
    stats.Mprot = (unsigned int)in_stats[2];
	stats.heal = (unsigned int)in_stats[3];
}
Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3], bool in_inside) : Tile(in_name, in_cost, in_stats){
    inside = in_inside;
}

std::unordered_map<std::string, unsigned char> Tile::getCostmap(){
	return(cost_map);
}

Movement_cost Tile::getCoststruct(){
	return(cost_struct);
}

std::string Tile::getName(){
	return(name);
}

Tile_stats Tile::getStats(){
	return(stats);
}

bool Tile::isInside(){
    return(inside);
}

bool Tile::isOutside(){
    return(!inside);
}

void Tile::setInside(bool in_inside){
    inside = in_inside;
}

void Tile::write(const char * filename, const char * mode){
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

std::unordered_map<int, Tile> all_tiles;

std::unordered_map<int, Tile> baseTiles(std::vector<int> toload) {
    printf("Loading certain base tiles\n");
    unsigned int * temp_stats[3];// dodge, def, heal
    std::unordered_map<int, Tile> temp_tiles;
    Movement_cost temp_cost;
    for (int i = 0; i < toload.size(); i++) {
        switch (toload[i]){
            case 10 : {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile plain("Plain", temp_cost, temp_stats);
                temp_tiles[10] = plain;
                break;
            }
            case 11: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)1;
                temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
                Tile bush("Bush", temp_cost, temp_stats);
                temp_tiles[11] = bush;
                break;
            }
            case 12: {
                Tile forest("Forest", temp_cost, temp_stats);
                temp_tiles[12] = forest;
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                break; 
            }
            case 13: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile thicket("Thicket", temp_cost, temp_stats);
                temp_tiles[13] = thicket;
                break; 
            }
            case 14: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile snag("Snag", temp_cost, temp_stats);
                temp_tiles[14] = snag;
                break; 
            }
            case 15: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile bridge("Bridge", temp_cost, temp_stats);
                temp_tiles[15] = bridge;
                break; 
            }
            case 16: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile hole("Hole", temp_cost, temp_stats);
                temp_tiles[16] = hole;
                break; 
            }
            case 20: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
                Tile sea("Sea", temp_cost, temp_stats);
                temp_tiles[20] = sea;
                break; 
            }
            case 21: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
                Tile lake("Lake", temp_cost, temp_stats);
                temp_tiles[21] = lake;
                break; 
            }
            case 22: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
                Tile river("River", temp_cost, temp_stats);
                temp_tiles[22] = river;
                break; 
            }
            case 23: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile waterfall("Waterfall", temp_cost, temp_stats);
                temp_tiles[23] = waterfall;
                break; 
            }
            case 24: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile cliff("cliff", temp_cost, temp_stats);
                temp_tiles[24] = cliff;
                break; 
            }
            case 30: {
                temp_stats[0] = (unsigned int *)15;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
                Tile hill("hill", temp_cost, temp_stats);
                temp_tiles[30] = hill;
                break; 
            }
            case 31: {
                temp_stats[0] = (unsigned int *)25;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)3;
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                Tile mountain("mountain", temp_cost, temp_stats);
                temp_tiles[31] = mountain;
                break;
            }
            case 32: {
                temp_stats[0] = (unsigned int *)30;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)4;
                temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
                Tile peak("Peak", temp_cost, temp_stats);
                temp_tiles[32] = peak;    
                break; 
            }
            case 33: {
                temp_stats[0] = (unsigned int *)20;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                Tile cave("Cave", temp_cost, temp_stats);
                temp_tiles[33] = cave;
                break; 
            }
            case 40: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                Tile sand("Sand", temp_cost, temp_stats);
                temp_tiles[40] = sand;
                break; 
            }
            case 41: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                Tile wasteland("Wasteland", temp_cost, temp_stats);
                temp_tiles[41] = wasteland;
                break; 
            }
            case 42: {
                temp_stats[0] = (unsigned int *)5;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)2;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
                Tile rock("Rock", temp_cost, temp_stats);
                temp_tiles[42] = rock;
                break; 
            }
            case 43: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile bones("Bones", temp_cost, temp_stats);
                temp_tiles[43] = bones;
                break; 
            }
            case 50: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile floor("Floor", temp_cost, temp_stats);
                temp_tiles[50] = floor;
                break;
                }
            case 51: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
                Tile pillar("Pillar", temp_cost, temp_stats);
                temp_tiles[51] = pillar;
                break; 
            }
            case 52: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile wall("Wall", temp_cost, temp_stats);
                temp_tiles[52] = wall;
                break; 
            }
            case 53: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile door("Door", temp_cost, temp_stats);
                temp_tiles[53] = door;
                break; 
            }
            case 54: {
                temp_stats[0] = (unsigned int *)20;
                temp_stats[1] = (unsigned int *)2;
                temp_stats[2] = (unsigned int *)15;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile throne("Throne", temp_cost, temp_stats);
                temp_tiles[54] = throne;
                break; 
            }
            case 60: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile gate("Gate", temp_cost, temp_stats);
                temp_tiles[60] = gate;
                break; 
            }
            case 61: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile fence("Fence", temp_cost, temp_stats);
                temp_tiles[61] = fence;
                break; 
            }
            case 62: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)1;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile shop("Shop", temp_cost, temp_stats);
                temp_tiles[62] = shop;
                break; 
            }
            case 63: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile armory("Armory", temp_cost, temp_stats);
                temp_tiles[63] = armory;
                break; 
            }
            case 64: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                Tile village("Village", temp_cost, temp_stats);
                temp_tiles[64] = village;
                break; 
            }
            case 65: {
                temp_stats[0] = (unsigned int *)10;
                temp_stats[1] = (unsigned int *)2;
                temp_stats[2] = (unsigned int *)10;
                temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
                Tile fort("Fort", temp_cost, temp_stats);
                temp_tiles[65] = fort;
                break; 
            }
            case 66: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                Tile castle("Castle", temp_cost, temp_stats);
                temp_tiles[66] = castle;
                break; 
            }
            case 70: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
                Tile snow("Snow", temp_cost, temp_stats);
                temp_tiles[70] = snow;
                break; 
            }            
            case 71: {
                temp_stats[0] = (unsigned int *)0;
                temp_stats[1] = (unsigned int *)0;
                temp_stats[2] = (unsigned int *)0;
                temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
                Tile glacier("Glacier", temp_cost, temp_stats);
                temp_tiles[71] = glacier;
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
    all_tiles[10] = plain;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)1;
    temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
    Tile bush("Bush", temp_cost, temp_stats);
    all_tiles[11] = bush;
    temp_stats[0] = (unsigned int *)15;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
    Tile forest("Forest", temp_cost, temp_stats);
    all_tiles[12] = forest;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile thicket("Thicket", temp_cost, temp_stats);
    all_tiles[13] = thicket;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile snag("Snag", temp_cost, temp_stats);
    all_tiles[14] = snag;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile bridge("Bridge", temp_cost, temp_stats);
    all_tiles[15] = bridge;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile hole("Hole", temp_cost, temp_stats);
    all_tiles[16] = hole;

    temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
    Tile sea("Sea", temp_cost, temp_stats);
    all_tiles[20] = sea;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
    Tile lake("Lake", temp_cost, temp_stats);
    all_tiles[21] = lake;
    temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
    Tile river("River", temp_cost, temp_stats);
    all_tiles[22] = river;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile waterfall("Waterfall", temp_cost, temp_stats);
    all_tiles[23] = waterfall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile cliff("cliff", temp_cost, temp_stats);
    all_tiles[24] = cliff;

    temp_stats[0] = (unsigned int *)15;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
    Tile hill("hill", temp_cost, temp_stats);
    all_tiles[30] = hill;
    temp_stats[0] = (unsigned int *)25;
    temp_stats[2] = (unsigned int *)3;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    Tile mountain("mountain", temp_cost, temp_stats);
    all_tiles[31] = mountain;
    temp_stats[0] = (unsigned int *)30;
    temp_stats[2] = (unsigned int *)4;
    temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
    Tile peak("Peak", temp_cost, temp_stats);
    all_tiles[32] = peak;    
    temp_stats[0] = (unsigned int *)20;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    Tile cave("Cave", temp_cost, temp_stats);
    all_tiles[33] = cave;

    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile sand("Sand", temp_cost, temp_stats);
    all_tiles[40] = sand;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile wasteland("Wasteland", temp_cost, temp_stats);
    all_tiles[41] = wasteland;
    temp_stats[0] = (unsigned int *)5;
    temp_stats[2] = (unsigned int *)2;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
    Tile rock("Rock", temp_cost, temp_stats);
    all_tiles[42] = rock;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile bones("Bones", temp_cost, temp_stats);
    all_tiles[43] = bones;

    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile floor("Floor", temp_cost, temp_stats);
    all_tiles[50] = floor;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    Tile pillar("Pillar", temp_cost, temp_stats);
    all_tiles[51] = pillar;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile wall("Wall", temp_cost, temp_stats);
    all_tiles[52] = wall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile door("Door", temp_cost, temp_stats);
    all_tiles[53] = door;
    temp_stats[0] = (unsigned int *)20;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)15;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile throne("Throne", temp_cost, temp_stats);
    all_tiles[54] = throne;

    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile gate("Gate", temp_cost, temp_stats);
    all_tiles[60] = gate;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile fence("Fence", temp_cost, temp_stats);
    all_tiles[61] = fence;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile shop("Shop", temp_cost, temp_stats);
    all_tiles[62] = shop;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile armory("Armory", temp_cost, temp_stats);
    all_tiles[63] = armory;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile village("Village", temp_cost, temp_stats);
    all_tiles[64] = village;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
    Tile fort("Fort", temp_cost, temp_stats);
    all_tiles[65] = fort;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile castle("Castle", temp_cost, temp_stats);
    all_tiles[66] = castle;

    temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
    Tile snow("Snow", temp_cost, temp_stats);
    all_tiles[70] = snow;
    temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
    Tile glacier("Glacier", temp_cost, temp_stats);
    all_tiles[71] = glacier;
}

std::vector<int> testTiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    chapTiles[0] = testTiles;
    return(temp_vector);
}

std::vector<int> chap1Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap2Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap3Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap4Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap5Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap6Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap7Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap8Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap9Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap10Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap11Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap12Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap13Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap14Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap15Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap16Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap17Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap18Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap19Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap20Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap21Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap22Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap23Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap24Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> chap25Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}

std::vector<int> gaiden1Tiles(){
    std::vector<int> temp_vector = {10, 12, 20, 30};
    return(temp_vector);
}
std::vector<int> (*chapTiles[25])() = {NULL, chap2Tiles, chap3Tiles, chap4Tiles,
    chap5Tiles, chap6Tiles, chap7Tiles, chap8Tiles, chap9Tiles, chap10Tiles,
    chap11Tiles, chap12Tiles, chap13Tiles, chap14Tiles, chap15Tiles, chap16Tiles,
    chap17Tiles, chap18Tiles, chap19Tiles, chap20Tiles, chap21Tiles, chap22Tiles,
    chap23Tiles, chap24Tiles, chap25Tiles,};
std::vector<int> (*gaidenTiles[25])() = {NULL, gaiden1Tiles} ;
