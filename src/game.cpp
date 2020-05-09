#include "game.hpp"
#include "controlsystemx.hpp"
#include "menusystemx.hpp"
#include "turnsystemx.hpp"
#include "rendersystemx.hpp"
#include "unitsystemx.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

SDL_Renderer * Game::renderer = nullptr;
TTF_Font * Game::font = NULL;

Game::Game() {

}

Game::Game(Settings in_settings) {
    setSettings(in_settings);
    init();
}

Game::~Game() {}

Settings * Game::getSettings() {
    return (&settings);
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
    SDL_Log("Making FPS entity");

    if (settings.FPS.entity.valid()) {
        SDL_Log("Destroying old FPS entity");
        settings.FPS.entity.destroy();
    }

    settings.FPS.entity = entities.create();
    settings.FPS.entity.assign<Position>();
    settings.FPS.entity.component<Position>()->setBounds(0, settings.res.x, 0, settings.res.y);
    settings.FPS.entity.component<Position>()->setPos(settings.FPS.pos.x, settings.FPS.pos.y);
    settings.FPS.entity.assign<Text>(settings.fontsize);
    settings.FPS.entity.component<Text>()->setText("60");
    settings.FPS.entity.component<Text>()->setColor(settings.FPS.textcolor);
    settings.FPS.entity.component<Text>()->setSizefactor(settings.FPS.sizefactor);
    settings.FPS.entity.component<Text>()->makeTextures();
    settings.FPS.entity.component<Text>()->setRects(settings.FPS.pos.x, settings.FPS.pos.y);
}

void Game::killMenu(short unsigned int index) {
    if (menus[index].valid()) {
        menus[index].destroy();
    }
}

void Game::hideMenu(short unsigned int index) {
    SDL_Log("Hiding Menu %d", index);

    if (menus[index].valid()) {
        menus[index].component<Sprite>()->hide();
        menus[index].component<Text>()->hide();
    }
}

void Game::showMenu(short unsigned int index) {
    if (menus[index].valid()) {
        menus[index].component<Text>()->show();
        menus[index].component<Sprite>()->show();
    }
}

entityx::Entity * Game::getMenu(unsigned char in_menu_index) {
    entityx::Entity * out;
    out = &menus[in_menu_index];
    return (out);
}

void Game::makeMenutext(unsigned char in_menu_index) {
    SDL_Log("Making menu text: %d", in_menu_index);

    if (menus[in_menu_index].valid()) {

        if (menuoptions.find(in_menu_index) != menuoptions.end()) {
            menus[in_menu_index].component<Text>()->setText(menuoptions2str(menuoptions[in_menu_index]));
        } else {
            SDL_Log("Menu options are invalid.");
        }

        menus[in_menu_index].component<Text>()->makeTextures();
    } else {
        SDL_Log("Menu %d is invalid", in_menu_index);
    }
}

void Game::makeTurntransition() {

    if (transition.valid()) {
        transition.destroy();
    }

    transition = entities.create();
    transition.assign<Position>();
    transition.assign<Sprite>();
    transition.component<Sprite>()->hide();
    SDL_Color white = {255, 255, 255};
    transition.assign<Text>(settings.fontsize);
    transition.component<Text>()->hide();
    // transition.component<Sprite>()->setTexture("..//assets//textbox.png");
    // transition.component<Sprite>()->setSrcrect(128, 128);
    // transition.component<Sprite>()->setDestrect(128, 128);
}

void Game::setTurntransitiontext(unsigned char in_army) {
    transition.component<Text>()->setText(armyNames[in_army]);

}


void Game::makeMenu(unsigned char in_menu_index) {
    SDL_Log("Making menu: %d", in_menu_index);

    if (menus[in_menu_index].valid()) {
        menus[in_menu_index].destroy();
    }

    menus[in_menu_index] = entities.create();
    menus[in_menu_index].assign<Position>();
    menus[in_menu_index].component<Position>()->setonTilemap(false);
    menus[in_menu_index].component<Position>()->setBounds(0, 2000, 0, 2000);
    menus[in_menu_index].assign<Sprite>();
    menus[in_menu_index].component<Sprite>()->hide();
    SDL_Color white = {255, 255, 255};
    menus[in_menu_index].assign<Text>(settings.fontsize);
    menus[in_menu_index].component<Text>()->setColor(white);
    menus[in_menu_index].component<Text>()->hide();

    switch (in_menu_index) {
        case MENU::UNIT:
            SDL_Log("Making unit menu\n");
            menus[MENU::UNIT].component<Sprite>()->setTexture("..//assets//textbox.png");
            menus[MENU::UNIT].component<Sprite>()->setSrcrect(128, 128);
            menus[MENU::UNIT].component<Sprite>()->setDestrect(128, 128);
            break;

        case MENU::MAPMENU:
            SDL_Log("Making map menu\n");
            menus[MENU::MAPMENU].component<Sprite>()->setTexture("..//assets//textbox.png");
            menus[MENU::MAPMENU].component<Sprite>()->setSrcrect(128, 128);
            menus[MENU::MAPMENU].component<Sprite>()->setDestrect(128, 128);
            break;
    }

    makeMenutext(in_menu_index);
}

