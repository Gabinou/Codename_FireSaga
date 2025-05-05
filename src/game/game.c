
#include "game/game.h"
#include "game/map.h"
#include "game/control.h"
#include "game/cursor.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/unit.h"
#include "map/map.h"
#include "bars/map_hp.h"
#include "systems/render.h"
#include "filesystem.h"
#include "noursclock.h"
#include "log.h"
#include "macros.h"
#include "RNG.h"
#include "platform.h"
#include "position.h"
#include "popup/popup.h"
#include "menu/menu.h"
#include "unit/boss.h"
#include "unit/anim.h"
#include "menu/loadout_select.h"
#include "menu/deployment.h"
#include "menu/stats.h"
#include "map/animation.h"
#include "unit/party.h"
#include "map/render.h"
#include "scene.h"
#include "cutscene.h"
#include "slider.h"
#include "pixelfonts.h"
#include "sprite.h"
#include "input.h"
#include "tile.h"
#include "hover.h"
#include "combat.h"
#include "music.h"
#include "controller/mouse.h"
#include "controller/touchpad.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "systems/time_system.h"
#include "systems/slide.h"
#include "AI.h"
#include "events.h"
#include "actor.h"
#include "fps_fsm.h"
#include "fsm.h"
#include "names.h"
#include "text.h"
#include "convoy.h"
#include "utilities.h"
#include "stb_sprintf.h"
#include "globals.h"

