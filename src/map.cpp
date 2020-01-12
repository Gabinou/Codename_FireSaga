#include "map.hpp"

void Map::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

int * Map::getTilesize() const {
    return ((int *)tilesize);
}

void Map::setTile(int x, int y, Entity * in_entity) {
    tiles[x][y] = in_entity;
}

void Map::removeTile(int x, int y) {
    tiles[x][y] = nullptr;
}

std::vector<std::vector<int>> Map::makeMvtCostmap(std::string unitmovetype){
    int tile = 0;
    std::vector<std::vector<int>> costmap((int)map2D.size(), std::vector<int> ((int)map2D[0].size()));
    for (int row = 0; row < map2D.size(); row++) {
        for (int col = 0; col < map2D[row].size(); col++) {
            tile = map2D[row][col];
            // printf("%d", tile);
            // printf("%d", all_tiles[tile].getCost().riders_slow);
            printf("%s", all_tiles[tile].getName().c_str());
        }
    }
    return(costmap);
}

std::vector<std::vector<int>> Map::get2D(){
    return(map2D);
}

void Map::moveTile(int x, int y, int new_x, int new_y) {
    tiles[new_x][new_y] = tiles[x][y];
    tiles[x][y] = nullptr;
}

Entity * Map::getTile(int x, int y) {
    return(tiles[x][y]);
}

void Map::loadTiles() {
    dirt = loadTexture("..//assets//dirt.png");
    grass = loadTexture("..//assets//grass.png");
    water = loadTexture("..//assets//water.png");
    // Make this into an array of types, and breaks the switch into an array index. Also put the assets into a single  string vector.
}

void Map::loadOverlays() {
    overlays[0] = loadTexture("..//assets//tile_overlay_move.png");
    overlays[1] = loadTexture("..//assets//tile_overlay_attack.png");
    overlays[2] = loadTexture("..//assets//tile_overlay_heal.png");
}

void Map::setOverlaymode(std::string in_mode) {
    overlay_mode = in_mode;
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
    tiles = temp;
}

Map::Map() {
    loadTiles();
    loadOverlays();
    initVars();
}

Map::Map(const short unsigned int width, const short unsigned int height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::loadMap(std::string filename) {
    map2D = readcsv_vec(filename.c_str(), 1);
    Entity_ptr_matrix temp(map2D.size(), std::vector<Entity*>(map2D[0].size()));

    tiles = temp;
    // printf("Tiles size %d %d \n", tiles.size(), tiles[0].size());
    for (int row = 0; row < tiles.size(); row++) {
        for (int col = 0; col < tiles[row].size(); col++) {
            tiles[row][col] =  static_cast<Entity*>(nullptr);
        }
    }
}

void Map::setList(std::string in_type, std::vector<std::vector<int>> in_list) {
    if ((in_type == "heal") || (in_type == "heallist")){
        heallist = in_list;
        if (overlay_mode.find("heal") == std::string::npos) {
            overlay_mode.append("heal");
        }
    }
    if ((in_type == "attack") || (in_type == "attacklist")){
        attacklist = in_list;
        if (overlay_mode.find("attack") == std::string::npos) {
            overlay_mode.append("attack");
        }
    } 
    if ((in_type == "move") || (in_type == "movelist")){
        movelist = in_list;
        if (overlay_mode.find("move") == std::string::npos) {
            overlay_mode.append("move");
        }
    } 
}

void Map::clearLists() {
    attacklist.clear();
    movelist.clear();
    heallist.clear();
    overlay_mode = "";
}

void Map::drawMap() {
    int tile = 0;
    // This is cache friendly.
    for (int row = 0; row < map2D.size(); row++) {
        for (int col = 0; col < map2D[row].size(); col++) {
            tile = map2D[row][col];
            destrect.x = (col + 1) * tilesize[0];
            destrect.y = (row + 1) * tilesize[1];

            switch (tile) {
            case 10:
                SDL_RenderCopy(Game::renderer, grass, &srcrect, &destrect);
                break;
            case 20:
                SDL_RenderCopy(Game::renderer, water, &srcrect, &destrect);
                break;
            case 30:
                SDL_RenderCopy(Game::renderer, dirt, &srcrect, &destrect);
                break;
            default:
                break;
            }
            if (show_overlay) {
                if ((overlay_mode.find("move") != std::string::npos) && (overlays[0] != NULL)) {
                    if (movelist[row][col] == 1) {
                        SDL_RenderCopy(Game::renderer, overlays[0], &srcrect, &destrect);
                    }
                }
                if ((overlay_mode.find("attack") != std::string::npos)  && (overlays[1] != NULL)) {
                    if (attacklist[row][col] == 1) {
                        SDL_RenderCopy(Game::renderer, overlays[1], &srcrect, &destrect);
                    }
                }
                if ((overlay_mode.find("heal") != std::string::npos) && (overlays[2] != NULL)) {
                    if (heallist[row][col] == 1) {
                        SDL_RenderCopy(Game::renderer, overlays[2], &srcrect, &destrect);
                    }
                }
            }
            // if (ss_looping == "pingpong") {
            //     srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
            // } else if ((ss_looping == "linear") || (ss_looping == "direct")) {
            //     srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            // } else if (ss_looping == "reverse") {
            //     srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
            // }

            // SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
        }
    }
}