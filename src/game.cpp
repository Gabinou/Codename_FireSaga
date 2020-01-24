#include "game.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "arrowcomponent.hpp"
#include "textcomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "unitcomponent.hpp"

Map * mapp;

SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;
TTF_Font * Game::font = NULL;
Manager Game::manager;
Settings Game::settings;

Entity & cursor = Game::manager.addEntity(); // FOR SOME REASON, having an 2, 4, 8 entities.... breaks something in the unitmove->unitmenu states move
Entity & player = Game::manager.addEntity();
Entity & player2 = Game::manager.addEntity();

Game::Game() {}
Game::~Game() {}

void Game::setFontsize(int in_fontsize) {
    fontsize = in_fontsize;
}

Game::getFontsize() {
    return(fontsize);
}

void Game::createUnitmenu(Entity & setting_entity) {
    printf("Creating unit menu\n");
    menu_entities.push(manager.getEntities().size());
    manager.addEntity();
    manager.getEntities()[menu_entities.top()]->addComponent<PositionComponent>();
    manager.getEntities()[menu_entities.top()]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
    manager.getEntities()[menu_entities.top()]->getComponent<PositionComponent>().setPos(
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[0] * tilesize[0]),
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[1] * tilesize[1]));
    SDL_Color black = {255, 255, 255};
    manager.getEntities()[menu_entities.top()]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); 
    manager.getEntities()[menu_entities.top()]->addComponent<TextComponent>(fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
    manager.getEntities()[menu_entities.top()]->addGroup(manager.groupUI);
}

void Game::destroyUnitmenu() {
    if (!menu_entities.empty()) {
        manager.getEntities()[menu_entities.top()]->destroy();
        menu_entities.pop();
    }  else {
        printf("Could not destroy unit menu.");
    }
}

void Game::moveUnit(Entity & cursor) {
    int newPos[2];
    newPos[0] = cursor.getComponent<PositionComponent>().getPos()[0];
    newPos[1] = cursor.getComponent<PositionComponent>().getPos()[1];
    manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>();
}

void Game::setState(Entity & setting_entity, std::string new_state) {
    printf("Game state changes from %s to %s\n", this->state.c_str(), new_state.c_str()); 

    if (new_state == "pause"){
        
    }
    if (this->state == "pause") {
         
    }
    if (this->state == "stats") {
        
    }
    if (this->state == "minimap") {
         
    }
    
    if (this->state == "map") {
        if (new_state == "unitmenu") {
            moveUnit(setting_entity);
            // Entity ** cursor = manager.getEntities()[unit_entities.top()];
        }
        if (new_state == "stats") {
            
        }
        if (new_state == "minimap") {
            
        }
        if (new_state == "unitmove") { // GREAT BIG FPS DROP HERE.
            std::vector<std::unique_ptr<Entity>> current_entities;
            std::vector<std::vector<int>> costmap;
            std::vector<std::vector<int>> movemapp;
            // std::vector<std::vector<int>> edges; 
            int start[2];
            std::string unitmvttype;
            int unit_move;
            unsigned char * range;

            for (int i = 0; i < manager.getEntities().size(); i++) {
                if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
                    unit_entities.push(i);
                }
            }
            unit_move = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getStats().move; //0 for horse.

            start[0] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[0]; // Start is (+1,+1)?
            start[1] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[1]; // Start is (+1,+1)?
            start[0] = start[0] - 1;
            start[1] = start[1] - 1;

            unitmvttype = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getMvttype();
            range = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getRange();// THIS DOESNT WORK.

            costmap = mapp->makeMvtCostmap(unitmvttype);

            movemapp = movemap(costmap, start, unit_move, "matrix");
            mapp->setMap("move", movemapp);
            // plot2Dvector(movemapp);

            // edges = matrix_edges(movemapp);
            // plot2Dvector(edges);

            if ((range[0] > 0) && (range[1] > 0)) {
                std::vector<std::vector<int>> attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
                // std::vector<std::vector<int>> attackmapp = attackmap(movemapp, start, unit_move + (int)range[0] - 1, (int)range[1], "matrix");
                // plot2Dvector(attackmapp);
                mapp->setMap("attack", attackmapp);
            }
     
            mapp->showOverlay();
        }
        if (new_state == "options") {
            
        }
        if (new_state == "cutscene") {
            
        }
    }
    
    if (this->state == "options") {
        if (new_state == "map") {
  
        }
        if (new_state == "startmenu") {
  
        }
    }
    
    if (this->state == "startmenu") {
        if (new_state == "introvideo") {
        }
        if (new_state == "saves") {
        }
        if (new_state == "soundroom") {
        }
        if (new_state == "options") {
        }
    }
    
    if (this->state == "preparation") {
        if (new_state == "placement") {
        }
        if (new_state == "chooseunits") {
        }
        if (new_state == "equipment") {
        }
        if (new_state == "forge") {
        }        
        if (new_state == "storage") {
        }        
        if (new_state == "bonusexp") {
        }        
    }
    if ((this->state == "placement") ||
     (this->state == "chooseunits") ||
     (this->state == "equipment") ||
     (this->state == "forge") ||
     (this->state == "storage") ||
     (this->state == "bonusexp")){
        if (new_state == "preparation") {
        }
        if (new_state == "map") {
        }     
    }
    
    if (this->state == "unitmove") {
        if (new_state == "unitmenu") {
            mapp->hideOverlay();
            createUnitmenu(setting_entity); 
            int *new_position = setting_entity.getComponent<PositionComponent>().getPos();
            int *old_position = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos();
            
            printf("Old position %d, %d \n", old_position[0], old_position[1]);
            printf("New position %d, %d \n", new_position[0], new_position[1]);

            manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().setPos(            
                new_position[0],
                new_position[1]);
            
            Entity * ontile = mapp->getTile(old_position[0], old_position[1]);
            mapp->removeTile(old_position[0], old_position[1]);
            mapp->setTile(new_position[0], new_position[1], ontile);
        }
        if (new_state == "map") {
            mapp->hideOverlay();
        }
    }        
    
    if (this->state == "unitmenu") {
        if (new_state == "map") { 
            destroyUnitmenu();
        }
        if (new_state == "attack") { 
        
        }
        if (new_state == "trade") { 
        
        }
        if (new_state == "items") { 
        
        }
        if (new_state == "conversation") { 
        
        }
    }

    if (this->state == "conversation") {
        if (new_state == "map") { 
        
        }
    }
    
    if (this->state == "attack") {
        if (new_state == "battle") { 
        
        }
    }
    
    state = std::string(new_state);
}

