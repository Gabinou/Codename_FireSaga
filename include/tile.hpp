#ifndef TILE_HPP
#define TILE_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"

class Tile {
    private:
        Movement_cost cost_struct;
        unsigned char cost_array[UNIT::MVT::END];
        short int id;
        bool inside;
        Tile_stats stats;
        std::string name;
    public:
        Tile();
        ~Tile();

        Tile(const short int in_id, const char * in_name, const Movement_cost in_cost, const Tile_stats in_stats, const bool in_inside);

        Movement_cost getCoststruct();
        unsigned char * getCost();
        std::string getName();
        short int getid();
        Tile_stats getStats();

        bool isInside();
        bool isOutside();
        void setInside(const bool in_inside);

        void makeMvtCostarray();

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

void testXMLTiles();
void baseTiles(std::unordered_map<int, Tile> * in_tiles, std::vector<short int> toload);

extern std::vector<Tile> all_tiles;
extern std::vector<Tile> loaded_tiles;
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