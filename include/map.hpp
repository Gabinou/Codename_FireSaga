#ifndef MAP_HPP
#define MAP_HPP

#include "game.hpp"
#include "tilesize.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"

class Map: public Tilesize {

    public:
        void loadTiles();
        void initVars();
        Map();
        Map(const short unsigned int width, const short unsigned int height);
        ~Map();
        void loadMap(std::string filename);
        void drawMap();

    private:

        SDL_Rect srcrect, destrect;

        SDL_Texture * dirt;
        SDL_Texture * grass;
        SDL_Texture * water;

        std::vector<std::vector<int>> map;

};


#endif /* MAP_HPP */