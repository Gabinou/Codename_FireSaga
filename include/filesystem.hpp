
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>
#include <string>
#include <cstring>
#include <cstring>
#include <stdlib.h>
#include <unordered_map>
#include "physfs.h"
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
extern void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pEquipment, Inventory_item * in_equipment, int size);
extern void writeXML_items(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItems, std::vector<Inventory_item> in_items);
extern void readXML_arrival(tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival);
extern void writeXML_arrival(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pArrival, Map_arrival * in_arrival);

extern void printXMLDoc(PHYSFS_file * in_fp, tinyxml2::XMLDocument * in_doc);
extern int parseXML(const char * filename, tinyxml2::XMLDocument * in_doc);

class XML_IO {
    private:
        std::string xmlElement = "";
    public:
        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
        virtual void readXML(tinyxml2::XMLElement * in_pEle);
        virtual void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pEle);
        void setXMLElement(std::string in_xmlElement);
        std::string getXMLElement();
};

#endif /* FILESYSTEM_HPP */