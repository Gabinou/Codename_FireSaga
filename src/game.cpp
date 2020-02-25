#include "game.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "arrowcomponent.hpp"
#include "textcomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "unitcontainer.hpp"

SDL_Renderer * Game::renderer = nullptr;
TTF_Font * Game::font = NULL;
Manager Game::manager;

Game::Game() {}
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
        hit = (getRN() < rate); //single_roll
        return(hit);
    }
    if (mode == 2) {
        hit = (((getRN() + getRN())/2) < rate); //doubleroll
        return(hit);
    }
    return(hit);
}

bool * Game::checkHitCrit(int hit_rate, int crit_rate, std::string mode) {
    static bool hitcrit[2];
    if (mode == "single") {
        hitcrit[0] = (getRN() < hit_rate);
        hitcrit[1] = (getRN() < crit_rate);
        return(hitcrit);
    }
    if (mode == "double") {
        hitcrit[0] = (((getRN() + getRN())/2) < hit_rate);
        hitcrit[1] = (((getRN() + getRN())/2) < crit_rate);
        return(hitcrit);
    } 
    if (mode == "gaussian") {
        int * RNs = getGRNs();
        hitcrit[0] = (RNs[0] < hit_rate);
        hitcrit[1] = (RNs[1] < crit_rate);
        return(hitcrit);
    }
    if (mode == "hybrid") {
    
    }

    return(hitcrit);
}

void Game::attack(Unit * attacker, Unit * defender) {
    printf("%s attacks %s\n", attacker->getName().c_str(), defender->getName().c_str());
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
    printf("%s fights %s\n", attacker->getName().c_str(), defender->getName().c_str());
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
    printf("Making unit menu\n");
    menus["Unit menu"] = manager.getEntities().size();
    manager.addEntity();
    manager.getEntities()[menus["Unit menu"]]->addComponent<PositionComponent>();
    manager.getEntities()[menus["Unit menu"]]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
    manager.getEntities()[menus["Unit menu"]]->getComponent<PositionComponent>().setPos(
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[0] * settings.tilesize[0]),
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[1] * settings.tilesize[1]));
    SDL_Color black = {255, 255, 255};
    manager.getEntities()[menus["Unit menu"]]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); 
    manager.getEntities()[menus["Unit menu"]]->addComponent<TextComponent>(settings.fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
    manager.getEntities()[menus["Unit menu"]]->addGroup(manager.groupUI);
}

void Game::killMenu(std::string name) {
    if (menus[name] > -1) {
        manager.getEntities()[menus[name]]->destroy();
        menus[name] = -1;
    } else {
        printf("Could not destroy %s.", name.c_str());
    }
}

void Game::moveUnit(Entity & cursor) {
    int newPos[2];
    newPos[0] = cursor.getComponent<PositionComponent>().getPos()[0];
    newPos[1] = cursor.getComponent<PositionComponent>().getPos()[1];
    manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>();
}

