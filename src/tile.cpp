#include "shared.hpp"
#include "tile.hpp"

Tile::~Tile() {
}

Tile::Tile(void) { 
}

Tile::Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3]) {
	name = in_name;
	cost = in_cost;
	stats.dodge = (unsigned int)in_stats[0];
	stats.def = (unsigned int)in_stats[1];
	stats.heal = (unsigned int)in_stats[2];
}

Movement_cost Tile::getCost(){
	return(cost);
}

std::string Tile::getName(){
	return(name);
}

Tilestats Tile::getStats(){
	return(stats);
}



