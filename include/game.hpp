#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <entityx/entityx.h>
#include "enums.hpp"
#include "structs.hpp"
#include "map.hpp"
#include "weapon.hpp"
#include "utilities.hpp"
#include "convoy.hpp"
#include "pathfinding.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include "script.hpp"
#include "probability.hpp"
#include <stack>
#include <vector>
#include <unordered_map>

class Game : public entityx::EntityX, public entityx::Receiver<Game> {

private:
    entityx::Entity mapEntx;
    entityx::Entity cursorx;
    short int cursor_lastpos[2] = {6, 6};
    entityx::ComponentHandle<Map> mapx;
    std::unordered_map<unsigned char, entityx::Entity> menus;
    std::unordered_map<unsigned char, std::vector<unsigned char>> menuoptions;
    Narrative narrative;

    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    SDL_Window * window;
    Settings settings;
    tinymt32_t tinymt;
    Convoy convoy;

    short unsigned int state;

    std::stack<int> unit_entities;
    bool isRunning;
    SDL_Event event;

    std::unordered_map<int, Unit> units;
    std::unordered_map<int, Unit> party;
    std::unordered_map<int, Unit> enemies;
    std::unordered_map<int, Weapon> weapons;

public:
    static SDL_Renderer * renderer;
    static TTF_Font * font;

    Game();
    ~Game();

    void init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen);
    entityx::ComponentHandle<Map> getMap();
    void loadMap(const int in_map_index);
    void unloadMap();
    void loadUnitEntities(std::vector<short unsigned int> names, std::vector<std::vector<int>> positions_list);
    void loadUnits(unsigned char in_chap);
    void loadUnits(std::vector<short int> to_load);
    void unloadUnits(std::vector<short int> to_unload);
    void loadMapArrivals();
    void loadCursor();
    void setCursorstate(const short unsigned int new_state);
    void setCursorlastpos(const short int x, const short int y);
    void unloadCursor();
    template <typename T> void loadTiles(std::vector<T> in_tiles);
    void moveUnit(entityx::Entity & cursor);

    std::vector<std::string> menuoptions2str(std::vector<unsigned char> in_options);
    std::vector<unsigned char> getMenuoptions(unsigned char in_menu_index);
    void makeUnitmenuoptions();
    void setMenuoptions(unsigned char in_menu_index, std::vector<unsigned char> in_options);
    void updateMenu(unsigned char in_menu_index);
    void makeMenu(unsigned char in_menu_index);
    entityx::Entity * getMenu(unsigned char in_menu_index);
    entityx::Entity * getUnitmenu();
    void killMenu(short unsigned int index);
    void hideMenu(short unsigned int index);
    void showMenu(short unsigned int index);

    void startTurns();

    void handleEvents();
    void update();
    void render();
    void clean();
    void fight(Unit * attacker, Unit * defender);
    void attack(Unit * attacker, Unit * defender);
    bool checkRate(int crit_rate, short unsigned int mode = 2);
    bool * checkHitCrit(int hit_rate, int crit_rate, short unsigned int mode = GAME::RN::GAUSSIAN);

    void makeFPSEntity();


    Settings * getSettings();
    void setSettings(Settings);

    KeyboardInputMap getKeyboardInputMap();
    void setKeyboardInputMap(const KeyboardInputMap in_keyboardInputMap);
    GamepadInputMap getGamepadInputMap();
    void setGamepadInputMap(const GamepadInputMap in_gpmap);

    void setState(const short unsigned int new_state);
    short unsigned int getState();

    void saveXML(const short int save_ind);
    void deleteSaveXML(const short int delete_ind);
    void copySaveXML(const short int from_ind, const short int to_ind);
    void loadXML(const short int save_ind);

    bool running();
    void configure(entityx::EventManager & event_manager);
    void update(entityx::TimeDelta dt);
};

#endif /* GAME_HPP */