const struct Game Game_default = {
    .cursor.lastpos         = {1, 1},
    .cursor.moved_direction = SOTA_DIRECTION_NULL,
    .flags.iscursor         = true,

    .camera = {
        .zoom = DEFAULT_CAMERA_ZOOM,
        .offset = {.x = DEFAULT_CAMERA_XOFFSET, .y = DEFAULT_CAMERA_YOFFSET}
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

void Game_Free(struct Game *sota) {
    AI_State_Free(&sota->state.ai);
    Game_Party_Free(sota);
    Party_Free(&sota->party);
    Game_Title_Destroy(sota);
    GameMap_Reinforcements_Free(sota);
    Game_DeploymentMenu_Free(sota);
    Game_Cursor_Free(sota);
    Game_PopUp_Tile_Free(sota);
    Game_Mouse_Free(sota);
    Game_menuStack_Free(sota);
    Game_PlayerSelectMenus_Free(sota);
    Game_PopUp_Unit_Free(sota);
    if (sota->combat.outcome.attacks != NULL) {
        DARR_FREE(sota->combat.outcome.attacks);
        sota->combat.outcome.attacks = NULL;
    }

    PixelFont_Free(sota->fonts.pixelnours,       true);
    PixelFont_Free(sota->fonts.pixelnours_big,   true);
    PixelFont_Free(sota->fonts.pixelnours_tight, true);

    if (sota->menus.stats > TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.stats, Menu);
        if (mc->data != NULL) {
            struct StatsMenu *stats_menu = mc->data;
            if (mc->n9patch.texture != NULL)
                SDL_DestroyTexture(mc->n9patch.texture);
            StatsMenu_Free(stats_menu);
            mc->data = NULL;
        }
    }

    if (sota->menus.item_select > TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.item_select, Menu);
        if (mc->data != NULL) {
            struct LoadoutSelectMenu *ism = mc->data;
            LoadoutSelectMenu_Free(ism);
            SDL_free(ism);
            mc->data = NULL;
        }
    }

    if (sota->render.er) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Control");
        // controlSDL_free();
    }
    if (sota->audio.soundfx_cursor != NULL) {
        Mix_FreeChunk(sota->audio.soundfx_cursor);
        sota->audio.soundfx_cursor = NULL;
    }
    if (sota->audio.soundfx_next_turn != NULL) {
        Mix_FreeChunk(sota->audio.soundfx_next_turn);
        sota->audio.soundfx_next_turn = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Map");

    if ((sota->map != NULL) && (sota->map->darrs.unitmap != NULL))
        Map_Unitmap_Free(sota->map);
    Game_Map_Free(sota);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Camera");
    // if (sota->camera != NULL)
    // SDL_free(sota->camera);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Events");
    Events_Data_Free();
    Events_Names_Free();
    Events_Receivers_Free();
#ifndef SOTA_OPENGL
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Renderer");
    if (sota->render.target != NULL)
        SDL_DestroyTexture(sota->render.target);
    if (sota->render.er != NULL) {
        SDL_DestroyRenderer(sota->render.er); /* SDL_free renderer before window */
    }
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Window");
    if (sota->render.window != NULL)
        SDL_DestroyWindow(sota->render.window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    Mix_Quit();
    SDL_Quit();
#else /* SOTA_OPENGL */
#endif /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free tnecs world");

    if (sota->ecs.world != NULL) {
        tnecs_world_destroy(&sota->ecs.world);
        sota->ecs.world = NULL;
    }
    if (sota->ecs.world_render != NULL) {
        tnecs_world_destroy(&sota->ecs.world_render);
        sota->ecs.world_render = NULL;
    }
    if (sota->ecs.world_control != NULL) {
        tnecs_world_destroy(&sota->ecs.world_control);
        sota->ecs.world_control = NULL;
    }

    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    if (sota->fonts.menu != NULL) {
        PixelFont_Free(sota->fonts.menu, false);
    }
    if (sota->targets.defendants != NULL) {
        DARR_FREE(sota->targets.defendants);
        sota->targets.defendants = NULL;
    }
    if (sota->targets.patients != NULL) {
        DARR_FREE(sota->targets.patients);
        sota->targets.patients = NULL;
    }
    if (sota->targets.victims != NULL) {
        DARR_FREE(sota->targets.victims);
        sota->targets.victims = NULL;
    }
    if (sota->targets.deployed != NULL) {
        DARR_FREE(sota->targets.deployed);
        sota->targets.deployed = NULL;
    }
    if (sota->targets.spectators != NULL) {
        DARR_FREE(sota->targets.spectators);
        sota->targets.spectators = NULL;
    }
    if (sota->targets.auditors != NULL) {
        DARR_FREE(sota->targets.auditors);
        sota->targets.auditors = NULL;
    }
    if (sota->targets.passives != NULL) {
        DARR_FREE(sota->targets.passives);
        sota->targets.passives = NULL;
    }
    if (sota->targets.openables != NULL) {
        DARR_FREE(sota->targets.openables);
        sota->targets.openables = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Game cleaned.");
    SDL_free(sota);
}

void Game_AI_Free(struct Game *sota) {
    if (sota->state.ai.npcs != NULL) {
        DARR_FREE(sota->state.ai.npcs);
        sota->state.ai.npcs = NULL;
    }
}

void Game_Post_Free(void) {
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Utilities\n");
    Utilities_Free();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Filesystem\n");
    Filesystem_Free();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Freeing Names\n");
    Names_Free();
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

u64 _Game_Step_PreFrame(struct Game *sota) {
    u64 currentTime_ns = tnecs_get_ns();
    sota->cursor.frame_moved = false;
    SDL_RenderClear(sota->render.er); /* RENDER clears the backbuffer */
    return (currentTime_ns);
}

void _Game_Step_Control(struct Game *sota) {
    /* --- Control: Get player inputs --- */
    /* -- player inputs -- */
    Game_Control_Gamepad( sota);
    Game_Control_Keyboard(sota);
    Game_Control_Touchpad(sota);

    /* -- fps_fsm -- */
    SDL_assert(fsm_cFrame_s[Game_State_Current(sota)] != NULL);
    fsm_cFrame_s[Game_State_Current(sota)](sota); /* CONTROL */
}

void _Game_Step_Render(struct Game *sota) {
    /* Render FSM */
    SDL_assert(fsm_rFrame_s[Game_State_Current(sota)] != NULL);
    fsm_rFrame_s[Game_State_Current(sota)](sota); /* RENDER */
    u64 updateTime_ns = SOTA_ns / sota->settings.FPS.cap;
    b32 success = tnecs_world_step(sota->ecs.world, updateTime_ns, sota); /* RENDER */
    if (!success) {
        SDL_Log("tnecs_world_step failed");
        SDL_assert(false);
        exit(ERROR_Generic);
    }

    /* -- Render to screen -- */
#ifndef RENDER2WINDOW
    SDL_SetRenderTarget(sota->render.er, NULL); /* RENDER */
    SDL_RenderCopy(     sota->render.er, sota->render.target, NULL, NULL);
    SDL_SetRenderTarget(sota->render.er, sota->render.target);
#endif
    SDL_RenderPresent(sota->render.er);
}

void _Game_Step_PostFrame(struct Game *sota, u64 currentTime_ns) {
    /* -- Synchronize timers -- */
    u64 elapsedTime_ns = tnecs_get_ns() - currentTime_ns;
    i64 delay_ms       = Game_FPS_Delay(sota, elapsedTime_ns);
    tnecs_ns time_ns   = (elapsedTime_ns + delay_ms * SOTA_ns / SOTA_ms);

    Game_Cursor_movedTime_Compute(sota, time_ns);
    // SDL_Log("sota->cursor.moved_time_ms %d\n", sota->cursor.moved_time_ms);
    tnecs_custom_system_run(sota->ecs.world, Time_Synchronize,
                            sota->ecs.timer_typeflag, time_ns, NULL);

    /* -- Delay until next frame -- */
    Game_Delay(sota, delay_ms, currentTime_ns, elapsedTime_ns);
}

void Game_Step(struct Game *sota) {
    /* TODO: deal with frame running LONGER than expected */
    // printf("Step\n");
    u64 currentTime_ns = _Game_Step_PreFrame(sota);
    _Game_Step_Control(sota);
    Events_Manage(sota); /* CONTROL */
    _Game_Step_Render(sota);
    _Game_Step_PostFrame(sota, currentTime_ns);
}

struct Game * Game_New(Settings settings) {
    /* -- Setting defaults -- */
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = settings;

    SDL_assert(sota->settings.FPS.cap > 0);
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Init game");
    sota->menus.filename = s8_literal(PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png"));

    _Game_New_Alloc(sota);

    /* Window flags */
    u32 flags = sota->settings.window;
    if (sota->settings.fullscreen)
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
    struct Music_settings music = sota->settings.music_settings;
    if (Mix_OpenAudio(music.frequency, music.format, music.channels, music.sample_size) < 0) {
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                        Mix_GetError());
        exit(1);
    }

    Game_Display_Bounds(sota);

    sota->render.window = SDL_CreateWindow(sota->settings.title, sota->settings.pos.x,
                                           sota->settings.pos.y, sota->settings.res.x, sota->settings.res.y, flags);
    SDL_assert(sota->render.window);
    // SDL_assert(SDL_ISPIXELFORMAT_INDEXED(SDL_GetWindowPixelFormat(sota->render.window)));
    int window_w;
    int window_h;
    SDL_GetWindowSize(sota->render.window, &window_w, &window_h);
    SDL_assert(window_w == sota->settings.res.x);
    SDL_assert(window_h == sota->settings.res.y);
    if (sota->render.window) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "Window created\n");
        sota->render.er = SDL_CreateRenderer(sota->render.window, -1, SDL_RENDERER_TARGETTEXTURE);
        SDL_assert(sota->render.er);
        if (sota->render.er) {
            Utilities_DrawColor_Reset(sota->render.er);
            SDL_LogVerbose(SOTA_LOG_SYSTEM, "Renderer created\n");
        }
    }
#ifndef RENDER2WINDOW
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Rendering to sota->render.target\n");
    SDL_assert(sota->settings.res.x > 0);
    SDL_assert(sota->settings.res.y > 0);
    SDL_assert(sota->render.er != NULL);
    sota->render.target = SDL_CreateTexture(sota->render.er, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, sota->settings.res.x, sota->settings.res.y);
    SDL_assert(sota->render.target != NULL);
    SDL_SetTextureBlendMode(sota->render.target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(sota->render.er, sota->render.target);
    SDL_assert(sota->render.target != NULL);
#endif /* RENDER2WINDOW */
#else /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Firesaga: OpenGL Window Initialization");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // TODO: set better window flags
    sota->gl_window = SDL_CreateWindow(sota->settings.title, sota->settings.pos.x,
                                       sota->settings.pos.y, sota->settings.res.x, sota->settings.res.y,
                                       flags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_assert(sota->gl_window);
    sota->gl_context = SDL_GL_CreateContext(sota->gl_window);
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

    _Game_New_Events(sota);
    _Game_New_Tnecs(sota);

    sota->inputs.keyboardInputMap  = KeyboardInputMap_default;
    sota->inputs.gamepadInputMap   = GamepadInputMap_switch_pro;

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Loading pixelfonts\n");
    sota->fonts.pixelnours = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");
    PixelFont_Load(sota->fonts.pixelnours, sota->render.er, path);
    sota->fonts.pixelnours->y_offset = pixelfont_y_offset;

    sota->fonts.pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(sota->fonts.pixelnours_big, sota->render.er, path);
    sota->fonts.pixelnours_big->y_offset = pixelfont_big_y_offset;

    sota->fonts.pixelnours_tight = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_tight.png");
    PixelFont_Load(sota->fonts.pixelnours_tight, sota->render.er, path);

    /* Sprite init */
    b32 absolute = false;
    b32 isCursor = false;
    dstrect_funcs[absolute][isCursor = true]        = &Cursor_Dstrect_Relative;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Relative;
    dstrect_funcs[absolute = true][isCursor = true] = &Cursor_Dstrect_Absolute;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Absolute;

    /* --- Soundfx --- */
    sota->audio.soundfx_cursor    = Soundfx_Load_Cursor();
    sota->audio.soundfx_next_turn = Soundfx_Load_Next_Turn();

    /* -- Load Cursor and mouse -- */
    SDL_ShowCursor(SDL_DISABLE); /* for default cursor */
    Game_FPS_Create(sota, SOTA_FPS_UPDATE_ns);
    Game_Cursor_Create(sota);
    Game_Mouse_Create(sota);

    if ((sota->settings.args.map_index == 0) && (sota->settings.args.scene == 0)) {
        Game_Startup_TitleScreen(sota);
    } else if (sota->settings.args.scene != 0) {
        Game_Startup_Scene(sota);
    } else if (sota->settings.args.map_index != 0) {
        Game_Startup_Map(sota);
    }

    /* --- Set default contextual inputs --- */
    fsm_Input_s[Game_State_Current(sota)](sota);

    /* -- Set color -- */
    Utilities_DrawColor_Reset(sota->render.er);

    /* -- Checks -- */
    SDL_assert(sota->mouse.entity);

    sota->flags.isrunning = true;
    return (sota);
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

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Allocating space for events\n");
    Events_Data_Malloc();
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Initializing user events\n");
    Events_Names_Declare();
    Events_Names_Alloc();
    Events_Receivers_Declare();

    return (0);
}

int _Game_New_Tnecs(void *data) {
    Game *IES = data;

    // TODO:
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Tnecs: Genesis\n");
    if (!tnecs_world_genesis(&IES->ecs.world)) {
        SDL_Log("Could not init tnecs_world");
        SDL_assert(false);
        exit(ERROR_Generic);
    }

    // Don't reuse entities.
    // If I forget to update an entity somewhere, it'll be invalid for sure.
    SDL_assert(IES->ecs.world->reuse_entities == false);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Components Registration\n");
    tnecs_world *world = IES->ecs.world;
#include "register_components.h"

    // TODO: Replace every
    // - IES_GET_COMPONENT(world, entity_id, ComponentName)
    // - tnecs_get_component(world, entity_id, COMPONENT_Position)

    IES->ecs.timer_typeflag = TNECS_COMPONENT_ID2TYPE(Timer_ID);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration\n");
#define REGISTER_ENUM(pfunc, phase, excl, ...) TNECS_REGISTER_SYSTEM(IES->ecs.world, pfunc, phase, excl, __VA_ARGS__);
#include "names/systems.h"
#undef REGISTER_ENUM

    /* --- SYSTEM REGISTERING: FIRST COME FIRST SERVED ---*/

    /* -- Animating and sliding systems before drawing --  */
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Scene,       1, Scene,       Position, Text, Timer);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Cutscene,    1, Cutscene,    Position, Text, Timer);

    // TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_Sprite,   0, Sprite, Position, Slider);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_PopUp_Offscreen, 1, PopUp, Slider,
    //                             SliderOffscreen, Position);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Hover_Any,      0, Hover,  Position);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Sprite, 0, Sprite, Position, Timer);
    // // Remove animated flag. Animated sprites must have a timer! Still sprites dont!

    // /* -- Scrolling Text -- */
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Scroll_Text,    0, PixelFont, Timer);

    // /* -- Drawing -- */
    // /* - for now only drawn sprites are map_units - */
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer, MapHPBar);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer, MapHPBar, AI);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
    //                             Boss);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, MapHPBar, AI);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, MapHPBar, AI, Boss);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
    //                             UnitMoveAnimation);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
    //                             UnitMoveAnimation, Boss);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite, MapHPBar);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   1, Unit, Position, Sprite);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,   0, Unit, Position, Sprite, RenderTop);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Boss_Icon,   0, Unit, Position, Sprite, Boss);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_HPBar, 0, Unit, Position, MapHPBar);

    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Menu,     0, Menu);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Text,     1, Text, Position);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Text_Timer, 0, Text, Position, Timer);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_PopUp,     0, PopUp, Position);
    // /* - draw Cursor and Mouse last -> on top - */
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,    0, Sprite,          Position, CursorFlag);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Sprite,     1, controllerMouse, Position, Sprite,
    //                             MouseFlag);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Combat_onMap, 1, CombatAnimation, Timer);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Map_Animation, 1, MapAnimation, Position, Text, Timer);
    // TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Unit_Move_onMap, 0, UnitMoveAnimation, Position, Timer,
    //                             Unit);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration DONE\n");

    return (0);
}

