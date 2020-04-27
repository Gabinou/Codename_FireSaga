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

Game::Game() {

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

    switch (in_menu_index) {
        case MENU::UNIT:
            out = getUnitmenu();
            break;

        case MENU::MAP:
            break;
    }

    return (out);
}

entityx::Entity * Game::getUnitmenu() {
    return (&menus[MENU::UNIT]);
}

std::vector<std::string> Game::menuoptions2str(std::vector<unsigned char> in_options) {
    std::vector<std::string> out;

    // THIS SHOULD BE AN UTILITY.
    for (short int i = 0; i < in_options.size(); i++) {
        switch (in_options[i]) {
            case UNIT::MENU::ITEMS:
                out.push_back("Items");
                break;

            case UNIT::MENU::TALK:
                out.push_back("Talk");
                break;

            case UNIT::MENU::RESCUE:
                out.push_back("Rescue");
                break;

            case UNIT::MENU::SEIZE:
                out.push_back("Seize");
                break;

            case UNIT::MENU::ESCAPE:
                out.push_back("Escape");
                break;

            case UNIT::MENU::ATTACK:
                out.push_back("Attack");
                break;

            case UNIT::MENU::TRADE:
                out.push_back("Trade");
                break;

            case UNIT::MENU::WAIT:
                out.push_back("Wait");
                break;
        }
    }

    return (out);
}


void Game::updateMenu(unsigned char in_menu_index) {
    SDL_Log("Updating menu: %d", in_menu_index);

    if (menus[in_menu_index].valid()) {
        switch (in_menu_index) {
            case MENU::UNIT:
                if (menuoptions.find(MENU::UNIT) != menuoptions.end()) {
                    menus[MENU::UNIT].component<Text>()->setText(menuoptions2str(menuoptions[MENU::UNIT]));
                } else {
                    menus[MENU::UNIT].component<Text>()->setText({"Default", "Items", "Attack", "Wait"});
                }

                menus[MENU::UNIT].component<Text>()->makeTextures();
        }
    } else {
        SDL_Log("Menu %d is invalid", in_menu_index);
    }
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
    menus[MENU::UNIT].component<Sprite>()->hide();
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

        case MENU::MAP:
            break;
    }

    updateMenu(in_menu_index);
}

void Game::makeUnitmenuoptions() {
    SDL_Log("Building unitmenu options");
    std::vector<unsigned char> options;
    options.push_back(UNIT::MENU::ITEMS);

    entityx::ComponentHandle<Unit> unit;

    entityx::ComponentHandle<Unit> top;
    entityx::ComponentHandle<Unit> bottom;
    entityx::ComponentHandle<Unit> right;
    entityx::ComponentHandle<Unit> left;
    unsigned char army;
    short int * bounds = mapx->getBounds();
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
                options.push_back(UNIT::MENU::SEIZE);
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
                        options.push_back(UNIT::MENU::TRADE);
                        options.push_back(UNIT::MENU::RESCUE);
                        break;

                    case UNIT::ARMY::ENEMY:
                    case UNIT::ARMY::BANDITS:
                    case UNIT::ARMY::KEWAC:
                    case UNIT::ARMY::NEUTRAL:
                    case UNIT::ARMY::IMPERIAL:
                        options.push_back(UNIT::MENU::ATTACK);
                        break;
                }
            } else {
                SDL_Log("No unit around");
            }
        }

        options.push_back(UNIT::MENU::WAIT);
        std::sort(options.begin(), options.end());
        menuoptions[MENU::UNIT] = options;
    } else {
        SDL_Log("Menuoptions: Unit not found.");
    }
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

