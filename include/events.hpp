#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <entityx/entityx.h>
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"

struct turnBegin {
    turnBegin(unsigned char army) : army(army) {}
    unsigned char army;
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

struct selectPC {
    selectPC(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct selectNPC {
    selectNPC(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct objectivesMenu {
    objectivesMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct unitsMenu {
    unitsMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct enemyunitsMenu {
    enemyunitsMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct optionsMenu {
    optionsMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct itemsMenu {
    itemsMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct mapMenu {
    mapMenu(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct cursorMoved {
    cursorMoved(entityx::Entity cursor, Point move) : cursor(cursor), move(move) {}
    entityx::Entity cursor;
    Point move;
};

struct unitSelect {
    unitSelect(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitDeselect {
    unitDeselect(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitReturn {
    unitReturn(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitMove {
    unitMove(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
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
    switchControl(unsigned char army) : army(army) {}
    unsigned char army;
};

struct refreshUnits {
    refreshUnits(unsigned char army) : army(army) {}
    unsigned char army;
};

struct disableMenu {
    disableMenu(entityx::Entity cursor, unsigned char menuind) : cursor(cursor), menuind(menuind) {}
    entityx::Entity cursor;
    unsigned char menuind;
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

struct return2Map {
    return2Map(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
};

struct unitmenuSelect {
    unitmenuSelect(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct mapmenuSelect {
    mapmenuSelect(entityx::Entity cursor) : cursor(cursor) {}
    entityx::Entity cursor;
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
    unitWait(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitTalk {
    unitTalk(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitRescue {
    unitRescue(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitAttack {
    unitAttack(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitTrade {
    unitTrade(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitEscape {
    unitEscape(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitStaff {
    unitStaff(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
    entityx::ComponentHandle<Unit> unit;
};

struct unitItems {
    unitItems(entityx::Entity cursor, entityx::ComponentHandle<Unit> unit) : cursor(cursor), unit(unit) {}
    entityx::Entity cursor;
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

struct enterVillage {
    enterVillage(entityx::Entity guest, entityx::Entity village) : guest(guest), village(village) {}
    entityx::Entity guest, village;
};




#endif /* EVENTS_HPP */