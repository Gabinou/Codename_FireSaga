#include "texturemanager.hpp"
#include "game.hpp"
#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "map.hpp"
#include "components.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "cursorcomponent.hpp"
#include "keyboardcontroller.hpp"
#include "vector2D.hpp"

Map* mapp;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
TTF_Font* Game::font = NULL;
        
Manager manager;
auto& player(manager.addEntity());
auto& cursor(manager.addEntity());
auto& textbox(manager.addEntity());

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    
    int flags = 0;
    
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    Game::font = TTF_OpenFont( "16_true_type_fonts/lazy.ttf", 28 );
    if( Game::font == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    // else
    // {
        // //Render text
        // SDL_Color textColor = { 0, 0, 0 };
        // if( !textTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor) )
        // {
            // printf("Failed to render text texture!\n");
        // }
    // }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        printf("SDL subsystems initialized.\n");
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            printf("Window created.\n");
        }
        
        if(TTF_Init() == -1) {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
    printf("Creating map \n");
    mapp =  new Map(32,32);
    mapp->loadMap("..//testmap.txt");
    player.addComponent<PositionComponent>(0, 0);
    cursor.addComponent<PositionComponent>(1, 1);
    cursor.addComponent<PositionComponent>(1, 1);
    textbox.addComponent<PositionComponent>(10, 10);

    cursor.addComponent<KeyboardController>();
    cursor.addComponent<KeyboardController>();
    
    player.addComponent<SpriteComponent>(mapp, "..//assets//horse.png");
    cursor.addComponent<CursorComponent>(mapp, "..//assets//cursor.png");
    // cursor.addComponent<CursorComponent>();
    // newPlayer.addComponent<PositionComponent>();
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

void Game::handleEvents() {

    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}
void Game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.
    // SDL_RenderCopy(renderer, playerTex, NULL, &destR);
    mapp->drawMap();
    manager.draw();
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
    return(isRunning);
}
