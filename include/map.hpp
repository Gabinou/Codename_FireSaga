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
        void loadDanger();

        void loadTilemap(std::string filename);
        std::vector<std::vector<int>> getTilemap();
        std::vector<std::vector<int>> makeMvtCostmap(std::string unitmovetype);

        void drawMap();
        void makeEntitymap(int row_size, int col_size);

        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;

        void setTile(int x, int y, Entity * in_entity);
        void removeTile(int x, int y);
        void moveTile(int x, int y, int new_x, int new_y);
        Entity * getTile(int x, int y);

        void setMap(std::string in_type, std::vector<std::vector<int>> in_map);
        void clearMaps();

        void setOverlaymode(std::string in_mode);
        void showOverlay();
        void hideOverlay();

        void setDangermode(std::string in_mode);
        void showDanger();
        void hideDanger();
        void addDanger(std::vector<std::vector<int>> in_danger);
        void subDanger(std::vector<std::vector<int>> in_danger);

    private:
        int tilesize[2];
        int frames = 10, speed = 50;
        bool show_overlay = false;
        bool show_danger = false;
        bool made_entitymap = false;

        std::vector<std::vector<int>> movemap, attackmap, healmap;
        std::vector<std::vector<int>> dangermap;
        std::vector<std::vector<int>> tilemap;

        SDL_Rect srcrect, destrect;

        std::string overlay_mode = "";
        std::string danger_mode = "";

        std::unordered_map<int, SDL_Texture *> textures;
        std::vector<int> unique_textures;
        std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
        std::vector<SDL_Texture *> dangers{NULL, NULL};
        std::string ss_looping = "linear";


        Entity_ptr_matrix entitymap;
};


#endif /* MAP_HPP */