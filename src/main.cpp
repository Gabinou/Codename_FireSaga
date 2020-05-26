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
#include "text.hpp"
#include "game.hpp"

Game * firesaga = nullptr;

int main(int argc, char * argv[]) {
    SDL_Log("Starting project codename FireSaga\n");

    SDL_Log("Setting log\n");
    fclose(fopen(LOGFILE, "w"));
    SDL_LogSetOutputFunction(&FILESYSTEM::log, NULL);

    SDL_Log("Initializing utilities\n");
    loadUtilities();

    SDL_Log("Initializing filesystem\n");
    char * buildDir = SDL_GetBasePath();
    char * assetsDir = strdup(buildDir);
    char * srcDir = strdup(buildDir); // srcDir, future home of release.exe
    path_removefolder(assetsDir);
    path_removefolder(srcDir);
    strncat(assetsDir, "assets\\", 6);
    FILESYSTEM::init(0, srcDir, assetsDir);

    SDL_Log("Initializing TinyMT RNG\n");
    init_tinyMT();

    SDL_Log("Creating game object\n");
    firesaga = new Game();
    firesaga->init();
    SDL_ShowCursor(SDL_DISABLE); // for default cursor.
    firesaga->initSystems();
    firesaga->makeFPS();
    firesaga->loadCursor();
    firesaga->loadMouse();

    SDL_Log("Loading in test Map\n");
    firesaga->loadMap(0);
    firesaga->updateSystems();
    firesaga->setCursorstate(MENU::MAP);

    SDL_Log("Loading in test party\n");
    std::vector<short int> unit_inds = {UNIT::NAME::SILOU};
    firesaga->loadUnits(unit_inds);
    std::vector<std::vector<int>> positions_list = {{6, 6}};
    firesaga->putPConMap(unit_inds, positions_list);
    SDL_Log("Loading in arrivals\n");
    firesaga->loadMapArrivals();

    SDL_Log("Starting turn system.\n");
    firesaga->startTurnSystem();

    SDL_Log("Starting main game loop\n");
    unsigned int currentTime = SDL_GetTicks();
    unsigned int elapsedTime = 0;
    short unsigned int cap = firesaga->getSettings()->FPS.cap;
    // SDL_Log("gotten cap: %d", cap);
    unsigned int delay = 0;

    while (firesaga->isRunning()) {
        elapsedTime = SDL_GetTicks() - currentTime;

        SDL_Log("elapsedTime: %d ", elapsedTime);
        SDL_Log("currentTime: %d ", currentTime);

        if ((elapsedTime * cap < 1000) || (elapsedTime == 0)) {
            delay = 1000 / cap - elapsedTime;
            SDL_Log("delay: %d ", delay);
            SDL_Delay(delay);
            elapsedTime = 1000 / cap;
        }

        currentTime = SDL_GetTicks();
        firesaga->update(elapsedTime);
        // getchar();
    }

    firesaga->clean();
    return (0);
}





































// 六月四日天安門广场大屠杀