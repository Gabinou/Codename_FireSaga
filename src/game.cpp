#include "game.hpp"
#include "systems.hpp"
#include "sprite.hpp"
#include "position.hpp"
#include "arrowcomponent.hpp"
#include "text.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

SDL_Renderer * Game::renderer = nullptr;
TTF_Font * Game::font = NULL;
// Manager Game::manager;

Game::Game() {
    menus.resize(GAME::STATE::END);
}

Game::~Game() {}

Settings Game::getSettings() {
    return (settings);
}

void Game::setSettings(Settings in_settings) {
    settings = in_settings;
}

bool Game::checkRate(int rate, short unsigned int mode) {
    bool hit;

    if (mode == GAME::RN::SINGLE) {
        hit = (getURN(tinymt) < rate);
        return (hit);
    }

    if (mode == GAME::RN::DOUBLE) {
        hit = (((getURN(tinymt) + getURN(tinymt)) / 2) < rate);
        return (hit);
    }

    return (hit);
}

bool * Game::checkHitCrit(int hit_rate, int crit_rate, short unsigned int mode) {
    static bool hitcrit[2];

    if (mode == GAME::RN::SINGLE) {
        hitcrit[0] = (getURN(tinymt) < hit_rate);
        hitcrit[1] = (getURN(tinymt) < crit_rate);
        return (hitcrit);
    }

    if (mode == GAME::RN::DOUBLE) {
        hitcrit[0] = (((getURN(tinymt) + getURN(tinymt)) / 2) < hit_rate);
        hitcrit[1] = (((getURN(tinymt) + getURN(tinymt)) / 2) < crit_rate);
        return (hitcrit);
    }

    if (mode == GAME::RN::GAUSSIAN) {
        unsigned char * RNs;
        RNs[0] = getURN(tinymt);
        RNs[1] = getURN(tinymt);
        RNs = boxmuller(RNs);
        hitcrit[0] = (RNs[0] < hit_rate);
        hitcrit[1] = (RNs[1] < crit_rate);
        return (hitcrit);
    }

    if (mode == GAME::RN::HYBRID) {

    }

    return (hitcrit);
}

void Game::attack(Unit * attacker, Unit * defender) {
    SDL_Log("%s attacks %s\n", attacker->getName().c_str(), defender->getName().c_str());
    Combat_stats attacker_stats = attacker->getCombatStats();
    Combat_stats defender_stats = defender->getCombatStats();
    bool * hitcrit;
    hitcrit = checkHitCrit((attacker_stats.hit - defender_stats.dodge), (attacker_stats.crit - defender_stats.favor));

    if (hitcrit[0]) {
        if (hitcrit[1]) {
            int damage = attacker->totalMight(false) -  defender->totalDef(false);
            defender->takesDamage(damage);
        }
    }
}

void Game::fight(Unit * attacker, Unit * defender) {
    SDL_Log("%s fights %s\n", attacker->getName().c_str(), defender->getName().c_str());
    bool defender_doubles;
    bool attacker_doubles = attacker->canDouble(defender);
    bool defender_retaliates = defender->canRetaliate(attacker);
    attack(attacker, defender);

    if (defender_retaliates) {
        attack(defender, attacker);
        defender_doubles = defender->canDouble(attacker);
    }

    if (attacker_doubles) {
        attack(attacker, defender);
    }

    if (defender_doubles) {
        attack(defender, attacker);
    }
}

void Game::makeFPSEntity() {
    settings.FPS.entity = entities.create();
    settings.FPS.entity.assign<Position>();
    settings.FPS.entity.component<Position>()->setBounds(0, settings.res.x, 0, settings.res.y);
    settings.FPS.entity.component<Position>()->setPos(settings.FPS.pos.x, settings.FPS.pos.y);
    settings.FPS.entity.assign<Text>(settings.fontsize, "60", settings.FPS.textcolor);
    settings.FPS.entity.component<Text>()->setSizefactor(settings.FPS.sizefactor);
    settings.FPS.entity.component<Text>()->setRects(settings.FPS.pos.x, settings.FPS.pos.y);
}

