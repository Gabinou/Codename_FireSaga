
#include "turnsystemx.hpp"

TurnSystemx::TurnSystemx() {

}

TurnSystemx::TurnSystemx(Game * in_game) {
    SDL_Log("Adding TurnSystemx");
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

uint8_t TurnSystemx::getArmyfront() {
    return (armies.front());
}

void TurnSystemx::addArmies(std::vector<uint8_t> in_armies) {
    for (int16_t i = 0; i < in_armies.size(); i++) {
        armies.push(in_armies[i]);
    }
}

void TurnSystemx::addArmy(uint8_t in_army) {
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
    event_manager->emit<turnBegin>(armies.front());
}

void TurnSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    // entityx::ComponentHandle<Unit> unit;
    if (check_frames) {
        frames++;
    }

}