// Code créé par Gabriel Taillon
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <unistd.h>
#include <chrono>
#include "textcomponent.hpp"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "game.hpp"
#include "map.hpp"
#include "script.hpp"
#include "weapon.hpp"
#include "unit.hpp"
#include "convoy.hpp"

Game * firesaga = nullptr;
Settings temp_settings;

int main(int argc, char * argv[]) {
    fclose(fopen(LOGFILE, "w"));
    SDL_LogSetOutputFunction(&FILESYSTEM::log, NULL);
    SDL_Log("Starting project codename FireSaga\n");

    SDL_Log("Initializing utilities\n");
    loadUtilities();

    SDL_Log("Initializing Filesystem\n");
    char * buildDir = SDL_GetBasePath();
    FILESYSTEM::init(0, buildDir, buildDir);

    SDL_Log("Initializing TinyMT\n");
    init_tinyMT();

    SDL_Log("Loading base weapons.");
    baseWeapons();
    // SDL_Log("Testing weapon: %s\n", all_weapons[ITEM::NAME::WOODEN_SWORD].getName().c_str());
    // testXMLWeapons();
    // writeallXMLWeapons();

    SDL_Log("Loading base units.");
    all_units = baseUnits();
    SDL_Log("Testing units: %s \n", all_units[UNIT::NAME::SILOU].getName().c_str());
    testXMLUnits();

    // testXMLTiles();

    firesaga = new Game();
    // SDL_Log("Testing game load->save XML");
    // firesaga->loadXML(1);
    // firesaga->saveXML(2);
    // getchar();


    SDL_Log("Game object created\n");

    SDL_Log("Initiating game\n");
    temp_settings = firesaga->getSettings();
    temp_settings.FPS.show = true; 
    temp_settings.fontsize = 28; 
    firesaga->setSettings(temp_settings);
    firesaga->init("FireSaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, firesaga->getSettings().res.x, firesaga->getSettings().res.y, false);
    firesaga->makeFPSEntity();

    // SDL_Log("Testing convoy\n");
    // testConvoy();

    // getchar();

    // firesaga->loadMap("..//maps//test_tilemap.txt");
    firesaga->loadMap(0);

    SDL_Log("Loading Cursor\n");
    firesaga->loadCursor();

    std::vector<short unsigned int> unit_inds = {UNIT::NAME::SILOU};
    std::vector<std::string> asset_names = {"..//assets//horse.png"};
    std::vector<std::vector<int>> positions_list = {{6, 6}};
    firesaga->loadUnits(unit_inds, asset_names, positions_list);

    std::chrono::system_clock::time_point frame_start, frame_end, frame_middle;
    int frame_time;
    char buffer[15];
    
    SDL_Log("Testing game save XML");
    firesaga->saveXML(1);
    firesaga->copySaveXML(1, 2);
    firesaga->copySaveXML(2, 3);
    firesaga->deleteSaveXML(2);
    
    SDL_Log("Starting main game loop\n");
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
