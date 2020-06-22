#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <entityx/entityx.h>
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "mousecontroller.hpp"

struct turnBegin {
    turnBegin(uint8_t army) : army(army) {}
    uint8_t army;
};

struct turnEnd {
    turnEnd() {}
};

struct disableMouse {
    disableMouse() {}
};

struct enableMouse {
    enableMouse() {}
};

struct disableCursor {
    disableCursor() {}
};

struct enableCursor {
    enableCursor() {}
};

struct selectPC {
    selectPC(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct selectNPC {
    selectNPC(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct objectivesMenu {
    objectivesMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct unitsMenu {
    unitsMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct enemyunitsMenu {
    enemyunitsMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct optionsMenu {
    optionsMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct itemsMenu {
    itemsMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct mapMenu {
    mapMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct cursorMoved {
    cursorMoved(entityx::Entity cursor, Point move) : cursor(cursor), move(move) {}
    entityx::Entity cursor;
    Point move;
};

struct unitSelect {
    unitSelect(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitDeselect {
    unitDeselect(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitReturn {
    unitReturn(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitMove {
    unitMove(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitHover {
    unitHover(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitDehover {
    unitDehover(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct switchControl {
    switchControl(uint8_t army) : army(army) {}
    uint8_t army;
};

struct refreshUnits {
    refreshUnits(uint8_t army) : army(army) {}
    uint8_t army;
};

struct disableMenu {
    disableMenu(entityx::Entity selector, uint8_t menuind) : selector(selector), menuind(menuind) {}
    entityx::Entity selector;
    uint8_t menuind;
};

struct unitDanger {
    unitDanger(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitMenu {
    unitMenu(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

struct return2Map {
    return2Map(entityx::Entity selector) : selector(selector) {}
    entityx::Entity selector;
};

// struct unitmenuSelect {
//     unitmenuSelect(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
//     entityx::Entity cursor;
//     entityx::ComponentHandle<Unit> unit;
// };

// struct mapmenuSelect {
//     mapmenuSelect(entityx::Entity cursor) : cursor(cursor) {}
//     entityx::Entity cursor;
// };

struct menuSelect {
    menuSelect(entityx::Entity selector, int8_t menu) : selector(selector), menu(menu) {}
    menuSelect(entityx::Entity selector, int8_t menu, entityx::ComponentHandle<Unit> unit) : selector(selector), menu(menu), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
    int8_t menu;
};

struct inputAccept {
    inputAccept(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputAccept(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    inputAccept(entityx::ComponentHandle<MouseController> mouse) : mouse(mouse) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
    entityx::ComponentHandle<MouseController> mouse;
};

struct inputCancel {
    inputCancel(entityx::ComponentHandle<KeyboardController> keyboard) : keyboard(keyboard) {}
    inputCancel(entityx::ComponentHandle<GamepadController> gamepad) : gamepad(gamepad) {}
    inputCancel(entityx::ComponentHandle<MouseController> mouse) : mouse(mouse) {}
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<MouseController> mouse;
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


struct unitSeize {
    unitSeize(entityx::ComponentHandle<Unit> victor) : victor(victor) {}
    entityx::ComponentHandle<Unit> victor;
};

struct gameOver {
    gameOver(entityx::Entity victor) : victor(victor) {}
    entityx::Entity victor;
};


struct unitRefresh {
    unitRefresh(entityx::Entity refresher) : refresher(refresher) {}
    entityx::Entity refresher;
};

struct unitWait {
    unitWait(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitTalk {
    unitTalk(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitRescue {
    unitRescue(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct defenderSelect {
    defenderSelect(entityx::Entity selector, entityx::ComponentHandle<Unit> defender) : selector(selector) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> defender;
};

struct unitAttack {
    unitAttack(entityx::Entity selector, entityx::ComponentHandle<Unit> attacker) : selector(selector) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> attacker;
};

struct unitTrade {
    unitTrade(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitEscape {
    unitEscape(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitStaff {
    unitStaff(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitItems {
    unitItems(entityx::Entity selector, entityx::ComponentHandle<Unit> unit) : selector(selector), unit(unit) {}
    entityx::Entity selector;
    entityx::ComponentHandle<Unit> unit;
};

struct unitHeal {
    //Should those be Component hanldes?
    unitHeal(entityx::Entity healer, entityx::Entity patient) : healer(healer), patient(patient) {}
    entityx::Entity healer, patient;
};

struct unitDie {
    unitDie(entityx::Entity victim, entityx::Entity killer) : killer(killer), victim(victim) {}
    entityx::Entity killer, victim;
};

struct enterShop {
    enterShop(entityx::Entity guest, entityx::Entity shop) : guest(guest), shop(shop) {}
    entityx::Entity guest, shop;
};

struct enterArmory {
    enterArmory(entityx::Entity guest, entityx::Entity shop) : guest(guest), shop(shop) {}
    entityx::Entity guest, shop;
};

struct checkConvoy {
    checkConvoy(entityx::Entity guest) : guest(guest) {}
    entityx::Entity guest;
};

struct checkMap {
    checkMap() {}
};

struct endTurn {
    endTurn() {}
};

struct enterVillage {
    enterVillage(entityx::Entity guest, entityx::Entity village) : guest(guest), village(village) {}
    entityx::Entity guest, village;
};

namespace EVENTID {
enum EVENTID {
    UNITATTACK,
};
}


#endif /* EVENTS_HPP */