#ifndef TILE_HPP
#define TILE_HPP

#include "shared.hpp"

class Tile {
    private:
        Movement_cost cost_struct;
        std::unordered_map<std::string, unsigned char> cost_map;
        bool inside;
        Tile_stats stats;
        std::string name;
    public:
        Tile();
        ~Tile();

        Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3]);
        Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3], bool in_inside);

        Movement_cost getCoststruct();
        std::unordered_map<std::string, unsigned char> getCostmap();
        std::string getName();
        Tile_stats getStats();
        bool isInside();
        bool isOutside();
        void setInside(bool in_inside);

        void write(const char * filename, const char * mode = "a");
};

extern std::unordered_map<int, Tile> all_tiles;

void baseTiles();
std::unordered_map<int, Tile> baseTiles(std::vector<int> toload);

extern std::vector<Map_enemy> (*chapTiles[25])();
extern std::vector<Map_enemy> (*gaidenTiles[25])();


#endif /* TILE_HPP */