void Game::makeUnitmenu(entityx::Entity & setter) {
    SDL_Log("Making unit menu\n");
    unitmenux = entities.create();
    unitmenux.assign<Position>();
    unitmenux.component<Position>()->setBounds(0, 2000, 0, 2000);
    unitmenux.component<Position>()->setPos(
        (int)(setter.component<Position>()->getPos()[0] * settings.tilesize[0]),
        (int)(setter.component<Position>()->getPos()[1] * settings.tilesize[1])
    );
    SDL_Log("Unitmenu setter position %d %d\n", setter.component<Position>()->getPos()[0], setter.component<Position>()->getPos()[1]);
    SDL_Log("Unitmenu position %d %d\n", unitmenux.component<Position>()->getPos()[0], unitmenux.component<Position>()->getPos()[1]);

    SDL_Color black = {255, 255, 255};
    unitmenux.assign<Sprite>("..//assets//textbox.png", (int []) {128, 128});
    // I think the menu textures should be loaded elsewhere when initted or first called. Then, should be only unloaded after a while.
    //Not loaded and unloaded after EACH CALL.
    unitmenux.assign<Text>(settings.fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
}

void Game::killMenu(short unsigned int index) {
    unitmenux.destroy();
}
void Game::hideMenu(short unsigned int index) {
    unitmenux.component<Sprite>()->hide();
    unitmenux.component<Text>()->hide();
}
void Game::showMenu(short unsigned int index) {
    unitmenux.component<Sprite>()->show();
    unitmenux.component<Text>()->show();
}

// I think this function is too big. Find a way to reduce it...
void Game::setState(entityx::Entity setter, short unsigned int new_state) {
    SDL_Log("Game state changes from %d to %d\n", this->state, new_state);

    switch (this->state) {
        case GAME::STATE::PAUSE:
            break;

        case GAME::STATE::STATS:
            break;

        case GAME::STATE::MINIMAP:
            break;

        case GAME::STATE::MAP:
            switch (new_state) {
                case GAME::STATE::STATS:
                    break;

                case GAME::STATE::MINIMAP:
                    break;

                case GAME::STATE::UNITMOVE: {
                    SDL_Log("Changing to unitmove\n");
                    std::vector<std::vector<short int>> costmap;
                    std::vector<std::vector<short int>> movemapp;
                    std::vector<std::vector<short int>> attackmapp;
                    entityx::ComponentHandle<Position> cursorpos; // because setter should be the cursor.
                    entityx::ComponentHandle<Unit> unitcomp;
                    short unsigned int * start;
                    short unsigned int unit_move;
                    short unsigned int current_unit_id;
                    unsigned char unitmvttype;
                    unsigned char * range;

                    cursorpos = setter.component<Position>();
                    unitcomp = setter.component<Unit>();
                    selected = unitcomp.entity();

                    if (cursorpos) {
                        start = (short unsigned int *)cursorpos->getPos();
                    } else {
                        SDL_Log("Could not get cursor position component");
                    }

                    if (unitcomp) {
                        unit_move = unitcomp->getStats().move;
                        unitmvttype = unitcomp->getMvttype();
                        range = unitcomp->getRange();
                    } else {
                        SDL_Log("Could not get unit component");
                    }

                    SDL_Log("start: %d %d", start[0], start[1]);
                    SDL_Log("unitmove: %d", unit_move);

                    costmap = mapx->makeMvtCostmap(unitmvttype);

                    movemapp = movemap(costmap, start, unit_move, "matrix");
                    mapx->setOverlay(MAP::OVERLAY::MOVE, movemapp);

                    attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
                    mapx->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

                    mapx->showOverlay();

                }
                break;

                case GAME::STATE::OPTIONS:
                    break;

                case GAME::STATE::CUTSCENE:
                    break;
            }

            break;

        case GAME::STATE::OPTIONS:
            switch (new_state) {
                case GAME::STATE::MAP:
                    break;

                case GAME::STATE::STARTMENU:
                    break;
            }

            break;

        case GAME::STATE::STARTMENU:
            switch (new_state) {
                case GAME::STATE::INTROVIDEO:
                    break;

                case GAME::STATE::SAVES:
                    break;

                case GAME::STATE::SOUNDROOM:
                    break;

                case GAME::STATE::OPTIONS:
                    break;
            }

            break;

        case GAME::STATE::PREPARATION:
            switch (new_state) {
                case GAME::STATE::PLACEMENT:
                    break;

                case GAME::STATE::CHOOSEUNITS:
                    break;

                case GAME::STATE::EQUIPMENT:
                    break;

                case GAME::STATE::FORGE:
                    break;

                case GAME::STATE::STORAGE:
                    break;

                case GAME::STATE::BONUSEXP:
                    break;
            }

            break;

        case GAME::STATE::PLACEMENT:
        case GAME::STATE::CHOOSEUNITS:
        case GAME::STATE::EQUIPMENT:
        case GAME::STATE::FORGE:
        case GAME::STATE::STORAGE:
        case GAME::STATE::BONUSEXP:
            switch (new_state) {
                case GAME::STATE::PREPARATION:
                    break;

                case GAME::STATE::MAP:
                    break;
            }

            break;

        case GAME::STATE::UNITMOVE:
            switch (new_state) {
                case GAME::STATE::UNITMENU: {
                    SDL_Log("Changing to unitmenu\n");
                    mapx->hideOverlay();

                    if (unitmenux.valid()) {
                        showMenu(GAME::STATE::UNITMENU);
                    } else {
                        makeUnitmenu(setter);
                    }

                    short int * new_position;
                    short int * old_position;

                    entityx::ComponentHandle<Position> setterpos;
                    entityx::ComponentHandle<Position> selectedpos;
                    entityx::ComponentHandle<Unit> unitcomp;
                    setterpos = setter.component<Position>();

                    if (selected.valid()) {
                        selectedpos = selected.component<Position>();

                        if (selectedpos) {
                            old_position = selectedpos->getPos();
                            // SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
                        } else {
                            SDL_Log("Could not get selectedx unit component");
                        }
                    } else {
                        SDL_Log("Could not get selected entity");
                    }

                    if (setterpos) {
                        new_position = setterpos->getPos();
                        // SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
                    } else {
                        SDL_Log("Could not get setter(unit) position component");
                    }

                    mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
                    unitmenux.component<Position>()->setPos(new_position[0] * settings.tilesize[0], new_position[1] * settings.tilesize[1]);
                    selectedpos->setPos(new_position); // move at the end, cause new and old_position are pointers!
                }
                break;

                case GAME::STATE::MAP:
                    mapx->hideOverlay();
                    break;
            }

            break;

        case GAME::STATE::UNITMENU:
            switch (new_state) {
                case GAME::STATE::ATTACK:
                    break;

                case GAME::STATE::TRADE:
                    break;

                case GAME::STATE::ITEMS:
                    break;

                case GAME::STATE::CONVERSATION:
                    break;

                case GAME::STATE::MAP:
                    hideMenu(GAME::STATE::UNITMENU);
                    break;
            }

            break;

        case GAME::STATE::CONVERSATION:
            switch (new_state) {
                case GAME::STATE::MAP:
                    break;
            }

            break;

        case GAME::STATE::ATTACK:
            switch (new_state) {
                case GAME::STATE::BATTLE:
                    break;
            }

            break;
    }

    state = new_state;
}

short unsigned int Game::getState() {
    return (state);
}

// template <typename T> void Game::loadTiles(std::vector<T> in_tiles) {
//     loaded_tiles = baseTiles(in_tiles);
// }

void Game::loadMap(const int in_map_index) {
    SDL_Log("Loading Map index: %d \n", in_map_index);

    if (!mapx) {
        mapEntx = entities.create();
        mapEntx.assign<Map>(settings.tilesize[0], settings.tilesize[1]); // mapx is a pointer
        mapx = mapEntx.component<Map>();
        mapx->loadTiles(in_map_index);
        mapx->setRenderer(renderer);
        mapx->loadTilemap(in_map_index);
        mapx->setArrivals(mapArrivals[in_map_index]());
        mapx->setArrivalEquipments(arrivalEquipments[in_map_index]());
        systems.system<RenderSystemx>()->setMap(mapx);
    } else {
        SDL_Log("Failed to loadMap. Was mapx deleted previously?");
    }
}

void Game::unloadMap() {
    SDL_Log("Unloading Map");

    if (mapx) {
        mapEntx.destroy();
    } else {
        SDL_Log("Failed to unloadMap. Was mapx deleted previously?");
    }
}

void Game::loadCursor() {
    // Map should be loaded before I think.
    if (this->state == GAME::STATE::MAP) {
        cursorx = entities.create();
        cursorx.assign<Position>(6, 6);
        cursorx.assign<KeyboardController>();
        cursorx.assign<GamepadController>();

        if (SDL_NumJoysticks() < 1) {
            SDL_Log("No joysticks connected.\n");
        } else {
            //     cursor.addComponent<GamepadController>(this, mapx);
        }

        cursorx.assign<Sprite>("..//assets//cursors.png", 10, 50);
        cursorx.component<Sprite>()->setSlidetype(SLIDETYPE::GEOMETRIC, mapx->getTilesize());
        cursorx.component<Position>()->setBounds(mapx->getBounds());
        cursorx.component<Sprite>()->init(cursorx.component<Position>()->getPos());
    }
}

// void Game::unloadCursor() {

// }


void Game::loadUnitEntities(std::vector<short unsigned int> unit_inds, std::vector<std::vector<int>> positions_list) {
    SDL_Log("Loading Units\n");
    std::string asset_name;
    Unit Utemp;
    entityx::Entity Uent;

    for (int i = 0; i < unit_inds.size(); i++) {
        Utemp = units[unit_inds[i]];
        asset_name = "..//assets//" +  Utemp.getName() + ".png";
        Uent = entities.create();
        Uent.assign<Unit>(units[unit_inds[i]]);
        Uent.assign<Position>(positions_list[i][0], positions_list[i][1]);
        Uent.assign<Sprite>(asset_name.c_str());
        mapx->putUnit(positions_list[i][0], positions_list[i][1], Uent.component<Unit>());
    }
}

void Game::loadMapArrivals() {
    SDL_Log("Loading map arrivals.\n");

    if (mapx) {
        std::vector<Map_arrival> map_arrivals = mapx->getArrivals();
        unsigned short int currentturn = mapx->getTurn();
        std::string asset_name;
        Unit Utemp;
        entityx::Entity Uent;

        for (int i = 0; i < map_arrivals.size(); i++) {
            if (map_arrivals[i].turn == currentturn) {
                Utemp = units[map_arrivals[i].id];
                asset_name = "..//assets//horse.png";
                // asset_name = "..//assets//" +  Utemp.getName() + ".png";
                Uent = entities.create();
                Uent.assign<Position>(map_arrivals[i].position.x, map_arrivals[i].position.y);
                Uent.assign<Sprite>(asset_name.c_str());
            }
        }
    } else {
        SDL_Log("Failed to loadMapArrivals.");
    }
}

void Game::loadUnits(unsigned char in_chap) {
    // Not necessary. I think would be better to load the party from a savefile or something.
    std::vector<short int> toload = baseParties[in_chap]();
    baseUnits(&units, toload);
}

void Game::loadUnits(std::vector<short int> toload) {
    baseUnits(&units, toload);
}

void Game::unloadUnits(std::vector<short int> to_unload) {
    for (int i = 0; i < to_unload.size(); i++) {
        units.erase(to_unload[i]);
    }
}


void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    Game::font = TTF_OpenFont("../fonts/arial.ttf", settings.fontsize); // Size translates to pixel size?
    //The srcrect does not change size with font pointsize.

    if (Game::font == NULL) {
        SDL_Log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        SDL_Log("SDL subsystems initialized.\n");
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window) {
            SDL_Log("Window created\n");
        }

        if (TTF_Init() == -1) {
            SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Log("Renderer created\n");
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    systems.add<RenderSystemx>(renderer);
    systems.add<ControlSystemx>(this);
    systems.configure();
    state = GAME::STATE::MAP;
};

void Game::loadXML(const short int save_ind) {
    char filename[DEFAULT::BUFFER_SIZE];

    if (!PHYSFS_exists(SAVE_FOLDER)) {
        SDL_Log("Could not find 'saves' folder!");
    }

    stbsp_snprintf(filename, DEFAULT::BUFFER_SIZE, "saves//save%04d.bsav", save_ind);
    SDL_Log("saveXML Game to: %s\n", filename);

    PHYSFS_file * fp = PHYSFS_openRead(filename);
    tinyxml2::XMLDocument xmlDoc;
    parseXML(filename, &xmlDoc);
    tinyxml2::XMLElement * ptemp;

    ptemp = xmlDoc.FirstChildElement("Convoy");
    convoy.readXML(ptemp);

    ptemp = xmlDoc.FirstChildElement("Unit");

    unsigned short int id;

    while (ptemp) {
        id = (unsigned short int)ptemp->IntAttribute("id");
        units[id] = Unit();
        units[id].readXML(ptemp);
        ptemp = xmlDoc.NextSiblingElement("Unit");
    }

    ptemp = xmlDoc.FirstChildElement("Narrative");
}

void Game::copySaveXML(const short int from_ind, const short int to_ind) {
    if (!PHYSFS_exists(SAVE_FOLDER)) {
        SDL_Log("Could not find save folder!");
    } else {
        char filenameto[DEFAULT::BUFFER_SIZE];
        char filenamefrom[DEFAULT::BUFFER_SIZE];
        stbsp_snprintf(filenamefrom, DEFAULT::BUFFER_SIZE, "saves//save%04d.bsav", from_ind);
        stbsp_snprintf(filenameto, DEFAULT::BUFFER_SIZE, "saves//save%04d.bsav", to_ind);
        SDL_Log("copy SaveXML Game from %s to %s\n", filenamefrom, filenameto);
        PHYSFS_file * pfrom = PHYSFS_openRead(filenamefrom);
        PHYSFS_file * pto = PHYSFS_openWrite(filenameto);
        int len = PHYSFS_fileLength(pfrom);
        char longbuffer[len];
        PHYSFS_readBytes(pfrom, longbuffer, len);
        PHYSFS_writeBytes(pto, longbuffer, len);
        PHYSFS_close(pfrom);
        PHYSFS_close(pto);
    }
}

void Game::deleteSaveXML(const short int save_ind) {
    if (!PHYSFS_exists(SAVE_FOLDER)) {
        SDL_Log("Could not find save folder!");
    } else {
        char filename[DEFAULT::BUFFER_SIZE];
        stbsp_snprintf(filename, DEFAULT::BUFFER_SIZE, "saves//save%04d.bsav", save_ind);
        SDL_Log("Deleting Game: %s\n", filename);
        PHYSFS_delete(filename);
    }
}

void Game::saveXML(const short int save_ind) {
    char filename[DEFAULT::BUFFER_SIZE];

    if (!PHYSFS_exists(SAVE_FOLDER)) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }

    stbsp_snprintf(filename, DEFAULT::BUFFER_SIZE, "saves//save%04d.bsav", save_ind);
    SDL_Log("saveXML Game to: %s\n", filename);
    PHYSFS_delete(filename);
    PHYSFS_file * fp = PHYSFS_openWrite(filename);

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());
    tinyxml2::XMLElement * pNarrative = xmlDoc.NewElement("Narrative");
    xmlDoc.InsertEndChild(pNarrative);
    writeXML_narrative(&xmlDoc, pNarrative, &narrative);

    tinyxml2::XMLElement * ptemp;

    for (auto it = units.begin(); it != units.end(); it++) {
        ptemp = xmlDoc.NewElement("Unit");
        it->second.writeXML(&xmlDoc, ptemp);
        xmlDoc.InsertEndChild(ptemp);
    }

    ptemp = xmlDoc.NewElement("Convoy");
    convoy.writeXML(&xmlDoc, ptemp);
    xmlDoc.InsertEndChild(ptemp);
    printXMLDoc(fp, &xmlDoc);
    PHYSFS_close(fp);
}