void Game_Startup_Map(Game *IES) {
    // sota->settings.args.map_index
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

    IES->narrative.scene      = TNECS_ENTITY_CREATE_wCOMPONENTS(IES->ecs.world, Scene_ID);
    Scene *scene    = IES_GET_COMPONENT(IES->ecs.world, IES->narrative.scene, Scene);
    *scene = Scene_default;
    Scene_Init(scene, IES->ecs.world);
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

void Game_Save_Delete( i16 save_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));
    SDL_LogDebug(SOTA_LOG_APP, "Deleting Game: %s\n", filename.data);
    PHYSFS_delete(filename.data);
    s8_free(&filename);
}

void _Game_loadJSON(struct Game *sota, s8  filename) {
    cJSON *json = jsonio_parseJSON(filename);

    /* --- Narrative --- */
    // readJSON_narrative(json, &sota->narrative);

    /* --- Camp --- */
    // cJSON * jcamp = cJSON_GetObjectItem(json, "Camp");
    // Camp_readJSON(&sota->camp, jcamp);

    /* --- RNG --- */
    cJSON *jRNG = cJSON_GetObjectItem(json, "RNG");
    RNG_readJSON(sota->RNG.s_xoshiro256ss, jRNG);
    RNG_Set_xoroshiro256ss(sota->RNG.s_xoshiro256ss[0],
                           sota->RNG.s_xoshiro256ss[1],
                           sota->RNG.s_xoshiro256ss[2],
                           sota->RNG.s_xoshiro256ss[3]);

    /* --- Convoy --- */
    // cJSON *jconvoy = cJSON_GetObjectItem(json, "Convoy");
    // Convoy_Clear(&sota->convoy);
    // Convoy_readJSON(&sota->convoy, jconvoy);
    //     convoy.setWeapons(&weapons);

    /* --- Chapter --- */
    cJSON *jchapter = cJSON_GetObjectItem(json, "Chapter");
    sota->state.chapter = cJSON_GetNumberValue(jchapter);

    /* --- Party --- */
    cJSON *jparty = cJSON_GetObjectItem(json, "Party");

    /* - Party filename may or may not be the current file - */
    cJSON   *jparty_filename    = cJSON_GetObjectItem(json, "filename");
    sota->party.filename = s8_mut(cJSON_GetStringValue(jparty_filename));

    /* - SDL_free - */
    cJSON_Delete(json);
}

