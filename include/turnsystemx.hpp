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
    std::queue<unsigned char> armies;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
    entityx::Entity transition;
    bool check_frames = false;
    short unsigned int frames = 0;
public:
    TurnSystemx();
    TurnSystemx(Game * in_game);

    void makeTurntransition();
    void setTurntransitiontext(unsigned char in_army);
    void addArmy(unsigned char in_army);
    void addArmies(std::vector<unsigned char> in_armies);
    unsigned char getArmyfront();
    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);
    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};


#endif /* TURNSYSTEMX_HPP */
