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
    /* -- atexit -- */
    atexit(SDL_Quit);

    /* -- Startup -- */
    Game_Pre_Init(argc, argv);

    assert(false);
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    Game_Init(sota, argc, argv);

    Game_Weapons_Rewrite(sota);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    while (sota->isrunning)
        Game_Step(sota);

    /* -- Cleaning & Quitting -- */
    Game_Free(sota);
    Game_Post_Free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
