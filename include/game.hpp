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
    std::unordered_map<uint8_t, entityx::Entity> menus;
    std::vector<entityx::Entity> buttons;
    std::unordered_map<uint8_t, std::vector<uint8_t>> menuoptions;
    Narrative narrative;

    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    MouseInputMap mouseInputMap;

    bool ismouse = false;
    bool iscursor = true;
    bool isrunning;

    SDL_Window * window;
    Settings settings;
    tinymt32_t tinymt32;
    Convoy convoy;
    Camp camp;

    int8_t chapter = -1;
    uint16_t state;
public:
    static SDL_Renderer * renderer;
    static TTF_Font * font;

    std::unordered_map<int16_t, Unit> party;
    std::unordered_map<int16_t, Weapon> weapons;

    Game();
    Game(Settings in_settings);
    ~Game();

    uint8_t getURN();

    void init();
    void initSystems();
    void updateSystems();

    tinymt32_t & getTinymt32();

    void setChapter(int8_t in_chapter);
    int8_t getChapter();
    entityx::ComponentHandle<Map> getMap();
    void loadMap(const int16_t in_map_index);
    void unloadMap();
    void putPConMap(std::vector<int16_t> in_units, std::vector<std::vector<int16_t>> in_pos_list);
    void initParty();
    void loadUnits(std::vector<int16_t> to_load);
    void unloadUnits(std::vector<int16_t> to_unload);
    void loadMapArrivals();

    void findDefenders(entityx::Entity in_attacker_ent);

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
    std::unordered_map<int16_t, Weapon> * getWeapons();
    void setMousestate(const int8_t in_menu);

    bool isRunning();
    void stopRunning();

    std::unordered_map<int16_t, Unit> getParty();

    void setCamp(Camp in_camp);
    Camp getCamp();
    void setConvoy(Convoy in_convoy);
    Convoy getConvoy();

    void setCursorstate(const int8_t in_menu);
    void setCursorlastpos(const int16_t x, const int16_t y);
    template <typename T> void loadTiles(std::vector<T> in_tiles);
    void moveUnit(entityx::Entity & cursor);
    void makeTurntransition();
    void setTurntransitiontext(int8_t in_army);

    std::vector<uint8_t> getMenuoptions(int8_t in_menu_index);
    void makeMenuoptions(int8_t in_menu_index);
    void setMenuoptions(int8_t in_menu_index, std::vector<uint8_t> in_options);
    void makeMenutext(int8_t in_menu_index);
    void makeMenu(int8_t in_menu_index);
    void makeButtons(int8_t in_menu_index, Point menu_pos);
    entityx::Entity makeButton(int8_t in_menu_option);
    void killButtons();
    std::vector<entityx::Entity> getButtons();
    entityx::Entity * getMenu(int8_t in_menu_index);
    void killMenu(int16_t index);
    void hideMenu(int16_t index);
    void showMenu(int16_t index);

    void loadItem(int16_t in_item);
    void loadItems(std::vector<int16_t> in_items);

    void startTurnSystem();

    void update();
    void clean();
    bool checkRate(int16_t crit_rate, int16_t mode = 2);
    bool * checkHitCrit(int16_t hit_rate, int16_t crit_rate, int16_t mode = GAME::RN::GAUSSIAN);

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

    void setState(const int16_t new_state);
    uint16_t getState();

    void deleteSave(const int16_t delete_ind);
    void copySave(const int16_t from_ind, const int16_t to_ind);
    void saveXML(const int16_t save_ind);
    void loadXML(const int16_t save_ind);

    void saveJSON(const int16_t save_ind);
    void loadJSON(const int16_t save_ind);

    void configure(entityx::EventManager & event_manager);
    void update(entityx::TimeDelta dt);
};

#endif /* GAME_HPP */