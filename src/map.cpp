#include "TextureManager.hpp"
#include "map.hpp"
#include "parser.hpp"

void Map::loadTiles() {
    dirt = TextureManager::LoadTexture("..//assets//dirt.png");
    grass = TextureManager::LoadTexture("..//assets//grass.png");
    water = TextureManager::LoadTexture("..//assets//water.png");
}

void Map::initVars() {
    src.x = src.y = 0;
    dest.x = dest.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    tilesize[0] = 32;
    tilesize[1] = 32;
}

void Map::TileSize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
    src.w = dest.w = width;
    src.h = dest.h = height;
}

Map::Map() {
    loadTiles();
    initVars();
    loadMap("a");
}

Map::Map(const short unsigned int width, const short unsigned int height) {
    loadTiles();
    initVars();
    TileSize(width, height);
    loadMap("a");
}

void Map::loadMap(std::string filename) {
    map = readcsv_vec("..//testmap.txt", 1);
}

void Map::drawMap() {
    int type = 0;
    for (int row = 0; row<map.size(); row++) {
        for (int col = 0; col<map[row].size(); col++) {
            type = map[row][col];
            
            dest.x = col * tilesize[0];
            dest.y = row * tilesize[1];
            
            switch (type){
            case 0:
                TextureManager::Draw(water, src, dest);
                break;
            case 1:
                TextureManager::Draw(grass, src, dest);
                break;
            case 2:
                TextureManager::Draw(dirt, src, dest);
                break;
            default:
                break;
                
            }
        }
    }
}