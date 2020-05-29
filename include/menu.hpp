#ifndef MENU_HPP
#define MENU_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "unit.hpp"
#include "events.hpp"
#include "physfs.h"
#include "filesystem.hpp"
#include "entityx/entityx.h"

class Clickable {
private:
    // Define rectangular clickable area using two points:
    Point topleft;
    Point bottomright;
    short int eventID = -1;
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Unit> unit;
public:
    void click(entityx::Entity mouse);
};

// class MENU : public JSON_IO {
// private:
//     // Movement_cost cost_struct;
//     // unsigned char cost_array[UNIT::MVT::END];
//     // short int id;
//     // bool inside;
//     // Tile_stats stats;
//     // std::string name;
// public:
//     // Tile();
//     // ~Tile();

//     // Tile(const short int in_id, const char * in_name, const Movement_cost in_cost, const Tile_stats in_stats, const bool in_inside);

//     // Movement_cost getCoststruct();
//     // unsigned char * getCost();
//     // std::string getName();
//     // short int getid();
//     // Tile_stats getStats();

//     // bool isInside();
//     // bool isOutside();
//     // void setInside(const bool in_inside);

//     // void makeMvtCostarray();

//     // using JSON_IO::writeJSON;
//     // using JSON_IO::readJSON;
//     // void readJSON(cJSON * in_json);
//     // void writeJSON(cJSON * in_json);

//     // using XML_IO::writeXML;
//     // using XML_IO::readXML;
//     // void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile);
//     // void readXML(tinyxml2::XMLElement * in_pTile);
// };

#endif /* MENU_HPP */