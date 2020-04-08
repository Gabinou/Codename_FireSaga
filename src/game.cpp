#include "game.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "arrowcomponent.hpp"
#include "textcomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "unitcontainer.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

SDL_Renderer * Game::renderer = nullptr;
TTF_Font * Game::font = NULL;
Manager Game::manager;

Game::Game() {
    menus.resize(GAME::STATE::END);
}
Game::~Game() {}

Settings Game::getSettings() {
    return(settings);
}

void Game::setSettings(Settings in_settings) {
    settings = in_settings;
}

bool Game::checkRate(int rate, short unsigned int mode) {
    bool hit;
    if (mode == 1) {
        hit = (getURN(tinymt) < rate); //single_roll
        return(hit);
    }
    if (mode == 2) {
        hit = (((getURN(tinymt) + getURN(tinymt))/2) < rate); //doubleroll
        return(hit);
    }
    return(hit);
}

bool * Game::checkHitCrit(int hit_rate, int crit_rate, short unsigned int mode) {
    static bool hitcrit[2];
    if (mode == GAME::RN::SINGLE) {
        hitcrit[0] = (getURN(tinymt) < hit_rate);
        hitcrit[1] = (getURN(tinymt) < crit_rate);
        return(hitcrit);
    }
    if (mode == GAME::RN::DOUBLE) {
        hitcrit[0] = (((getURN(tinymt) + getURN(tinymt))/2) < hit_rate);
        hitcrit[1] = (((getURN(tinymt) + getURN(tinymt))/2) < crit_rate);
        return(hitcrit);
    } 
    if (mode == GAME::RN::GAUSSIAN) {
        unsigned char * RNs;
        RNs[0] = getURN(tinymt);
        RNs[1] = getURN(tinymt);
        RNs = boxmuller(RNs);
        hitcrit[0] = (RNs[0] < hit_rate);
        hitcrit[1] = (RNs[1] < crit_rate);
        return(hitcrit);
    }
    if (mode == GAME::RN::HYBRID) {
    
    }

    return(hitcrit);
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
    settings.FPS.entity = manager.getEntities().size();
    manager.addEntity();
    manager.getEntities()[settings.FPS.entity]->addComponent<PositionComponent>();
    manager.getEntities()[settings.FPS.entity]->getComponent<PositionComponent>().setBounds(0, settings.res.x, 0, settings.res.y);
    manager.getEntities()[settings.FPS.entity]->getComponent<PositionComponent>().setPos(settings.FPS.pos.x, settings.FPS.pos.y);
    manager.getEntities()[settings.FPS.entity]->addComponent<TextComponent>(settings.fontsize, "60", settings.FPS.textcolor);
    manager.getEntities()[settings.FPS.entity]->getComponent<TextComponent>().setSizefactor(settings.FPS.sizefactor);
    manager.getEntities()[settings.FPS.entity]->addGroup(manager.groupUI);
}

void Game::makeUnitmenu(Entity & setting_entity) {
    SDL_Log("Making unit menu\n");
    menus[GAME::STATE::UNITMENU] = manager.getEntities().size();
    manager.addEntity();
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->addComponent<PositionComponent>();
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->getComponent<PositionComponent>().setPos(
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[0] * settings.tilesize[0]),
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[1] * settings.tilesize[1]));
    SDL_Color black = {255, 255, 255};
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); 
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->addComponent<TextComponent>(settings.fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
    manager.getEntities()[menus[GAME::STATE::UNITMENU]]->addGroup(manager.groupUI);
}

void Game::killMenu(short unsigned int index) {
    if (menus[index] > -1) {
        manager.getEntities()[menus[index]]->destroy();
        menus[index] = -1;
    } else {
        SDL_Log("Could not destroy menu %d.", index);
    }
}

void Game::moveUnit(Entity & cursor) {
    // USELESS?
    int newPos[2];
    newPos[0] = cursor.getComponent<PositionComponent>().getPos()[0];
    newPos[1] = cursor.getComponent<PositionComponent>().getPos()[1];
    manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>();
}

