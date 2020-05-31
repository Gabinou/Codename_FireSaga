#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <unordered_map>
#include "entityx/entityx.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "enums.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "structs.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include "convoy.hpp"
#include "camp.hpp"
#include "pathfinding.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "linalg.hpp"
#include "probability.hpp"
#include "sprite.hpp"
#include "menu.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "mousecontroller.hpp"
#include "touchpadcontroller.hpp"

class Game : public entityx::EntityX, public entityx::Receiver<Game> {

private:
    entityx::Entity mapEntx;
    entityx::Entity cursorx;
    entityx::Entity mousex;
    entityx::Entity transition;
    entityx::Entity highlight;
    entityx::Entity shadow;
    entityx::Entity fps;

    Point cursor_lastpos = {6, 6};
    entityx::ComponentHandle<Map> mapx;
    std::unordered_map<unsigned char, entityx::Entity> menus;
    std::unordered_map<unsigned char, std::vector<unsigned char>> menuoptions;
    Narrative narrative;

    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    MouseInputMap mouseInputMap;

    bool ismouse = false;
    bool iscursor = true;
    bool isrunning;

    SDL_Window * window;
    Settings settings;
    tinymt32_t tinymt;
    Convoy convoy;
    Camp camp;

    char chapter = -1;

    short unsigned int state;

    std::unordered_map<short int, Unit> party;
    std::unordered_map<short int, Weapon> weapons;

public:
    static SDL_Renderer * renderer;
    static TTF_Font * font;

    Game();
    Game(Settings in_settings);
    ~Game();

    void init();
    void initSystems();
    void updateSystems();

    void setChapter(char in_chapter);
    char getChapter();
    entityx::ComponentHandle<Map> getMap();
    void loadMap(const int in_map_index);
    void unloadMap();
    void putPConMap(std::vector<short int> in_units, std::vector<std::vector<int>> in_pos_list);
    void initParty();
    void loadUnits(std::vector<short int> to_load);
    void unloadUnits(std::vector<short int> to_unload);
    void loadMapArrivals();

    void loadHighlight();
    void unloadHighlight();

    void loadCursor();
    void unloadCursor();
    void enableCursorx();
    entityx::Entity * getCursorx();
    void disableCursorx();
    bool isCursor();

    SDL_Window * getWindow();

    void loadMouse();
    void unloadMouse();
    void enableMousex();
    void disableMousex();
    entityx::Entity * getMousex();
    bool isMouse();
    std::unordered_map<short int, Weapon> * getWeapons();
    void setMousestate(const char in_menu);

    bool isRunning();
    void stopRunning();

    std::unordered_map<short int, Unit> getParty();

    void setCamp(Camp in_camp);
    Camp getCamp();
    void setConvoy(Convoy in_convoy);
    Convoy getConvoy();

    void setCursorstate(const char in_menu);
    void setCursorlastpos(const short int x, const short int y);
    template <typename T> void loadTiles(std::vector<T> in_tiles);
    void moveUnit(entityx::Entity & cursor);
    void makeTurntransition();
    void setTurntransitiontext(char in_army);

    std::vector<unsigned char> getMenuoptions(char in_menu_index);
    void makeMenuoptions(char in_menu_index);
    void setMenuoptions(char in_menu_index, std::vector<unsigned char> in_options);
    void makeMenutext(char in_menu_index);
    void makeMenu(char in_menu_index);
    entityx::Entity * getMenu(char in_menu_index);
    void killMenu(short int index);
    void hideMenu(short int index);
    void showMenu(short int index);

    void loadItem(short int in_item);
    void loadItems(std::vector<short int> in_items);

    void startTurnSystem();

    void update();
    void clean();
    void fight(Unit * attacker, Unit * defender);
    void attack(Unit * attacker, Unit * defender);
    bool checkRate(int crit_rate, short int mode = 2);
    bool * checkHitCrit(int hit_rate, int crit_rate, short int mode = GAME::RN::GAUSSIAN);

    void makeFPS();
    entityx::Entity * getFPS();

    Settings * getSettings();
    void setSettings(Settings);

    KeyboardInputMap getKeyboardInputMap();
    void setKeyboardInputMap(const KeyboardInputMap in_keyboardInputMap);
    GamepadInputMap getGamepadInputMap();
    void setGamepadInputMap(const GamepadInputMap in_gpmap);
    MouseInputMap getMouseInputMap();
    void setMouseInputMap(const MouseInputMap in_gpmap);

    void setState(const short int new_state);
    short unsigned int getState();

    void deleteSave(const short int delete_ind);
    void copySave(const short int from_ind, const short int to_ind);
    void saveXML(const short int save_ind);
    void loadXML(const short int save_ind);

    void saveJSON(const short int save_ind);
    void loadJSON(const short int save_ind);

    void configure(entityx::EventManager & event_manager);
    void update(entityx::TimeDelta dt);
};

#endif /* GAME_HPP */