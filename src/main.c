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
*  Source code is SDL_free to own, modify and compile
*  Game is SDL_free to play only if you can compile it
*  Please do not distribute compiled executables
*/

#include "game/game.h"

int main(int argc, char *argv[]) {
    /* -- atexit -> Quit SDL -- */
    atexit(SDL_Quit);

    u64 before_ns = tnecs_get_ns();

    /* -- Startup -- */
    Settings settings   = Settings_default;
    settings.args       = IES_Init(argc, argv);

    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_us  = (after_ns - before_ns) / SOTA_ms;
    u64 elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("IES_Init %d us", elapsed_us);
    SDL_Log("IES_Init %d ms", elapsed_ms);

    SDL_Log("IES start.\n");
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = Game_New(settings);

    after_ns    = tnecs_get_ns();
    elapsed_us  = (after_ns - before_ns) / SOTA_ms;
    elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("IES_Init + Game_New %d us", elapsed_ms);
    SDL_Log("IES_Init + Game_New %d us", elapsed_us);
    // exit(0);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    while (sota->isrunning) {
        Game_Step(sota);
    }

    /* -- Cleaning & Quitting -- */
    Game_Free(sota);
    Game_Post_Free();
    SDL_Log("IES quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
