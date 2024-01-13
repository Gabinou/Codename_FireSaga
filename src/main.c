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
/* Sword of the Anointed */
// Turn-based strategy Role Playing Game (RPG)
// Strongly inspired by Fire Emblem and Tear Ring Saga
// Source code is free to own, modify and compile
// Game is free to play only if you can compile it
// Please do not distribute compiled executables

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "physfs.h"
#include "log.h"
#include "convoy.h"
#include "filesystem.h"
#include "SDL.h"
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
#include "systems/time_system.h"
#include "position.h"
#include "utilities.h"
#include "input.h"
#include "text.h"
#include "events.h"
#include "platform.h"
#include "macros.h"
#include "music.h"

int main(int argc, char *argv[]) {
    /* --- LOGGING --- */
    Log_Init();

    #ifdef SDL_ASSERT_LEVEL
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL_ASSERT_LEVEL %d\n", SDL_ASSERT_LEVEL);
    #endif /* SDL_ASSERT_LEVEL */

    /* --- STARTUP --- */
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Starting SOTA\n");
    /* -- Platform detection -- */
    atexit(SDL_Quit);
    if (PLATFORM != platform_fromSDL()) {
        SDL_LogCritical(0, "C Platform not the same as SDL platform");
        exit(ERROR_PlatformMismatch);
    }
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Running on "PLATFORM_NAME);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL version  %d %d %d ", SDL_MAJOR_VERSION,  SDL_MINOR_VERSION,
                 SDL_PATCHLEVEL);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SotA version %d %d %d ", SOTA_VER_MAJOR,    SOTA_VER_MINOR,
                 SOTA_VER_PATCH);
    if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL endianness is SDL_LIL_ENDIAN");
    else
        SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL endianness is SDL_BIG_ENDIAN");

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Checking input arguments\n");
    /* -- Input parsing -- */
    struct Input_Arguments args = Input_parseInputs(argc, argv);

    /* -- Actual startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing filesystem \n");
    Filesystem_Init(argv[0]);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing utilities\n");
    Utilities_Load();
    #ifndef __SOTA_RELEASE__
    #if HAVE_STDIO_H
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Printing all names\n");
    Names_Print_All("");
    #else
    SDL_LogError(SOTA_LOG_SYSTEM, "Could not print all names, no stdio\n");
    #endif /* HAVE_STDIO_H */
    #endif /* __SOTA_RELEASE__ */

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    Game_Init(sota);
    Utilities_DrawColor_Reset(sota->renderer);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing RNG\n");
    RNG_Init_xoroshiro256ss();

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Game startup, according to user inputs\n");
    Game_Startup(sota, args);
    SDL_assert(sota->entity_mouse);

    /* -- Starting master loop -- */
    u64 currentTime_ns      = 0;
    u64 elapsedTime_ns      = 0;
    i64 delay_ms            = 0;
    tnecs_ns time_ns = 0;
    SDL_LogDebug(SOTA_LOG_SYSTEM, "FPS cap %d \n", sota->settings.FPS.cap);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    u64 updateTime_ns = SOTA_ns / sota->settings.FPS.cap;
    while (sota->isrunning) {
        /* --- PRE-FRAME --- */
        sota->cursor_frame_moved = false;
        currentTime_ns = tnecs_get_ns();
        SDL_RenderClear(sota->renderer); /* RENDER clears the backbuffer */

        /* --- FRAME --- */
        /* -- fps_fsm -- */
        SDL_assert(fsm_rFrame_s[sota->state] != NULL);
        SDL_assert(fsm_cFrame_s[sota->state] != NULL);

        fsm_rFrame_s[sota->state](sota); /* RENDER */
        tnecs_world_step_wdata(sota->world, updateTime_ns, sota); /* CONTROL+RENDER */
        fsm_cFrame_s[sota->state](sota); /* CONTROL */
        /* -- Events -- */
        Events_Manage(sota); /* CONTROL */

        /* -- Render to screen -- */
        #ifndef RENDER2WINDOW
        SDL_SetRenderTarget(sota->renderer, NULL); /* RENDER */
        SDL_RenderCopy(     sota->renderer, sota->render_target, NULL, NULL);
        SDL_SetRenderTarget(sota->renderer, sota->render_target);
        #endif
        SDL_RenderPresent(sota->renderer);

        /* --- POST-FRAME --- */
        /* -- Synchronize timers -- */
        elapsedTime_ns = tnecs_get_ns() - currentTime_ns;
        delay_ms       = Game_FPS_Delay(sota, elapsedTime_ns);
        time_ns        = (elapsedTime_ns + delay_ms * SOTA_ns / SOTA_ms);

        Game_Cursor_movedTime_Compute(sota, time_ns);
        // printf("sota->cursor_moved_time_ms %d\n", sota->cursor_moved_time_ms);
        tnecs_custom_system_run(sota->world, Time_Synchronize,
                                sota->timer_typeflag, time_ns, NULL);

        /* -- Delay until next frame -- */
        Game_Delay(sota, delay_ms, currentTime_ns, elapsedTime_ns);
    }

    /* -- Cleaning & Quitting -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Utilities\n");
    Utilities_Free();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Game\n");
    Game_Free(sota);
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Filesystem\n");
    Filesystem_Free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

// 六月四日天安門广场大屠杀
