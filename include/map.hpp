#ifndef MAP_HPP
#define MAP_HPP

#include "game.hpp"
#include "SDL2/SDL.h"

class Map {
    
    public:
        void loadTiles();
<<<<<<< HEAD
        void initMembers();
=======
        void initVars();
        Map(const short int width, const short int height);
>>>>>>> maptest
        Map();
        Map(const short unsigned int width, const short unsigned int height);
        ~Map();
<<<<<<< HEAD
        void TileSize(const short unsigned int width, const short unsigned int height);
        void loadMap(const std::vector<std::vector<int>> arr);
=======
        void TileSize(const short int width, const short int height);
        void loadMap(std::string filename);
>>>>>>> maptest
        void drawMap();
    
    private:
    
        SDL_Rect src, dest;
        
        SDL_Texture* dirt;    
        SDL_Texture* grass;    
        SDL_Texture* water;    

        int tilesize[2] = {32, 32};
        
        std::vector<std::vector<int>> map;


};


#endif /* MAP_HPP */