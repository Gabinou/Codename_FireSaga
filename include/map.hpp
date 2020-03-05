#ifndef MAP_HPP
#define MAP_HPP

#include "ECS.hpp"
#include "SDL2/SDL.h"
#include "shared.hpp"
#include "parser.hpp"
#include "filesystem.hpp"
#include "tile.hpp"

using Entity_ptr_matrix = std::vector<std::vector<Entity *>>;

class Map {
    private:
        short unsigned int tilesize[2];
        short unsigned int frames = 10, speed = 50;
        bool show_overlay = false;
        bool show_danger = false;
        bool show_grid = false;
        bool made_entitymap = false;

        std::vector<short unsigned int> enemy_inds;

        std::vector<std::vector<short int>> moveoverlay, attackoverlay, healoverlay;
        std::vector<std::vector<short int>> dangeroverlay;
        std::vector<std::vector<short int>> tilemap;
        std::vector<std::vector<short int>> tileupdate;

        SDL_Rect srcrect, destrect;

        unsigned char overlay_mode = 0;
        unsigned char danger_mode = 0;

        std::unordered_map<short int, SDL_Texture *> textures;
        std::vector<short int> unique_textures;
        std::vector<SDL_Texture *> overlays{NULL, NULL, NULL};
        std::vector<SDL_Texture *> dangers{NULL, NULL};

        Entity_ptr_matrix entitymap;

    public:
        SDL_Renderer * renderer;
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

        std::vector<std::vector<short int>> getTilemap();
        void setTilemap(const std::vector<std::vector<short int>> in_tilemap);
        std::vector<std::vector<short int>> makeMvtCostmap(const unsigned char unitmovetype);

        void drawMap(SDL_Renderer * renderer);
        void makeEntitymap(const short unsigned int row_size, const short unsigned int col_size);

        void setTilesize(const short int unsigned width, const short int unsigned height);
        short unsigned int * getTilesize() const;

        void setTile(const short unsigned int x, const short unsigned int y, Entity * in_entity);
        void removeTile(const short unsigned int x, const short unsigned int y);
        void moveTile(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y);
        Entity * getTile(const short unsigned int x, const short unsigned int y);

        void setOverlay(const unsigned char in_mode, std::vector<std::vector<short int>> in_map);
        void clearOverlays();

        void setOverlaymode(const unsigned char in_mode);
        void showOverlay();
        void hideOverlay();

        void loadGrid();
        void showGrid();
        void hideGrid();

        void setEnemies(const std::vector<short unsigned int> in_enemy_inds);
        void loadEnemyinds(const short unsigned int in_chap_ind);
        void addEnemy(const short unsigned int in_enemy_ind);
        std::vector<short unsigned int> getEnemies();

        void setDangermode(const unsigned char in_mode);
        void showDanger();
        void hideDanger();
        void addDanger(const std::vector<std::vector<short int>> in_danger);
        void subDanger(const std::vector<std::vector<short int>> in_danger);

};

extern std::vector<std::vector<short int>> (*chapTilemaps[40])();
extern std::vector<short unsigned int> (*chapEnemyinds[40])();

std::vector<short unsigned int> testEnemies();
std::vector<std::vector<short int>> testTilemap();
std::vector<std::vector<short int>> chap2Tilemap();
std::vector<std::vector<short int>> chap3Tilemap();
std::vector<std::vector<short int>> chap4Tilemap();
std::vector<std::vector<short int>> chap5Tilemap();
std::vector<std::vector<short int>> chap6Tilemap();
std::vector<std::vector<short int>> chap7Tilemap();
std::vector<std::vector<short int>> chap8Tilemap();
std::vector<std::vector<short int>> chap9Tilemap();
std::vector<std::vector<short int>> chap10Tilemap();
std::vector<std::vector<short int>> chap11Tilemap();
std::vector<std::vector<short int>> chap12Tilemap();
std::vector<std::vector<short int>> chap13Tilemap();
std::vector<std::vector<short int>> chap14Tilemap();
std::vector<std::vector<short int>> chap15Tilemap();
std::vector<std::vector<short int>> chap16Tilemap();
std::vector<std::vector<short int>> chap17Tilemap();
std::vector<std::vector<short int>> chap18Tilemap();
std::vector<std::vector<short int>> chap19Tilemap();
std::vector<std::vector<short int>> chap20Tilemap();
std::vector<std::vector<short int>> chap21Tilemap();
std::vector<std::vector<short int>> chap22Tilemap();
std::vector<std::vector<short int>> chap23Tilemap();
std::vector<std::vector<short int>> chap24Tilemap();
std::vector<std::vector<short int>> chap25Tilemap();

#endif /* MAP_HPP */