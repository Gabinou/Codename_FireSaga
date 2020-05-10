#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <unordered_map>
#include "entityx/entityx.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "enums.hpp"
#include "structs.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include "convoy.hpp"
#include "pathfinding.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include "probability.hpp"
#include "sprite.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "mousecontroller.hpp"
#include "touchpadcontroller.hpp"

class Game : public entityx::EntityX, public entityx::Receiver<Game> {

private:
    entityx::Entity mapEntx;
    entityx::Entity cursorx;
    entityx::Entity transition;
    entityx::Entity fps;

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

    unsigned int previous_mouse;

    short unsigned int state;

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
    Game(Settings in_settings);
    ~Game();

    void init();
    entityx::ComponentHandle<Map> getMap();
    void loadMap(const int in_map_index);
    void unloadMap();
    void loadMapUnits(std::vector<short unsigned int> in_units, std::vector<std::vector<int>> in_pos_list);
    void makeUnits(unsigned char in_chap);
    void makeUnits(std::vector<short int> to_load);
    void unmakeUnits(std::vector<short int> to_unload);
    void loadMapArrivals();
    void loadCursor();
    void setCursorstate(const unsigned char in_menu);
    void setCursorlastpos(const short int x, const short int y);
    void unloadCursor();
    template <typename T> void loadTiles(std::vector<T> in_tiles);
    void moveUnit(entityx::Entity & cursor);
    void makeTurntransition();
    void setTurntransitiontext(unsigned char in_army);

    std::vector<unsigned char> getMenuoptions(unsigned char in_menu_index);
    void makeMenuoptions(unsigned char in_menu_index);
    void setMenuoptions(unsigned char in_menu_index, std::vector<unsigned char> in_options);
    void makeMenutext(unsigned char in_menu_index);
    void makeMenu(unsigned char in_menu_index);
    entityx::Entity * getMenu(unsigned char in_menu_index);
    void killMenu(short unsigned int index);
    void hideMenu(short unsigned int index);
    void showMenu(short unsigned int index);

    void startTurnSystem();

    void handleEvents();
    void update();
    void render();
    void clean();
    void fight(Unit * attacker, Unit * defender);
    void attack(Unit * attacker, Unit * defender);
    bool checkRate(int crit_rate, short unsigned int mode = 2);
    bool * checkHitCrit(int hit_rate, int crit_rate, short unsigned int mode = GAME::RN::GAUSSIAN);

    void makeFPS();
    entityx::Entity * getFPS();

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