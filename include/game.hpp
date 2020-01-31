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

        void killUnitmenu();
        void makeUnitmenu(Entity & setting_entity);

        //TO BE IMPLEMENTED:
        void killDefendermenu();
        void makeDefendermenu(Entity & setting_entity);
        void killWeaponmenu();
        void makeWeaponmenu(Entity & setting_entity);
        void killOptionmenu();
        void makeOptionsmenu(Entity & setting_entity);
        void killSmallmap();
        void makeSmallmap(Entity & setting_entity);
        void killCombatindow();
        void makeCombatindow(Unit * ally, Unit * enemy);

        std::string state;
        std::stack <int> menu_entities;
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
        void loadCursor();

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