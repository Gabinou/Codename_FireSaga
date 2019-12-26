#include <stdio.h>
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "map.hpp"
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

Manager manager;

Entity & cursor = manager.addEntity(); // FOR SOME REASON, having an 2, 4, 8 entities.... breaks something in the unitmove->unitmenu states move
Entity & player = manager.addEntity();
Entity & player2 = manager.addEntity();

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
    // unit_entities.top().getComponent<PositionComponent>().setPos(newPos[0], newPos[1]);
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
        if (new_state == "unitmove") {
            std::vector<std::unique_ptr<Entity>> current_entities;
            for (int i=0; i < manager.getEntities().size(); i++) {
                if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
                    unit_entities.push(i);
                }
            }

            // int arrow_ind = manager.getEntities().size();
            // manager.addEntity();
            // manager.getEntities()[menu_entities.top()]->addComponent<PositionComponent>();         
            // // findpath();           
            // drawpath();
            // waitforotheraccept();            
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
            createUnitmenu(setting_entity); //Problem here even since slidecomponent was deleted.
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

void Game::baseWeapons(){
    Weapon_stats temp_wpn;
    Unit_stats temp_unit;
    // dmg, hit, dodge, crit, favor, wgt, range, hand, dmg_stype
    temp_wpn = {3, 80, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon wooden_sword("Wooden sword", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon kitchen_knife("Kitchen knife", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon fleuret("Fleuret", "sword", temp_wpn);
    fleuret.set_effective({"Armor"});
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon rapiere("Rapière", "sword", temp_wpn);
    rapiere.set_effective({"Armor"});
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon glaive("Glaive", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon saber("Saber", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon fauchon("Fauchon", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon iron_sword("Iron sword", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon steel_sword("Steel sword", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon damas_sword("Damas sword", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon merciful_blade("Merciful blade", "sword", temp_wpn);
    merciful_blade.set_effect("spare");
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon broadsword("Broadsword", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon espadon("Espadon", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon oodachi("Oodachi", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon uchigatana("Uchigatana", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon exsangue("Exsangue", "sword", temp_wpn);
    exsangue.set_effect("lifesteal");
    rapiere.set_effective({"Angel"});
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Hauteclaire("Hauteclaire", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Secundus("Secundus", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Excalibur("Excalibur", "sword", temp_wpn);
    rapiere.set_effective({"Demon"});
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Galatine("Galatine", "sword", temp_wpn);
    Galatine.set_effective({"Armor"});
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Raijintou("Raijintou", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Fuujintou("Fuujintou", "sword", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Honjou_Masamune("Honjou Masamune", "sword", temp_wpn);


    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon pitchfork("Pitchfork", "lance", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon fighthing_stick("Fighthing stick", "lance", temp_wpn);
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Achilles("Achilles", "lance", temp_wpn);
    Achilles.set_effect("1%kill");
    temp_wpn = {1, 50, 0, 0, 0, 3, {1,1}, 0, 0};
    Weapon Tonbokiri("Tonbokiri", "lance", temp_wpn);
 
}

void Game::baseUnits(){
    manager.addEntity();
    // Entity current_unit;
    Unit_stats temp;
          //hp,str,mag,skl,spd,luck,def,res,con,move 
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Main", "Mercenary", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(0); 
    manager.getEntities().back()->getComponent<UnitComponent>().write("unit_test.txt");
    all_units["Main"] = manager.getEntities().size() - 1;
    // getchar();

    manager.addEntity();
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    manager.getEntities().back()->addComponent<UnitComponent>("Reliable", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(100);
    all_units["Reliable"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    manager.getEntities().back()->addComponent<UnitComponent>("Coward", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(200);
    all_units["Coward"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager.getEntities().back()->addComponent<UnitComponent>("1H Jaigen", "Fencermaster", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(2200);
    all_units["1H Jaigen"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    manager.getEntities().back()->addComponent<UnitComponent>("Lovely", "Priestess", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(100);
    all_units["Lovely"] = manager.getEntities().size() - 1;
    
    manager.addEntity();
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager.getEntities().back()->addComponent<UnitComponent>("Hottie", "Thief", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(2200);
    all_units["Hottie"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(400);
    all_units["Silou"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    manager.getEntities().back()->addComponent<UnitComponent>("Servil", "Knight", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp); 
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(500);
    all_units["Servil"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Pérignon", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(1200);
    all_units["Pérignon"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(400);
    all_units["Poet"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Arm Thief", "Mercenary", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().set_caps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().set_growths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().set_exp(400);
    all_units["Arm Thief"] = manager.getEntities().size() - 1;

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
        
    // manager.setGame(this);
    
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

    printf("Creating map \n");
    tilesize[0] = 32;
    tilesize[1] = 32;
    mapp =  new Map(tilesize[0], tilesize[1]); // mapp is a pointer
    mapp->loadMap("..//testmap.txt");
    // allEntities["horse"] = &player;
    // allEntities["cursor"] = &cursor;
    
    cursor.addComponent<PositionComponent>(2, 2);
    
    player.addComponent<PositionComponent>(2, 2);
    player.getComponent<PositionComponent>().setMap(mapp);

    SDL_Color black = {255,255,255};
    cursor.addComponent<KeyboardController>(this, mapp);

    if( SDL_NumJoysticks() < 1 ) {
        printf( "No joysticks connected.\n" );
    } else {
        cursor.addComponent<GamepadController>(this, mapp);
    }

    player.addComponent<SpriteComponent>(mapp, "..//assets//horse.png");
    player.addComponent<UnitComponent>();
    cursor.addComponent<SpriteComponent>(mapp, "..//assets//cursors.png", 10, 50);
    cursor.getComponent<SpriteComponent>().setSlidetype("geometric");
    
    player.addGroup(manager.groupUnits);
    cursor.addGroup(manager.groupUI);


    Unit_stats test = {1,2,3};
    player.getComponent<UnitComponent>().set_caps(test);

    baseWeapons();
    baseUnits();

    // cursor.getComponent<SpriteComponent>().hide();

    this->setState("map");
    
};

void Game::update() {
    // player->update();
    // Map->loadMap();
    // mapp->loadMap("a");
    manager.refresh();
    manager.update();
    // player.getComponent<PositionComponent>().position.Add(Vector2D(1,2));
    // printf("%d, %d \n", newPlayer.getComponent<PositionComponent>().x(), newPlayer.getComponent<PositionComponent>().y());
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

// auto& tiles(manager.getGroup(groupMap))
std::vector<Entity *> & units = manager.getGroup(manager.groupUnits);
std::vector<Entity *> & uxs = manager.getGroup(manager.groupUI);

void Game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.
    // SDL_RenderCopy(renderer, playerTex, NULL, &destR);
    // textbox.destroy();
    mapp->drawMap();
    // manager.draw();
    for (auto& u : units) {
        // printf("unit\n");
        u->draw();
    }
    for (auto& u : uxs) {
        // printf("ux\n");
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
