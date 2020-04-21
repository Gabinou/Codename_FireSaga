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
    entityx::Entity unitmenux;
    entityx::ComponentHandle<Map> mapx;
    entityx::Entity selected; // Cursor can select other things than units? Shops? doors? other things?

    Narrative narrative;

    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    SDL_Window * window;
    Settings settings;
    tinymt32_t tinymt;
    Convoy convoy;

    void moveUnit(entityx::Entity & cursor);

    void killMenu(short unsigned int index);
    void hideMenu(short unsigned int index);
    void showMenu(short unsigned int index);

    //TO BE IMPLEMENTED:
    void makeDefendermenu(entityx::Entity & setting_entity);
    void makeWeaponmenu(entityx::Entity & setting_entity);
    void makeOptionsmenu(entityx::Entity & setting_entity);
    void makeSmallmap(entityx::Entity & setting_entity);
    void makeCombatWindow(Unit * ally, Unit * enemy);

    short unsigned int state;

    std::vector<int> menus;
    std::stack<int> unit_entities;
    bool isRunning;
    SDL_Event event;

    std::unordered_map<int, Unit> units;
    std::unordered_map<int, Unit> party;
    std::unordered_map<int, Unit> enemies;
    std::unordered_map<int, Weapon> weapons;

public:
    static SDL_Renderer * renderer;
    // static Manager manager;
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
    void unloadCursor();
    template <typename T> void loadTiles(std::vector<T> in_tiles);

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

    void setState(entityx::Entity in_entity, short unsigned int new_state);
    void setState(const short unsigned int new_state);
    short unsigned int getState();

    void saveXML(const short int save_ind);
    void deleteSaveXML(const short int delete_ind);
    void copySaveXML(const short int from_ind, const short int to_ind);
    void loadXML(const short int save_ind);
    void testXMLMap();

    bool running();
    void configure(entityx::EventManager & event_manager);
    void update(entityx::TimeDelta dt);
};

#endif /* GAME_HPP */