// Code créé par Gabriel Taillon
/*! \page handle tile.cpp
* \file tile.cpp
* \brief Tile object.
*/
#include "shared.hpp"
#include "tile.hpp"

Tile::~Tile() {
    // printf("tile %s is being deleted.\n" , name);
}

Tile::Tile(void) {
    // printf("tile %s is being deleted.\n" , name);
}
Tile::Tile(std::string in_name, Movement_cost in_cost, unsigned int* in_stats[3]) {
	name = in_name;
	cost = in_cost;
	stats.hit = *in_stats[0];
	stats.dodge = *in_stats[1];
	stats.def = *in_stats[2];
	stats.heal = *in_stats[3];

}