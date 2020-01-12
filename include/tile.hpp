#ifndef TILE_HPP
#define TILE_HPP
#include "shared.hpp"

class Tile {
    private:
        Movement_cost cost;

        std::string name;

        struct {
            unsigned int hit;
            unsigned int dodge;
            unsigned int def;
            unsigned int heal;
        } stats;

    public:
        Tile();
        ~Tile();
        Tile(std::string in_name, Movement_cost in_cost, unsigned int * in_stats[3]);

};

#endif /* TILE_HPP */