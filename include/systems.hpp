#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SDL2/SDL.h>
#include <queue>
#include "map.hpp"
#include "sprite.hpp"
#include "events.hpp"
#include "text.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include <entityx/entityx.h>

struct Controllers {
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

class RenderSystemx: public entityx::System<RenderSystemx>, public entityx::Receiver<RenderSystemx> {
    // I think the rendering system needs a way to buffer/transition animations. So that everything doesn't just appear suddenly.
private:
    SDL_Renderer * renderer = NULL;
    short unsigned int * tilesize;
    short unsigned int linespace;
    short int offset[2];
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Map> mapx;
public:
    RenderSystemx();
    RenderSystemx(SDL_Renderer * in_renderer);

    void setRenderer(SDL_Renderer * in_renderer);
    void setMap(entityx::ComponentHandle<Map> in_map);
    void setTilesize(const short int unsigned width, const short int unsigned height);
    void setLinespace(const short int unsigned in_linespace);
    SDL_Rect loopSprites(entityx::ComponentHandle<Sprite> in_sprite);
    void slideSprites(entityx::Entity * in_ent, short int * slidepos, short int * objectivepos);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const unitHeal & heal);
    void receive(const unitDie & death);
    void receive(const unitWait & wait);
    void receive(const unitRefresh & refresh);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class UnitSystemx: public entityx::System<UnitSystemx>, public entityx::Receiver<UnitSystemx> {
private:
    Game * game;
    Settings * settings;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
    entityx::Entity * unitmenux;
    short int old_position[2];
    std::vector<unsigned char> unitmenuoptions;
    entityx::Entity selected;
public:
    UnitSystemx();
    UnitSystemx(Game * in_game);

    void killMenu(short unsigned int index);
    void hideMenu(short unsigned int index);
    void showMenu(short unsigned int index);
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const unitHeal & heal);
    void receive(const unitDie & death);
    void receive(const unitWait & wait);
    void receive(const unitRefresh & refreshes);
    void receive(const unitTalk & talk);
    void receive(const unitRescue & rescue);
    void receive(const unitAttack & attack);
    void receive(const unitTrade & trade);
    void receive(const unitEscape & escape);
    void receive(const unitStaff & staff);
    void receive(const unitItems & item);
    void receive(const unitHover & hover);
    void receive(const unitDehover & dehover);
    void receive(const unitReturn & Return);
    void receive(const unitSelect & selected);
    void receive(const unitDeselect & selected);
    void receive(const unitMove & move);
    void receive(const unitMenu & menu);
    void receive(const unitMap & map);
    void receive(const unitDanger & danger);
    void receive(const unitmenuSelect & select);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class MenuSystemx: public entityx::System<MenuSystemx>, public entityx::Receiver<MenuSystemx> {
private:
    Game * game;
    Settings * settings;
    entityx::EventManager * event_manager;
    entityx::Entity * unitmenux;
    entityx::Entity * mapmenux;
    entityx::Entity selected;
    short int old_position[2];
    entityx::ComponentHandle<Map> mapx;
    std::vector<unsigned char> unitmenuoptions;
public:
    MenuSystemx();
    MenuSystemx(Game * in_game);

    // void killMenu(short unsigned int index);
    // void hideMenu(short unsigned int index);
    // void showMenu(short unsigned int index);
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);

    void configure(entityx::EventManager & event_manager);
    void receive(const unitMenu & menu);
    void receive(const unitSelect & menu);
    void receive(const mapMenu & menu);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class MapSystemx: public entityx::System<MapSystemx>, public entityx::Receiver<MapSystemx> {
private:
    Game * game;
    std::queue<unsigned char> armies;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
public:
    MapSystemx();
    MapSystemx(Game * in_game);

    void addArmy(unsigned char in_army);
    void addArmies(std::vector<unsigned char> in_armies);
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);
    void switchControl(unsigned char in_army);
    void refreshUnits(unsigned char in_army);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class ControlSystemx: public entityx::System<ControlSystemx>, public entityx::Receiver<ControlSystemx> {
private:
    short unsigned int * tilesize;
    SDL_Rect srcrect;
    SDL_Rect destrect;
    Game * game;
    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    entityx::EventManager * event_manager;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;
    entityx::ComponentHandle<Unit> selected;

public:
    ControlSystemx();
    ControlSystemx(Game * in_game);
    unsigned int getHeldbutton(Controllers in_controllers);
    entityx::Entity getInputent(Controllers in_controllers);
    void AIturn(unsigned char in_army);

    void configure(entityx::EventManager & in_events);
    void receive(const inputMenuRight & menuright);
    void receive(const inputMenuLeft & menuleft);
    void receive(const inputMinimap & minimap);
    void receive(const inputFaster & faster);
    void receive(const cursorMoved & moved);
    void receive(const inputPause & pause);
    void receive(const inputCancel & cancel);
    void receive(const inputAccept & accept);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* SYSTEMS_HPP */