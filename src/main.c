/*---------------------------------------
|           Average Bear Games          |
|        _         _                    |
|       ( \_.---._/ )                   |
|        \  .   .  /       ,''',        |
|         |  (@)  |       (*****)       |
|         \   ⋏   /        \   /        |
|          '-._.-'         |   |        |
|        Made by Gabriel Taillon        |
/--------------------------------------*/
/* Imperial Eagle Bearer? Standard? */
/* Turn-based strategy Role Playing Game (RPG)
*  Strongly inspired by Fire Emblem and Tear Ring Saga
*  Source code is SDL_free to own, modify and compile
*  Game is SDL_free to play only if you can compile it
*  Please do not distribute compiled executables
*/

#include "game/game.h"
#include "log.h"

int main(int argc, char *argv[]) {
    atexit(Game_Quit);

    /* -- Startup -- */
    Settings settings   = Settings_default;
    settings.args       = IES_Init(argc, argv);

    SDL_Log("IES start.\n");
    SDL_LogInfo(SOTA_LOG_SYSTEM,
                "Creating game object\n");
    struct Game *sota = Game_New(settings);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM,
                "Starting main game loop\n");
    while (sota->flags.isrunning) {
        Game_Step(sota);
    }

    /* -- Cleaning & Quitting -- */
    Game_Free(sota);
    Game_Post_Free();
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
