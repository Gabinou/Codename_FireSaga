#ifndef TILE_HPP
#define TILE_HPP
#include "shared.hpp"

class Tile {
    private:
        Movement_cost cost;

        std::string name;

        struct Stats {
            unsigned int dodge;
            unsigned int def;
            unsigned int heal; // %
        } stats;

    public:
        Tile();
        ~Tile();
        Tile(std::string in_name, Movement_cost in_cost, unsigned int * in_stats[3]);

        Movement_cost getCost();
        std::string getName();
        Stats getStats();

};

extern std::unordered_map<int, Tile> all_tiles;

#endif /* TILE_HPP */