// I think this function is too big. Find a way to reduce it...
void Game::setState(Entity & setting_entity, short unsigned int new_state) {
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
                case GAME::STATE::UNITMENU:
                    moveUnit(setting_entity);
                    break;
                case GAME::STATE::STATS:
                    break;
                case GAME::STATE::MINIMAP:
                    break;                
                case GAME::STATE::UNITMOVE: {
                    SDL_Log("Changing to unitmove\n");
                    std::vector<std::vector<short int>> costmap;
                    std::vector<std::vector<short int>> movemapp;
                    std::vector<std::vector<short int>> attackmapp;
                    short unsigned int start[2];
                    short unsigned int unit_move;
                    unsigned char unitmvttype;
                    unsigned char * range;
                    
                    for (int i = 0; i < manager.getEntities().size(); i++) {
                        if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
                            unit_entities.push(i);
                        }
                    }

                    short unsigned int current_unit_id = manager.getEntities()[unit_entities.top()]->getComponent<UnitContainer>().getID();
                    unit_move = units[current_unit_id].getStats().move;

                    start[0] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[0]; // Start is (+1,+1)?
                    start[1] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[1]; // Start is (+1,+1)?
                    start[0] = start[0] - 1;
                    start[1] = start[1] - 1;

                    unitmvttype = units[current_unit_id].getMvttype();
                    range = units[current_unit_id].getRange();
                    costmap = mapp->makeMvtCostmap(unitmvttype);

                    movemapp = movemap(costmap, start, unit_move, "matrix");
                    mapp->setOverlay(MAP::OVERLAY::MOVE, movemapp);

                    attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
                    mapp->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

                    mapp->showOverlay();

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
                    mapp->hideOverlay();
                    makeUnitmenu(setting_entity); 
                    short int *new_position = setting_entity.getComponent<PositionComponent>().getPos();
                    short int *old_position = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos();
                    
                    SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
                    SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);

                    manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().setPos(            
                        new_position[0],
                        new_position[1]);
                    
                    Entity * ontile = mapp->getTile(old_position[0], old_position[1]);
                    mapp->removeTile(old_position[0], old_position[1]);
                    mapp->setTile(new_position[0], new_position[1], ontile);
                    }
                    break;
                case GAME::STATE::MAP:
                    mapp->hideOverlay();
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
                    killMenu(GAME::STATE::UNITMENU);
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
    return(state);
}

// template <typename T> void Game::loadTiles(std::vector<T> in_tiles) {
//     loaded_tiles = baseTiles(in_tiles);
// }

void Game::loadMap(const std::string filename) {
    SDL_Log("Loading Map: %s \n", filename);
    // For this function, tiles have to be loaded manually somwhere else.
    if (!mapp) {
        mapp = new Map(settings.tilesize[0], settings.tilesize[1]); // mapp is a pointer
        mapp->setRenderer(renderer);
        mapp->loadTilemap(filename);
    }  else {
        SDL_Log("Failed to loadMap. Was mapp deleted previously?");
    }
}

void Game::loadMap(const int in_map_index) {
    SDL_Log("Loading Map index: %d \n", in_map_index);
    if (!mapp) {
        mapp = new Map(settings.tilesize[0], settings.tilesize[1]);
        mapp->loadTiles(in_map_index);
        mapp->setRenderer(renderer);
        mapp->loadTilemap(in_map_index);
        // mapp->loadEnemyinds(in_map_index);
    } else {
        SDL_Log("Failed to loadMap. Was mapp deleted previously?");
    }
}

void Game::unloadMap() {
    SDL_Log("Unloading Map");
    if (!mapp) {
        delete mapp;
    } else {
        SDL_Log("Failed to unloadMap. Was mapp deleted previously?");
    }
}

void Game::loadCursor() {
    // Map should be loaded before I think.
    if (this->state == GAME::STATE::MAP) {
        cursor.addComponent<PositionComponent>(6, 6);
        cursor.addComponent<KeyboardController>(this, mapp);
        if (SDL_NumJoysticks() < 1) {
            SDL_Log( "No joysticks connected.\n" );
        } else {
            cursor.addComponent<GamepadController>(this, mapp);
        }
        cursor.addComponent<SpriteComponent>(mapp, "..//assets//cursors.png", 10, 50);
        cursor.getComponent<SpriteComponent>().setSlidetype("geometric");
        cursor.addGroup(manager.groupUI);
    }
}

