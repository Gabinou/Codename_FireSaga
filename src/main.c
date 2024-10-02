/*--------------------------------------------------------------------
* |           Average Bear Games                                     |
* |        _         _                    Available on....           |
* |       ( \_.---._/ )                                              |
* |        \  .   .  /       ,''',                                   |
* |         |  (@)  |       (*****)                                  |
* |         \   ⋏   /        \   /                                   |
* |          '-._.-'         |   |                                   |
* |        Made by Gabriel Taillon                                   |
* --------------------------------------------------------------------*/
/* Imperial Eagle Bearer */
/* Turn-based strategy Role Playing Game (RPG)
*  Strongly inspired by Fire Emblem and Tear Ring Saga
*  Source code is free to own, modify and compile
*  Game is free to play only if you can compile it
*  Please do not distribute compiled executables
*/

#include "game/game.h"

int main(int argc, char *argv[]) {
    /* -- atexit -> Quit SDL -- */
    atexit(SDL_Quit);

    /* -- Startup -- */
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_SHOWN;
    settings.args       = IES_Init(argc, argv);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    Game_Init(sota, settings);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    u64 i = 1;
    while (sota->isrunning) {
        Game_Step(sota);
    }

    /* -- Cleaning & Quitting -- */
    Game_Free(sota);
    Game_Post_Free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