void Game::setState(const short unsigned int new_state) {
    state = new_state;
}

KeyboardInputMap Game::getKeyboardInputMap() {
    return (keyboardInputMap);
}
void Game::setKeyboardInputMap(KeyboardInputMap in_keyboardInputMap) {
    keyboardInputMap = in_keyboardInputMap;
}
GamepadInputMap Game::getGamepadInputMap() {
    return (gamepadInputMap);
}
void Game::setGamepadInputMap(GamepadInputMap in_gamepadInputMap) {
    gamepadInputMap = in_gamepadInputMap;
}

void Game::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    SDL_Log("Game cleanded.");
    SDL_Delay(5000);
}
bool Game::running() {
    return (isRunning);
}

void Game::update(entityx::TimeDelta dt) {
    // systems.update_all(dt);
    systems.update<ControlSystemx>(dt);
    systems.update<RenderSystemx>(dt);
}

void Game::testXMLMap() {
    SDL_Log("Testing Map xml writing and reading\n");
    // Must be run when units are in memory.
    Map map(32, 32); // mapx is a pointer
    map.loadTiles(0);
    map.loadTilemap(0);
    map.setArrivals(mapArrivals[0]());
    map.setArrivalEquipments(arrivalEquipments[0]());
    std::string asset_name;
    entityx::EntityX ex;
    entityx::Entity Uent = ex.entities.create();;
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    std::vector<short int> temp_supports;
    Equipped temp_equipped;
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
    temp_unit = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(400);
    temp_wpn.id = ITEM::NAME::BALL_LIGHTNING;
    temp_unit.addEquipment(temp_wpn);

    asset_name = "..//assets//" +  temp_unit.getName() + ".png";
    Uent.assign<Unit>(temp_unit);
    Uent.assign<Position>(6, 6);
    Uent.assign<Sprite>(asset_name.c_str());
    map.putUnit(6, 6, Uent.component<Unit>());
    map.writeXML("map_test.xml");

    Map test;
    test.setManager(&entities);
    test.readXML("map_test.xml");
    test.writeXML("map_rewrite.xml");
}

