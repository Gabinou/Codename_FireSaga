#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <entityx/entityx.h>

struct beginTurnEvent {
    beginTurnEvent(entityx::Entity beginner) : beginner(beginner) {}
    entityx::Entity beginner;
    //Game? or AI ender.
};

struct inputAccept {
    inputAccept(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputCancel {
    inputCancel(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputStats {
    inputStats(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputMenuRight {
    inputMenuRight(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputMenuLeft {
    inputMenuLeft(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputMinimap {
    inputMinimap(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputFaster {
    inputFaster(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputPause {
    inputPause(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputCancels {
    inputCancels(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputCancels {
    inputCancels(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct endTurnEvent {
    endTurnEvent(entityx::Entity ender) : ender(ender) {}
    entityx::Entity ender;
    //Player cursor or AI ender.
};

struct seizeEvent {
    seizeEvent(entityx::Entity victor) : victor(victor) {}
    entityx::Entity victor;
    //unit.
};

struct gameOver {
    gameOver(entityx::Entity victor) : victor(victor) {}
    entityx::Entity victor;
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
    unitDieEvent(entityx::Entity victim, entityx::Entity killer) : killer(killer), victim(victim) {}
    entityx::Entity killer, victim;
};

struct enterShopEvent {
    enterShopEvent(entityx::Entity guest, entityx::Entity shop) : guest(guest), shop(shop) {}
    entityx::Entity guest, shop;
};

struct enterVillageEvent {
    enterVillageEvent(entityx::Entity guest, entityx::Entity village) : guest(guest), village(village) {}
    entityx::Entity guest, village;
};

#endif /* EVENTS_HPP */