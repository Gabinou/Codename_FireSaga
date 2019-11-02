#ifndef MAP_HPP
#define MAP_HPP

#include "game.hpp"
#include "SDL2/SDL.h"

class Map {
    
    public:
        void loadTiles();
        void initVars();
        Map(const short int width, const short int height);
        Map();
        ~Map();
        void TileSize(const short int width, const short int height);
        void loadMap(std::string filename);
        void drawMap();
    
    private:
    
        SDL_Rect src, dest;
        
        SDL_Texture* dirt;    
        SDL_Texture* grass;    
        SDL_Texture* water;    

        int tile_size[2] = {32, 32};
        std::vector<std::vector<int>> map;


};


#endif /* MAP_HPP */