// loss conditions
bool essentialDied(Map * in_map, const Narrative * in_narrative) {
    std::vector<short unsigned int> essentials = in_map->getEssentials();

    for (int i = 0; i < essentials.size(); i++) {
        if (in_narrative->death[essentials[i]]) {
            return (true);
        }
    }

    return (false);
}

// win conditions
bool escaped(Map * in_map) {
    return (false);
}

bool Seized(Map * in_map) {
    //Maybe not necessary? maybe win is called when the throne is seized?
    return (in_map->getSeized());
}

bool talked() {
    return (false);
}

bool bossDied(Map * in_map) {
    return (in_map->getBossDeath());
}

bool rout(Map * in_map) {
    return ((in_map->getnumArrivals() < 1));
}

bool areAlive() {
    return (false);
}

bool positionCondition(Unit * in_unit, Map_condition * in_mcond) {
    // Called by the game everytime a unit moves.
    if (in_mcond->army > 0) {
        if (in_mcond->army != in_unit->getArmy()) {
            return (false);
        }
    }

    if (in_mcond->unit > 0) {
        if (in_mcond->unit != in_unit->getid()) {
            return (false);
        }
    }

    // short int * unitpos = in_unit->getPos();

    // if (unitpos[0] < std::max(0, (int)in_mcond->colmin)) {
    //     return(false);
    // }
    // if (unitpos[0] > std::min(255, (int)in_mcond->colmax)) {
    //     return(false);
    // }
    // if (unitpos[1] < std::max(0, (int)in_mcond->rowmin)) {
    //     return(false);
    // }
    // if (unitpos[1] > std::min(255, (int)in_mcond->rowmax)) {
    //     return(false);
    // }
    return (true);
}


