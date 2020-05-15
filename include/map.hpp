#ifndef MAP_HPP
#define MAP_HPP

#include "enums.hpp"
#include "structs.hpp"
#include <entityx/entityx.h>
#include "SDL2/SDL.h"
#include "utilities.hpp"
#include "filesystem.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include <unordered_map>

class Map : public XML_IO {
private:
    entityx::EntityManager * manager;

    SDL_Rect srcrect, destrect;
    SDL_Renderer * renderer = NULL;
    short unsigned int frames = 10, speed = 50;
    short unsigned int tilesize[2];

    std::unordered_map<short int, SDL_Texture *> textures;
    std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
    std::vector<SDL_Texture *> dangers{NULL, NULL};

    std::vector<std::vector<short int>> moveoverlay, attackoverlay, healoverlay;
    bool show_overlay = false;
    bool show_danger = false;
    bool show_grid = false;
    unsigned char overlay_mode = 0;
    std::vector<std::vector<short int>> dangeroverlay;
    unsigned char danger_mode = 0;

    std::vector<std::vector<short int>> tilemap;
    std::unordered_map<int, Tile> tiles;
    std::vector<short int> tilesindex;
    std::vector<std::string> tilenames;

    short int bounds[4] = {0, 255, 0, 255}; //rowmin, rowmax, colmin, colmax
    short int offset[2] = {DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET};
    unsigned char num_friendlies = 0;
    unsigned char num_neutral = 0;
    unsigned char num_enemies = 0;
    unsigned char turn = 0; // Automatic loss if turn 255.
    unsigned char chapter = 0;

    std::vector<Map_arrival> map_arrivals;
    std::vector<Map_arrival> loaded_map_arrivals;
    std::vector<std::vector<Inventory_item>> arrival_equipments;

    std::vector<Point> starting_positions;
    std::vector<unsigned char> armies_onfield;
    std::unordered_map<unsigned char, std::vector<entityx::ComponentHandle<Unit>>> units_onfield;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;
    std::vector<unsigned short int> essentials = {UNIT::NAME::ERWIN};
    unsigned short int boss;
    bool bossdied;
    bool seized; // maybe unecessary if turn system.

public:
    Map();
    ~Map();
    Map(const short unsigned int width, const short unsigned int height);

    void loadTiletextures();
    void loadOverlays();
    void loadDanger();

    void loadTiles(const int in_map_index);
    void loadTiles(std::vector<short int> to_load);
    void unloadTiles(std::vector<short int> to_unload);
    std::unordered_map<int, Tile> getTiles();
    std::vector<short int> getTilesindex();

    Point pixel2tile(Point in_point);
    Point pixel2tile(short int pixel_x, short int pixel_y);

    void loadTilemap(const std::string filename);
    void loadTilemap(const short unsigned int in_map_index);
    void postTilemap();
    std::vector<std::vector<short int>> getTilemap();
    void setTilemap(const std::vector<std::vector<short int>> in_tilemap);
    void setTilesize(const short int unsigned width, const short int unsigned height);
    short unsigned int * getTilesize() const;

    std::vector<std::vector<short int>> makeMvtCostmap(entityx::ComponentHandle<Unit> in_unit);

    void setManager(entityx::EntityManager * in_manager);
    void setRenderer(SDL_Renderer * in_renderer);
    void draw();

    std::vector<std::vector<entityx::ComponentHandle<Unit>>> getUnitmap();
    void clearUnitmap();


    void addArmy(unsigned char in_army);
    std::vector<unsigned char> getArmies();
    void putUnit(const short unsigned int x, const short unsigned int y, entityx::ComponentHandle<Unit> in_unit);
    std::vector<entityx::ComponentHandle<Unit>> getUnits(unsigned char in_army);
    entityx::ComponentHandle<Unit> getUnit(const short unsigned int x, const short unsigned int y);
    void moveUnit(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y);

    void setOverlay(const unsigned char in_mode, std::vector<std::vector<short int>> in_map);
    void clearOverlays();

    unsigned char getTurn();

    void setOverlaymode(const unsigned char in_mode);
    void showOverlay();
    void hideOverlay();

    void setOffset(short int in_offset[2]);
    void setOffset(short int xoffset, short int yoffset);
    short int * getOffset();

    void loadGrid();
    void showGrid();
    void hideGrid();

    void setArrivalEquipments(const std::vector<std::vector<Inventory_item>> in_arrival_equipments);
    std::vector<std::vector<Inventory_item>> getArrivalEquipments();

    void setArrivals(const std::vector<Map_arrival> in_arrivals);
    void addArrival(const Map_arrival in_arrival);
    void removeMapArrival(const unsigned char index);
    std::vector<Map_arrival> getArrivals();
    unsigned char getnumArrivals();

    void setDangermode(const unsigned char in_mode);
    void showDanger();
    void hideDanger();
    void switchDanger();
    bool isDanger();
    void addDanger(const std::vector<std::vector<short int>> in_danger);
    void subDanger(const std::vector<std::vector<short int>> in_danger);
    void setDanger(const std::vector<std::vector<short int>> in_danger);

    unsigned short int getBoss();
    bool getBossDeath();
    bool getSeized();
    std::vector<unsigned short int> getEssentials();

    short int * getBounds();

    std::vector<bool (*)(Map)> win_conditions;
    std::vector<bool (*)(Map)> sidequests;
    std::vector<bool (*)(Map)> lose_conditions;

    void defeat();
    void victory();

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pMap);
    void readXML(tinyxml2::XMLElement * in_pMap);

};

extern std::vector<std::vector<short int>> (*chapTilemaps[40])();
extern std::vector<short unsigned int> (*chapArrivalinds[40])();
extern std::vector<short int> (*baseParties[CHAPTER::CHAP1 - CHAPTER::TEST + 1])();
extern std::vector<std::vector<Inventory_item>> (*arrivalEquipments[CHAPTER::END - CHAPTER::TEST + 1])();
extern std::vector<Map_arrival> (*mapArrivals[CHAPTER::END - CHAPTER::TEST + 1])();

std::vector<short unsigned int> testArrivals();
std::vector<std::vector<short int>> testTilemap();
std::vector<std::vector<short int>> chap2Tilemap();
std::vector<std::vector<short int>> chap3Tilemap();
std::vector<std::vector<short int>> chap4Tilemap();
std::vector<std::vector<short int>> chap5Tilemap();
std::vector<std::vector<short int>> chap6Tilemap();
std::vector<std::vector<short int>> chap7Tilemap();
std::vector<std::vector<short int>> chap8Tilemap();
std::vector<std::vector<short int>> chap9Tilemap();
std::vector<std::vector<short int>> chap10Tilemap();
std::vector<std::vector<short int>> chap11Tilemap();
std::vector<std::vector<short int>> chap12Tilemap();
std::vector<std::vector<short int>> chap13Tilemap();
std::vector<std::vector<short int>> chap14Tilemap();
std::vector<std::vector<short int>> chap15Tilemap();
std::vector<std::vector<short int>> chap16Tilemap();
std::vector<std::vector<short int>> chap17Tilemap();
std::vector<std::vector<short int>> chap18Tilemap();
std::vector<std::vector<short int>> chap19Tilemap();
std::vector<std::vector<short int>> chap20Tilemap();
std::vector<std::vector<short int>> chap21Tilemap();
std::vector<std::vector<short int>> chap22Tilemap();
std::vector<std::vector<short int>> chap23Tilemap();
std::vector<std::vector<short int>> chap24Tilemap();
std::vector<std::vector<short int>> chap25Tilemap();

#endif /* MAP_HPP */