void Game::setCursorstate(const short unsigned int new_state) {
    SDL_Log("Changing cursor");
    SDL_Rect temprect;

    short unsigned int * temp_tilesize;
    short int * bounds;
    temp_tilesize = mapx->getTilesize();

    if (cursorx.valid()) {
        switch (new_state) {
            case GAME::STATE::MAP:
                SDL_Log("Changed Cursor to Map");
                cursorx.component<Sprite>()->init(cursorx.component<Position>()->getPos());
                cursorx.component<Sprite>()->animate();
                cursorx.component<Sprite>()->setTexture("..//assets//mapcursors.png");
                cursorx.component<Sprite>()->setAnimation(10, 50);
                cursorx.component<Sprite>()->setTilesize(mapx->getTilesize());
                cursorx.component<Sprite>()->setSlidetype(SLIDETYPE::GEOMETRIC);
                // bounds = mapx->getBounds();
                // SDL_Log("bounds: %d %d %d %d", bounds[0], bounds[1], bounds[2], bounds[3]);
                // bounds[0] += DEFAULT::TILEMAP_XOFFSET;
                // bounds[2] += DEFAULT::TILEMAP_YOFFSET;
                cursorx.component<Position>()->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
                cursorx.component<Position>()->setBounds(mapx->getBounds());
                cursorx.component<Position>()->setPos(cursor_lastpos);
                cursorx.component<Position>()->setonTilemap(true);
                systems.system<RenderSystemx>()->setTilesize(temp_tilesize[0], temp_tilesize[1]);
                break;

            case GAME::STATE::UNITMENU:
                SDL_Log("Changed Cursor to unitmenu");
                temprect = {0, 0, 16, 16}; //x,y,w,h
                short int * unitmenupos;
                short int linespace = 1;
                cursorx.component<Sprite>()->still();
                cursorx.component<Sprite>()->setSrcrect(temprect);
                cursorx.component<Sprite>()->setDestrect(temprect);
                cursorx.component<Sprite>()->setTexture("..//assets//menucursor.png");

                if (menus[MENU::UNIT].valid()) {
                    unitmenupos = menus[MENU::UNIT].component<Position>()->getPos();
                    linespace = menus[MENU::UNIT].component<Text>()->getLinespacing();
                }

                short int menubounds[4];
                menubounds[0] = unitmenupos[0] / linespace;
                menubounds[1] = unitmenupos[0] / linespace;
                menubounds[2] = (short int)(unitmenupos[1] / linespace + 1);
                menubounds[3] = (short int)(unitmenupos[1] / linespace + menuoptions[MENU::UNIT].size());
                cursor_lastpos[0] = cursorx.component<Position>()->getPos()[0];
                cursor_lastpos[1] = cursorx.component<Position>()->getPos()[1];
                cursorx.component<Position>()->setBounds(menubounds);
                cursorx.component<Position>()->setPos(menubounds[0], menubounds[2]);
                cursorx.component<Position>()->setonTilemap(false);
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

    setCursorstate(state);
}

void Game::unloadCursor() {
    if (cursorx.valid()) {
        cursorx.destroy();
    }
}


void Game::loadUnitEntities(std::vector<short unsigned int> unit_inds, std::vector<std::vector<int>> positions_list) {
    SDL_Log("Loading Units\n");
    std::string asset_name;
    entityx::Entity Uent;

    for (int i = 0; i < unit_inds.size(); i++) {
        asset_name = "..//assets//" + units[unit_inds[i]].getName() + ".png";
        Uent = entities.create();
        Uent.assign<Unit>(units[unit_inds[i]]);
        Uent.assign<Position>();
        Uent.component<Position>()->setOffset(DEFAULT::TILEMAP_XOFFSET, DEFAULT::TILEMAP_YOFFSET);
        Uent.component<Position>()->setPos(positions_list[i][0], positions_list[i][1]);
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
        entityx::Entity Uent;

        for (int i = 0; i < map_arrivals.size(); i++) {
            if (map_arrivals[i].turn == currentturn) {

                if (units.find(map_arrivals[i].id) == units.end()) {
                    SDL_Log("unloaded units loading %d", map_arrivals[i].id);
                    loadUnits(std::vector<short int> {map_arrivals[i].id});
                    asset_name = "..//assets//" + units[map_arrivals[i].id].getName() + ".png";
                    SDL_Log("Loaded: %s", units[map_arrivals[i].id].getName().c_str());
                }

                Uent = entities.create();
                // Uent.assign<Position>(map_arrivals[i].position.x, map_arrivals[i].position.y);
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
        SDL_Log("Failed to loadMapArrivals.");
    }
}

void Game::loadUnits(unsigned char in_chap) {
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

    Game::font = TTF_OpenFont("../fonts/arial.ttf", settings.fontsize);

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
    systems.add<UnitSystemx>(this);
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

void configure(entityx::EventManager & event_manager) {

}

void Game::update(entityx::TimeDelta dt) {
    systems.update<ControlSystemx>(dt);
    systems.update<RenderSystemx>(dt);
}

void Game::testXMLMap() {
    SDL_Log("Testing Map xml writing and reading\n");
    Map map(32, 32);
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


