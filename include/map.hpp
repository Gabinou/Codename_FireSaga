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
        ~Map();
        Map(const short unsigned int width, const short unsigned int height);
        void loadOverlays();
        void loadMap(std::string filename);
        void drawMap();
        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;

        void setOverlaymode(std::string in_mode);

        std::vector<std::vector<int>> get2D();
        void setTile(int x, int y, Entity * in_entity);
        void removeTile(int x, int y);
        void moveTile(int x, int y, int new_x, int new_y);
        Entity * getTile(int x, int y);

        void setList(std::string in_type, std::vector<std::vector<int>> in_list);
        void clearLists();

        void showOverlay();
        void hideOverlay();
    private:
        int tilesize[2];
        int frames = 10, speed = 50;
        bool show_overlay = false;

        std::vector<std::vector<int>> movelist, attacklist, heallist;

        std::vector<std::vector<int>> map2D;

        SDL_Rect srcrect, destrect;

        std::string overlay_mode = "attackhealmove";

        SDL_Texture * dirt;
        SDL_Texture * grass;
        SDL_Texture * water;
        SDL_Texture * move;
        SDL_Texture * attack;
        SDL_Texture * heal;
        std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
        std::string ss_looping = "linear";

        Entity_ptr_matrix tiles;

};


#endif /* MAP_HPP */