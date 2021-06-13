/*--------------------------------------------------------------------
* |           Average Bear Games                                     |
* |        _         _                    Available on....           |
* |       ( \_.---._/ )                                              |
* |        \  .   .  /       ,''',                                   |
* |         |  (@)  |       (*****)                                  |
* |         \   ᆺ  /        \    /                                  |
* |          '-._.-'         |   |                                   |
* |        Made by Gabriel Taillon                                   |
* --------------------------------------------------------------------*/
// #undef main

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "physfs.h"
#include "convoy.h"
#include "filesystem.h"
#include "SDL2/SDL.h"
#include "flecs.h"
#include "game.h"
#include "str.h"
#include "scene_includer.h"
#include "systemRender.h"
#include "systemControl.h"
#include "position.h"
#include "utilities.h"
#include "text.h"
#include "events.h"
#include "platform.h"
#include "macros.h"


int main(int32_t argc, char * argv[]) {
    fclose(fopen(LOGFILE, "w"));
    SDL_LogSetOutputFunction(filesystem_log, NULL);
    SDL_Log("Logfile set\n");
#ifdef SDL_ASSERT_LEVEL
    SDL_Log("SDL_ASSERT_LEVEL %d\n", SDL_ASSERT_LEVEL);
#endif
    SDL_Log("Starting project codename FireSaga\n");
    atexit(SDL_Quit);

    if (PLATFORM != platform_fromSDL()) {
        SDL_Log("C Platform not the same as SDL platform");
        exit(ERROR_PlatformMismatch);
    }

    SDL_Log("Running on "PLATFORM_NAME);

    SDL_Log("Checking input arguments\n");
    struct Input_Arguments args = utilities_parseInputs(argc, argv);

    SDL_Log("Initializing utilities\n");
    utilities_allNames_Load();

    SDL_Log("Initializing filesystem\n");
    char * buildDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    char * assetsDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    char * srcDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strcpy(buildDir, SDL_GetBasePath());
    strcpy(assetsDir, strdup(buildDir));
    strcpy(srcDir, strdup(buildDir)); // srcDir, future home of release.exe
    assetsDir = path_Top_Remove(assetsDir, DIR_SEPARATOR[0]);
    srcDir = path_Top_Remove(srcDir, DIR_SEPARATOR[0]);
    strcat(assetsDir, DIR_SEPARATOR);
    strcat(assetsDir, "assets");
    filesystem_init(0, srcDir, assetsDir);

    utilities_allNames_Print("");

    SDL_Log("Creating game object\n");
    struct Game * firesaga = (struct Game *) SDL_malloc(sizeof(struct Game));
    *firesaga = Game_default;
    Game_init(firesaga);

    SDL_Log("Initializing ECS\n");
    firesaga->world = ecs_init();
    ECS_IMPORT(firesaga->world, systemRenderModule);
    ECS_IMPORT(firesaga->world, systemControlModule);
    ECS_IMPORT(firesaga->world, TextModule);
    ECS_IMPORT(firesaga->world, TraitsModule);

    SDL_Log("Initializing TinyMT RNG\n");
    init_tinyMT(&firesaga->tinymt32);

    SDL_Log("Game startup, according to user inputs\n");
    Game_startup(firesaga, args);
    SDL_Log("Starting main game loop\n");
    uint16_t currentTime_ms = SDL_GetTicks(), elapsedTime_ms = 0, delay = 0 ;
    float updateTime_s = 1.0f / firesaga->settings.FPS.cap;
    while (firesaga->isrunning) {
        // SDL_Log("HERE\n");
        elapsedTime_ms = SDL_GetTicks() - currentTime_ms;
        if ((elapsedTime_ms * firesaga->settings.FPS.cap < 1000) || (elapsedTime_ms == 0)) {
            delay = ceil(1000.0f / firesaga->settings.FPS.cap) - elapsedTime_ms;
            SDL_Delay(delay);
            elapsedTime_ms += delay;
        }
        currentTime_ms = SDL_GetTicks();

        SDL_RenderClear(Game_renderer);
        Map_draw(firesaga->map_ptr); // Only one map -> no entity
        // SDL_Log("PROGRESS\n");
        ecs_progress(firesaga->world, updateTime_s);
        // SDL_Log("PROGRESSAFTER\n");
        SDL_RenderPresent(Game_renderer);
        Events_Manage(firesaga);
    }
    Game_clean(firesaga);
    ecs_fini(firesaga->world);
    SDL_free(firesaga);
    SDL_Delay(5000);
    return (NO_ERROR);
}





































// 六月四日天安門广场大屠杀
