/*--------------------------------------------------------------------
* |           Average Bear Games                                     |
* |        _         _                    Available on....           |
* |       ( \_.---._/ )                                              |
* |        \  .   .  /       ,''',                                   |
* |         |  (@)  |       (*****)                                  |
* |         \   ᆺ  /        \   /                                   |
* |          '-._.-'         |   |                                   |
* |        Made by Gabriel Taillon                                   |
* --------------------------------------------------------------------*/
#include <chrono>
#include "text.hpp"
#include "game.hpp"

Game * firesaga = nullptr;
Settings settings;

int main(int argc, char * argv[]) {
    // freopen("log.txt", "w+", stdout);
    fclose(fopen(LOGFILE, "w"));
    SDL_LogSetOutputFunction(&FILESYSTEM::log, NULL);
    SDL_Log("Starting project codename FireSaga\n");

    SDL_Log("Initializing utilities\n");
    loadUtilities();

    SDL_Log("Initializing filesystem\n");
    char * buildDir = SDL_GetBasePath();
    FILESYSTEM::init(0, buildDir, buildDir);

    SDL_Log("Initializing TinyMT RNG\n");
    init_tinyMT();

    SDL_Log("Loading base weapons");
    baseWeapons();

    // SDL_Log("Loading base units.");
    // all_units = baseUnits();

    settings.FPS.show = true;
    settings.fontsize = 28;
    SDL_Log("Creating game\n");
    firesaga = new Game(settings);
    firesaga->makeUnits(0);
    firesaga->makeFPSEntity();
    firesaga->loadMap(0);

    std::vector<short unsigned int> unit_inds = {UNIT::NAME::SILOU};
    std::vector<std::vector<int>> positions_list = {{6, 6}};
    firesaga->loadMapUnits(unit_inds, positions_list);
    firesaga->loadMapArrivals();
    firesaga->startTurnSystem();
    firesaga->loadCursor();

    std::chrono::system_clock::time_point frame_start, frame_end, frame_middle;
    int frame_time;
    char buffer[15];
    SDL_Log("Starting main game loop\n");

    while (firesaga->running()) {
        frame_start = std::chrono::high_resolution_clock::now();
        firesaga->handleEvents();
        firesaga->update(0.001);
        frame_middle = std::chrono::high_resolution_clock::now();

        frame_time = (int)(std::chrono::duration_cast<std::chrono::nanoseconds>(frame_middle - frame_start).count() / 1E6);

        if (firesaga->getSettings()->FPS.frame_delay > frame_time) {
            SDL_Delay(firesaga->getSettings()->FPS.frame_delay - frame_time);
        }

        frame_end = std::chrono::high_resolution_clock::now();

        if (firesaga->getSettings()->FPS.show) {
            settings = *firesaga->getSettings();
            settings.FPS.current = (1E9 / std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start).count());
            settings.FPS.held++;
            firesaga->setSettings(settings);

            if (firesaga->getSettings()->FPS.held == 4) {
                sprintf(buffer, "%.1f", firesaga->getSettings()->FPS.current);
                firesaga->getSettings()->FPS.entity.component<Text>()->setText(buffer);
                firesaga->getSettings()->FPS.entity.component<Text>()->makeTextures();
                settings = *firesaga->getSettings();
                settings.FPS.held = 0;
                firesaga->setSettings(settings);
            }
        }
    }

    firesaga->clean();
    return (0);

}





































// 六月四日天安門广场大屠杀