void Game::setState(const char * new_state) {
    state = std::string(new_state);
}

std::string Game::getState() {
    return(state);
}

// void Game::baseUnits(){
//     manager.addEntity();
//     Unit_stats temp;
//     //hp,str,mag,skl,spd,luck,def,res,con,move 
//     temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Main", "Mercenary", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(0); 
//     manager.getEntities().back()->getComponent<UnitComponent>().write("unit_test.txt");
//     all_units["Main"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
//     manager.getEntities().back()->addComponent<UnitComponent>("Reliable", "Cavalier", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(100);
//     all_units["Reliable"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
//     manager.getEntities().back()->addComponent<UnitComponent>("Coward", "Cavalier", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(200);
//     all_units["Coward"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
//     manager.getEntities().back()->addComponent<UnitComponent>("1H Jaigen", "Fencermaster", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(2200);
//     all_units["1H Jaigen"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Lovely", "Priestess", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(100);
//     all_units["Lovely"] = manager.getEntities().size() - 1;
    
//     manager.addEntity();
//     temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
//     manager.getEntities().back()->addComponent<UnitComponent>("Hottie", "Thief", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(2200);
//     all_units["Hottie"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
//     manager.getEntities().back()->addComponent<UnitComponent>("Servil", "Knight", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp); 
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(500);
//     all_units["Servil"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Pérignon", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(1200);
//     all_units["Pérignon"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Poet"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Silou"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Arm Thief"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Mage2"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Mage2"] = manager.getEntities().size() - 1;

//     manager.addEntity();
//     temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
//     manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
//     temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
//     manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
//     temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
//     manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
//     manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
//     all_units["Mage3"] = manager.getEntities().size() - 1;
// }

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
            
    Game::font = TTF_OpenFont("../fonts/arial.ttf", Game::fontsize); // Size translates to pixel size? 
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

    // printf("Making units\n");
    // baseUnits();

    // printf("Making tiles\n");
    // baseTiles();

    printf("Creating map \n");
    tilesize[0] = 32;
    tilesize[1] = 32;
    mapp =  new Map(tilesize[0], tilesize[1]); // mapp is a pointer
    mapp->loadTilemap("..//maps//test_tilemap.txt");

    cursor.addComponent<PositionComponent>(6, 6);

    manager.getEntities()[all_units["Silou"]]->addComponent<PositionComponent>(6, 6);
    manager.getEntities()[all_units["Silou"]]->getComponent<PositionComponent>().setMap(mapp);

    SDL_Color black = {255, 255, 255};
    cursor.addComponent<KeyboardController>(this, mapp);

    if( SDL_NumJoysticks() < 1 ) {
        printf( "No joysticks connected.\n" );
    } else {
        cursor.addComponent<GamepadController>(this, mapp);
    }

    manager.getEntities()[all_units["Silou"]]->addComponent<SpriteComponent>(mapp, "..//assets//horse.png");

    cursor.addComponent<SpriteComponent>(mapp, "..//assets//cursors.png", 10, 50);
    cursor.getComponent<SpriteComponent>().setSlidetype("geometric");
    
    manager.getEntities()[all_units["Silou"]]->addGroup(manager.groupUnits);
    cursor.addGroup(manager.groupUI);
    
    printf("in game: %s\n", all_scripts["Chapter 1"].getScene("Intro").getLine("1").line.c_str());

    this->setState("map");
    
};

void Game::update() {
    manager.refresh();
    manager.update();
}
 
KeyboardInputMap Game::getKeyboardInputMap() {
    return(keyboardInputMap);
}

GamepadInputMap Game::getGamepadInputMap() {
    return(gamepadInputMap);
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

    mapp->drawMap();

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
