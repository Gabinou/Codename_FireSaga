#ifndef TILE_HPP
#define TILE_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"

class Tile : public XML_IO, JSON_IO {
private:
    Movement_cost cost_struct;
    uint8_t cost_array[UNIT::MVT::END];
    int16_t id;
    bool inside;
    Tile_stats stats;
    std::string name;
public:
    Tile();
    ~Tile();

    Tile(const int16_t in_id, const char * in_name, const Movement_cost in_cost, const Tile_stats in_stats, const bool in_inside);

    Movement_cost getCoststruct();
    uint8_t * getCost();
    std::string getName();
    int16_t getid();
    Tile_stats getStats();

    bool isInside();
    bool isOutside();
    void setInside(const bool in_inside);

    void makeMvtCostarray();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile);
    void readXML(tinyxml2::XMLElement * in_pTile);
};

#endif /* TILE_HPP */