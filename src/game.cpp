#include "texturemanager.hpp"
#include "game.hpp"
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "gameobject.hpp"
#include "map.hpp"

gameobject* player;
Map* mapp;

SDL_Renderer* game::renderer = nullptr;

game::game() {}
game::~game() {}

void game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    
    int flags = 0;
    
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        printf("SDL subsystems initialized.\n");
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            printf("Window created.\n");
        }
            
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Renderer created.\n");
        }        
        isRunning = true;
    } else {
        isRunning = false;
    }
    
    // playerTex = texturemanager::loadtexture("..//assets//horse.png", renderer);
    player =  new gameobject("..//assets//horse.png", 0, 0);
    mapp =  new Map();
    
};

void game::update() {

    player->update();
    // Map->loadMap();
    printf("%d \n", count);
    
}

void game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}
void game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.
    // SDL_RenderCopy(renderer, playerTex, NULL, &destR);
    mapp->drawMap();
    player->render();
    
    SDL_RenderPresent(renderer);
}
void game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    printf("Game cleanded.");
    SDL_Delay(5000);
}
bool game::running() {
    return(isRunning);
}
