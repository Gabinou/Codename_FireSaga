#ifndef MAP_HPP
#define MAP_HPP

#include "tilesize.hpp"
#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"
#include "game.hpp"

class Map: public Tilesize {

    public:
        void loadTiles();
        void initVars();
        Map();
        Map(const short unsigned int width, const short unsigned int height);
        // ~Map();
        void loadMap(std::string filename);
        void drawMap();
        Entity * getTile(int x, int y) {
            return (tiles[x][y]);
        }
        bool onTile(int x, int y) {
            printf("ontile");
            bool a = !(tiles[x][y]); // Doesn't work, but ONLY FOR KEYBOARD CONTROLLER!
            return (true);
        }
        void setTile(int x, int y, Entity * ptr) {
            tiles[x][y] = ptr;
            getTile(0, 0);
            printf("settile");
        }


    private:

        SDL_Rect srcrect, destrect;

        SDL_Texture * dirt;
        SDL_Texture * grass;
        SDL_Texture * water;

        std::vector<std::vector<int>> map;

        Entity *** tiles;

};


#endif /* MAP_HPP */