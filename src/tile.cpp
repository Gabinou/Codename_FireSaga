#include "shared.hpp"
#include "tile.hpp"

Tile::~Tile() {
}

Tile::Tile(void) { 
}

Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3]) {
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
	stats.def = (unsigned int)in_stats[1];
	stats.heal = (unsigned int)in_stats[2];
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

Tilestats Tile::getStats(){
	return(stats);
}



