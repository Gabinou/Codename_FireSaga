/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Game object: high level API to everything
**
*/

#include "AI.h"
#include "log.h"
#include "fsm.h"
#include "RNG.h"
#include "tile.h"
#include "text.h"
#include "item.h"
#include "names.h"
#include "actor.h"
#include "input.h"
#include "hover.h"
#include "scene.h"
#include "music.h"
#include "sprite.h"
#include "events.h"
#include "weapon.h"
#include "combat.h"
#include "macros.h"
#include "slider.h"
#include "convoy.h"
#include "structs.h"
#include "globals.h"
#include "platform.h"
#include "position.h"
#include "cutscene.h"
#include "utilities.h"
#include "pixelfonts.h"
#include "noursclock.h"
#include "filesystem.h"

#include "bars/map_hp.h"

#include "controller/fsm.h"
#include "controller/mouse.h"
#include "controller/gamepad.h"
#include "controller/touchpad.h"
#include "controller/keyboard.h"

#include "game/map.h"
#include "game/game.h"
#include "game/menu.h"
#include "game/unit.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "map/map.h"
#include "map/render.h"
#include "map/animation.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/deployment.h"
#include "menu/loadout_select.h"

#include "popup/popup.h"

#include "systems/slide.h"
#include "systems/render.h"
#include "systems/map_end.h"
#include "systems/control.h"
#include "systems/turn_end.h"
#include "systems/time_system.h"

#include "unit/anim.h"
#include "unit/boss.h"
#include "unit/party.h"

#include "stb_sprintf.h"

struct dtab *gl_items_dtab      = NULL;
struct dtab *gl_weapons_dtab    = NULL;

tnecs_world *gl_world           = NULL;

const struct Game Game_default = {
    .cursor.lastpos         = {1, 1},
    .cursor.moved_direction = SOTA_DIRECTION_NULL,
    .flags.iscursor         = true,

    .party = {
        .jsonio_header.json_element   = JSON_PARTY,
        .jsonio_header.json_filename  = {0},
    },

    .selected.unit_moved_position   = {-1, -1},
    .selected.unit_initial_position = {-1, -1},
    // .convoy = Convoy_default,
    // .camp = Camp_default,

    .state.chapter          = -1,
    .state.top.current      = GAME_STATE_Title_Screen,
    .state.sub.current      = GAME_SUBSTATE_MENU,
    .state.top.previous     = GAME_STATE_START,
    .state.sub.previous     = GAME_SUBSTATE_START,
};

/* --- Constructors/Destructors --- */

