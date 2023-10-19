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
#include <math.h>
#include "physfs.h"
#include "convoy.h"
#include "filesystem.h"
#include "SDL2/SDL.h"
#ifdef SOTA_OPENGL
    #include <GL/glew.h>
    #include <SDL2/SDL_opengl.h>
    #include <GL/glu.h>
#endif
#include "globals.h"
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

int main(int argc, char *argv[]) {
    /* --- LOGGING --- */
    fclose(fopen(LOGFILE, "w"));

    #ifndef __SOTA_RELEASE__
    SDL_LogSetOutputFunction(Filesystem_Log2file, NULL);
    SOTA_Log_Debug("Logfile set\n");
    #else
    SDL_LogSetOutputFunction(Filesystem_Log, NULL);
    #endif /* __SOTA_RELEASE__ */

    // SOTA_Log_Debug("%d\n", SDL_ISPIXELFORMAT_INDEXED(SDL_PIXELFORMAT_RGBA32));
    // SOTA_Log_Debug("%d\n", SDL_BITSPERPIXEL(SDL_PIXELFORMAT_RGBA32));

    #ifdef SDL_ASSERT_LEVEL
    SOTA_Log_Debug("SDL_ASSERT_LEVEL %d\n", SDL_ASSERT_LEVEL);
    #endif /* SDL_ASSERT_LEVEL */

    /* --- STARTUP --- */
    SOTA_Log_Debug("Starting SOTA\n");
    /* -- Platform detection -- */
    atexit(SDL_Quit);
    if (PLATFORM != platform_fromSDL()) {
        SOTA_Log_Debug("C Platform not the same as SDL platform");
        exit(ERROR_PlatformMismatch);
    }
    SOTA_Log_Debug("Running on "PLATFORM_NAME);
    SOTA_Log_Debug("SDL version  %d %d %d ", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
    SOTA_Log_Debug("SotA version %d %d %d ", SOTA_VER_MAJOR,    SOTA_VER_MINOR,    SOTA_VER_PATCH);
    if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        SOTA_Log_Debug("SDL endianness is SDL_LIL_ENDIAN");
    else
        SOTA_Log_Debug("SDL endianness is SDL_BIG_ENDIAN");
    SOTA_Log_Debug("Checking input arguments\n");

    /* -- Input parsing -- */
    struct Input_Arguments args = Input_parseInputs(argc, argv);

    /* -- Actual startup -- */
    SOTA_Log_Debug("Initializing filesystem \n");
    Filesystem_Init(argv[0]);

    SOTA_Log_Debug("Initializing utilities\n");
    Utilities_Load();
    #ifndef __SOTA_RELEASE__
    SOTA_Log_Debug("Printing all names\n");
    Names_Print_All("");
    #endif /* __SOTA_RELEASE__ */

    SOTA_Log_Debug("Creating game object\n");
    struct Game *sota = Game_Init();
    Utilities_DrawColor_Reset(sota->renderer);

    SOTA_Log_Debug("Initializing TinyMT RNG\n");
    RNG_Init_tinymt(&sota->tinymt32);
    tinyMT_global = &sota->tinymt32;

    SOTA_Log_Debug("Game startup, according to user inputs\n");
    Game_Startup(sota, args);
    SDL_assert(sota->entity_mouse);

    /* -- Starting master loop -- */
    u64 currentTime_ns      = 0;
    u64 elapsedTime_ns      = 0;
    i64 delay_ms            = 0;
    tnecs_time_ns_t time_ns = 0;
    SOTA_Log_Debug("sota->settings.FPS.cap %d \n", sota->settings.FPS.cap);

    SOTA_Log_Debug("Starting main game loop\n");
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
        fsm_cFrame_s[sota->state](sota); /* CONTROl */
        /* -- Events -- */
        Events_Manage(sota); /* CONTROL */

        /* -- Render to screen -- */
        #ifndef RENDER2WINDOW
        SDL_SetRenderTarget(sota->renderer, NULL); /* RENDER */
        SDL_RenderCopy(sota->renderer,      sota->render_target, NULL, NULL);
        SDL_SetRenderTarget(sota->renderer, sota->render_target);
        #endif
        SDL_RenderPresent(sota->renderer);

        /* --- POST-FRAME --- */
        /* -- Synchronize timers -- */
        elapsedTime_ns = tnecs_get_ns() - currentTime_ns;
        delay_ms       = Game_FPS_Delay(sota, elapsedTime_ns);
        time_ns        = (elapsedTime_ns + delay_ms * SOTA_ns / SOTA_ms);

        Game_Cursor_movedTime_Compute(sota, time_ns);
        tnecs_custom_system_run(sota->world, timeSynchronize, sota->timer_typeflag, time_ns, NULL);

        /* -- Delay until next frame -- */
        Game_Delay(sota, delay_ms, currentTime_ns, elapsedTime_ns);
    }

    /* -- Cleaning & Quitting -- */
    Utilities_Free();
    Game_Free(sota);
    SOTA_Log_Debug("Freeing filesystem\n");
    Filesystem_Free();
    SOTA_Log_Debug("SotA quit.\n");
    return (NO_ERROR);
}

// 六月四日天安門广场大屠杀
