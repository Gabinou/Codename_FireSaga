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

void Map::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

short unsigned int * Map::getTilesize() const {
    return ((short unsigned int *)tilesize);
}

// void Map::setTile(const short unsigned int x, const short unsigned int y, Entity * in_entity) {
    // entitymap[x][y] = in_entity;
// }

void Map::removeTile(const short unsigned int x, const short unsigned int y) {
    // entitymap[x][y] = nullptr;
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
    tilesasset_ind = chapTiles[in_map_index]();
    baseTiles(&tiles, tilesasset_ind);
}

void Map::loadTiles(std::vector<short int> to_load) {
    tilesasset_ind = to_load;
    baseTiles(&tiles, tilesasset_ind);
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

// void Map::moveTile(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y) {
//     entitymap[new_x][new_y] = entitymap[x][y];
//     entitymap[x][y] = nullptr;
// }

// Entity * Map::getTile(const short unsigned int x, const short unsigned int y) {
//     return(entitymap[x][y]);
// }

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
    for (short unsigned int i = 0; i < tilesasset_ind.size(); i++) {
        tile_ind = (tilesasset_ind[i]/DEFAULT::TILE_DIVISOR);
        texturename = "..//assets//" + tiles[tile_ind].getName() + "_" + std::to_string(tilesasset_ind[i]) + ".png";
        textures[tilesasset_ind[i]] = loadTexture(renderer, texturename.c_str());
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
    // std::vector<std::vector<Entity *>> temp(DEFAULT::LINE_LENGTH, std::vector<Entity*>(DEFAULT::LINE_LENGTH));
    // entitymap = temp;
}

// void Map::makeEntitymap(const short unsigned int row_size, const short unsigned int col_size){
//     if (!made_entitymap) {
//         std::vector<std::vector<Entity *>> temp(row_size, std::vector<Entity*>(col_size));
//         entitymap = temp;
//         for (short unsigned int col = 0; col < entitymap.size(); col++) {
//             for (short unsigned int row = 0; row < entitymap[0].size(); row++) {
//                 entitymap[col][row] =  static_cast<Entity*>(nullptr);
//             }
//         }
//     }
//     made_entitymap = true;
// }

// std::vector<std::vector<Entity *>> Map::getEntitymap() {
//     return(entitymap);
// }

void Map::loadTilemap(const std::string filename) {
    tilemap = readcsv_vec<short int>(filename.c_str(), 1);
    postTilemap();
}

void Map::loadTilemap(const short unsigned int in_map_index) {
    tilemap = chapTilemaps[in_map_index]();
    postTilemap();
}

void Map::postTilemap() {
    loadTiletextures();
    short unsigned int col_size = tilemap.size();
    short unsigned int row_size = tilemap[0].size();
    // makeEntitymap(row_size, col_size);
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
    SDL_Log("Drawing Map");
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