void Game::loadUnitEntities(std::vector<short unsigned int> unit_inds, std::vector<std::vector<int>> positions_list) {
    SDL_Log("Loading Units\n");
    std::string asset_name;
    Unit Utemp;
    for (int i = 0; i < unit_inds.size(); i++) { 
        Utemp = units[unit_inds[i]];
        asset_name = "..//assets//" +  Utemp.getName() + ".png";
        Utemp.setEntity(manager.getEntities().size());
        manager.addEntity();
        manager.getEntities()[Utemp.getEntity()]->addComponent<PositionComponent>(positions_list[i][0], positions_list[i][1]);
        manager.getEntities()[Utemp.getEntity()]->addComponent<UnitContainer>(unit_inds[i]);
        manager.getEntities()[Utemp.getEntity()]->getComponent<PositionComponent>().setMap(mapp);
        manager.getEntities()[Utemp.getEntity()]->addComponent<SpriteComponent>(mapp, asset_name.c_str());
        manager.getEntities()[Utemp.getEntity()]->addGroup(manager.groupUnits);
    }
}
void Game::loadUnits(unsigned char in_chap) {
    // Not necessary. I think would be better to load the party from a savefile or something.
    std::vector<short int> toload = chapBaseUnitsInds[in_chap]();
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

void Game::loadMapEnemies() {
    SDL_Log("Loading map enemies. \n");
    if (mapp) {
        std::vector<Map_arrival> map_arrivals = mapp->getEnemies(); 
        unsigned short int currentturn = mapp->getTurn();
        std::string asset_name;
        Unit Utemp;
        for (int i = 0; i < map_arrivals.size(); i++) {
            if (map_arrivals[i].arrivalturn == currentturn) {
                Utemp = units[map_arrivals[i].id];
                asset_name = "..//assets//" +  Utemp.getName() + ".png";
                Utemp.setEntity(manager.getEntities().size());
                manager.addEntity();
                manager.getEntities()[Utemp.getEntity()]->addComponent<PositionComponent>(map_arrivals[i].position.x, map_arrivals[i].position.y);
                manager.getEntities()[Utemp.getEntity()]->addComponent<UnitContainer>(Utemp.getid());
                manager.getEntities()[Utemp.getEntity()]->getComponent<PositionComponent>().setMap(mapp);
                manager.getEntities()[Utemp.getEntity()]->addComponent<SpriteComponent>(mapp, asset_name.c_str());
                manager.getEntities()[Utemp.getEntity()]->addGroup(manager.groupUnits);
            }
        }
    } else {
        SDL_Log("Failed to loadMapEnemies.");
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
    while(ptemp) {
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
        SDL_Log("copSaveXML Game from %s to %s\n", filenamefrom, filenameto);
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

void Game::update() {
    manager.refresh();
    manager.update();
}
 
KeyboardInputMap Game::getKeyboardInputMap() {
    return(keyboardInputMap);
}
void Game::setKeyboardInputMap(KeyboardInputMap in_keyboardInputMap) {
    keyboardInputMap = in_keyboardInputMap;
}
GamepadInputMap Game::getGamepadInputMap() {
    return(gamepadInputMap);
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

std::vector<Entity *> & unitEntities = Game::manager.getGroup(Game::manager.groupUnits);
std::vector<Entity *> & uxEntities = Game::manager.getGroup(Game::manager.groupUI);

void Game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.

    mapp->drawMap();

    for (auto& u : unitEntities) {
        u->draw();
    }

    for (auto& u : uxEntities) {
        u->draw();
    }

    SDL_RenderPresent(renderer);
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

// loss conditions
bool essentialDied(Map * in_map, const Narrative * in_narrative) { 
    std::vector<short unsigned int> essentials = in_map->getEssentials();
    for (int i = 0; i < essentials.size(); i++) {
        if (in_narrative->death[essentials[i]]) {
            return(true);
        }
    }
    return(false);
}

// win conditions
bool escaped(Map * in_map) {
    return(false);
}

bool Seized(Map * in_map) {
    //Maybe not necessary? maybe win is called when the throne is seized?
    return(in_map->getSeized());
}

bool talked() {
    return(false);
}

bool bossDied(Map * in_map) {
    return(in_map->getBossDeath());
}

bool rout(Map * in_map) {
    return((in_map->getnumEnemies() < 1));
}

bool areAlive() {
    return(false);
}

bool positionCondition(Unit * in_unit, Map_condition * in_mcond) {
    // Called by the game everytime a unit moves.
    if (in_mcond->army > 0) {
        if (in_mcond->army != in_unit->getArmy()) {
            return(false);
        }
    }
    if (in_mcond->unit > 0) {
        if (in_mcond->unit != in_unit->getid()) {
            return(false);
        }
    }
    short int * unitpos = in_unit->getPos();

    if (unitpos[0] < std::max(0, (int)in_mcond->colmin)) {
        return(false);
    }
    if (unitpos[0] > std::min(255, (int)in_mcond->colmax)) {
        return(false);
    }
    if (unitpos[1] < std::max(0, (int)in_mcond->rowmin)) {
        return(false);
    }
    if (unitpos[1] > std::min(255, (int)in_mcond->rowmax)) {
        return(false);
    }
    return(true);
}
