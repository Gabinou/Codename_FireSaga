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

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "physfs.h"
#include "log.h"
#include "convoy.h"
#include "filesystem.h"
#include "SDL2/SDL.h"
#ifdef SOTA_OPENGL
    #include <GL/glew.h>
    #include <SDL_opengl.h>
    #include <GL/glu.h>
#endif
#include "game/game.h"
#include "game/cursor.h"
#include "nstr.h"
#include "fps_fsm.h"
#include "systems/render.h"
#include "systems/control.h"
#include "position.h"
#include "utilities.h"
#include "input.h"
#include "text.h"
#include "events.h"
#include "platform.h"
#include "macros.h"
#include "music.h"

int main(int argc, char *argv[]) {
    /* -- atexit -- */
    atexit(SDL_Quit);

    /* -- Startup -- */
    Pre_Game_Startup(argc, argv);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    Game_Init(sota, argc, argv);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    while (sota->isrunning)
        Game_Step(sota);

    /* -- Cleaning & Quitting -- */
    Pre_Game_Free();
    Game_Free(sota);
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
