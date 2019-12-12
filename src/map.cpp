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
    initVars();
}

Map::Map(const short unsigned int width, const short unsigned int height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::loadMap(std::string filename) {
    map = readcsv_vec(filename.c_str(), 1);
    Entity_ptr_matrix temp(map.size(), std::vector<Entity*>(map[0].size()));

    tiles = temp;
    // printf("Tiles size %d %d \n", tiles.size(), tiles[0].size());
    for (int row = 0; row < tiles.size(); row++) {
        for (int col = 0; col < tiles[row].size(); col++) {
            tiles[row][col] =  static_cast<Entity*>(nullptr);
        }
    }
}

void Map::drawMap() {
    int type = 0;
    // This is cache friendly.
    for (int row = 0; row < map.size(); row++) {
        for (int col = 0; col < map[row].size(); col++) {
            type =  map[row][col];
            destrect.x = (col + 1) * tilesize[0];
            destrect.y = (row + 1) * tilesize[1];

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