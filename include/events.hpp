#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <entityx/entityx.h>
#include "keyboardcontroller.hpp"
// #include "structs.hpp"
#include "gamepadcontroller.hpp"

struct turnBegin {
    turnBegin(entityx::Entity beginner) : beginner(beginner) {}
    entityx::Entity beginner;
    //Game? or AI ender.
};

struct selectPC {
    selectPC(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct selectNPC {
    selectNPC(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct mapMenu {
    mapMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct unitSelect {
    unitSelect(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitMove {
    unitMove(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitDanger {
    unitDanger(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitMenu {
    unitMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct unitMap {
    unitMap(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct inputAccept {
    inputAccept(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputAccept(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputCancel {
    inputCancel(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputCancel(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputMenuRight {
    inputMenuRight(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputMenuRight(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputMenuLeft {
    inputMenuLeft(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputMenuLeft(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputMinimap {
    inputMinimap(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputMinimap(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputFaster {
    inputFaster(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputFaster(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct inputPause {
    inputPause(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputPause(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
};

struct turnEnd {
    turnEnd(entityx::Entity ender) : ender(ender) {}
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