void _Game_saveJSON(struct Game *sota, s8  filename) {
    /* --- Open file --- */
    PHYSFS_delete(filename.data);
    PHYSFS_file *fp = PHYSFS_openWrite(filename.data);
    SDL_assert(fp);

    /* --- Create json object file --- */
    cJSON *json         = cJSON_CreateObject();

    /* --- Narrative --- */
    // writeJSON_narrative(json, &sota->narrative);

    /* --- Camp --- */
    // cJSON * jcamp = cJSON_CreateObject();
    // Camp_writeJSON(&sota->camp, jcamp);

    /* --- Chapter --- */
    cJSON *jchapter     = cJSON_CreateNumber(sota->state.chapter);

    /* --- RNG --- */
    cJSON *jRNG         = cJSON_CreateArray();
    RNG_Get_xoroshiro256ss(sota->RNG.s_xoshiro256ss);
    RNG_writeJSON(sota->RNG.s_xoshiro256ss, jRNG);

    /* --- Convoy --- */
    cJSON *jconvoy      = cJSON_CreateObject();
    Convoy_writeJSON(&sota->convoy, jconvoy);

    /* --- Party --- */
    cJSON *jparty       = cJSON_CreateObject();
    Party_writeJSON(&sota->party, jparty);

    /* --- Adding to parent JSON object --- */
    cJSON_AddItemToObject(json, "RNG",          jRNG);
    cJSON_AddItemToObject(json, "Chapter",      jchapter);
    cJSON_AddItemToObject(json, "Party",        jparty);
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
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));
    return (filename);
}