void Game::setState(Entity & setting_entity, short unsigned int new_state) {
    printf("Game state changes from %d to %d\n", this->state, new_state); 

    switch (this->state) {
        case GAME::STATE::PAUSE:
            break;
        // case GAME::STATE::STATS:
        //     break;
        // case GAME::STATE::MINIMAP:
        //     break;
        // case GAME::STATE::MAP:
        //     switch (new_state) {
        //         case GAME::STATE::UNITMENU:
        //             moveUnit(setting_entity);
        //             break;
        //         case GAME::STATE::STATS:
        //             break;
        //         case GAME::STATE::MINIMAP:
        //             break;                
        //         case GAME::STATE::UNITMOVE:
        //             std::vector<std::unique_ptr<Entity>> current_entities;
        //             std::vector<std::vector<short int>> costmap;
        //             std::vector<std::vector<short int>> movemapp;
        //             std::vector<std::vector<short int>> attackmapp;
        //             short unsigned int start[2];
        //             unsigned char unitmvttype;
        //             short unsigned int unit_move;
        //             unsigned char * range;
                    
        //             for (int i = 0; i < manager.getEntities().size(); i++) {
        //                 if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
        //                     unit_entities.push(i);
        //                 }
        //             }

        //             short unsigned int current_unit_id = manager.getEntities()[unit_entities.top()]->getComponent<UnitContainer>().getID();
        //             unit_move = all_units[current_unit_id].getStats().move;

        //             start[0] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[0]; // Start is (+1,+1)?
        //             start[1] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[1]; // Start is (+1,+1)?
        //             start[0] = start[0] - 1;
        //             start[1] = start[1] - 1;

        //             unitmvttype = all_units[current_unit_id].getMvttype();
        //             range = all_units[current_unit_id].getRange();
        //             costmap = mapp->makeMvtCostmap(unitmvttype);

        //             movemapp = movemap(costmap, start, unit_move, "matrix");
        //             mapp->setOverlay(MAP::OVERLAY::MOVE, movemapp);

        //             attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
        //             mapp->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

        //             mapp->showOverlay();
        //             break;
        //         case GAME::STATE::OPTIONS:
        //             break;    
        //         case GAME::STATE::CUTSCENE:
        //             break;    
        //     }
        //     break;
        // case GAME::STATE::OPTIONS:
        //     switch (new_state) {
        //         case GAME::STATE::MAP:
        //             break;                
        //         case GAME::STATE::STARTMENU:
        //             break;
        //         }
        //     break;
        // case GAME::STATE::STARTMENU:
        //     switch (new_state) {
        //         case GAME::STATE::INTROVIDEO:
        //             break;                
        //         case GAME::STATE::SAVES:
        //             break;
        //         case GAME::STATE::SOUNDROOM:
        //             break;
        //         case GAME::STATE::OPTIONS:
        //             break;
        //         }
        //     break;
        // case GAME::STATE::PREPARATION:
        //     switch (new_state) {
        //         case GAME::STATE::PLACEMENT:
        //             break;                
        //         case GAME::STATE::CHOOSEUNITS:
        //             break;
        //         case GAME::STATE::EQUIPMENT:
        //             break;
        //         case GAME::STATE::FORGE:
        //             break;
        //         case GAME::STATE::STORAGE:
        //             break;               
        //         case GAME::STATE::BONUSEXP:
        //             break;       
        //         }
        //     break;
        // case GAME::STATE::PLACEMENT: 
        // case GAME::STATE::CHOOSEUNITS: 
        // case GAME::STATE::EQUIPMENT: 
        // case GAME::STATE::FORGE: 
        // case GAME::STATE::STORAGE: 
        // case GAME::STATE::BONUSEXP: 
        //     switch (new_state) {            
        //         case GAME::STATE::PREPARATION:
        //             break;
        //         case GAME::STATE::MAP:
        //             break;       
        //         }
        //     break;
        // case GAME::STATE::UNITMOVE:
        //     switch (new_state) {            
        //         case GAME::STATE::UNITMENU:
        //             mapp->hideOverlay();
        //             makeUnitmenu(setting_entity); 
        //             short int *new_position = setting_entity.getComponent<PositionComponent>().getPos();
        //             short int *old_position = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos();
                    
        //             printf("Old position %d, %d \n", old_position[0], old_position[1]);
        //             printf("New position %d, %d \n", new_position[0], new_position[1]);

        //             manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().setPos(            
        //                 new_position[0],
        //                 new_position[1]);
                    
        //             Entity * ontile = mapp->getTile(old_position[0], old_position[1]);
        //             mapp->removeTile(old_position[0], old_position[1]);
        //             mapp->setTile(new_position[0], new_position[1], ontile);
        //             break;
        //         case GAME::STATE::MAP:
        //             mapp->hideOverlay();
        //             break;       
        //         }
        //     break;
        // case GAME::STATE::UNITMENU:
        //     switch (new_state) {            
        //         case GAME::STATE::ATTACK:
        //             break;                
        //         case GAME::STATE::TRADE:
        //             break;                
        //         case GAME::STATE::ITEMS:
        //             break;                
        //         case GAME::STATE::CONVERSATION:
        //             break;
        //         case GAME::STATE::MAP:
        //             killMenu("Unit menu");
        //             break;       
        //         }
        //     break;
        // case GAME::STATE::CONVERSATION:
        //     switch (new_state) {            
        //         case GAME::STATE::MAP:
        //             break;                
        //         }
        //     break;
        // case GAME::STATE::ATTACK:
        //     switch (new_state) {            
        //         case GAME::STATE::BATTLE:
        //             break;                
        //         }
        //     break;
        }

    // if (this->state == "conversation") {
    //     if (new_state == "map") { 
        
    //     }
    // }
    
    // if (this->state == "attack") {
    //     if (new_state == "battle") { 
        
    //     }
    // }
    // if (this->state == "unitmenu") {
    //     if (new_state == "map") { 
    //         killMenu("Unit menu");
    //     }
    //     if (new_state == "attack") { 
        
    //     }
    //     if (new_state == "trade") { 
        
    //     }
    //     if (new_state == "items") { 
        
    //     }
    //     if (new_state == "conversation") { 
        
    //     }
    // }
    // if (this->state == "unitmove") {
    //     if (new_state == "unitmenu") {
    //         mapp->hideOverlay();
    //         makeUnitmenu(setting_entity); 
    //         short int *new_position = setting_entity.getComponent<PositionComponent>().getPos();
    //         short int *old_position = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos();
            
    //         printf("Old position %d, %d \n", old_position[0], old_position[1]);
    //         printf("New position %d, %d \n", new_position[0], new_position[1]);

    //         manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().setPos(            
    //             new_position[0],
    //             new_position[1]);
            
    //         Entity * ontile = mapp->getTile(old_position[0], old_position[1]);
    //         mapp->removeTile(old_position[0], old_position[1]);
    //         mapp->setTile(new_position[0], new_position[1], ontile);
    //     }
    //     if (new_state == "map") {
    //         mapp->hideOverlay();
    //     }
    // }    
    // if ((this->state == "placement") ||
    //  (this->state == "chooseunits") ||
    //  (this->state == "equipment") ||
    //  (this->state == "forge") ||
    //  (this->state == "storage") ||
    //  (this->state == "bonusexp")){
    //     if (new_state == "preparation") {
    //     }
    //     if (new_state == "map") {
    //     }     
    // }
    
    // if (this->state == "preparation") {
    //     if (new_state == "placement") {
    //     }
    //     if (new_state == "chooseunits") {
    //     }
    //     if (new_state == "equipment") {
    //     }
    //     if (new_state == "forge") {
    //     }        
    //     if (new_state == "storage") {
    //     }        
    //     if (new_state == "bonusexp") {
    //     }        
    // }
    // if (this->state == "startmenu") {
    //     if (new_state == "introvideo") {
    //     }
    //     if (new_state == "saves") {
    //     }
    //     if (new_state == "soundroom") {
    //     }
    //     if (new_state == "options") {
    //     }
    // }
    // if (this->state == "map") {
    //     if (new_state == "unitmenu") {
    //         moveUnit(setting_entity);
    //         // Entity ** cursor = manager.getEntities()[unit_entities.top()];
    //     }
    //     if (new_state == "stats") {
            
    //     }
    //     if (new_state == "minimap") {
            
    //     }
    //     if (new_state == "unitmove") {
    //         std::vector<std::unique_ptr<Entity>> current_entities;
    //         std::vector<std::vector<short int>> costmap;
    //         std::vector<std::vector<short int>> movemapp;
    //         std::vector<std::vector<short int>> attackmapp;
    //         short unsigned int start[2];
    //         unsigned char unitmvttype;
    //         short unsigned int unit_move;
    //         unsigned char * range;
            
    //         for (int i = 0; i < manager.getEntities().size(); i++) {
    //             if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
    //                 unit_entities.push(i);
    //             }
    //         }

    //         short unsigned int current_unit_id = manager.getEntities()[unit_entities.top()]->getComponent<UnitContainer>().getID();
    //         unit_move = all_units[current_unit_id].getStats().move;

    //         start[0] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[0]; // Start is (+1,+1)?
    //         start[1] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[1]; // Start is (+1,+1)?
    //         start[0] = start[0] - 1;
    //         start[1] = start[1] - 1;

    //         unitmvttype = all_units[current_unit_id].getMvttype();
    //         range = all_units[current_unit_id].getRange();
    //         costmap = mapp->makeMvtCostmap(unitmvttype);

    //         movemapp = movemap(costmap, start, unit_move, "matrix");
    //         mapp->setOverlay(MAP::OVERLAY::MOVE, movemapp);

    //         attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
    //         mapp->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

    //         mapp->showOverlay();
    //     }
    //     if (new_state == "options") {
            
    //     }
    //     if (new_state == "cutscene") {
            
    //     }
    // }
    
    // if (this->state == "options") {
    //     if (new_state == "map") {
  
    //     }
    //     if (new_state == "startmenu") {
  
    //     }
    // }

    state = new_state;
}

