#include "map.hpp"

// ECS_DEFINE_TYPE(Map);

Map::Map() {
    initVars();
    overlay_mode = MAP::OVERLAY::MOVE + MAP::OVERLAY::ATTACK;
}

Map::~Map() {
    
}

Map::Map(const short unsigned int width, const short unsigned int height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::readXML(tinyxml2::XMLElement * in_pMap) {

}

void Map::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pMap) {
    char buffer[DEFAULT::BUFFER_SIZE];
    
    in_pMap->SetAttribute("chapter", chapter);
    
    tinyxml2::XMLElement * pTiles = in_doc->NewElement("Tiles");
    in_pMap->InsertEndChild(pTiles);
    tinyxml2::XMLElement * pTile = in_doc->NewElement("Tile");
    tinyxml2::XMLElement * pName = in_doc->NewElement("Name");

    for (int i = 0; i < tilesindex.size(); i++) {
        pTile->SetAttribute("id", tilesindex[i]);
        pTiles->InsertEndChild(pTile);
        pTile->InsertEndChild(pName);
        pName->SetText(tilenames[i].c_str());
    }

    tinyxml2::XMLElement * pBounds = in_doc->NewElement("Bounds");
    tinyxml2::XMLElement * pRowmax = in_doc->NewElement("row_max");
    tinyxml2::XMLElement * pColmax = in_doc->NewElement("col_max");
    in_pMap->InsertEndChild(pBounds);
    pBounds->InsertEndChild(pRowmax);
    pBounds->InsertEndChild(pColmax);
    pRowmax->SetText(bounds[1]);
    pColmax->SetText(bounds[3]);

    tinyxml2::XMLElement * pTilemap = in_doc->NewElement("Tilemap");
    tinyxml2::XMLElement * pIndex = in_doc->NewElement("Index");
    in_pMap->InsertEndChild(pTilemap);

    for (int row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
        for (int col = 0; col < tilemap[row].size(); col++) {
            pTilemap->InsertEndChild(pIndex);
            in_pMap->SetAttribute("row", row);
            in_pMap->SetAttribute("col", col);
            pIndex->SetText(tilemap[row][col]);
        }
    }

    tinyxml2::XMLElement * pArrivals = in_doc->NewElement("Arrivals");
    tinyxml2::XMLElement * pArrival = in_doc->NewElement("Arrival");
    for (int i = 0; i < map_arrivals.size(); i++) {
        pBounds->InsertEndChild(pArrivals);
        writeXML_arrival(in_doc, pArrival, &map_arrivals[i]);
    }
    tinyxml2::XMLElement * pArrivalEqs = in_doc->NewElement("ArrivalEqs");    
    tinyxml2::XMLElement * pArrivalEq = in_doc->NewElement("ArrivalEq");    
    in_pMap->InsertEndChild(pArrivalEqs);
    for (int i = 0; i < arrival_equipments.size(); i++) {
        pArrivalEqs->InsertEndChild(pArrivalEq);
        pArrivalEq->SetAttribute("unitid", map_arrivals[i].id);
        writeXML_items(in_doc, pArrivalEq, arrival_equipments[i]);
    }
}

void Map::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

short unsigned int * Map::getTilesize() const {
    return ((short unsigned int *)tilesize);
}

void Map::putUnit(const short unsigned int x, const short unsigned int y, entityx::ComponentHandle<Unit> in_unit) {
    unitmap[x][y] = in_unit; 
}

entityx::ComponentHandle<Unit> Map::getUnit(const short unsigned int x, const short unsigned int y) {
    return(unitmap[x][y]);
}

void Map::moveUnit(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y) {
    SDL_Log("Move Unit %d %d %d %d", x, y, new_x, new_y);
    entityx::ComponentHandle<Unit> buffer = unitmap[new_x][new_y];
    unitmap[new_x][new_y] = unitmap[x][y];
    unitmap[x][y] = buffer;
}

void Map::setArrivalEquipments(const std::vector<std::vector<Inventory_item>> in_arrival_equipments) {
    arrival_equipments = in_arrival_equipments;
}

