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
        Tile(const std::string in_name, const Movement_cost in_cost, unsigned int * in_stats[3], const bool in_inside);

        Movement_cost getCoststruct();
        std::unordered_map<std::string, unsigned char> getCostmap();
        std::string getName();
        Tile_stats getStats();

        bool isInside();
        bool isOutside();
        void setInside(const bool in_inside);

        void write(const char * filename, const char * mode = "a");
};

void baseTiles();
std::unordered_map<short int, Tile> baseTiles(std::vector<short int> toload);

extern std::unordered_map<short int, Tile> all_tiles;
extern std::unordered_map<short int, Tile> loaded_tiles;
extern std::vector<short int> (*chapTiles[40])();

std::vector<short int> testTiles();
std::vector<short int> chap1Tiles();
std::vector<short int> gaiden1Tiles();
std::vector<short int> chap2Tiles();
std::vector<short int> chap3Tiles();
std::vector<short int> chap4Tiles();
std::vector<short int> chap5Tiles();
std::vector<short int> chap6Tiles();
std::vector<short int> chap7Tiles();
std::vector<short int> chap8Tiles();
std::vector<short int> chap9Tiles();
std::vector<short int> chap10Tiles();
std::vector<short int> chap11Tiles();
std::vector<short int> chap12Tiles();
std::vector<short int> chap13Tiles();
std::vector<short int> chap14Tiles();
std::vector<short int> chap15Tiles();
std::vector<short int> chap16Tiles();
std::vector<short int> chap17Tiles();
std::vector<short int> chap18Tiles();
std::vector<short int> chap19Tiles();
std::vector<short int> chap20Tiles();
std::vector<short int> chap21Tiles();
std::vector<short int> chap22Tiles();
std::vector<short int> chap23Tiles();
std::vector<short int> chap24Tiles();
std::vector<short int> chap25Tiles();

#endif /* TILE_HPP */