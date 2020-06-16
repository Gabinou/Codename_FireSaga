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


    SDL_Log("Creating game object\n");
    firesaga = new Game();
    SDL_Log("Initializing TinyMT RNG\n");
    init_tinyMT(firesaga->getTinymt32());
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
    firesaga->setMousestate(MENU::MAP);

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
    unsigned int currentTime_ms = SDL_GetTicks();
    unsigned int elapsedTime_ms = 0;
    float updateTime_s = 0.0f;
    short unsigned int cap_s = firesaga->getSettings()->FPS.cap;
    unsigned int delay = 0;

    while (firesaga->isRunning()) {
        elapsedTime_ms = SDL_GetTicks() - currentTime_ms;

        if ((elapsedTime_ms * cap_s < 1000) || (elapsedTime_ms == 0)) {
            delay = 1000 / cap_s - elapsedTime_ms;
            SDL_Delay(delay);
        }

        updateTime_s = 1.0f / cap_s;

        currentTime_ms = SDL_GetTicks();
        firesaga->update(updateTime_s);
    }

    firesaga->clean();
    return (0);
}





































// 六月四日天安門广场大屠杀