// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle main.cpp
* \file main.cpp
* \brief Main game algorithm page.
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include "shared.hpp"
#include "textcomponent.hpp"
#include "game.hpp"
#include "script.hpp"
#include "weapon.hpp"

// Settings settings;

Game * firesaga = nullptr;

int main(int argc, char * argv[]) {
    printf("TESTING THIS BITCH\n");

    baseScript();
    printf("Testing script: %s\n", all_scripts["Chapter 1"].getScene("Intro").getLine("1").line.c_str());

    baseWeapons();
    printf("Testing weapon: %s\n", all_weapons["wooden_sword"].getName().c_str());

    baseTiles();
    printf("Testing tiles: %s\n");

    firesaga = new Game();
    printf("Made game.\n");
    firesaga->settings.FPS.show = true;
    firesaga->setFontsize(28);
    firesaga->init("FireSaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, firesaga->settings.res.x, firesaga->settings.res.y, false);
    printf("Initiated game.\n");

    std::chrono::system_clock::time_point frame_start, frame_end, frame_middle;
    int frame_time;
    char buffer[15];

    firesaga->settings.FPS.ind = firesaga->manager.getEntities().size();
    firesaga->manager.addEntity();

    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->addComponent<PositionComponent>();
    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->getComponent<PositionComponent>().setBounds(0, firesaga->settings.res.x, 0, firesaga->settings.res.y);
    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->getComponent<PositionComponent>().setPos(firesaga->settings.FPS.pos.x, firesaga->settings.FPS.pos.y);
    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->addComponent<TextComponent>(firesaga->settings.fontsize, "60", firesaga->settings.FPS.color);
    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->getComponent<TextComponent>().setSizefactor(firesaga->settings.FPS.sizefactor);
    firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->addGroup(firesaga->manager.groupUI);

    while (firesaga->running()) {
        frame_start = std::chrono::high_resolution_clock::now();
        firesaga->handleEvents();
        firesaga->update();
        firesaga->render();
        frame_middle = std::chrono::high_resolution_clock::now();
        
        frame_time = (int)(std::chrono::duration_cast<std::chrono::nanoseconds>(frame_middle - frame_start).count()/1E6);
        if (firesaga->settings.FPS.frame_delay > frame_time) {
            SDL_Delay(firesaga->settings.FPS.frame_delay - frame_time);
        }
        frame_end = std::chrono::high_resolution_clock::now();
        if (firesaga->settings.FPS.show) { 
            firesaga->settings.FPS.current = (1E9/std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start).count());
            firesaga->settings.FPS.held++;
            if (firesaga->settings.FPS.held == 4) {
                sprintf(buffer, "%.1f", firesaga->settings.FPS.current);
                firesaga->manager.getEntities()[firesaga->settings.FPS.ind]->getComponent<TextComponent>().setText(buffer);
                firesaga->settings.FPS.held = 0;   
            }
        }
    }

    firesaga->clean();
    return (0);

}
