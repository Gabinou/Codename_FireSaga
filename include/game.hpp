#ifndef GAME_HPP
#define GAME_HPP

#include "shared.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "ECS.hpp"
#include "map.hpp"
#include "weapon.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include "script.hpp"

class Game {

    private:
        Map * mapp;
        Entity & cursor = manager.addEntity();
        KeyboardInputMap keyboardInputMap;
        GamepadInputMap gamepadInputMap;
        SDL_Window * window;
        Settings settings;

        void moveUnit(Entity & cursor);

        void killMenu(std::string name);
        void makeUnitmenu(Entity & setting_entity);

        //TO BE IMPLEMENTED:
        void makeDefendermenu(Entity & setting_entity);
        void makeWeaponmenu(Entity & setting_entity);
        void makeOptionsmenu(Entity & setting_entity);
        void makeSmallmap(Entity & setting_entity);
        void makeCombatindow(Unit * ally, Unit * enemy);

        std::string state;
        // std::stack <int> menu_entities; // I think stacks are bad. I don't want to only be able to delete menus from the top down. Sometimes I want to eliminate menus. I guess.
        std::unordered_map<std::string, int> menus;
        std::stack <int> unit_entities;
        bool isRunning;
        SDL_Event event;

    public:
        static SDL_Renderer * renderer;
        static TTF_Font * font;
        static Manager manager;

        Game();
        ~Game();

        void init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen);
        void loadMap(std::string filename);
        void loadUnits(std::vector<std::string> names, std::vector<std::string> asset_names, std::vector<std::vector<int>> positions_list);
        void loadMapEnemies();
        void loadCursor();
        void loadTiles(std::vector<int> in_tiles);

        void handleEvents();
        void update();
        void render();
        void clean();
        void fight(Unit * attacker, Unit * defender);
        void attack(Unit * attacker, Unit * defender);
        bool checkRate(int crit_rate, std::string mode = "double");
        bool * checkHitCrit(int hit_rate, int crit_rate, std::string mode = "gaussian");

        void makeFPSEntity();

        Settings getSettings();
        void setSettings(Settings);

        SDL_Renderer * getRenderer(Settings);

        KeyboardInputMap getKeyboardInputMap();
        void setKeyboardInputMap(const KeyboardInputMap in_keyboardInputMap);
        GamepadInputMap getGamepadInputMap();
        void setGamepadInputMap(const GamepadInputMap in_gpmap);

        void setState(Entity & in_entity, std::string new_state);
        void setState(const char * new_state);
        std::string getState();

        bool running();
};

#endif /* GAME_HPP */