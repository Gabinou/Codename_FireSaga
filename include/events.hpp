#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <entityx/entityx.h>

struct beginTurnEvent {
    beginTurnEvent(entityx::Entity beginner) : beginner(beginner) {}

    entityx::Entity beginner;
    //Game? or AI ender.
};

struct endTurnEvent {
    endTurnEvent(entityx::Entity ender) : ender(ender) {}

    entityx::Entity ender;
    //Player cursor or AI ender.
};

struct unitRefreshEvent {
    unitRefreshEvent(entityx::Entity refresher) : refresher(refresher) {}

    entityx::Entity refresher;
};

struct unitWaitEvent {
    unitWaitEvent(entityx::Entity ender) : ender(ender) {}

    entityx::Entity ender;
};

struct unitHealEvent {
    //Should those be Component hanldes?
    unitHealEvent(entityx::Entity healer, entityx::Entity patient) : healer(healer), patient(patient) {}

    entityx::Entity healer, patient;
};

struct unitDieEvent {
    unitHealEvent(entityx::Entity victim, entityx::Entity killer) : killer(killer), victim(victim) {}

    entityx::Entity killer, victim;
}


#endif /* EVENTS_HPP */