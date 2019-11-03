#include "TextureManager.hpp"
#include "map.hpp"
#include "parser.hpp"

void Map::loadTiles() {
    dirt = TextureManager::loadTexture("..//assets//dirt.png");
    grass = TextureManager::loadTexture("..//assets//grass.png");
    water = TextureManager::loadTexture("..//assets//water.png");
}

void Map::initVars() {
    srcrect.x = srcrect.y = 0;
    destrect.x = destrect.y = 0;
    srcrect.w = destrect.w = 32;
    srcrect.h = destrect.h = 32;
    tilesize[0] = 32;
    tilesize[1] = 32;
}

void Map::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

Map::Map() {
    loadTiles();
    initVars();
}

Map::Map(const short unsigned int width, const short unsigned int height) {
    loadTiles();
    initVars();
    setTilesize(width, height);
}

void Map::loadMap(std::string filename) {
    map = readcsv_vec(filename.c_str(), 1);
}

void Map::drawMap() {
    int type = 0;
    for (int row = 0; row<map.size(); row++) {
        for (int col = 0; col<map[row].size(); col++) {
            type = map[row][col];
            
            destrect.x = col * tilesize[0];
            destrect.y = row * tilesize[1];
            
            switch (type){
            case 0:
                TextureManager::draw(water, srcrect, destrect);
                break;
            case 1:
                TextureManager::draw(grass, srcrect, destrect);
                break;
            case 2:
                TextureManager::draw(dirt, srcrect, destrect);
                break;
            default:
                break;
                
            }
        }
    }
}