void Game_Free(struct Game *IES) {
    if (NULL == IES) {
        return;
    }
    Game_AI_Free(&IES->ai);
    Game_Party_Free(IES);
    Party_Free(&IES->party);
    Game_Title_Destroy(IES);
    GameMap_Reinforcements_Free(IES);
    Game_DeploymentMenu_Free(IES);
    Game_Cursor_Free(IES);
    Game_PopUp_Tile_Free(IES);
    Game_Mouse_Free(IES);
    Game_menuStack_Free(IES);
    Game_PlayerSelectMenus_Free(IES);
    Game_PopUp_Unit_Free(IES);
    if (IES->combat.outcome.attacks != NULL) {
        DARR_FREE(IES->combat.outcome.attacks);
        IES->combat.outcome.attacks = NULL;
    }

    PixelFont_Free(IES->fonts.pixelnours,       true);
    PixelFont_Free(IES->fonts.pixelnours_big,   true);
    PixelFont_Free(IES->fonts.pixelnours_tight, true);

    if (IES->menus.stats > TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_C(gl_world, IES->menus.stats, Menu);
        if (mc->data != NULL) {
            struct StatsMenu *stats_menu = mc->data;
            if (mc->n9patch.texture != NULL)
                SDL_DestroyTexture(mc->n9patch.texture);
            StatsMenu_Free(stats_menu);
            mc->data = NULL;
        }
    }

    if (IES->menus.item_select > TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_C(gl_world, IES->menus.item_select, Menu);
        if (mc->data != NULL) {
            struct LoadoutSelectMenu *ism = mc->data;
            LoadoutSelectMenu_Free(ism);
            SDL_free(ism);
            mc->data = NULL;
        }
    }

    if (IES->render.er) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Control");
        // controlSDL_free();
    }
    if (IES->audio.soundfx_cursor != NULL) {
        Mix_FreeChunk(IES->audio.soundfx_cursor);
        IES->audio.soundfx_cursor = NULL;
    }
    if (IES->audio.soundfx_next_turn != NULL) {
        Mix_FreeChunk(IES->audio.soundfx_next_turn);
        IES->audio.soundfx_next_turn = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Map");
    Map *map = Game_Map(IES);
    if ((map != NULL) && (map->darrs.unitmap != NULL))
        Map_Unitmap_Free(map);
    Game_Map_Free(IES);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Camera");
    // if (IES->camera != NULL)
    // SDL_free(IES->camera);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Events");
    Events_Data_Free();
    Events_Names_Free();
    Events_Receivers_Free();
#ifndef SOTA_OPENGL
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Renderer");
    if (IES->render.target != NULL)
        SDL_DestroyTexture(IES->render.target);
    if (IES->render.er != NULL) {
        SDL_DestroyRenderer(IES->render.er); /* SDL_free renderer before window */
    }
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Window");
    if (IES->render.window != NULL)
        SDL_DestroyWindow(IES->render.window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    Mix_Quit();
    SDL_Quit();
#else /* SOTA_OPENGL */
#endif /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free tnecs world");

    if (gl_world != NULL) {
        tnecs_world_destroy(&gl_world);
        gl_world = NULL;
    }

    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    if (IES->fonts.menu != NULL) {
        PixelFont_Free(IES->fonts.menu, false);
    }
    if (IES->targets.defendants != NULL) {
        DARR_FREE(IES->targets.defendants);
        IES->targets.defendants = NULL;
    }
    if (IES->targets.patients != NULL) {
        DARR_FREE(IES->targets.patients);
        IES->targets.patients = NULL;
    }
    if (IES->targets.victims != NULL) {
        DARR_FREE(IES->targets.victims);
        IES->targets.victims = NULL;
    }
    if (IES->targets.deployed != NULL) {
        DARR_FREE(IES->targets.deployed);
        IES->targets.deployed = NULL;
    }
    if (IES->targets.spectators != NULL) {
        DARR_FREE(IES->targets.spectators);
        IES->targets.spectators = NULL;
    }
    if (IES->targets.auditors != NULL) {
        DARR_FREE(IES->targets.auditors);
        IES->targets.auditors = NULL;
    }
    if (IES->targets.passives != NULL) {
        DARR_FREE(IES->targets.passives);
        IES->targets.passives = NULL;
    }
    if (IES->targets.openables != NULL) {
        DARR_FREE(IES->targets.openables);
        IES->targets.openables = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Game cleaned.");
    SDL_free(IES);
    gl_world = NULL;
}

void Game_Post_Free(void) {
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Utilities\n");
    Utilities_Free();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Filesystem\n");
    Filesystem_Free();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Names\n");
    Names_Free();
}

void Game_Quit(void) {
    SDL_Log("IES quit.\n");
    SDL_Quit();
}

Input_Arguments IES_Init(int argc, char *argv[]) {
    /* --- LOGGING --- */
    Log_Init();
#ifdef SDL_ASSERT_LEVEL
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL_ASSERT_LEVEL %d\n", SDL_ASSERT_LEVEL);
#endif /* SDL_ASSERT_LEVEL */

    SDL_LogDebug(SOTA_LOG_SYSTEM, "Starting IES\n");

    /* --- ASSERTS --- */
#ifdef SOTA_ASSERT_ALWAYS_BREAK
    SDL_SetAssertionHandler(sota_assert_handler, NULL);
#endif /* SOTA_ASSERT_ALWAYS_BREAK */

    /* -- Platform detection -- */
    if (PLATFORM != platform_fromSDL()) {
        SDL_LogCritical(0, "C Platform not the same as SDL platform");
        exit(ERROR_PlatformMismatch);
    }

    /* -- IES startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing filesystem \n");
    Filesystem_Init(argv[0]);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing utilities\n");
    Utilities_Load();

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Initializing RNG\n");
    RNG_Init_xoroshiro256ss();

    /* -- Input parsing -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Checking input arguments\n");
    return (Input_parseInputs(argc, argv));
}

u64 _Game_Step_PreFrame(struct Game *IES) {
    u64 currentTime_ns = nours_get_ns();
    IES->cursor.frame_moved = false;
    SDL_RenderClear(IES->render.er); /* RENDER clears the backbuffer */

    return (currentTime_ns);
}

void _Game_Step_Control(struct Game *IES) {
    /* --- Control: Get player inputs --- */

    /* -- player inputs, movement -- */
    u64 updateTime_ns = SOTA_ns / IES->settings.FPS.cap;
    b32 success = tnecs_pipeline_step(gl_world, updateTime_ns, IES, TNECS_PIPELINE_CONTROL);
    if (!success) {
        SDL_Log("Pipeline %d failed", TNECS_PIPELINE_CONTROL);
        SDL_assert(false);
        exit(ERROR_Generic);
    }
}

void _Game_Step_Render(struct Game *IES) {
    /* Render FSM */
    // TODO: convert to systems in render pipeline
    // SDL_assert(fsm_rFrame_s[Game_State_Current(IES)] != NULL);
    // fsm_rFrame_s[Game_State_Current(IES)](IES); /* RENDER */
    u64 updateTime_ns = SOTA_ns / IES->settings.FPS.cap;
    b32 success = tnecs_pipeline_step(gl_world, updateTime_ns, IES, TNECS_PIPELINE_RENDER);
    if (!success) {
        SDL_Log("Pipeline %d failed", TNECS_PIPELINE_RENDER);
        SDL_assert(false);
        exit(ERROR_Generic);
    }

    /* -- Render to screen -- */
#ifndef RENDER2WINDOW
    SDL_SetRenderTarget(IES->render.er, NULL); /* RENDER */
    SDL_RenderCopy(     IES->render.er, IES->render.target, NULL, NULL);
    SDL_SetRenderTarget(IES->render.er, IES->render.target);
#endif
    SDL_RenderPresent(IES->render.er);
}

void Game_FPS_Instant(Game *IES,
                      u32   frame_count,
                      i64   last_update_ns) {
    /* Don't change fps_instant if timer
    ** was just reset */
    if ((last_update_ns == 0) || (frame_count == 0)) {
        return;
    }
    float ratio         = (float)SOTA_ns / (float)last_update_ns;
    float fps           = (frame_count * ratio);
    IES->fps.instant    = fps;
}

void _Game_Step_PostFrame(struct Game *IES, u64 currentTime_ns) {
    /* -- Synchronize timers -- */
    u64 elapsedTime_ns = nours_get_ns() - currentTime_ns;
    i64 delay_ms       = Game_FPS_Delay(IES, elapsedTime_ns);
    tnecs_ns time_ns   = (elapsedTime_ns + delay_ms * SOTA_ns / SOTA_ms);

    Game_Cursor_movedTime_Compute(IES, time_ns);
    // SDL_Log("IES->cursor.moved_time_ms %d\n", IES->cursor.moved_time_ms);
    tnecs_custom_system_run(gl_world, Time_Synchronize,
                            IES->ecs.timer_typeflag, time_ns, NULL);

    /* -- Compute FPS -- */
    // Note: Timers are updated in Time_Synchronize.
    if (IES->fps.entity != TNECS_NULL) {
        Timer *ut = IES_GET_C(gl_world, IES->fps.entity, Timer);
        SDL_assert(ut != NULL);
        // Note: FPS timer is reset by Draw_Text_Timer;
        Game_FPS_Instant(IES, ut->frame_count, ut->time_ns);
    }

    /* -- Delay until next frame -- */
    Game_Delay(IES, delay_ms, currentTime_ns, elapsedTime_ns);
}

void Game_Step(struct Game *IES) {
    /* -- preframe -- */
    u64 currentTime_ns = _Game_Step_PreFrame(IES);

    /* -- frame -- */
    _Game_Step_Control(IES);
    Events_Manage(IES); /* CONTROL */
    _Game_Step_Render(IES);

    /* -- simulated lag -- */
    /* Note cursor still moves slow with delay:
    **  - Cause: Slower Polling? anyway, not Slider */
    // u32 delay_ms =  30; /* 30fps */
    // u32 delay_ms =  40; /* 20fps */
    // u32 delay_ms =  90; /* 10fps */
    // u32 delay_ms = 180; /* 5fps */
    /* SDL_Delay(delay_ms); */

    /* -- postframe -- */
    _Game_Step_PostFrame(IES, currentTime_ns);
}

struct Game *Game_New(Settings settings) {
    /* -- Setting defaults -- */
    struct Game *IES = SDL_malloc(sizeof(struct Game));
    *IES = Game_default;
    IES->settings = settings;

    SDL_assert(IES->settings.FPS.cap > 0);
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Init game");
    IES->menus.filename = s8_literal(PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png"));

    _Game_New_Alloc(IES);

    /* Window flags */
    u32 flags = IES->settings.window;
    if (IES->settings.fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;

#ifndef SOTA_OPENGL
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Firesaga: Window Initialization");

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        /*  NOTE: --- SDL_INIT LEAKS A LOT OF MEMORY --- */
        //  Ex:     reachable 224,425 bytes in 1,445 blocks
        //          suppressed 16 bytes in 1 blocks
        //          It may be cause of X11.
        //      Someone on Stack overflow said his NVIDIA drivers leak 10Mb.
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    /* Initialize SDL_mixer */
    struct Music_settings music = IES->settings.music_settings;
    if (Mix_OpenAudio(music.frequency, music.format, music.channels, music.sample_size) < 0) {
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                        Mix_GetError());
        exit(1);
    }

    Game_Display_Bounds(IES);

    IES->render.window = SDL_CreateWindow(IES->settings.title, IES->settings.pos.x,
                                          IES->settings.pos.y, IES->settings.res.x, IES->settings.res.y, flags);
    SDL_assert(IES->render.window);
    // SDL_assert(SDL_ISPIXELFORMAT_INDEXED(SDL_GetWindowPixelFormat(IES->render.window)));
    int window_w;
    int window_h;
    SDL_GetWindowSize(IES->render.window, &window_w, &window_h);
    SDL_assert(window_w == IES->settings.res.x);
    SDL_assert(window_h == IES->settings.res.y);
    if (IES->render.window) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "Window created\n");
        IES->render.er = SDL_CreateRenderer(IES->render.window, -1, SDL_RENDERER_TARGETTEXTURE);
        SDL_assert(IES->render.er);
        if (IES->render.er) {
            Utilities_DrawColor_Reset(IES->render.er);
            SDL_LogVerbose(SOTA_LOG_SYSTEM, "Renderer created\n");
        }
    }
#ifndef RENDER2WINDOW
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Rendering to IES->render.target\n");
    SDL_assert(IES->settings.res.x > 0);
    SDL_assert(IES->settings.res.y > 0);
    SDL_assert(IES->render.er != NULL);
    IES->render.target = SDL_CreateTexture(IES->render.er, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_TARGET, IES->settings.res.x, IES->settings.res.y);
    SDL_assert(IES->render.target != NULL);
    SDL_SetTextureBlendMode(IES->render.target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(IES->render.er, IES->render.target);
    SDL_assert(IES->render.target != NULL);
#endif /* RENDER2WINDOW */
#else /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Firesaga: OpenGL Window Initialization");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // TODO: set better window flags
    IES->gl_window = SDL_CreateWindow(IES->settings.title, IES->settings.pos.x,
                                      IES->settings.pos.y, IES->settings.res.x, IES->settings.res.y,
                                      flags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_assert(IES->gl_window);
    IES->gl_context = SDL_GL_CreateContext(IES->gl_window);
    // glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        SDL_LogCritical(0, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    // Initialize OpenGL
    if (!initGL()) {
        SDL_LogCritical(0, "Unable to initialize OpenGL!\n");
        exit(1);
    }
#endif /* SOTA_OPENGL */

    char *temp_base = SDL_GetBasePath();
    SDL_assert(DEFAULT_BUFFER_SIZE >= strlen(temp_base));
    s8 path_mapping =  s8_mut(temp_base);
    SDL_free(temp_base);

    path_mapping = s8_Path_Remove_Top(path_mapping, DIR_SEPARATOR[0]);
    const char *separator = PHYSFS_getDirSeparator();
    path_mapping = s8cat(path_mapping, s8_var(separator));
    path_mapping = s8cat(path_mapping, s8_literal(DIR_SEPARATOR"gamecontrollerdb.txt"));
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Path to gamecontrollerdb: %s\n", path_mapping.data);
    if (SDL_GameControllerAddMappingsFromFile(path_mapping.data) < 0)
        SDL_LogWarn(SOTA_LOG_SYSTEM, "gamecontrollerdb.txt not found!\n");
    s8_free(&path_mapping);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Allocating space for globals\n");

    _Game_New_Events(IES);
    _Game_New_Tnecs(IES);

    IES->inputs.keyboardInputMap  = KeyboardInputMap_default;
    IES->inputs.gamepadInputMap   = GamepadInputMap_switch_pro;

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Loading pixelfonts\n");
    IES->fonts.pixelnours = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");
    PixelFont_Load(IES->fonts.pixelnours, IES->render.er, path);
    IES->fonts.pixelnours->y_offset = pixelfont_y_offset;

    IES->fonts.pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(IES->fonts.pixelnours_big, IES->render.er, path);
    IES->fonts.pixelnours_big->y_offset = pixelfont_big_y_offset;

    IES->fonts.pixelnours_tight = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_tight.png");
    PixelFont_Load(IES->fonts.pixelnours_tight, IES->render.er, path);

    /* Sprite init */
    b32 absolute = false;
    b32 isCursor = false;
    dstrect_funcs[absolute][isCursor = true]        = &Cursor_Dstrect_Relative;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Relative;
    dstrect_funcs[absolute = true][isCursor = true] = &Cursor_Dstrect_Absolute;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Absolute;

    /* --- Soundfx --- */
    IES->audio.soundfx_cursor    = Soundfx_Load_Cursor();
    IES->audio.soundfx_next_turn = Soundfx_Load_Next_Turn();

    /* -- Load Cursor and mouse -- */
    SDL_ShowCursor(SDL_DISABLE); /* for default cursor */
    Game_FPS_Create(IES, SOTA_FPS_UPDATE_ns);
    Game_Cursor_Create(IES);
    Game_Mouse_Create(IES);

    if ((IES->settings.args.map_index == 0) && (IES->settings.args.scene == 0)) {
        Game_Startup_TitleScreen(IES);
    } else if (IES->settings.args.scene != 0) {
        Game_Startup_Scene(IES);
    } else if (IES->settings.args.map_index != 0) {
        Game_Startup_Map(IES);
    }

    /* --- Set default contextual inputs --- */
    fsm_Input_s[Game_State_Current(IES)](IES);

    /* -- Set color -- */
    Utilities_DrawColor_Reset(IES->render.er);

    /* -- Checks -- */
    SDL_assert(IES->mouse.entity);

    IES->flags.isrunning = true;
    return (IES);
}

int _Game_New_Events(void *data) {
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Allocating space for events\n");
    Events_Data_Malloc();
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Initializing user events\n");
    Events_Names_Declare();
    Events_Names_Alloc();
    Events_Receivers_Declare();

    return (0);
}

int _Game_New_Alloc(void *data) {
    Game *IES = data;

    /* --- Allocations --- */
    /* -- Alloc weapons, items DTAB -- */
    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    DTAB_INIT(gl_weapons_dtab,    struct Weapon);
    DTAB_INIT(gl_items_dtab,      struct Item);
    SDL_assert(gl_weapons_dtab    != NULL);
    SDL_assert(gl_items_dtab      != NULL);

    /* -- Alloc arrays of entities -- */
    IES->targets.defendants    = DARR_INIT(IES->targets.defendants,   tnecs_entity,  4);
    IES->targets.patients      = DARR_INIT(IES->targets.patients,     tnecs_entity,  4);
    IES->targets.victims       = DARR_INIT(IES->targets.victims,      tnecs_entity,  4);
    IES->targets.deployed      = DARR_INIT(IES->targets.deployed,     tnecs_entity,  4);
    IES->targets.spectators    = DARR_INIT(IES->targets.spectators,   tnecs_entity,  4);
    IES->targets.auditors      = DARR_INIT(IES->targets.auditors,     tnecs_entity,  4);
    IES->targets.passives      = DARR_INIT(IES->targets.passives,     tnecs_entity,  4);
    IES->targets.openables     = DARR_INIT(IES->targets.openables,    tnecs_entity,  4);

    /* -- Alloc combat arrays -- */
    IES->combat.outcome.attacks = DARR_INIT(IES->combat.outcome.attacks, struct Combat_Attack,
                                            SOTA_COMBAT_MAX_ATTACKS);

    /* -- Alloc menu array -- */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Allocating Menu Array\n");
    Game_Menus_Init(IES);

    return (0);
}

int _Game_New_Tnecs(void *data) {
    Game *IES = data;

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Tnecs: Genesis\n");
    if (!tnecs_world_genesis(&gl_world)) {
        SDL_Log("Could not init tnecs_world");
        SDL_assert(false);
        exit(ERROR_Generic);
    }
    SDL_assert(gl_world != NULL);

    // Don't reuse entities.
    // If I forget to update an entity somewhere, it'll be invalid for sure.
    SDL_assert(gl_world->reuse_entities == false);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Components Registration\n");
    tnecs_world *world = gl_world;
#include "register/components.h"
    IES->ecs.timer_typeflag = TNECS_COMPONENT_ID2TYPE(Timer_ID);
    SDL_assert(TNECS_PIPELINE_RENDER == 1);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Pipeline Registration\n");
#include "register/pipelines.h"
    SDL_assert(TNECS_PIPELINE_VALID(world, TNECS_PIPELINE_RENDER));
    SDL_assert(TNECS_PIPELINE_VALID(world, TNECS_PIPELINE_CONTROL));
    SDL_assert(TNECS_PIPELINE_VALID(world, TNECS_PIPELINE_TURN_START));
    SDL_assert(TNECS_PIPELINE_VALID(world, TNECS_PIPELINE_TURN_END));

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Phase Registration\n");
#include "register/phases.h"
    SDL_assert(TNECS_PHASE_VALID(world, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_NULL));
    SDL_assert(TNECS_PHASE_VALID(world, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_MOVE));
    SDL_assert(TNECS_PHASE_VALID(world, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE));
    SDL_assert(TNECS_PHASE_VALID(world, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW));
    SDL_assert(TNECS_PHASE_VALID(world, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_CURSOR));

    tnecs_phases *byphase   = TNECS_PIPELINE_GET(world, TNECS_NULL);
    SDL_assert(byphase->num == TNECS_NULLSHIFT);

    byphase = TNECS_PIPELINE_GET(world, TNECS_PIPELINE_RENDER);
    SDL_assert(byphase->num == TNECS_RENDER_PHASE_NUM);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration\n");
#include "register/systems.h"
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration DONE\n");

    return (0);
}

void Game_Startup_Map(Game *IES) {
    // IES->settings.args.map_index
    SDL_Log("IMPLEMENT ME");
    exit(1);

    strncpy(IES->debug.reason, "Starting Map", sizeof(IES->debug.reason));
    if (Game_State_Current(IES) != GAME_STATE_Gameplay_Map)
        Game_State_Set(IES, GAME_STATE_Gameplay_Map, IES->debug.reason);
    if (Game_Substate_Current(IES) != GAME_SUBSTATE_MENU)
        Game_subState_Set(IES, GAME_SUBSTATE_MENU, IES->debug.reason);
}

void Game_Startup_Scene(Game *IES) {
    SDL_assert(IES              != NULL);
    SDL_assert(IES->fonts.pixelnours  != NULL);
    /* --- Get scene JSON file from filename --- */
    // TODO: differentiate between cutscene and talk scene
    s8 filename = Scene_Filename(IES->settings.args.scene);
    // SDL_Log("Loading Scene '%s'", filename.data);

    IES->narrative.scene      = IES_E_CREATE_wC(gl_world, Scene_ID);
    Scene *scene    = IES_GET_C(gl_world, IES->narrative.scene, Scene);
    *scene = Scene_default;
    Scene_Init(scene);
    // TODO: Remove quit event on scene finish
    scene->event = event_Quit;

    scene->pixelnours = IES->fonts.pixelnours;
    Scene_Text_Box_Init(scene, IES->render.er);
    Scene_Texture_Create(scene, IES->render.er);

    jsonio_readJSON(filename, scene);

    strncpy(IES->debug.reason, "Starting Scene", sizeof(IES->debug.reason));
    if (Game_State_Current(IES) != GAME_SUBSTATE_STANDBY)
        Game_State_Set(IES, GAME_SUBSTATE_STANDBY, IES->debug.reason);
    if (Game_Substate_Current(IES) != GAME_STATE_Scene_Talk)
        Game_subState_Set(IES, GAME_STATE_Scene_Talk, IES->debug.reason);

    SDL_Log("POST jsonio_readJSON");
    Scene_Statement_Next(scene);
    SDL_Log("POST Scene_Statement_Next %d", scene->current_statement);
}

void Game_Startup_TitleScreen(Game *IES) {
    Game_titleScreen_Load(IES, IES->settings.args);
    Game_Mouse_Disable(IES);
    Game_cursorFocus_onMenu(IES);

    /* -- Checks -- */
    strncpy(IES->debug.reason, "Starting Title Screen", sizeof(IES->debug.reason));
    if (Game_State_Current(IES) != GAME_STATE_Title_Screen)
        Game_State_Set(IES, GAME_STATE_Title_Screen, IES->debug.reason);
    if (Game_Substate_Current(IES) != GAME_SUBSTATE_MENU)
        Game_subState_Set(IES, GAME_SUBSTATE_MENU, IES->debug.reason);
}

void Game_Save_Copy(i32 from_ind,  i32 to_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    s8 filenameto      = Savefile_Path(to_ind);
    s8 filenamefrom    = Savefile_Path(from_ind);

    SDL_LogDebug(SOTA_LOG_APP, "copy saveJSON Game from %s to %s\n", filenamefrom.data,
                 filenameto.data);
    PHYSFS_file *pfrom  = PHYSFS_openRead(filenamefrom.data);
    PHYSFS_file *pto    = PHYSFS_openWrite(filenameto.data);
    SDL_assert(pfrom    != NULL);
    SDL_assert(pto      != NULL);

    i16 len = PHYSFS_fileLength(pfrom);
    char longbuffer[len];
    PHYSFS_readBytes(pfrom, longbuffer, len);
    PHYSFS_writeBytes(pto, longbuffer, len);
    PHYSFS_close(pfrom);
    PHYSFS_close(pto);
    s8_free(&filenameto);
    s8_free(&filenamefrom);
}

void Game_Save_Delete( i32 save_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    s8 filename = Savefile_Path(save_ind);
    SDL_LogDebug(SOTA_LOG_APP, "Deleting Game: %s\n", filename.data);
    PHYSFS_delete(filename.data);
    s8_free(&filename);
}

void _Game_loadJSON(struct Game *IES, s8 filename) {
    // SDL_Log("Loading save: '%s'", filename.data);
    cJSON *json = jsonio_parseJSON(filename);

    /* --- Narrative --- */
    // readJSON_narrative(json, &IES->narrative);

    /* --- Camp --- */
    // cJSON * jcamp = cJSON_GetObjectItem(json, "Camp");
    // Camp_readJSON(&IES->camp, jcamp);

    /* --- RNG --- */
    cJSON *jRNG = cJSON_GetObjectItem(json, "RNG");
    RNG_readJSON(IES->RNG.s_xoshiro256ss, jRNG);
    RNG_Set_xoroshiro256ss(IES->RNG.s_xoshiro256ss[0],
                           IES->RNG.s_xoshiro256ss[1],
                           IES->RNG.s_xoshiro256ss[2],
                           IES->RNG.s_xoshiro256ss[3]);

    /* --- Convoy --- */
    // cJSON *jconvoy = cJSON_GetObjectItem(json, "Convoy");
    // Convoy_Clear(&IES->convoy);
    // Convoy_readJSON(&IES->convoy, jconvoy);
    //     convoy.setWeapons(&weapons);

    /* --- Chapter --- */
    cJSON *jchapter = cJSON_GetObjectItem(json, "Chapter");
    IES->state.chapter = cJSON_GetNumberValue(jchapter);

    /* --- Party --- */
    Party_Free(&IES->party);
    Party_Init(&IES->party);

    cJSON *jparty           = cJSON_GetObjectItem(json, "Party");
    cJSON *jparty_filename  = cJSON_GetObjectItem(jparty, "filename");
    char    *party_filename     = cJSON_GetStringValue(jparty_filename);

    if (party_filename == NULL) {
        /* - If no filename in Party, Party is in the current file - */
        Party_readJSON(&IES->party, jparty);
    } else {
        /* - Reading party json - */
        SDL_assert(party_filename != NULL);
        IES->party.load_filename = s8_mut(party_filename);

        /* party filename should include folder */
        Party_Folder(&IES->party, "");
        jsonio_readJSON(IES->party.load_filename, &IES->party);
    }

    /* - SDL_free - */
    cJSON_Delete(json);
}

void _Game_saveJSON(struct Game *IES, s8  filename) {
    /* --- Open file --- */
    PHYSFS_delete(filename.data);
    PHYSFS_file *fp = PHYSFS_openWrite(filename.data);
    SDL_assert(fp);

    /* --- Create json object file --- */
    cJSON *json         = cJSON_CreateObject();

    /* --- Narrative --- */
    // writeJSON_narrative(json, &IES->narrative);

    /* --- Camp --- */
    // cJSON * jcamp = cJSON_CreateObject();
    // Camp_writeJSON(&IES->camp, jcamp);

    /* --- Chapter --- */
    cJSON *jchapter     = cJSON_CreateNumber(IES->state.chapter);

    /* --- RNG --- */
    cJSON *jRNG         = cJSON_CreateArray();
    RNG_Get_xoroshiro256ss(IES->RNG.s_xoshiro256ss);
    RNG_writeJSON(IES->RNG.s_xoshiro256ss, jRNG);

    /* --- Convoy --- */
    cJSON *jconvoy      = cJSON_CreateObject();
    Convoy_writeJSON(&IES->convoy, jconvoy);

    /* --- Party --- */
    if (IES->party.save_filename.data != NULL) {
        /* Write party as separate json file */
        jsonio_writeJSON(IES->party.save_filename, &IES->party, false);
    } else {
        /* Write party as list in save json */
        cJSON *jparty       = cJSON_CreateObject();
        Party_writeJSON(&IES->party, jparty);
        cJSON_AddItemToObject(json, "Party",        jparty);
    }

    /* --- Adding to parent JSON object --- */
    cJSON_AddItemToObject(json, "RNG",          jRNG);
    cJSON_AddItemToObject(json, "Chapter",      jchapter);
    cJSON_AddItemToObject(json, "Convoy",       jconvoy);
    // cJSON_AddItemToObject(json, "Camp",         jcamp);
    // cJSON_AddItemToObject(json, "Narrative",    jnarrative);

    /* --- Print to file --- */
    jsonio_Print(fp, json);

    /* Clean the file */
    PHYSFS_close(fp);
    if (json != NULL)
        cJSON_Delete(json);
}
s8 Savefile_Path(i32 save_ind) {
    /* Creating load path */
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));
    return (filename);
}

void Game_Save_Load(struct Game *IES, i32 save_ind) {
    /* Load a savefile */
    /* Checking save folder */
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));

    s8 filename = Savefile_Path(save_ind);

    /* Reading JSON save file */
    _Game_loadJSON(IES, filename);

    /* -- Loading map taken from savefile -- */
    Game_Map_Load(IES, IES->state.chapter);

    /* -- Loading party taken from savefile -- */
    /* - Reading party json - */
    SDL_assert(IES->party.load_filename.data != NULL);

    /* party filename should include folder */

    /* - Loading party units json - */
    Game_Party_Load(IES);

    Party_Size(&IES->party);
    SDL_assert(IES->party.size > 0);

    s8_free(&filename);
}

