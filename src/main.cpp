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

    // std::chrono::system_clock::time_point frame_start, frame_end;
    // int frame_time;
    // char buffer[15];
    SDL_Log("Starting main game loop\n");

    float lastTime = 0.;
    float currentTime = 0.;
    float elapsedSeconds;
    // unsigned int lastTime = 0, currentTime;

    while (firesaga->running()) {
        firesaga->handleEvents();
        // frame_start = std::chrono::high_resolution_clock::now();
        currentTime = (float)SDL_GetTicks();
        // elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(frame_start - frame_end).count();
        elapsedSeconds = (currentTime - lastTime) / 1000.;
        SDL_Log("elapsedSeconds: %.3f", elapsedSeconds);
        // SDL_Log("elapsedms: %d", );
        firesaga->update(elapsedSeconds);
        // frame_end = std::chrono::high_resolution_clock::now();
        lastTime = (float)SDL_GetTicks();
    }

    firesaga->clean();
    return (0);

}





































// 六月四日天安門广场大屠杀