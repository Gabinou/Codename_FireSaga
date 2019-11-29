#include "map.hpp"

void Map::loadTiles() {
    dirt = loadTexture("..//assets//dirt.png");
    grass = loadTexture("..//assets//grass.png");
    water = loadTexture("..//assets//water.png");
    // Make this into an array of types, and breaks the switch into an array index. Also put the assets into a single  string vector.
}

void Map::initVars() {
    srcrect.x = srcrect.y = 0;
    destrect.x = destrect.y = 0;
    srcrect.w = destrect.w = 32;
    srcrect.h = destrect.h = 32;
    setTilesize(32, 32);
}

Map::Map() {
    loadTiles();
    initVars();
}

Map::Map(const short unsigned int width, const short unsigned int height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::loadMap(std::string filename) {
    map = readcsv_vec(filename.c_str(), 1);
    name(width, std::vector<Entity*>(height));
    // tiles = new Entity**[map.size()];
    // for (int row = 0; row<map.size(); row++){
        // tiles[row] = new Entity*[map[row].size()];
    // }
    // for (int row = 0; row < map.size(); row++) {
        // for (int col = 0; col < map[row].size(); col++) {
            // tiles[row][col] = NULL;
        // }
    // }
  
}

void Map::drawMap() {
    int type = 0;
    static int * currenttilesize = (getTilesize());
    // This is cache friendly.
    for (int row = 0; row < map.size(); row++) {
        for (int col = 0; col < map[row].size(); col++) {
            type =  map[row][col];
            destrect.x = (col + 1) * currenttilesize[0];
            destrect.y = (row + 1) * currenttilesize[1];

            switch (type) {
            case 0:
                SDL_RenderCopy(Game::renderer, water, &srcrect, &destrect);
                break;

            case 1:
                SDL_RenderCopy(Game::renderer, grass, &srcrect, &destrect);
                break;

            case 2:
                SDL_RenderCopy(Game::renderer, dirt, &srcrect, &destrect);
                break;

            default:
                break;
            }
        }
    }
}