void Game::makeMenuoptions(unsigned char in_menu_index) {
    SDL_Log("Making Menu options");
    std::vector<unsigned char> options;

    entityx::ComponentHandle<Unit> unit;
    entityx::ComponentHandle<Unit> top;
    entityx::ComponentHandle<Unit> bottom;
    entityx::ComponentHandle<Unit> right;
    entityx::ComponentHandle<Unit> left;
    short int * bounds;
    unsigned char army;

    switch (in_menu_index) {
        case MENU::UNIT:
            options.push_back(MENU::OPTION::ITEMS);
            bounds = mapx->getBounds();
            unit = mapx->getUnit(cursor_lastpos[0], cursor_lastpos[1]);
            SDL_Log("last position: %d %d", cursor_lastpos[0], cursor_lastpos[1]);

            if (unit) {
                SDL_Log("Making Menuoptions for: %s", unit->getName().c_str());

                std::vector<std::vector<short int>> tilemap = mapx->getTilemap();

                if ((cursor_lastpos[1] + 1) < bounds[3]) {
                    top = mapx->getUnit(cursor_lastpos[0], cursor_lastpos[1] + 1);
                }

                if ((cursor_lastpos[1] - 1) > bounds[2]) {
                    bottom = mapx->getUnit(cursor_lastpos[0], cursor_lastpos[1] - 1);
                }

                if ((cursor_lastpos[0] + 1) < bounds[1]) {
                    right = mapx->getUnit(cursor_lastpos[0] + 1, cursor_lastpos[1]);
                }

                if ((cursor_lastpos[0] - 1) > bounds[0]) {
                    left = mapx->getUnit(cursor_lastpos[0] - 1, cursor_lastpos[1]);
                }

                if (tilemap[cursor_lastpos[0]][cursor_lastpos[1]] / DEFAULT::TILE_DIVISOR == TILE::THRONE) {
                    if (unit->getid() ==  UNIT::NAME::ERWIN) {
                        options.push_back(MENU::OPTION::SEIZE);
                    }
                }

                std::vector<entityx::ComponentHandle<Unit>> units_around = {top, right, bottom, left};

                for (short int i = 0; i < units_around.size(); i++) {
                    if (units_around[i]) {
                        army = units_around[i]->getArmy();
                        SDL_Log("Unit_around: %s", units_around[i]->getName().c_str());
                        SDL_Log("Army: %s", units_around[i]->getArmyName().c_str());

                        switch (army) {
                            case UNIT::ARMY::FRIENDLY:
                            case UNIT::ARMY::ERWIN:
                            case UNIT::ARMY::FREE_MILITIA:
                                options.push_back(MENU::OPTION::TRADE);
                                options.push_back(MENU::OPTION::RESCUE);
                                break;

                            case UNIT::ARMY::ENEMY:
                            case UNIT::ARMY::BANDITS:
                            case UNIT::ARMY::KEWAC:
                            case UNIT::ARMY::NEUTRAL:
                            case UNIT::ARMY::IMPERIAL:
                                options.push_back(MENU::OPTION::ATTACK);
                                break;
                        }
                    } else {
                        SDL_Log("No unit around");
                    }
                }

                options.push_back(MENU::OPTION::WAIT);
            } else {
                SDL_Log("Menuoptions: Unit not found.");
            }

            break;

        case MENU::MAPMENU:
            options.push_back(MENU::OPTION::UNITS);
            options.push_back(MENU::OPTION::ENEMYUNITS);
            options.push_back(MENU::OPTION::ITEMS);
            options.push_back(MENU::OPTION::OBJECTIVES);
            options.push_back(MENU::OPTION::OPTIONS);
            options.push_back(MENU::OPTION::ENDTURN);
            break;
    }

    std::sort(options.begin(), options.end());
    menuoptions[in_menu_index] = options;
}

short unsigned int Game::getState() {
    return (state);
}

entityx::ComponentHandle<Map> Game::getMap() {
    return (mapx);
}