std::vector<std::vector<Inventory_item>> Map::getArrivalEquipments() {
    return(arrival_equipments);
}

void Map::setArrivals(const std::vector<Map_arrival> in_arrivals) {
    map_arrivals = in_arrivals;
}

void Map::addArrival(const Map_arrival in_arrival) {
    map_arrivals.push_back(in_arrival);
}

void Map::removeMapArrival(const unsigned char index) {
    loaded_map_arrivals.push_back(map_arrivals[index]);
    map_arrivals.erase(map_arrivals.begin() + index);
}

std::vector<Map_arrival> Map::getArrivals() {
    return(map_arrivals);
}

std::vector<std::vector<short int>> Map::makeMvtCostmap(const unsigned char unitmovetype) {
    // SDL_Log("Making MvtCostmap");
    short int tile_ind = 0;
    std::vector<std::vector<short int>> costmap((short int)tilemap.size(), std::vector<short int> ((short int)tilemap[0].size()));
    for (short int row = 0; row < tilemap.size(); row++) {
        for (short int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col]/DEFAULT::TILE_DIVISOR;
            costmap[row][col] = tiles[tile_ind].getCost()[unitmovetype];
        }
    }
    return(costmap);
}
void Map::loadTiles(const int in_map_index) {
    tilesindex = chapTiles[in_map_index]();
    tilenames = getTilenames(tilesindex);
    baseTiles(&tiles, tilesindex);
}

void Map::loadTiles(std::vector<short int> to_load) {
    tilesindex = to_load;
    tilenames = getTilenames(tilesindex);
    baseTiles(&tiles, tilesindex);
}

void Map::unloadTiles(std::vector<short int> to_unload) {
    for (int i = 0; i < to_unload.size(); i++) {
        tiles.erase(to_unload[i]);
    }
}

std::vector<std::vector<short int>> Map::getTilemap(){
    return(tilemap);
}

void Map::setTilemap(const std::vector<std::vector<short int>> in_tilemap){
    tilemap = in_tilemap;
}

unsigned char Map::getTurn() {
    return(turn);
}

void Map::setRenderer(SDL_Renderer * in_renderer){
    SDL_Log("Setting Map renderer");
    if (in_renderer) {
        renderer = in_renderer;
    } else {
        SDL_Log("Could not set map renderer");
    }
    loadOverlays();
}

void Map::loadTiletextures() {
    short unsigned int tile_ind;
    std::string texturename;
    for (short unsigned int i = 0; i < tilesindex.size(); i++) {
        tile_ind = (tilesindex[i]/DEFAULT::TILE_DIVISOR);
        texturename = "..//assets//" + tiles[tile_ind].getName() + "_" + std::to_string(tilesindex[i]) + ".png";
        textures[tilesindex[i]] = loadTexture(renderer, texturename.c_str());
    }
}

void Map::loadOverlays() {
    SDL_Log("Loading Map overlays");
    overlays[0] = loadTexture(renderer, "tile_overlay_move.png", true);
    overlays[1] = loadTexture(renderer, "tile_overlay_attack.png", true);
    overlays[2] = loadTexture(renderer, "tile_overlay_heal.png", true);
}

void Map::loadDanger() {
    SDL_Log("Loading Map dangerzone");
    dangers[0] = loadTexture(renderer, "..//assets//danger.png", false);
    // dangers[1] = loadTexture("..//assets//danger_grid.png");
}

void Map::loadGrid() {
}

void Map::addDanger(const std::vector<std::vector<short int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger);
}

void Map::subDanger(const std::vector<std::vector<short int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger, -1);
}

void Map::setOverlaymode(const unsigned char in_mode) {
    char message[DEFAULT::BUFFER_SIZE];
    sprintf(message, "Set Map ovelay mode: %d", in_mode);
    SDL_Log(message);
    overlay_mode = in_mode;
}

void Map::setDangermode(const unsigned char in_mode) {
    danger_mode = in_mode;
}

