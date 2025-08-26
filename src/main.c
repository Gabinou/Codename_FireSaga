/*--------------------------------------/
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

#include "log.h"
#include "game/game.h"

int main(int argc, char *argv[]) {
    /* -- Startup -- */
    Settings settings   = Settings_default;
    settings.args       = IES_Init(argc, argv);

    SDL_Log("IES start");
    Game *IES = Game_New(settings);

    /* -- Sets up atexit to free IES on exit() -- */
    Game_atexit(IES);

    /* -- Master loop -- */
    while (IES->flags.isrunning) {
        Game_Step(IES);
    }

    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
