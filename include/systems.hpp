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

class RenderSystemx: public entityx::System<RenderSystemx>, public entityx::Receiver<RenderSystemx> {
private:
    SDL_Renderer * renderer = NULL;
    short unsigned int * tilesize;
    short unsigned int linespace;
    entityx::ComponentHandle<Map> map;
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
    void receive(const unitHealEvent & heal);
    void receive(const unitDieEvent & death);
    void receive(const unitWaitEvent & wait);
    void receive(const unitRefreshEvent & refresh);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class UnitSystemx: public entityx::System<UnitSystemx>, public entityx::Receiver<UnitSystemx> {
private:
    Game * game;
    Settings * settings;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
    entityx::Entity * unitmenux;
    std::vector<unsigned char> * unitmenuoptions;
    entityx::Entity selected; // Cursor can select other things than units? Shops? doors? other things?
public:
    UnitSystemx();
    UnitSystemx(Game * in_game);

    void killMenu(short unsigned int index);
    void hideMenu(short unsigned int index);
    void showMenu(short unsigned int index);
    void makeUnitmenu(entityx::Entity & setter);
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const unitHealEvent & heal);
    void receive(const unitDieEvent & death);
    void receive(const unitWaitEvent & wait);
    void receive(const unitRefreshEvent & refreshes);
    void receive(const unitSelect & selected);
    void receive(const unitMove & move);
    void receive(const unitMenu & menu);
    void receive(const unitMap & map);
    void receive(const unitDanger & danger);
    void receive(const unitmenuSelect & select);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class MapSystemx: public entityx::System<MapSystemx>, public entityx::Receiver<MapSystemx> {
private:
    std::queue<unsigned char> armies;
    entityx::Entity * mapmenux;
public:
    explicit MapSystemx();

    void addArmy(unsigned char in_army);
    void addArmies(std::vector<unsigned char> in_armies);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const mapMenu & end);

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
    struct Candidate {
        entityx::Entity ent;
    };
    std::vector<Candidate> candidates;
public:
    ControlSystemx();
    ControlSystemx(Game * in_game);

    void configure(entityx::EventManager & in_events);

    void receive(const inputMenuRight & menuright);
    void receive(const inputMenuLeft & menuleft);
    void receive(const inputMinimap & minimap);
    void receive(const inputFaster & faster);
    void receive(const inputPause & pause);
    void receive(const inputCancel & cancel);
    void receive(const inputAccept & accept);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* SYSTEMS_HPP */