
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>
#include <string>
#include <cstring>
#include <cstring>
#include <stdlib.h>
#include <unordered_map>
#include "physfs.h"
#include "cjson.h"
#include "LodePNG.h"
#include "structs.hpp"
#include "enums.hpp"
#include "tinyxml2.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"

#define ASSET_LIB "assets.binou"
#define LOGFILE "log.txt"

namespace FILESYSTEM {
int init(char * argvZero, char * baseDir, char * assetsPath);
void loadFileToMemory(const char * name, unsigned char ** mem,
                      size_t * len, bool addnull = false);
void log(void * userdata, int category, SDL_LogPriority priority, const char * message);
}
char * getlineFS();
extern SDL_Texture * loadTexture(SDL_Renderer * in_renderer, const char * filename, const bool ZIP = false);
extern SDL_Texture * textToTexture(SDL_Renderer * in_renderer, std::string textureText, SDL_Color textColor, TTF_Font * in_font);
extern void writeText(int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font, SDL_Renderer * in_renderer);

extern void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats);
extern void readXML_stats(tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats);
extern void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats);
extern void readXML_stats(tinyxml2::XMLElement * in_pStats, Weapon_stats * in_stats);
extern void writeXML_stats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Tile_stats * in_stats);
extern void readXML_stats(tinyxml2::XMLElement * in_pStats, Tile_stats * in_stats);
extern void writeXML_mvtcost(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pCost, Movement_cost * in_cost);
extern void readXML_mvtcost(tinyxml2::XMLElement * in_pCost, Movement_cost * in_cost);
extern void writeXML_narrative(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pNarrative, Narrative * in_state);
extern void readXML_narrative(tinyxml2::XMLElement * in_pNarrative, Narrative * in_state);
extern void readXML_items(tinyxml2::XMLElement * in_pEquipment, Inventory_item * equipment);
extern void readXML_items(tinyxml2::XMLElement * in_pEquipment, std::vector<Inventory_item> * equipment);
extern void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pEquipment, Inventory_item * in_equipment, int size);
extern void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItems, std::vector<Inventory_item> in_items);
extern void readXML_arrival(tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival);
extern void writeXML_arrival(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival);
extern void printXMLDoc(PHYSFS_file * in_fp, tinyxml2::XMLDocument * in_doc);
extern int parseXML(const char * filename, tinyxml2::XMLDocument * in_doc);

extern void readJSON_mvtcost(cJSON * in_jcost, Movement_cost * in_cost);
extern void readJSON_stats(cJSON * in_jstats, Tile_stats * in_stats);
extern void readJSON_stats(cJSON * in_jstats, Unit_stats * in_stats);
extern void readJSON_stats(cJSON * in_jstats, Weapon_stats * in_stats);
extern void readJSON_item(cJSON * in_jitem, Inventory_item * in_item);
extern std::vector<std::vector<short int>> readJSON_tilemap(cJSON * in_jtilemap);
extern void readJSON_narrative(cJSON * in_jcost, Narrative * in_state);
extern void writeJSON_narrative(cJSON * in_jcost, Narrative * in_state);
extern void writeJSON_arrival(cJSON * in_jitems, Map_arrival * in_arrival);
extern void writeJSON_tilemap(cJSON * in_jtilemap, std::vector<std::vector<short int>> in_tilemap);
extern void writeJSON_items(cJSON * in_jitems, std::vector<Inventory_item> in_items);
extern void writeJSON_items(cJSON * in_jitems, Inventory_item * in_equipment, int size);
extern void writeJSON_stats(cJSON * in_jstats, Weapon_stats * in_stats);
extern void writeJSON_stats(cJSON * in_jstats, Unit_stats * in_stats);
extern void writeJSON_stats(cJSON * in_jstats, Tile_stats * in_stats);
extern void writeJSON_mvtcost(cJSON * in_jcost, Movement_cost * in_cost);
extern void printJSON(PHYSFS_file * in_fp, cJSON * in_json);
extern cJSON * parseJSON(const char * filename);

extern const char * fextension(const char * filename);
extern void  path_removefolder(char * path);
extern char * path_topfolder(char * path);

class XML_IO {
private:
    std::string xmlElement = "";
public:
    void writeXML(const char * filename, const bool append = false);
    void readXML(const char * filename);
    virtual void readXML(tinyxml2::XMLElement * in_pEle);
    virtual void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pEle);
    void setXMLElement(std::string in_xmlElement);
    std::string getXMLElement();
};

class JSON_IO {
private:

public:
    virtual void writeJSON(cJSON * in_json);
    virtual void readJSON(cJSON * in_json);
    void writeJSON(const char * filename, const bool append = false);
    void readJSON(const char * filename);
};

#endif /* FILESYSTEM_HPP */