void Game_Save_Load(struct Game *sota, i32 save_ind) {
    /* Checking save folder */
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));

    s8 filename = Savefile_Path(save_ind);

    /* Reading JSON save file */
    Party_Init(&sota->party);
    Party_Free(&sota->party);
    _Game_loadJSON(sota, filename);

    /* -- Loading map taken from savefile -- */
    Game_Map_Load(sota, sota->state.chapter);

    /* -- Loading party taken from savefile -- */
    /* - Reading party json - */
    SDL_assert(sota->party.filename.data != NULL);
    SDL_Log("party_filename %s", sota->party.filename.data);

    /* party filename should include folder */
    // TODO: remove party.folder
    Party_Folder(&sota->party, "");
    jsonio_readJSON(sota->party.filename, &sota->party);

    /* - Loading party units json - */
    Party_Load(&sota->party, sota);
    Party_Size(&sota->party);
    SDL_assert(sota->party.size > 0);

    s8_free(&filename);
}

void Game_Save(struct Game *sota, i32 save_ind) {
    /* Making save folder if it doesn't exist */
    if (!PHYSFS_exists(SAVE_FOLDER))
        PHYSFS_mkdir(SAVE_FOLDER);

    s8 filename = Savefile_Path(save_ind);

    /* Actual saving game to JSON file */
    _Game_saveJSON(sota, filename);

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

void Game_subState_Set(struct Game *sota,  i8 new_substate,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Substate set to %d because: %s", new_substate, reason);
    SDL_assert(new_substate > 0);
    if (Game_Substate_Current(sota) == new_substate) {
        SDL_assert(false);
        return;
    }

    sota->state.sub.previous = Game_Substate_Current(sota);
    sota->state.sub.current  = new_substate;
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game substate changed %d->%d: %s->%s",
                 Game_Substate_Previous(sota), Game_Substate_Current(sota),
                 gamesubStatenames[Game_Substate_Previous(sota)].data,
                 gamesubStatenames[Game_Substate_Current(sota)].data);
    if (new_substate == GAME_SUBSTATE_STANDBY)
        sota->cursor.diagonal = true;
    else
        sota->cursor.diagonal = false;

    if (fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)](sota);
}

