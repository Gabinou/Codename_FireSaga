#include "TextureManager.hpp"
#include "map.hpp"
#include "parser.hpp"

void Map::loadTiles() {
    dirt = TextureManager::LoadTexture("..//assets//dirt.png");
    grass = TextureManager::LoadTexture("..//assets//grass.png");
    water = TextureManager::LoadTexture("..//assets//water.png");
    printf("loaded tiles");
}

void Map::initMembers() {
    src.x = src.y = 0;
    dest.x = dest.y = 0;
    tilesize[0] = tilesize[1] = 32;
}

Map::Map() {
    loadTiles();
    initMembers();
}

Map::Map(const short unsigned int width, const short unsigned int height){
    loadTiles();
    initMembers();
    tilesize[0] = width;
    tilesize[1] = height;
}

void Map::TileSize(const short unsigned int width, const short unsigned int height){
    tilesize[0] = width;
    tilesize[1] = height;
}

void Map::loadMap(const std::string filename) {
    map = readcsv_vec("..//testmap.txt", 1);
    // int type = 0;
    // for (int row = 0; row<map.size(); row++) {
        // for (int col = 0; col<map[row].size(); col++) {
            // type = map[row][col];
            // printf("%d\n", type);
        // }
    // }
}

void Map::drawMap() {
    int type = 0;
    for (int row = 0; row<map.size(); row++) {
        for (int col = 0; col<map[row].size(); col++) {
            type = map[row][col];
            
            dest.x = col * tilesize[0];
            dest.y = row * tilesize[1];
            // printf("%d %d\n", col, row);
            
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