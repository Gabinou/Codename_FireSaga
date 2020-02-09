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
extern std::unordered_map<int, Tile> loaded_tiles;

void baseTiles();
void makechapTiles();
std::unordered_map<int, Tile> baseTiles(std::vector<int> toload);

std::vector<int> testTiles();
std::vector<int> chap1Tiles();
std::vector<int> gaiden1Tiles();
std::vector<int> chap2Tiles();
std::vector<int> chap3Tiles();
std::vector<int> chap4Tiles();
std::vector<int> chap5Tiles();
std::vector<int> chap6Tiles();
std::vector<int> chap7Tiles();
std::vector<int> chap8Tiles();
std::vector<int> chap9Tiles();
std::vector<int> chap10Tiles();
std::vector<int> chap11Tiles();
std::vector<int> chap12Tiles();
std::vector<int> chap13Tiles();
std::vector<int> chap14Tiles();
std::vector<int> chap15Tiles();
std::vector<int> chap16Tiles();
std::vector<int> chap17Tiles();
std::vector<int> chap18Tiles();
std::vector<int> chap19Tiles();
std::vector<int> chap20Tiles();
std::vector<int> chap21Tiles();
std::vector<int> chap22Tiles();
std::vector<int> chap23Tiles();
std::vector<int> chap24Tiles();
std::vector<int> chap25Tiles();


extern std::vector<int> (*chapTiles[25])();
extern std::vector<int> (*gaidenTiles[25])();

#endif /* TILE_HPP */