void Game_State_Set(struct Game *sota,  i8 new_state,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "State set to %d, because: %s", new_state, reason);
    SDL_assert(new_state > 0);
    SDL_assert(Game_State_Current(sota) != new_state);
    sota->state.top.previous = Game_State_Current(sota);
    sota->state.top.current  = new_state;

    /* --- Set contextual inputs --- */
    if (fsm_Input_s[Game_State_Current(sota)] != NULL)
        fsm_Input_s[Game_State_Current(sota)](sota);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game state changed %d->%d: %s->%s",
                 Game_State_Previous(sota), Game_State_Current(sota),
                 gameStatenames[Game_State_Previous(sota)].data, gameStatenames[Game_State_Current(sota)].data);
}

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota) {
    struct Position *cursor_position;
    cursor_position = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
    SDL_assert(cursor_position != NULL);
    if (!cursor_position->onTilemap)
        return;

    struct Sprite *cursor_sprite;
    cursor_sprite = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Sprite);
    SDL_assert(cursor_sprite != NULL);

    float factor_max = (CAMERA_BOUNDS - CAMERA_BOUNDS_SCALE * sota->camera.zoom /
                        MAX_CAMERA_ZOOM);
    float factor_min = 1.0f - factor_max;

    int x = cursor_sprite->dstrect.x, y = cursor_sprite->dstrect.y;
    int w = cursor_sprite->dstrect.w, h = cursor_sprite->dstrect.h;
    int offset = CAMERA_SCROLL_SPEED * sota->camera.zoom / CAMERA_SCROLL_ZOOMFACTOR;

    if ((x + w / 2) >= (factor_max * sota->settings.res.x)) {
        sota->camera.offset.x   -= offset;
        sota->map->flags.camera_moved = true;
    } else if ((x + w / 2) <= (factor_min * sota->settings.res.x)) {
        sota->camera.offset.x   += offset;
        sota->map->flags.camera_moved = true;
    }
    if ((y + h / 2) >= (factor_max * sota->settings.res.y)) {
        sota->camera.offset.y   -= offset;
        sota->map->flags.camera_moved = true;
    } else if ((y + h / 2) <= (factor_min * sota->settings.res.y)) {
        sota->camera.offset.y   += offset;
        sota->map->flags.camera_moved = true;
    }
}

/* --- Time --- */
void Game_Delay(struct Game *sota, i64 delay_ms, u64 currentTime_ns,
                u64 elapsedTime_ns) {
    SDL_assert(sota != NULL);

    /* - Skip if negative delay - */
    if (delay_ms < 0) {
        return;
    }

    /* - Delay game in case synchronization took > 1ms - */
    u64 new_elapsedTime_ns = tnecs_get_ns() - currentTime_ns;

    u32 delay   = 0;
    u64 elapsed = (new_elapsedTime_ns - elapsedTime_ns) / 1000000;
    if (delay_ms > elapsed)
        delay = delay_ms - elapsed;

    if (delay > 0)
        SDL_Delay(delay);

    sota->timers.runtime_ns += new_elapsedTime_ns + (delay > 0) * delay * SOTA_us;

}

