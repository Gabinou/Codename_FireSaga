#ifndef MAP_HPP
#define MAP_HPP

#include "enums.hpp"
#include "structs.hpp"
#include <entityx/entityx.h>
#include <entityx/entityx.h>
#include "SDL2/SDL.h"
#include "utilities.hpp"
#include "filesystem.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include <unordered_map>

class Map : public XML_IO, JSON_IO, entityx::Receiver<Map> {
private:
    entityx::EntityManager * manager;

    SDL_Rect srcrect, destrect;
    SDL_Renderer * renderer = NULL;
    uint16_t frames = 10, speed = 50;
    uint16_t tilesize[2];

    std::unordered_map<int16_t, SDL_Texture *> textures;
    std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
    std::vector<SDL_Texture *> dangers{NULL, NULL};

    std::vector<std::vector<int16_t>> moveoverlay, attackoverlay, healoverlay;
    bool show_overlay = false;
    bool show_danger = false;
    bool show_grid = false;
    int8_t overlay_mode = 0;
    std::vector<std::vector<int16_t>> dangeroverlay;
    int8_t danger_mode = 0;

    std::vector<std::vector<int16_t>> tilemap;
    std::unordered_map<int16_t, Tile> tiles;
    std::vector<int16_t> tilesindex;
    std::vector<std::string> tilenames;

    int16_t bounds[4] = {0, 255, 0, 255}; //rowmin, rowmax, colmin, colmax
    int16_t offset[2] = {DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET}; //  isnt offset[0] juste bounds[0]? offset[2] == bounds[1]?
    int8_t num_friendlies = 0;
    int8_t num_neutral = 0;
    int8_t num_enemies = 0;
    int8_t turn = 0; // Automatic loss if turn 255.
    int8_t chapter = 0;

    std::vector<Map_arrival> map_arrivals;
    std::vector<Map_arrival> loaded_map_arrivals;
    std::vector<std::vector<Inventory_item>> arrival_equipments;

    std::vector<Point> starting_positions;
    std::vector<uint8_t> armies_onfield;
    std::unordered_map<int8_t, std::vector<entityx::ComponentHandle<Unit>>> units_onfield;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;
    std::vector<uint16_t> essentials = {UNIT::NAME::ERWIN};
    uint16_t boss;
    bool bossdied;
    bool seized; // maybe unecessary if turn system.

public:
    Map();
    ~Map();
    Map(const uint16_t width, const uint16_t height);

    void loadTiletextures();
    void loadOverlays();
    void loadDanger();

    void loadTiles(const int32_t in_map_index);
    void loadTiles(std::vector<int16_t> to_load);
    void loadTilesJSON();
    void unloadTiles(std::vector<int16_t> to_unload);
    std::unordered_map<int16_t, Tile> getTiles();
    std::vector<int16_t> getTilesindex();

    void postTilemap();
    std::vector<std::vector<int16_t>> getTilemap();
    void setTilemap(const std::vector<std::vector<int16_t>> in_tilemap);
    void setTilesize(const uint16_t width, const uint16_t height);
    uint16_t * getTilesize() const;

    std::vector<std::vector<int16_t>> makeMvtCostmap(entityx::ComponentHandle<Unit> in_unit);

    void setManager(entityx::EntityManager * in_manager);
    void setRenderer(SDL_Renderer * in_renderer);
    void draw();

    std::vector<std::vector<entityx::ComponentHandle<Unit>>> getUnitmap();
    void clearUnitmap();

    std::vector<Point> getStartingpos();
    void setStartingpos(std::vector<Point> in_positions);
    void addStartingpos(Point in_position);

    void addArmy(uint8_t in_army);
    std::vector<uint8_t> getArmies();
    void putUnit(const uint16_t x, const uint16_t y, entityx::ComponentHandle<Unit> in_unit);
    std::vector<entityx::ComponentHandle<Unit>> getUnits(int8_t in_army);
    entityx::ComponentHandle<Unit> getUnit(const uint16_t x, const uint16_t y);
    void moveUnit(const uint16_t x, const uint16_t y, const uint16_t new_x, const uint16_t new_y);

    void setOverlay(const uint8_t in_mode, std::vector<std::vector<int16_t>> in_map);
    void clearOverlays();

    uint8_t getTurn();

    void setOverlaymode(const uint8_t in_mode);
    void showOverlay();
    void hideOverlay();

    void setOffset(int16_t in_offset[2]);
    void setOffset(int16_t xoffset, int16_t yoffset);
    int16_t * getOffset();

    void loadGrid();
    void showGrid();
    void hideGrid();

    void setArrivalEquipments(const std::vector<std::vector<Inventory_item>> in_arrival_equipments);
    std::vector<std::vector<Inventory_item>> getArrivalEquipments();

    void setArrivals(const std::vector<Map_arrival> in_arrivals);
    void addArrival(const Map_arrival in_arrival);
    void removeMapArrival(const uint8_t index);
    std::vector<Map_arrival> getArrivals();
    uint8_t getnumArrivals();

    void setDangermode(const uint8_t in_mode);
    void showDanger();
    void hideDanger();
    void switchDanger();
    bool showsDanger();
    void addDanger(const std::vector<std::vector<int16_t>> in_danger);
    void subDanger(const std::vector<std::vector<int16_t>> in_danger);
    void setDanger(const std::vector<std::vector<int16_t>> in_danger);

    uint16_t getBoss();
    bool getBossDeath();
    bool getSeized();
    std::vector<uint16_t> getEssentials();

    int16_t * getBounds();

    std::vector<bool (*)(Map)> win_conditions;
    std::vector<bool (*)(Map)> sidequests;
    std::vector<bool (*)(Map)> lose_conditions;

    void defeat();
    void victory();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pMap);
    void readXML(tinyxml2::XMLElement * in_pMap);

};

extern std::vector<std::string> mapNames;
extern std::vector<int16_t> baseParty();

#endif /* MAP_HPP */