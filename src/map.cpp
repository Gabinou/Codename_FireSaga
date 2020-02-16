#include "map.hpp"

Map::Map() {
    loadOverlays();
    initVars();
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

int * Map::getTilesize() const {
    return ((int *)tilesize);
}

void Map::setTile(int x, int y, Entity * in_entity) {
    entitymap[x][y] = in_entity;
}

void Map::removeTile(int x, int y) {
    entitymap[x][y] = nullptr;
}

void Map::setEnemies(std::vector<std::string> in_enemy_names) {
    enemy_names = in_enemy_names;
}

void Map::addEnemy(std::string in_enemy_name) {
    enemy_names.push_back(in_enemy_name);
}

std::vector<std::string> Map::getEnemies() {
    return(enemy_names);
}

std::vector<std::vector<int>> Map::makeMvtCostmap(std::string unitmovetype){
    int tile_ind = 0;
    std::vector<std::vector<int>> costmap((int)tilemap.size(), std::vector<int> ((int)tilemap[0].size()));
    for (int row = 0; row < tilemap.size(); row++) {
        for (int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col]/10; // /10 eliminates one digit ont int's right
            costmap[row][col] = loaded_tiles[tile_ind].getCostmap()[unitmovetype];
        }
    }
    return(costmap);
}

std::vector<std::vector<int>> Map::getTilemap(){
    return(tilemap);
}

void Map::setTilemap(std::vector<std::vector<int>> in_tilemap){
    tilemap = in_tilemap;
}

void Map::moveTile(int x, int y, int new_x, int new_y) {
    entitymap[new_x][new_y] = entitymap[x][y];
    entitymap[x][y] = nullptr;
}

Entity * Map::getTile(int x, int y) {
    return(entitymap[x][y]);
}

void Map::loadTiletextures() {
    int tileindex;
    std::string texturename;
    for (int i = 0; i < unique_textures.size() - 1; i++) {
        tileindex = (unique_textures[i]/10);
        texturename = "..//assets//" + loaded_tiles[tileindex].getName() + "_" + std::to_string(unique_textures[i]) + ".png";
        textures[unique_textures[i]] = loadTexture(texturename.c_str());
    }
}

void Map::loadOverlays() {
    overlays[0] = loadTexture("..//assets//tile_overlay_move.png");
    overlays[1] = loadTexture("..//assets//tile_overlay_attack.png");
    overlays[2] = loadTexture("..//assets//tile_overlay_heal.png");
}

void Map::loadDanger() {
    dangers[0] = loadTexture("..//assets//danger.png");
    // dangers[1] = loadTexture("..//assets//danger_grid.png");
}

void Map::loadGrid() {
}

void Map::addDanger(std::vector<std::vector<int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger);
}

void Map::subDanger(std::vector<std::vector<int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger, -1);
}

void Map::setOverlaymode(std::string in_mode) {
    overlay_mode = in_mode;
}

void Map::setDangermode(std::string in_mode) {
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
    srcrect.w = destrect.w = 32;
    srcrect.h = destrect.h = 32;
    setTilesize(32, 32);
    Entity_ptr_matrix temp(255, std::vector<Entity*>(255));
    entitymap = temp;
}

void Map::makeEntitymap(int row_size, int col_size){
    if (!made_entitymap) {
        Entity_ptr_matrix temp(row_size, std::vector<Entity*>(col_size));
        entitymap = temp;
        // printf("Made Entitymap size: %d, %d\n", entitymap.size(), entitymap[0].size());
        for (int col = 0; col < entitymap.size(); col++) {
            for (int row = 0; row < entitymap[0].size(); row++) {
                entitymap[col][row] =  static_cast<Entity*>(nullptr);
            }
        }
    }
    made_entitymap = true;
}

void Map::loadTilemap(const std::string filename) {
    tilemap = readcsv_vec(filename.c_str(), 1);
    postTilemap();
}

void Map::loadTilemap(const int in_map_index) {
    tilemap = chapTilemaps[in_map_index]();
    postTilemap();
}

void Map::postTilemap() {
    unique_textures = unique2D(tilemap);
    loadTiletextures();
    int col_size = tilemap.size();
    int row_size = tilemap[0].size();
    makeEntitymap(row_size, col_size);
}

void Map::setOverlay(std::string in_type, std::vector<std::vector<int>> in_map) {
    if ((in_type == "heal") || (in_type == "healoverlay")){
        healoverlay = in_map;
        if (overlay_mode.find("heal") == std::string::npos) {
            overlay_mode.append("heal");
        }
    }
    if ((in_type == "attack") || (in_type == "attackoverlay")){
        attackoverlay = in_map;
        if (overlay_mode.find("attack") == std::string::npos) {
            overlay_mode.append("attack");
        }
    } 
    if ((in_type == "move") || (in_type == "moveoverlay")){
        moveoverlay = in_map;
        if (overlay_mode.find("move") == std::string::npos) {
            overlay_mode.append("move");
        }
    } 
}

void Map::clearOverlays() {
    attackoverlay.clear();
    moveoverlay.clear();
    healoverlay.clear();
    overlay_mode = "";
}

void Map::drawMap(SDL_Renderer * renderer) {
    int tile_ind = 0;
    for (int row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
        for (int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col];
            destrect.x = (col + 1) * tilesize[0];
            destrect.y = (row + 1) * tilesize[1];
            SDL_RenderCopy(renderer, textures[tile_ind], &srcrect, &destrect);

            if (show_overlay) {
                if ((overlay_mode.find("move") != std::string::npos) && (overlays[0] != NULL)) {
                    if (moveoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[0], &srcrect, &destrect);
                    }
                }
                if ((overlay_mode.find("attack") != std::string::npos)  && (overlays[1] != NULL)) {
                    if (attackoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[1], &srcrect, &destrect);
                    }
                }
                if ((overlay_mode.find("heal") != std::string::npos) && (overlays[2] != NULL)) {
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
            // if (ss_looping == "pingpong") {
            //     srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
            // } else if ((ss_looping == "linear") || (ss_looping == "direct")) {
            //     srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            // } else if (ss_looping == "reverse") {
            //     srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
            // }

            // SDL_RenderCopy(renderer, texture, &srcrect, &destrect);
        }
    }
}


std::vector<std::vector<int>> testTilemap(){
    std::vector<std::vector<int>> tilemap = { 
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

std::vector<std::vector<int>> (*chapTilemaps[40])() = {testTilemap,};
