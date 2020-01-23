#ifndef TILE_HPP
#define TILE_HPP

#include "shared.hpp"

struct Tilestats {
    unsigned int dodge;
    unsigned int def;
    unsigned int heal; // %
};

class Tile {
    private:
        Movement_cost cost_struct;
        std::unordered_map<std::string, unsigned char> cost_map;
        Tilestats stats;
        std::string name;
    public:
        Tile();
        ~Tile();

        Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3]);

        Movement_cost getCoststruct();
        std::unordered_map<std::string, unsigned char> getCostmap();
        std::string getName();
        Tilestats getStats();
};

extern std::unordered_map<int, Tile> all_tiles;

void baseTiles();


#endif /* TILE_HPP */