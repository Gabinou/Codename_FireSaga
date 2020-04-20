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
public:
    RenderSystemx();
    RenderSystemx(SDL_Renderer * in_renderer);

    void setRenderer(SDL_Renderer * in_renderer);
    void setMap(entityx::ComponentHandle<Map> in_map);

    void configure(entityx::EventManager & event_manager);
    void receive(const beginTurnEvent & begin);
    void receive(const endTurnEvent & end);
    void receive(const unitHealEvent & heal);
    void receive(const unitDieEvent & death);
    void receive(const unitWaitEvent & wait);
    void receive(const unitRefreshEvent & refresh);
    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class UnitSystemx: public entityx::System<UnitSystemx>, public entityx::Receiver<UnitSystemx> {
private:

public:
    UnitSystemx();
    void configure(entityx::EventManager & event_manager);
    void receive(const beginTurnEvent & begin);
    void receive(const endTurnEvent & end);
    void receive(const unitHealEvent & heal);
    void receive(const unitDieEvent & death);
    void receive(const unitWaitEvent & wait);
    void receive(const unitRefreshEvent & refreshes);
    void receive(const unitMove & move);
    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

class TurnSystemx: public entityx::System<TurnSystemx>, public entityx::Receiver<TurnSystemx> {
private:
    std::queue<unsigned char> armies;

public:
    explicit TurnSystemx();

    void addArmy(unsigned char in_army);

    void configure(entityx::EventManager & event_manager);
    void receive(const beginTurnEvent & begin);
    void receive(const endTurnEvent & end);

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
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;
    struct Candidate {
        entityx::Entity ent;
    };
    std::vector<Candidate> candidates;
public:
    ControlSystemx();
    ControlSystemx(Game * in_game);

    void configure(entityx::EventManager & event_manager);

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