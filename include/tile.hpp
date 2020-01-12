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
            unsigned int heal;
        } stats;

    public:
        Tile();
        ~Tile();

};

#endif /* TILE_HPP */