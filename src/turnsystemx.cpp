
#include "turnsystemx.hpp"

TurnSystemx::TurnSystemx() {
}

TurnSystemx::TurnSystemx(Game * in_game) {
    SDL_Log("Adding TurnSystemx.");
    game = in_game;
    updateMap();
}


void TurnSystemx::updateMap() {
    mapx = game->getMap();
}

void TurnSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
        // tilesize = mapx->getTilesize();
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}


void TurnSystemx::addArmies(std::vector<unsigned char> in_armies) {
    for (short int i = 0; i < in_armies.size(); i++) {
        armies.push(in_armies[i]);
    }
}

void TurnSystemx::addArmy(unsigned char in_army) {
    armies.push(in_army);
}

void TurnSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<turnEnd>(*this);
}

void TurnSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event");
    event_manager->emit<refreshUnits>(armies.front());
    event_manager->emit<switchControl>(armies.front());
}

void TurnSystemx::receive(const turnEnd & end) {
    SDL_Log("Received turnEnd event");
    armies.push(armies.front());
    armies.pop();
    event_manager->emit<turnBegin>();
}

void TurnSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    entityx::ComponentHandle<Unit> unit;
}