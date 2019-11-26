#include <stdio.h>
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "map.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "slidecomponent.hpp"
#include "textcomponent.hpp"
#include "keyboardcontroller.hpp"

Map * mapp;

SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;
TTF_Font * Game::font = NULL;

Manager manager;

Entity & player = manager.addEntity();
Entity & cursor = manager.addEntity();
// Entity & textbox = manager.addEntity();

Game::Game() {}
Game::~Game() {}

void Game::setFontsize(int in_fontsize) {
    fontsize = in_fontsize;
}

Game::getFontsize() {
    return(fontsize);
}

void Game::unitmenu(Entity & setting_entity) {
    Manager & manager = setting_entity.getManager();
    int entity_num = manager.getEntities().size();
    if (unitmenuIndex == -1) {
        printf("Making textbox const char\n");
        manager.addEntity();
        manager.getEntities()[entity_num]->addComponent<PositionComponent>();
        manager.getEntities()[entity_num]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
        manager.getEntities()[entity_num]->getComponent<PositionComponent>().setPos(
        (int)(setting_entity.getComponent<PositionComponent>().getPos().x * tilesize[0]),
        (int)(setting_entity.getComponent<PositionComponent>().getPos().y * tilesize[1]));
        SDL_Color black = {255, 255, 255};
        manager.getEntities()[entity_num]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); 
        manager.getEntities()[entity_num]->addComponent<TextComponent>(fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
        manager.getEntities()[entity_num]->addGroup(manager.groupUI);
        unitmenuIndex = entity_num;
    } else if (unitmenuIndex < entity_num) {
        printf("Trying to destroy textbox number %d \n", manager.getEntities().size());
        manager.getEntities()[unitmenuIndex]->destroy();
        unitmenuIndex = -1;
    } else {
        printf("Unit Menu creation/destruction error.");
    }
}

void Game::setState(Entity & setting_entity, const char * in_state) {

    if ((this->state == "map") && (in_state == "unitmenu")) {
        unitmenu(setting_entity);
    }
    
    if ((this->state == "unitmenu") && (in_state == "map")) { 
        unitmenu(setting_entity);
    }
    state = std::string(in_state);
}

void Game::setState(const char * in_state) {
    state = std::string(in_state);
}

std::string Game::getState() {
    return(state);
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
    mapp =  new Map(tilesize[0], tilesize[1]);
    mapp->loadMap("..//testmap.txt");
    // allEntities["horse"] = &player;
    // allEntities["cursor"] = &cursor;
    
    cursor.addComponent<PositionComponent>(2, 2);
    
    player.addComponent<PositionComponent>(0, 0);

    // textbox.addComponent<PositionComponent>(200, 200);

    SDL_Color black = {255,255,255};
    cursor.addComponent<KeyboardController>(this );
    player.addComponent<SpriteComponent>(mapp, "..//assets//horse.png");
    cursor.addComponent<SlideComponent>(mapp, "..//assets//cursors.png", 10, 50);
    // textbox.addComponent<SpriteComponent>("..//assets//textbox.png", (int []){128, 128});
    // textbox.addComponent<TextComponent>(Game::fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
    
    player.addGroup(manager.groupUnits);
    cursor.addGroup(manager.groupUI);

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
 
InputMap Game::getInputMap(){
    return(inputMap);
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