void Map::showDanger(){
    show_danger = true;
}

void Map::hideDanger(){
    show_overlay = false;
}

void Map::showOverlay(){
    show_overlay = true;
}

void Map::hideOverlay(){
    show_overlay = false;
}

void Map::initVars() {
    srcrect.x = srcrect.y = 0;
    destrect.x = destrect.y = 0;
    setTilesize(DEFAULT::TILESIZE, DEFAULT::TILESIZE);
    srcrect.w = destrect.w = DEFAULT::TILESIZE;
    srcrect.h = destrect.h = DEFAULT::TILESIZE;
}

// std::vector<std::vector<entityx::Entity *>> Map::getEntitymap() {
//     return(entitymap);
// }

std::vector<std::vector<entityx::ComponentHandle<Unit>>> Map::getUnitmap() {
    return(unitmap);
}

void Map::loadTilemap(const short unsigned int in_map_index) {
    tilemap = chapTilemaps[in_map_index]();
    postTilemap();
}

void Map::postTilemap() {
    loadTiletextures();
    short unsigned int col_size = tilemap.size();
    short unsigned int row_size = tilemap[0].size();
    bounds[1] = row_size;
    bounds[3] = col_size;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> tempunit(row_size, std::vector<entityx::ComponentHandle<Unit>>(col_size));
    unitmap = tempunit; 
}

short int * Map::getBounds() {
    return(bounds);
}

unsigned char Map::getnumArrivals() {
    return(num_enemies);
}

unsigned short int Map::getBoss() {
    return(boss);
}

bool Map::getBossDeath() {
    return(bossdied);
}

bool Map::getSeized() {
    return(bossdied);
}

std::vector<unsigned short int> Map::getEssentials() {
    return(essentials);
}

void Map::setOverlay(const unsigned char in_mode, const std::vector<std::vector<short int>> in_map) {
    if ((in_mode & MAP::OVERLAY::HEAL) > 0) {
        healoverlay = in_map;
    }
    if ((in_mode & MAP::OVERLAY::ATTACK) > 0) {
        attackoverlay = in_map;
    } 
    if ((in_mode & MAP::OVERLAY::MOVE) > 0) {
        moveoverlay = in_map;
    } 
}

void Map::clearOverlays() {
    attackoverlay.clear();
    moveoverlay.clear();
    healoverlay.clear();
    overlay_mode = 0;
}

void Map::draw() {
    // SDL_Log("Drawing Map");
    int tile_ind = 0;
    for (int row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
        for (int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col];
            destrect.x = (col + 1) * tilesize[0];
            destrect.y = (row + 1) * tilesize[1];
            SDL_RenderCopy(renderer, textures[tile_ind], &srcrect, &destrect);

            if (show_overlay) {
                if (((overlay_mode & MAP::OVERLAY::MOVE) > 0) && (overlays[0] != NULL)) {
                    if (moveoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[0], &srcrect, &destrect);
                    }
                }
                if (((overlay_mode & MAP::OVERLAY::ATTACK) > 0)  && (overlays[1] != NULL)) {
                    if (attackoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[1], &srcrect, &destrect);
                    }
                }
                if (((overlay_mode & MAP::OVERLAY::HEAL) > 0) && (overlays[2] != NULL)) {
                    if (healoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[2], &srcrect, &destrect);
                    }
                }
            }

            if (show_danger) {
                if (dangeroverlay[row][col] > 1) {
                    SDL_RenderCopy(renderer, dangers[0], &srcrect, &destrect);
                }
            }
        }
    }
}

std::vector<std::vector<short int>> testTilemap(){
    std::vector<std::vector<short int>> tilemap = { 
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 100, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}
    };
    return(tilemap);
}

std::vector<short unsigned int> testArrivalinds(){
    std::vector<short unsigned int> enemies = {UNIT::NAME::SILOU};
    return(enemies);
}

std::vector<std::vector<short int>> (*chapTilemaps[40])() = {testTilemap,};
std::vector<short unsigned int> (*chapArrivalinds[40])() = {testArrivalinds,};