void Game::loadMap(const int in_map_index) {
    SDL_Log("Loading Map index: %d \n", in_map_index);

    if (!mapx) {
        mapEntx = entities.create();
        mapEntx.assign<Map>(settings.tilesize[0], settings.tilesize[1]);
        mapx = mapEntx.component<Map>();
        mapx->loadTiles(in_map_index);
        mapx->setRenderer(renderer);
        mapx->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
        mapx->loadTilemap(in_map_index);
        mapx->setArrivals(mapArrivals[in_map_index]());
        mapx->setArrivalEquipments(arrivalEquipments[in_map_index]());
        systems.system<RenderSystemx>()->setMap(mapx);
        systems.system<UnitSystemx>()->updateMap();
        systems.system<TurnSystemx>()->updateMap();
        systems.system<MenuSystemx>()->updateMap();
    } else {
        SDL_Log("Failed to loadMap. Was mapx deleted previously?");
    }
}

std::vector<unsigned char> Game::getMenuoptions(unsigned char in_menu_index) {
    return (menuoptions[in_menu_index]);
}

void Game::setMenuoptions(unsigned char in_menu_index, std::vector<unsigned char> in_options) {
    menuoptions[in_menu_index] = in_options;
}


void Game::setCursorlastpos(const short int x, const short int y) {
    cursor_lastpos[0] = x;
    cursor_lastpos[1] = y;
}

void Game::unloadMap() {
    SDL_Log("Unloading Map");

    if (mapx) {
        mapEntx.destroy();
    } else {
        SDL_Log("Failed to unloadMap. Was mapx deleted previously?");
    }
}

void Game::setCursorstate(const unsigned char in_menu) {
    SDL_Log("Changing cursor");
    SDL_Rect temprect;

    short unsigned int * temp_tilesize;
    short int * bounds;
    temp_tilesize = mapx->getTilesize();

    if (cursorx.valid()) {
        switch (in_menu) {
            case MENU::MAP:
                SDL_Log("Changed Cursor to Map");
                cursorx.component<Sprite>()->init(cursorx.component<Position>()->getPos());
                cursorx.component<Sprite>()->animate();
                cursorx.component<Sprite>()->setTexture("..//assets//mapcursors.png");
                cursorx.component<Sprite>()->setAnimation(10, 50);
                cursorx.component<Sprite>()->setTilesize(mapx->getTilesize());
                cursorx.component<Sprite>()->setSlidetype(SLIDETYPE::GEOMETRIC);
                bounds = mapx->getBounds();
                SDL_Log("bounds: %d %d %d %d", bounds[0], bounds[1], bounds[2], bounds[3]);
                // bounds[0] += DEFAULT::TILEMAP_XOFFSET;
                // bounds[2] += DEFAULT::TILEMAP_YOFFSET;
                cursorx.component<Position>()->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
                cursorx.component<Position>()->setBounds(mapx->getBounds());
                cursorx.component<Position>()->setPos(cursor_lastpos);
                cursorx.component<Position>()->setonTilemap(true);
                cursorx.component<Position>()->setPeriodic(false);
                systems.system<RenderSystemx>()->setTilesize(temp_tilesize[0], temp_tilesize[1]);
                break;

            case MENU::UNIT:
                SDL_Log("Changed Cursor to mapmenu");

            case MENU::MAPMENU:
                SDL_Log("Changed Cursor to unitmenu");
                temprect = {0, 0, 16, 16}; //x,y,w,h
                short int * menupos;
                short int linespace = 1;
                cursorx.component<Sprite>()->still();
                cursorx.component<Sprite>()->setSrcrect(temprect);
                cursorx.component<Sprite>()->setDestrect(temprect);
                cursorx.component<Sprite>()->setTexture("..//assets//menucursor.png");

                if (menus[in_menu].valid()) {
                    menupos = menus[in_menu].component<Position>()->getPos();
                    linespace = menus[in_menu].component<Text>()->getLinespacing();
                }

                short int menubounds[4];
                menubounds[0] = menupos[0] / linespace;
                menubounds[1] = menupos[0] / linespace;
                menubounds[2] = (short int)(menupos[1] / linespace + 1);
                menubounds[3] = (short int)(menupos[1] / linespace + menuoptions[in_menu].size());
                cursor_lastpos[0] = cursorx.component<Position>()->getPos()[0] - cursorx.component<Position>()->getOffset()[0];
                cursor_lastpos[1] = cursorx.component<Position>()->getPos()[1] - cursorx.component<Position>()->getOffset()[1];
                SDL_Log("Menubounds: %d %d %d %d", menubounds[0], menubounds[1], menubounds[2], menubounds[3]);
                cursorx.component<Position>()->setBounds(menubounds);
                cursorx.component<Position>()->setPos(menubounds[0] - 1, menubounds[2]);
                cursorx.component<Position>()->setonTilemap(false);
                cursorx.component<Position>()->setPeriodic(true);
                systems.system<RenderSystemx>()->setLinespace(linespace);
                break;
        }
    }
}

