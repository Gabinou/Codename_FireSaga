#ifndef MAP_HPP
#define MAP_HPP

#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"
#include "game.hpp"
#include "tile.hpp"

using Entity_ptr_matrix = std::vector<std::vector<Entity *>>;

class Map {

    public:
        Map();
        ~Map();
        Map(const short unsigned int width, const short unsigned int height);
        void loadTiles();
        void initVars();
        void loadOverlays();
        void loadTilemap(std::string filename);
        void loadTexturemap(std::string filename);
        void drawMap();
        void makeEntitymap(int row_size, int col_size);

        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;

        std::vector<std::vector<int>> getTilemap();
        void setTile(int x, int y, Entity * in_entity);
        void removeTile(int x, int y);
        void moveTile(int x, int y, int new_x, int new_y);
        Entity * getTile(int x, int y);

        void setList(std::string in_type, std::vector<std::vector<int>> in_list);
        void clearLists();

        void setOverlaymode(std::string in_mode);
        void showOverlay();
        void hideOverlay();
        std::vector<std::vector<int>> makeMvtCostmap(std::string unitmovetype);
    private:
        int tilesize[2];
        int frames = 10, speed = 50;
        bool show_overlay = false;
        bool made_entitymap = false;

        std::vector<std::vector<int>> movelist, attacklist, heallist;
        std::vector<std::vector<int>> tilemap;
        std::vector<std::vector<int>> texturemap;

        SDL_Rect srcrect, destrect;

        std::string overlay_mode = "";

        SDL_Texture * dirt;
        SDL_Texture * grass;
        SDL_Texture * water;
        SDL_Texture * tree;
        SDL_Texture * move;
        SDL_Texture * attack;
        SDL_Texture * heal;
        std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
        std::string ss_looping = "linear";

        Entity_ptr_matrix entitymap;
};


#endif /* MAP_HPP */