void Game_Save(struct Game *IES, i32 save_ind) {
    /* Making save folder if it doesn't exist */
    if (!PHYSFS_exists(SAVE_FOLDER))
        PHYSFS_mkdir(SAVE_FOLDER);

    s8 filename = Savefile_Path(save_ind);

    /* Actual saving game to JSON file */
    _Game_saveJSON(IES, filename);

    s8_free(&filename);
}

/* --- State --- */
i32 Game_Chapter(const struct Game *IES) {
    if (IES == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (IES->state.chapter);
}

i32 Game_State_Current(const struct Game *IES) {
    if (IES == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (IES->state.top.current);
}

i32 Game_Substate_Current(const struct Game *IES) {
    if (IES == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (IES->state.sub.current);
}

i32 Game_State_Previous(const struct Game *IES) {
    if (IES == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (IES->state.top.previous);
}

i32 Game_Substate_Previous(const struct Game *IES) {
    if (IES == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (IES->state.sub.previous);
}

void Game_subState_Set(struct Game *IES,  i8 new_substate,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Substate set to %d because: %s", new_substate, reason);
    SDL_assert(new_substate > 0);
    if (Game_Substate_Current(IES) == new_substate) {
        SDL_assert(false);
        return;
    }

    IES->state.sub.previous = Game_Substate_Current(IES);
    IES->state.sub.current  = new_substate;
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game substate changed %d->%d: %s->%s",
                 Game_Substate_Previous(IES), Game_Substate_Current(IES),
                 gamesubStatenames[Game_Substate_Previous(IES)].data,
                 gamesubStatenames[Game_Substate_Current(IES)].data);
    if (new_substate == GAME_SUBSTATE_STANDBY)
        IES->cursor.diagonal = true;
    else
        IES->cursor.diagonal = false;

    if (fsm_Input_sGmpMap_ss[Game_Substate_Current(IES)] != NULL)
        fsm_Input_sGmpMap_ss[Game_Substate_Current(IES)](IES);
}

void Game_State_Set(struct Game *IES,  i8 new_state,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "State set to %d, because: %s", new_state, reason);
    SDL_assert(new_state > 0);
    SDL_assert(Game_State_Current(IES) != new_state);
    IES->state.top.previous = Game_State_Current(IES);
    IES->state.top.current  = new_state;

    /* --- Set contextual inputs --- */
    if (fsm_Input_s[Game_State_Current(IES)] != NULL)
        fsm_Input_s[Game_State_Current(IES)](IES);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game state changed %d->%d: %s->%s",
                 Game_State_Previous(IES), Game_State_Current(IES),
                 gameStatenames[Game_State_Previous(IES)].data, gameStatenames[Game_State_Current(IES)].data);
}

/* --- Time --- */
void Game_Delay(Game   *IES,
                i64     delay_ms,
                u64     currentTime_ns,
                u64     elapsedTime_ns) {
    SDL_assert(IES != NULL);

    /* - Skip if negative delay - */
    if (delay_ms < 0) {
        return;
    }

    /* - Delay game less if sync took > 1ms - */
    u64 new_elapsedTime_ns = nours_get_ns() - currentTime_ns;

    u32 delay   = 0;
    u64 elapsed_ms = (new_elapsedTime_ns - elapsedTime_ns) / SOTA_us;
    if (delay_ms > elapsed_ms)
        delay = delay_ms - elapsed_ms;

    /* - Actually delay game - */
    if (delay > 0)
        SDL_Delay(delay);

    /* - Update runtime - */
    IES->timers.runtime_ns += new_elapsedTime_ns + (delay > 0) * delay * SOTA_us;
}

/* --- FPS --- */
i64 Game_FPS_Delay(struct Game *IES, u64 elapsedTime_ns) {
    i64 delay       = 0;
    int fps_cap     = IES->settings.FPS.cap;    /* [s^-1] */
    int ff_cap      = IES->settings.FPS.ff_cap; /* [%]    */

    /* 0 frame delay if fast-forwarding (ff) without limit */
    if ((IES->flags.fast_forward) && (ff_cap <= SOTA_100PERCENT)) {
        return (delay);
    }

    /* Compute delay for ff_cap */
    if ((IES->flags.fast_forward) && (ff_cap > SOTA_100PERCENT)) {
        int ff_fps_cap = fps_cap * ff_cap / SOTA_100PERCENT;
        delay = ceil((1000.0f / ff_fps_cap) - (elapsedTime_ns / 1e6));
        return (delay);
    }

    /* Compute delay for no ff */
    if (((elapsedTime_ns * fps_cap) < 1e9) || (elapsedTime_ns == 0))
        delay = ceil((1000.0f / fps_cap) - (elapsedTime_ns / 1e6));

    SDL_assert(delay >= 0);
    return (delay);
}

void Game_FPS_Create(struct Game *IES, i64 in_update_time_ns) {
    if (IES->fps.entity != 0)
        tnecs_entity_destroy(gl_world, IES->fps.entity);
    IES->fps.entity = IES_E_CREATE_wC(gl_world, Position_ID, Text_ID, Timer_ID);

    /* -- Get timer -- */
    struct Timer *timer;
    timer  = IES_GET_C(gl_world, IES->fps.entity, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    /* -- Get position -- */
    struct Position *position;
    position = IES_GET_C(gl_world, IES->fps.entity, Position);
    *position = Position_default;

    SDL_assert(position != NULL);
    position->onTilemap = false;
    Position_Bounds_Set(position, 0, IES->settings.res.x, 0, IES->settings.res.y);
    i16 in_x = IES->settings.res.x * 0.88f;
    i16 in_y = IES->settings.res.y * 0.02f;
    Position_Pos_Set(position, in_x, in_y);
    position->scale[0] = FPS_SCALE;
    position->scale[1] = FPS_SCALE;

    /* -- Get Text -- */
    Text *text = IES_GET_C(gl_world, IES->fps.entity, Text);
    SDL_assert(text != NULL);
    Text_Init(text);
    P_Text_Init(text, IES->render.er);
    SDL_assert(text->plat != NULL);
    text->pixelfont         = IES->fonts.pixelnours_big;
    text->onUpdate          = &Text_onUpdate_FPS;
    text->update_time_ns    = in_update_time_ns;
    Text_Set(text, "60.1", PIXELNOURS_BIG_Y_OFFSET);
}

/* --- SETTINGS --- */
void Game_Brightness_Set(struct Game *IES, float bright) {
    bright = bright < SOTA_BRIGHTNESS_MIN ? SOTA_BRIGHTNESS_MIN : bright;
    bright = bright > SOTA_BRIGHTNESS_MAX ? SOTA_BRIGHTNESS_MAX : bright;
    SDL_SetWindowBrightness(IES->render.window, bright);
}

float Game_Brightness_Get(struct Game *IES) {
    return (SDL_GetWindowBrightness(IES->render.window));
}

/* --- DISPLAY --- */
void Game_Display_Bounds(struct Game *IES) {
    // Find bounds of display.

    // Skip if fullscreen
    if (IES->settings.fullscreen)
        return;

    SDL_Rect bounds;
    if (SDL_GetDisplayBounds(0, &bounds) != 0) {
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL could not get display bounds! SDL Error: %s\n",
                        SDL_GetError() );
        exit(1);
    }

    // Clamp resolution to display if window is bigger
    if (IES->settings.res.x > bounds.w)
        IES->settings.res.x = bounds.w;

    if (IES->settings.res.y > bounds.h)
        IES->settings.res.y = bounds.h;
}


/* --- AUDIO --- */

/* -- Music -- */

void Game_Music_Play(struct Game *IES) {
    if (IES->audio.music == NULL) {
        SDL_LogWarn(SOTA_LOG_AUDIO, "Sota has no song to play.");
        return;
    }

#ifndef DEBUG_NO_MUSIC
    if (!Mix_PlayingMusic())
        Mix_FadeInMusic(IES->audio.music, -1, SOTA_MUSIC_FADEIN_ms);
    else if (Mix_PausedMusic())
        Mix_ResumeMusic();
#endif /* DEBUG_NO_MUSIC */
}

void Game_Music_Stop(struct Game *IES) {
    Mix_FadeOutMusic(SOTA_MUSIC_FADEOUT_ms);
}

void Game_Music_Pause(struct Game *IES) {
    Mix_PauseMusic();
}

/* -- Volume -- */
void Game_Volume_Set(struct Game *IES, int volume) {
    Game_Volume_Music_Set(  IES, volume);
    Game_Volume_SoundFX_Set(IES, volume);
}

void Game_Volume_Music_Set(struct Game *IES, int volume) {
    volume = volume <        0       ?        0       : volume;
    volume = volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume;
    Mix_VolumeMusic(volume);
}

void Game_Volume_SoundFX_Set(struct Game *IES, int volume) {
    volume = volume <        0       ?        0       : volume;
    volume = volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume;
    Mix_MasterVolume(volume); /* - Only for chunks - */
}

int Game_Volume_Music_Get(  struct Game *IES) {
    return (Mix_VolumeMusic(-1));
}

int Game_Volume_SoundFX_Get(struct Game *IES) {
    return (Mix_MasterVolume(-1));
}

/* -- Battle -- */
void  Game_Battle_Start(struct Game *IES, struct Menu *mc) {
    /* -- Place units in deployment slots on map -- */
    struct DeploymentMenu *dm = mc->data;
    if (dm->_selected_num == 0) {
        return;
    }

    /* -- Game substate to on Map -- */
    strncpy(IES->debug.reason, "Preparation done. March!", sizeof(IES->debug.reason));
    if (Game_State_Current(IES) != GAME_STATE_Gameplay_Map)
        Game_State_Set(IES, GAME_STATE_Gameplay_Map, IES->debug.reason);
    if (Game_Substate_Current(IES) != GAME_SUBSTATE_STANDBY)
        Game_subState_Set(IES, GAME_SUBSTATE_STANDBY, IES->debug.reason);

    /* -- Load map -- */
    Game_PopUp_Tile_Create(IES);
    Game_PopUp_Unit_Create(IES);
    Game_cursorFocus_onMap(IES);

    /* -- Destroy Deployment Menu -- */
    b32 destroy = true;
    u64 menu_num = DARR_NUM(IES->menus.stack);
    if (IES->menus.stack[menu_num - 1] == IES->menus.deployment) {
        Game_menuStack_Pop(IES, destroy);
    }

    /* -- Disable palette map during turn transition -- */
    Map *map = Game_Map(IES);
    Map_Palettemap_Autoset(map, 0, TNECS_NULL);

    /* -- Set cursor position to first starting position -- */
    SDL_assert(IES                     != NULL);
    SDL_assert(IES->cursor.entity      != TNECS_NULL);
    struct Position *position = IES_GET_C(gl_world, IES->cursor.entity, Position);
    SDL_assert(position             != NULL);
    SDL_assert(map                  != NULL);
    SDL_assert(map->start_pos.arr   != NULL);
    SDL_assert(DARR_NUM(map->start_pos.arr) > 0);
    Position_Pos_Set(position, map->start_pos.arr[0].x, map->start_pos.arr[0].y);

    // Game_cursorFocus_onMap resets position to cursor_lastpos;
    // TODO: bool flag to disable Game_cursorFocus_onMap resetting cursor pos.
    IES->cursor.lastpos.x = map->start_pos.arr[0].x;
    IES->cursor.lastpos.y = map->start_pos.arr[0].y;

    /* -- Set popups position -- */
    Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, IES->cursor.entity, Position);
    Point *pos = &cursor_pos->tilemap_pos;

    /* -- Set popup_unit position -- */

    tnecs_entity popup_ent = IES->popups.arr[POPUP_TYPE_HUD_UNIT];
    SDL_assert(popup_ent != TNECS_NULL);

    Slider          *popup_unit_slider;
    Position        *popup_unit_pos;
    SliderOffscreen *popup_unit_offscreen;
    popup_unit_slider = IES_GET_C(gl_world, popup_ent, Slider);
    popup_unit_pos = IES_GET_C(gl_world, popup_ent, Position);
    popup_unit_offscreen = IES_GET_C(gl_world, popup_ent, SliderOffscreen);
    SDL_assert(popup_unit_slider != TNECS_NULL);
    SDL_assert(popup_unit_pos != TNECS_NULL);
    SDL_assert(popup_unit_offscreen != TNECS_NULL);

    Game_PopUp_Unit_Place(IES, *pos);

    popup_unit_pos->pixel_pos = popup_unit_offscreen->midpoint;

    /* -- Set popup_tile position -- */
    popup_ent = IES->popups.arr[POPUP_TYPE_HUD_TILE];
    SDL_assert(popup_ent != TNECS_NULL);

    Slider          *popup_tile_slider;
    Position        *popup_tile_pos;
    SliderOffscreen *popup_tile_offscreen;
    popup_tile_slider = IES_GET_C(gl_world, popup_ent, Slider);
    popup_tile_pos = IES_GET_C(gl_world, popup_ent, Position);
    popup_tile_offscreen = IES_GET_C(gl_world, popup_ent, SliderOffscreen);
    SDL_assert(popup_tile_slider != TNECS_NULL);
    SDL_assert(popup_tile_pos != TNECS_NULL);
    SDL_assert(popup_tile_offscreen != TNECS_NULL);

    Game_PopUp_Tile_Place(IES, *pos);

    popup_tile_pos->pixel_pos.x = popup_tile_offscreen->midpoint.x;
    popup_tile_pos->pixel_pos.y = popup_tile_slider->target.y;

    /* -- Start turn transition -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}

/* -- Scene -- */
struct Scene *Game_Scene(struct Game *IES) {
    if (IES->narrative.scene <= TNECS_NULL) {
        return (NULL);
    }
    Scene *scene = IES_GET_C(gl_world, IES->narrative.scene, Scene);
    SDL_assert(scene != NULL);
    return (scene);
}

i32 Game_Control(const struct Game *const IES) {
    /* --- Who controls the game? --- */
    i32 out = (IES->ai.control != TNECS_NULL) ? SOTA_AI : SOTA_PLAYER;
    return (out);
}