short unsigned int Game::getState() {
    return(state);
}

template <typename T> void Game::loadTiles(std::vector<T> in_tiles) {
    loaded_tiles = baseTiles(in_tiles);
}

void Game::loadMap(const std::string filename) {
    printf("Loading map \n");
    // For this function, tiles have to be loaded manually somwhere else.
    mapp =  new Map(settings.tilesize[0], settings.tilesize[1]); // mapp is a pointer
    mapp->loadTilemap(filename);
}

void Game::loadMap(const int in_map_index) {
    printf("Loading map \n");
    loaded_tiles = baseTiles(chapTiles[in_map_index]());
    mapp =  new Map(settings.tilesize[0], settings.tilesize[1]); // mapp is a pointer
    mapp->loadTilemap(in_map_index);
    mapp->loadEnemyinds(in_map_index);
    // printf("Testing tiles: %s\n", loaded_tiles[10].getName().c_str());
}

void Game::loadCursor() {
    // Map should be loaded before I think.
    if (this->state == GAME::STATE::MAP) {
        cursor.addComponent<PositionComponent>(6, 6);
        cursor.addComponent<KeyboardController>(this, mapp);
        if (SDL_NumJoysticks() < 1) {
            printf( "No joysticks connected.\n" );
        } else {
            cursor.addComponent<GamepadController>(this, mapp);
        }
        cursor.addComponent<SpriteComponent>(mapp, "..//assets//cursors.png", 10, 50);
        cursor.getComponent<SpriteComponent>().setSlidetype("geometric");
        cursor.addGroup(manager.groupUI);
    }
}

