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

struct Point{
    int x;
    int y;
};

struct Fps{
    Point pos = {700, 0};
    bool show = false;
    float sizefactor[2] = {0.5, 0.5};
    char cap = 60;
    char hold = 4;
    char frame_delay = 1000 / cap;
    SDL_Color color = {0, 0, 0};
};

struct Settings{
    Point res = {800, 600};
    char fontsize = 28;
    Fps FPS;
};

Settings settings;

Game * firesaga = nullptr;

int main(int argc, char * argv[]) {
    printf("TESTING THIS BITCH\n");
    // read_all_weapons("..\\weapons.txt");
    // read_all_units("..\\units_FE1.txt");
    // write_all_units("..\\units_test.txt"); // OKAY
    // int start[2] = {1, 2};
    // int end[2] = {8, 8};

    // std::vector<std::vector<int>> map{
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    // };
    // std::vector<std::vector<int>> path;
    // std::string at = "matrix";
    // path = A_star(map, start, end, at);
    // printf("Map\n");
    // plot2Dvector(map);
    // printf("Path\n");
    // plot2Dvector(path);
    settings.FPS.show = true;

    firesaga = new Game();
    printf("Made game.\n");
    firesaga->setFontsize(28);
    firesaga->init("FireSaga", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.res.x, settings.res.y, false);
    printf("Initiated game.\n");
  
    float FPS_avg;
    std::chrono::system_clock::time_point frame_start, frame_end, frame_middle, fps_shown;
    int frame_time, FPS_hold = 0;
    char buffer[15];

    int FPS_entity_ind = firesaga->manager.getEntities().size();
    firesaga->manager.addEntity();

    firesaga->manager.getEntities()[FPS_entity_ind]->addComponent<PositionComponent>();
    firesaga->manager.getEntities()[FPS_entity_ind]->getComponent<PositionComponent>().setBounds(0, settings.res.x, 0, settings.res.y);
    firesaga->manager.getEntities()[FPS_entity_ind]->getComponent<PositionComponent>().setPos(settings.FPS.pos.x, settings.FPS.pos.y);
    firesaga->manager.getEntities()[FPS_entity_ind]->addComponent<TextComponent>(settings.fontsize, "60", settings.FPS.color);
    firesaga->manager.getEntities()[FPS_entity_ind]->getComponent<TextComponent>().setSizefactor(settings.FPS.sizefactor);
    firesaga->manager.getEntities()[FPS_entity_ind]->addGroup(firesaga->manager.groupUI);

    while (firesaga->running()) {
        frame_start = std::chrono::high_resolution_clock::now();
        firesaga->handleEvents();
        firesaga->update();
        firesaga->render();
        frame_middle = std::chrono::high_resolution_clock::now();
        
        frame_time = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(frame_middle - frame_start).count()/1E6;
        if (settings.FPS.frame_delay > frame_time) {
            SDL_Delay(settings.FPS.frame_delay - frame_time);
        }
        frame_end = std::chrono::high_resolution_clock::now();
        if (settings.FPS.show) { 
            FPS_avg = 1E9/(int)std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start).count();
            FPS_hold++;
            if (FPS_hold == 4) {
                sprintf(buffer, "%.1f", FPS_avg);
                firesaga->manager.getEntities()[FPS_entity_ind]->getComponent<TextComponent>().setText(buffer);
                FPS_hold = 0;   
            }
        }
    }

    firesaga->clean();
    return (0);
}
