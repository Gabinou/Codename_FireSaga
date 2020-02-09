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

        void loadTiles();
        void initVars();
        void loadOverlays();
        void loadDanger();

        void loadTilemap(std::string filename);
        std::vector<std::vector<int>> getTilemap();
        std::vector<std::vector<int>> makeMvtCostmap(std::string unitmovetype);

        void drawMap(SDL_Renderer * renderer);
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

        void loadGrid();
        void showGrid();
        void hideGrid();

        void setEnemies(std::vector<Map_enemy> in_map_enemies);
        void addEnemy(Map_enemy in_map_enemy);
        std::vector<Map_enemy> getEnemies();

        void setDangermode(std::string in_mode);
        void showDanger();
        void hideDanger();
        void addDanger(std::vector<std::vector<int>> in_danger);
        void subDanger(std::vector<std::vector<int>> in_danger);

    private:
        char index;
        int tilesize[2];
        int frames = 10, speed = 50;
        bool show_overlay = false;
        bool show_danger = false;
        bool show_grid = false;
        bool made_entitymap = false;

        std::vector<Map_enemy> map_enemies;

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

// std::vector<std::vector<Map_enemy>(*)()> chapter_enemy_funcs;

// std::vector<Map_enemy> Chap1_mapEnemies();
// std::vector<Map_enemy> Chap2_mapEnemies();
// chapter_enemy_funcs.push_back(&Chap1_mapEnemies);

extern std::vector<Map_enemy> (*mapEnemiesChap[25])();
extern std::vector<Map_enemy> (*mapEnemiesGaiden[25])();


#endif /* MAP_HPP */