void Game::loadUnits(std::vector<short unsigned int> unit_inds, std::vector<std::string> asset_names, std::vector<std::vector<int>> positions_list) {
    printf("Loading Units. \n");
    for (int i = 0; i < unit_inds.size(); i++) { 
        all_units[unit_inds[i]].setEntity(manager.getEntities().size());
        manager.addEntity();
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->addComponent<PositionComponent>(positions_list[i][0], positions_list[i][1]);
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->addComponent<UnitContainer>(unit_inds[i]);
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->getComponent<PositionComponent>().setMap(mapp); //Should mapp be an input? No mapp is always same ?
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->addComponent<SpriteComponent>(mapp, asset_names[i].c_str());
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->addGroup(manager.groupUnits);
        manager.getEntities()[all_units[unit_inds[i]].getEntity()]->addGroup(manager.groupUnits);
    }
}

void Game::loadMapEnemies() {
    printf("Loading map enemies. \n");
    std::vector<short unsigned int> map_enemies = mapp->getEnemies(); 
    for (int i = 0; i < map_enemies.size(); i++) {
        all_units[map_enemies[i]].setEntity(manager.getEntities().size());
        manager.addEntity();
    }
}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
            
    Game::font = TTF_OpenFont("../fonts/arial.ttf", settings.fontsize); // Size translates to pixel size? 
    //The srcrect does not change size with font pointsize.

    if (Game::font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        printf("SDL subsystems initialized.\n");
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window) {
            printf("Window created.\n");
        }

        if (TTF_Init() == -1) {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Renderer created.\n");
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    // printf("Testing script in game: %s\n", all_scripts["Chapter 1"].getScene("Intro").getLine("1").line.c_str());

    this->setState(GAME::STATE::MAP);
    
};

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

std::vector<Entity *> & units = Game::manager.getGroup(Game::manager.groupUnits);
std::vector<Entity *> & uxs = Game::manager.getGroup(Game::manager.groupUI);

void Game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.

    mapp->drawMap(renderer);

    for (auto& u : units) {
        u->draw();
    }

    for (auto& u : uxs) {
        u->draw();
    }

    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    printf("Game cleanded.");
    SDL_Delay(5000);
}
bool Game::running() {
    return (isRunning);
}
