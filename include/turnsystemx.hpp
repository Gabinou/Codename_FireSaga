#ifndef TURNSYSTEMX_HPP
#define TURNSYSTEMX_HPP

#include <SDL2/SDL.h>
#include <queue>
#include "map.hpp"
#include "events.hpp"
#include <entityx/entityx.h>

class TurnSystemx: public entityx::System<TurnSystemx>, public entityx::Receiver<TurnSystemx> {
private:
    Game * game;
    std::queue<uint8_t> armies;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
    entityx::Entity transition;
    bool check_frames = false;
    uint16_t frames = 0;
public:
    TurnSystemx();
    TurnSystemx(Game * in_game);

    void makeTurntransition();
    void setTurntransitiontext(uint8_t in_army);
    void addArmy(uint8_t in_army);
    void addArmies(std::vector<uint8_t> in_armies);
    uint8_t getArmyfront();
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);
    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};


#endif /* TURNSYSTEMX_HPP */
