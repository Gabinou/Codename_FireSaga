#ifndef MAP_HPP
#define MAP_HPP

#include "game.hpp"
#include "SDL2/SDL.h"

class Map {
    
    public:
        Map();
        ~Map();
    
        void loadMap(int arr[20][25]);
        void loadMapVec(std::vector<std::vector<int>> arr);
        void drawMap();
        void drawMapVec();
    
    private:
    
        SDL_Rect src, dest;
        
        SDL_Texture* dirt;    
        SDL_Texture* grass;    
        SDL_Texture* water;    

        int map[20][25];
        int tile_size[2] = {32, 32};
        std::vector<std::vector<int>> map_vec;


};


#endif /* MAP_HPP */