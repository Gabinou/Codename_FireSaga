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

    settings.FPS.show = false;
    settings.fontsize = 28;
    settings.FPS.cap = 120;
    SDL_Log("Creating game\n");
    firesaga = new Game(settings);
    firesaga->makeUnits(0);
    firesaga->makeFPS();
    firesaga->loadMap(0);

    std::vector<short unsigned int> unit_inds = {UNIT::NAME::SILOU};
    std::vector<std::vector<int>> positions_list = {{6, 6}};
    firesaga->loadMapUnits(unit_inds, positions_list);
    firesaga->loadMapArrivals();
    firesaga->startTurnSystem();
    firesaga->loadCursor();

    SDL_Log("Starting main game loop\n");

    float lastTime = (float)SDL_GetTicks();
    float currentTime = (float)SDL_GetTicks();
    float elapsedSeconds;
    short unsigned int cap = firesaga->getSettings()->FPS.cap;
    int delay;

    while (firesaga->running()) {
        // currentTime = ;
        elapsedSeconds = ((float)SDL_GetTicks() - currentTime) / 1000.;

        // SDL_Log("elapsedSeconds: %.3f", elapsedSeconds);

        if ((elapsedSeconds * cap < 1.) || (elapsedSeconds == 0)) {
            delay = int ((1. / cap - elapsedSeconds) * 1000);
            // SDL_Log("Delay: %d ms", delay);
            SDL_Delay(delay);
            elapsedSeconds = 1. / (float)cap;
        }

        currentTime = (float)SDL_GetTicks();
        firesaga->handleEvents();
        firesaga->update(elapsedSeconds);
    }

    firesaga->clean();
    return (0);

}





































// 六月四日天安門广场大屠杀