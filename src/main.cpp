// Code créé par Gabriel Taillon
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include "shared.hpp"
#include "textcomponent.hpp"
#include "game.hpp"
#include "script.hpp"
#include "weapon.hpp"
#include "unit.hpp"

Game * firesaga = nullptr;
Settings temp_settings;

int main(int argc, char * argv[]) {
    printf("TESTING THIS BITCH\n");

    baseScript();
    printf("Testing script: %s\n", all_scripts["Chapter 1"].getScene("Intro").getLine("1").line.c_str());
    printf("Testing script: %s\n", all_scripts["Supports"].getScene("Main_Lovely_C").getLine("1").line.c_str());

    baseWeapons();
    printf("Testing weapon: %s\n", all_weapons["wooden_sword"].getName().c_str());

    baseTiles();
    printf("Testing tiles: %s\n", all_tiles[10].getName().c_str());

    baseUnits();
    printf("Testing units: %s \n", all_units["Silou"].getName().c_str());

    firesaga = new Game();
    printf("Made game.\n");

    printf("Initiating game.\n");
    temp_settings = firesaga->getSettings();
    temp_settings.FPS.show = true; 
    temp_settings.fontsize = 28; 
    firesaga->setSettings(temp_settings);
    firesaga->init("FireSaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, firesaga->getSettings().res.x, firesaga->getSettings().res.y, false);
    firesaga->makeFPSEntity();
    printf("Initiated game.\n");

    firesaga->loadMap("..//maps//test_tilemap.txt");
    printf("Map loaded\n");

    firesaga->loadCursor();
    printf("Cursor loaded\n");

    std::vector<std::string> names = {"Silou"};
    std::vector<std::string> asset_names = {"..//assets//horse.png"};
    std::vector<std::vector<int>> positions_list = {{6, 6}};
    firesaga->loadUnits(names, asset_names, positions_list);
    printf("Units loaded\n");

    std::chrono::system_clock::time_point frame_start, frame_end, frame_middle;
    int frame_time;
    char buffer[15];

    printf("Starting main game loop.\n");
    while (firesaga->running()) {
        frame_start = std::chrono::high_resolution_clock::now();
        firesaga->handleEvents();
        firesaga->update();
        firesaga->render();
        frame_middle = std::chrono::high_resolution_clock::now();
        
        frame_time = (int)(std::chrono::duration_cast<std::chrono::nanoseconds>(frame_middle - frame_start).count()/1E6);
        if (firesaga->getSettings().FPS.frame_delay > frame_time) {
            SDL_Delay(firesaga->getSettings().FPS.frame_delay - frame_time);
        }
        frame_end = std::chrono::high_resolution_clock::now();
        if (firesaga->getSettings().FPS.show) { 
            temp_settings = firesaga->getSettings();
            temp_settings.FPS.current = (1E9/std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start).count());
            temp_settings.FPS.held++; 
            firesaga->setSettings(temp_settings);
            if (firesaga->getSettings().FPS.held == 4) {
                sprintf(buffer, "%.1f", firesaga->getSettings().FPS.current);
                firesaga->manager.getEntities()[firesaga->getSettings().FPS.entity]->getComponent<TextComponent>().setText(buffer);
                temp_settings = firesaga->getSettings();
                temp_settings.FPS.held = 0; 
                firesaga->setSettings(temp_settings);
                // firesaga->getSettings().FPS.held = 0;   
            }
        }
    }

    firesaga->clean();
    return (0);

}
