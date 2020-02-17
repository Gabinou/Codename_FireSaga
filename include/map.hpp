#ifndef MAP_HPP
#define MAP_HPP

#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"
#include "tile.hpp"

using Entity_ptr_matrix = std::vector<std::vector<Entity *>>;

class Map {

    public:
        Map();
        ~Map();
        Map(const short unsigned int width, const short unsigned int height);

        void loadTiletextures();
        void initVars();
        void loadOverlays();
        void loadDanger();

        void loadTilemap(const std::string filename);
        void loadTilemap(const short unsigned int in_map_index);
        void postTilemap();

        std::vector<std::vector<int>> getTilemap();
        void setTilemap(const std::vector<std::vector<int>> in_tilemap);
        std::vector<std::vector<int>> makeMvtCostmap(const std::string unitmovetype);

        void drawMap(SDL_Renderer * renderer);
        void makeEntitymap(const short unsigned int row_size, const short unsigned int col_size);

        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;

        void setTile(const short unsigned int x, const short unsigned int y, Entity * in_entity);
        void removeTile(const short unsigned int x, const short unsigned int y);
        void moveTile(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y);
        Entity * getTile(const short unsigned int x, const short unsigned int y);

        void setOverlay(const std::string in_type, std::vector<std::vector<int>> in_map);
        void clearOverlays();

        void setOverlaymode(const std::string in_mode);
        void showOverlay();
        void hideOverlay();

        void loadGrid();
        void showGrid();
        void hideGrid();

        void setEnemies(const std::vector<std::string> in_enemy_names);
        void addEnemy(const std::string in_enemy_name);
        std::vector<std::string> getEnemies();

        void setDangermode(const std::string in_mode);
        void showDanger();
        void hideDanger();
        void addDanger(const std::vector<std::vector<int>> in_danger);
        void subDanger(const std::vector<std::vector<int>> in_danger);

    private:
        char index;
        int tilesize[2];
        int frames = 10, speed = 50;
        bool show_overlay = false;
        bool show_danger = false;
        bool show_grid = false;
        bool made_entitymap = false;

        std::vector<std::string> enemy_names;

        std::vector<std::vector<int>> moveoverlay, attackoverlay, healoverlay;
        std::vector<std::vector<int>> dangeroverlay;
        std::vector<std::vector<int>> tilemap;
        std::vector<std::vector<int>> tileupdate;

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

extern std::vector<std::vector<int>> (*chapTilemaps[40])();

std::vector<std::vector<int>> testTilemap();
std::vector<std::vector<int>> chap2Tilemap();
std::vector<std::vector<int>> chap3Tilemap();
std::vector<std::vector<int>> chap4Tilemap();
std::vector<std::vector<int>> chap5Tilemap();
std::vector<std::vector<int>> chap6Tilemap();
std::vector<std::vector<int>> chap7Tilemap();
std::vector<std::vector<int>> chap8Tilemap();
std::vector<std::vector<int>> chap9Tilemap();
std::vector<std::vector<int>> chap10Tilemap();
std::vector<std::vector<int>> chap11Tilemap();
std::vector<std::vector<int>> chap12Tilemap();
std::vector<std::vector<int>> chap13Tilemap();
std::vector<std::vector<int>> chap14Tilemap();
std::vector<std::vector<int>> chap15Tilemap();
std::vector<std::vector<int>> chap16Tilemap();
std::vector<std::vector<int>> chap17Tilemap();
std::vector<std::vector<int>> chap18Tilemap();
std::vector<std::vector<int>> chap19Tilemap();
std::vector<std::vector<int>> chap20Tilemap();
std::vector<std::vector<int>> chap21Tilemap();
std::vector<std::vector<int>> chap22Tilemap();
std::vector<std::vector<int>> chap23Tilemap();
std::vector<std::vector<int>> chap24Tilemap();
std::vector<std::vector<int>> chap25Tilemap();

#endif /* MAP_HPP */