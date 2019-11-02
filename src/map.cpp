#include "TextureManager.hpp"
#include "map.hpp"
#include "parser.hpp"

void Map::loadTiles() {
    dirt = TextureManager::LoadTexture("..//assets//dirt.png");
    grass = TextureManager::LoadTexture("..//assets//grass.png");
    water = TextureManager::LoadTexture("..//assets//water.png");
}

Map::Map() {
    loadTiles();
    loadMap(readcsv_vec("..//testmap.txt", 1));
    src.x = src.y = 0;
    dest.x = dest.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
}

void Map::loadMap(std::vector<std::vector<int>> arr) {
    map = arr;
}

void Map::drawMap() {
    int type = 0;
    for (int row = 0; row<map.size(); row++) {
        for (int col = 0; col<map[row].size(); col++) {
            type = map[row][col];
            
            dest.x = col * tile_size[0];
            dest.y = row * tile_size[1];
            
            
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