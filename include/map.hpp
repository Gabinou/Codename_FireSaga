#ifndef MAP_HPP
#define MAP_HPP

#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"
#include "game.hpp"

using Entity_ptr_matrix = std::vector<std::vector<Entity *>>;

class Map {

    public:
        void loadTiles();
        void initVars();
        Map();
        Map(const short unsigned int width, const short unsigned int height);
        // ~Map();
        void loadMap(std::string filename);
        void drawMap();

        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;
        void setTile(int x, int y, Entity * in_entity);
        // Entity * getTile(int x, int y);
        void getTile(int x, int y);

    private:
        int tilesize[2];

        std::vector<std::vector<int>> map;

        SDL_Rect srcrect, destrect;

        SDL_Texture * dirt;
        SDL_Texture * grass;
        SDL_Texture * water;

        Entity_ptr_matrix tiles;

};


#endif /* MAP_HPP */