void Game::loadCursor() {
    unloadCursor();

    cursorx = entities.create();
    cursorx.assign<KeyboardController>();
    cursorx.assign<Position>();
    cursorx.assign<Sprite>();

    if (SDL_NumJoysticks() < 1) {
        SDL_Log("No joysticks connected.\n");
    } else {
        SDL_Log("Is the joystick supported by the game controller interface? %d.\n", SDL_IsGameController(0));
        cursorx.assign<GamepadController>();
    }

    setCursorstate(MENU::MAP);
}

void Game::unloadCursor() {
    if (cursorx.valid()) {
        cursorx.destroy();
    }
}


void Game::loadMapUnits(std::vector<short unsigned int> in_units, std::vector<std::vector<int>> in_pos_list) {
    SDL_Log("Loading Units\n");
    std::string asset_name;
    entityx::Entity Uent;

    for (int i = 0; i < in_units.size(); i++) {
        asset_name = "..//assets//" + units[in_units[i]].getName() + ".png";
        Uent = entities.create();
        Uent.assign<Unit>(units[in_units[i]]);
        Uent.assign<Position>();
        Uent.component<Position>()->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
        Uent.component<Position>()->setPos(in_pos_list[i][0], in_pos_list[i][1]);
        Uent.assign<Sprite>(asset_name.c_str());
        mapx->putUnit(in_pos_list[i][0], in_pos_list[i][1], Uent.component<Unit>());
    }
}

void Game::loadMapArrivals() {
    SDL_Log("Loading map arrivals.\n");

    if (mapx) {
        std::vector<Map_arrival> map_arrivals = mapx->getArrivals();
        unsigned short int currentturn = mapx->getTurn();
        std::string asset_name;
        entityx::Entity Uent;

        for (int i = 0; i < map_arrivals.size(); i++) {
            if (map_arrivals[i].turn == currentturn) {

                if (units.find(map_arrivals[i].id) == units.end()) {
                    SDL_Log("unloaded units loading %d", map_arrivals[i].id);
                    makeUnits(std::vector<short int> {map_arrivals[i].id});
                    asset_name = "..//assets//" + units[map_arrivals[i].id].getName() + ".png";
                    SDL_Log("Loaded: %s", units[map_arrivals[i].id].getName().c_str());
                }

                Uent = entities.create();
                Uent.assign<Position>();
                Uent.component<Position>()->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
                Uent.component<Position>()->setPos(map_arrivals[i].position.x, map_arrivals[i].position.y);
                Uent.assign<Sprite>(asset_name.c_str());
                Uent.assign<Unit>(units[map_arrivals[i].id]);
                SDL_Log("Arrival position: %d %d", map_arrivals[i].position.x, map_arrivals[i].position.y);
                mapx->putUnit(map_arrivals[i].position.x, map_arrivals[i].position.y, Uent.component<Unit>());
            }
        }
    } else {
        SDL_Log("Failed to loadMapArrivals: mapx does not exist");
    }
}

void Game::makeUnits(unsigned char in_chap) {
    std::vector<short int> toload = baseParties[in_chap]();
    baseUnits(&units, toload);
}

void Game::makeUnits(std::vector<short int> toload) {
    baseUnits(&units, toload);
}

void Game::unmakeUnits(std::vector<short int> to_unload) {
    for (int i = 0; i < to_unload.size(); i++) {
        units.erase(to_unload[i]);
    }
}


void Game::init() {
    int flags = 0;

    if (settings.fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    Game::font = TTF_OpenFont("../fonts/arial.ttf", settings.fontsize);

    if (Game::font == NULL) {
        SDL_Log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        SDL_Log("SDL subsystems initialized.\n");
        window = SDL_CreateWindow(settings.title, settings.pos.x, settings.pos.y, settings.res.x, settings.res.x, flags);

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

    systems.add<RenderSystemx>(renderer, this);
    systems.add<ControlSystemx>(this);
    systems.add<UnitSystemx>(this);
    systems.add<TurnSystemx>(this);
    systems.add<MenuSystemx>(this);
    systems.configure();

}

void Game::startTurnSystem() {
    std::vector<unsigned char> armies = mapx->getArmies();

    if (armies.size() > 0) {
        systems.system<TurnSystemx>()->addArmies(armies);
    } else {
        SDL_Log("No armies loaded on the current map.");
    }

    state = GAME::STATE::TURNBEGIN;
    events.emit<turnBegin>(systems.system<TurnSystemx>()->getArmyfront());
}


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

    if (!ptemp) {
        SDL_Log("Cannot access element Convoy.");
    } else {
        convoy.readXML(ptemp);
    }

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
    if (state != new_state) {
        state = new_state;
        SDL_Log("New game state: %s", gamestate2str(state).c_str());
    }
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

void configure(entityx::EventManager & event_manager) {

}

void Game::update(entityx::TimeDelta dt) {
    systems.update<ControlSystemx>(dt);
    systems.update<TurnSystemx>(dt);
    systems.update<RenderSystemx>(dt);
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