/* --- FPS --- */
i64 Game_FPS_Delay(struct Game *sota, u64 elapsedTime_ns) {
    i64 delay       = 0;
    int fps_cap     = sota->settings.FPS.cap;    /* [s^-1] */
    int ff_cap      = sota->settings.FPS.ff_cap; /* [%]    */

    /* 0 frame delay if fast-forwarding (ff) without limit */
    if ((sota->flags.fast_forward) && (ff_cap <= SOTA_100PERCENT)) {
        return (delay);
    }

    /* Compute delay for ff_cap */
    if ((sota->flags.fast_forward) && (ff_cap > SOTA_100PERCENT)) {
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

void Game_FPS_Create(struct Game *sota, i64 in_update_time_ns) {
    if (sota->fps.entity != 0)
        tnecs_entity_destroy(sota->ecs.world, sota->fps.entity);
    sota->fps.entity = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Position_ID, Text_ID, Timer_ID);

    /* -- Get timer -- */
    struct Timer *timer;
    timer  = IES_GET_COMPONENT(sota->ecs.world, sota->fps.entity, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    /* -- Get position -- */
    struct Position *position;
    position = IES_GET_COMPONENT(sota->ecs.world, sota->fps.entity, Position);
    *position = Position_default;

    SDL_assert(position != NULL);
    position->onTilemap = false;
    Position_Bounds_Set(position, 0, sota->settings.res.x, 0, sota->settings.res.y);
    i16 in_x = sota->settings.res.x * 0.88f;
    i16 in_y = sota->settings.res.y * 0.02f;
    Position_Pos_Set(position, in_x, in_y);
    position->scale[0] = FPS_SCALE;
    position->scale[1] = FPS_SCALE;

    /* -- Get Text -- */
    struct Text *text = IES_GET_COMPONENT(sota->ecs.world, sota->fps.entity, Text);
    *text = Text_default;
    SDL_assert(text != NULL);
    text->pixelfont         = sota->fonts.pixelnours_big;
    text->onUpdate          = &Text_onUpdate_FPS;
    text->update_time_ns    = in_update_time_ns;
    Text_Set(text, "60.1", PIXELNOURS_BIG_Y_OFFSET);

}

/* --- SETTINGS --- */
void Game_Brightness_Set(struct Game *sota, float bright) {
    bright = bright < SOTA_BRIGHTNESS_MIN ? SOTA_BRIGHTNESS_MIN : bright;
    bright = bright > SOTA_BRIGHTNESS_MAX ? SOTA_BRIGHTNESS_MAX : bright;
    SDL_SetWindowBrightness(sota->render.window, bright);
}

float Game_Brightness_Get(struct Game *sota) {
    return (SDL_GetWindowBrightness(sota->render.window));
}

/* --- DISPLAY --- */
void Game_Display_Bounds(struct Game *sota) {
    // Find bounds of display.

    // Skip if fullscreen
    if (sota->settings.fullscreen)
        return;

    SDL_Rect bounds;
    if (SDL_GetDisplayBounds(0, &bounds) != 0) {
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL could not get display bounds! SDL Error: %s\n",
                        SDL_GetError() );
        exit(1);
    }

    // Clamp resolution to display if window is bigger
    if (sota->settings.res.x > bounds.w)
        sota->settings.res.x = bounds.w;

    if (sota->settings.res.y > bounds.h)
        sota->settings.res.y = bounds.h;
}


/* --- AUDIO --- */

/* -- Music -- */

void Game_Music_Play(struct Game *sota) {
    if (sota->audio.music == NULL) {
        SDL_LogWarn(SOTA_LOG_AUDIO, "Sota has no song to play.");
        return;
    }

#ifndef DEBUG_NO_MUSIC
    if (!Mix_PlayingMusic())
        Mix_FadeInMusic(sota->audio.music, -1, SOTA_MUSIC_FADEIN_ms);
    else if (Mix_PausedMusic())
        Mix_ResumeMusic();
#endif /* DEBUG_NO_MUSIC */
}

void Game_Music_Stop(struct Game *sota) {
    Mix_FadeOutMusic(SOTA_MUSIC_FADEOUT_ms);
}

void Game_Music_Pause(struct Game *sota) {
    Mix_PauseMusic();
}

/* -- Volume -- */
void Game_Volume_Set(struct Game *sota, int volume) {
    Game_Volume_Music_Set(  sota, volume);
    Game_Volume_SoundFX_Set(sota, volume);
}

void Game_Volume_Music_Set(struct Game *sota, int volume) {
    volume = volume <        0       ?        0       : volume;
    volume = volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume;
    Mix_VolumeMusic(volume);
}

void Game_Volume_SoundFX_Set(struct Game *sota, int volume) {
    volume = volume <        0       ?        0       : volume;
    volume = volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume;
    Mix_MasterVolume(volume); /* - Only for chunks - */
}

int Game_Volume_Music_Get(  struct Game *sota) {
    return (Mix_VolumeMusic(-1));
}

int Game_Volume_SoundFX_Get(struct Game *sota) {
    return (Mix_MasterVolume(-1));
}

/* -- Battle -- */
void  Game_Battle_Start(struct Game *sota, struct Menu *mc) {
    /* -- Place units in deployment slots on map -- */
    struct DeploymentMenu *dm = mc->data;
    if (dm->_selected_num == 0) {
        return;
    }

    /* -- Game substate to on Map -- */
    strncpy(sota->debug.reason, "Preparation done. March!", sizeof(sota->debug.reason));
    if (Game_State_Current(sota) != GAME_STATE_Gameplay_Map)
        Game_State_Set(sota, GAME_STATE_Gameplay_Map, sota->debug.reason);
    if (Game_Substate_Current(sota) != GAME_SUBSTATE_STANDBY)
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->debug.reason);

    /* -- Load map -- */
    Game_PopUp_Tile_Create(sota);
    Game_PopUp_Unit_Create(sota);
    Game_cursorFocus_onMap(sota);

    /* -- Destroy Deployment Menu -- */
    b32 destroy = true;
    u64 menu_num = DARR_NUM(sota->menus.stack);
    if (sota->menus.stack[menu_num - 1] == sota->menus.deployment) {
        Game_menuStack_Pop(sota, destroy);
    }

    /* -- Disable palette map during turn transition -- */
    Map_Palettemap_Autoset(sota->map, 0, TNECS_NULL);

    /* -- Set cursor position to first starting position -- */
    SDL_assert(sota                     != NULL);
    SDL_assert(sota->cursor.entity      != TNECS_NULL);
    struct Position *position = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
    SDL_assert(position                 != NULL);
    SDL_assert(sota->map                != NULL);
    SDL_assert(sota->map->start_pos.arr     != NULL);
    SDL_assert(DARR_NUM(sota->map->start_pos.arr) > 0);
    Position_Pos_Set(position, sota->map->start_pos.arr[0].x, sota->map->start_pos.arr[0].y);

    // Game_cursorFocus_onMap resets position to cursor_lastpos;
    // TODO: bool flag to disable Game_cursorFocus_onMap resetting cursor pos.
    sota->cursor.lastpos.x = sota->map->start_pos.arr[0].x;
    sota->cursor.lastpos.y = sota->map->start_pos.arr[0].y;

    /* -- Set popups position -- */
    Position *cursor_pos;
    cursor_pos = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
    Point *pos = &cursor_pos->tilemap_pos;

    /* -- Set popup_unit position -- */

    tnecs_entity popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    SDL_assert(popup_ent != TNECS_NULL);

    Slider          *popup_unit_slider;
    Position        *popup_unit_pos;
    SliderOffscreen *popup_unit_offscreen;
    popup_unit_slider = IES_GET_COMPONENT(sota->ecs.world, popup_ent, Slider);
    popup_unit_pos = IES_GET_COMPONENT(sota->ecs.world, popup_ent, Position);
    popup_unit_offscreen = IES_GET_COMPONENT(sota->ecs.world, popup_ent, SliderOffscreen);
    SDL_assert(popup_unit_slider != TNECS_NULL);
    SDL_assert(popup_unit_pos != TNECS_NULL);
    SDL_assert(popup_unit_offscreen != TNECS_NULL);

    Game_PopUp_Unit_Place(sota, *pos);

    popup_unit_pos->pixel_pos = popup_unit_offscreen->midpoint;

    /* -- Set popup_tile position -- */
    popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    SDL_assert(popup_ent != TNECS_NULL);

    Slider          *popup_tile_slider;
    Position        *popup_tile_pos;
    SliderOffscreen *popup_tile_offscreen;
    popup_tile_slider = IES_GET_COMPONENT(sota->ecs.world, popup_ent, Slider);
    popup_tile_pos = IES_GET_COMPONENT(sota->ecs.world, popup_ent, Position);
    popup_tile_offscreen = IES_GET_COMPONENT(sota->ecs.world, popup_ent, SliderOffscreen);
    SDL_assert(popup_tile_slider != TNECS_NULL);
    SDL_assert(popup_tile_pos != TNECS_NULL);
    SDL_assert(popup_tile_offscreen != TNECS_NULL);

    Game_PopUp_Tile_Place(sota, *pos);

    popup_tile_pos->pixel_pos.x = popup_tile_offscreen->midpoint.x;
    popup_tile_pos->pixel_pos.y = popup_tile_slider->target.y;

    /* -- Start turn transition -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}
