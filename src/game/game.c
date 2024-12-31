
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
#include "fps_fsm.h"
#include "names.h"
#include "text.h"
#include "convoy.h"
#include "utilities.h"
#include "stb_sprintf.h"

const struct Game Game_default = {
    .cursor_lastpos         = {1, 1},
    .moved_direction        = SOTA_DIRECTION_NULL,
    .iscursor               = true,

    .camera = {
        .zoom = DEFAULT_CAMERA_ZOOM,
        .offset = {.x = DEFAULT_CAMERA_XOFFSET, .y = DEFAULT_CAMERA_YOFFSET}
    },

    .selected_unit_moved_position   = {-1, -1},
    .selected_unit_initial_position = {-1, -1},
    // .convoy = Convoy_default,
    // .camp = Camp_default,

    .chapter                = -1,
    .state                  = GAME_STATE_Title_Screen,
    .substate               = GAME_SUBSTATE_MENU,
    .state_previous         = GAME_STATE_START,
    .substate_previous      = GAME_SUBSTATE_START,
};

/* --- Constructors/Destructors --- */

void Game_Free(struct Game *sota) {
    AI_State_Free(&sota->ai_state);
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
    if (sota->combat_outcome.attacks != NULL) {
        DARR_FREE(sota->combat_outcome.attacks);
        sota->combat_outcome.attacks = NULL;
    }

    if (sota->map_enemies != NULL) {
        DARR_FREE(sota->map_enemies);
        sota->map_enemies = NULL;
    }

    PixelFont_Free(sota->pixelnours,       true);
    PixelFont_Free(sota->pixelnours_big,   true);
    PixelFont_Free(sota->pixelnours_tight, true);

    if (sota->stats_menu > TNECS_NULL) {
        struct Menu *mc;
        mc = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, Menu);
        if (mc->data != NULL) {
            struct StatsMenu *stats_menu = mc->data;
            if (mc->n9patch.texture != NULL)
                SDL_DestroyTexture(mc->n9patch.texture);
            StatsMenu_Free(stats_menu);
            mc->data = NULL;
        }
    }

    if (sota->item_select_menu > TNECS_NULL) {
        struct Menu *mc;
        mc = TNECS_GET_COMPONENT(sota->world, sota->item_select_menu, Menu);
        if (mc->data != NULL) {
            struct LoadoutSelectMenu *ism = mc->data;
            LoadoutSelectMenu_Free(ism);
            SDL_free(ism);
            mc->data = NULL;
        }
    }

    if (sota->renderer) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Control");
        // controlSDL_free();
    }
    if (sota->soundfx_cursor != NULL) {
        Mix_FreeChunk(sota->soundfx_cursor);
        sota->soundfx_cursor = NULL;
    }
    if (sota->soundfx_next_turn != NULL) {
        Mix_FreeChunk(sota->soundfx_next_turn);
        sota->soundfx_next_turn = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Map");

    if ((sota->map != NULL) && (sota->map->unitmap != NULL))
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
    if (sota->render_target != NULL)
        SDL_DestroyTexture(sota->render_target);
    if (sota->renderer != NULL) {
        SDL_DestroyRenderer(sota->renderer); /* SDL_free renderer before window */
    }
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free Window");
    if (sota->window != NULL)
        SDL_DestroyWindow(sota->window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    Mix_Quit();
    SDL_Quit();
#else /* SOTA_OPENGL */
#endif /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "SDL_free tnecs world");

    if (sota->world != NULL) {
        tnecs_world_destroy(sota->world);
        sota->world = NULL;
    }
    if (sota->world_render != NULL) {
        tnecs_world_destroy(sota->world_render);
        sota->world_render = NULL;
    }
    if (sota->world_control != NULL) {
        tnecs_world_destroy(sota->world_control);
        sota->world_control = NULL;
    }

    Game_Items_Free(&sota->items_dtab);
    Game_Weapons_Free(&sota->weapons_dtab);
    if (sota->menu_pixelfont != NULL) {
        PixelFont_Free(sota->menu_pixelfont, false);
    }
    if (sota->defendants != NULL) {
        DARR_FREE(sota->defendants);
        sota->defendants = NULL;
    }
    if (sota->patients != NULL) {
        DARR_FREE(sota->patients);
        sota->patients = NULL;
    }
    if (sota->victims != NULL) {
        DARR_FREE(sota->victims);
        sota->victims = NULL;
    }
    if (sota->deployed != NULL) {
        DARR_FREE(sota->deployed);
        sota->deployed = NULL;
    }
    if (sota->spectators != NULL) {
        DARR_FREE(sota->spectators);
        sota->spectators = NULL;
    }
    if (sota->auditors != NULL) {
        DARR_FREE(sota->auditors);
        sota->auditors = NULL;
    }
    if (sota->passives != NULL) {
        DARR_FREE(sota->passives);
        sota->passives = NULL;
    }
    if (sota->openables != NULL) {
        DARR_FREE(sota->openables);
        sota->openables = NULL;
    }

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Game cleaned.");
    SDL_free(sota);
}

void Game_AI_Free(struct Game *sota) {
    if (sota->ai_state.npcs != NULL) {
        DARR_FREE(sota->ai_state.npcs);
        sota->ai_state.npcs = NULL;
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
    sota->cursor_frame_moved = false;
    SDL_RenderClear(sota->renderer); /* RENDER clears the backbuffer */
    return (currentTime_ns);
}

void _Game_Step_Control(struct Game *sota) {
    /* --- Control: Get player inputs --- */
    /* -- player inputs -- */
    Game_Control_Gamepad( sota);
    Game_Control_Keyboard(sota);
    Game_Control_Touchpad(sota);

    /* -- fps_fsm -- */
    SDL_assert(fsm_cFrame_s[sota->state] != NULL);
    fsm_cFrame_s[sota->state](sota); /* CONTROL */
}

void _Game_Step_Render(struct Game *sota) {
    /* Render FSM */
    SDL_assert(fsm_rFrame_s[sota->state] != NULL);
    fsm_rFrame_s[sota->state](sota); /* RENDER */
    u64 updateTime_ns = SOTA_ns / sota->settings.FPS.cap;
    tnecs_world_step_wdata(sota->world, updateTime_ns, sota); /* RENDER */

    /* -- Render to screen -- */
#ifndef RENDER2WINDOW
    SDL_SetRenderTarget(sota->renderer, NULL); /* RENDER */
    SDL_RenderCopy(     sota->renderer, sota->render_target, NULL, NULL);
    SDL_SetRenderTarget(sota->renderer, sota->render_target);
#endif
    SDL_RenderPresent(sota->renderer);
}

void _Game_Step_PostFrame(struct Game *sota, u64 currentTime_ns) {
    /* -- Synchronize timers -- */
    u64 elapsedTime_ns = tnecs_get_ns() - currentTime_ns;
    i64 delay_ms       = Game_FPS_Delay(sota, elapsedTime_ns);
    tnecs_ns time_ns   = (elapsedTime_ns + delay_ms * SOTA_ns / SOTA_ms);

    Game_Cursor_movedTime_Compute(sota, time_ns);
    // SDL_Log("sota->cursor_moved_time_ms %d\n", sota->cursor_moved_time_ms);
    tnecs_custom_system_run(sota->world, Time_Synchronize,
                            sota->timer_typeflag, time_ns, NULL);

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
    sota->filename_menu = s8_literal(PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png"));

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

    sota->window = SDL_CreateWindow(sota->settings.title, sota->settings.pos.x,
                                    sota->settings.pos.y, sota->settings.res.x, sota->settings.res.y, flags);
    SDL_assert(sota->window);
    // SDL_assert(SDL_ISPIXELFORMAT_INDEXED(SDL_GetWindowPixelFormat(sota->window)));
    int window_w;
    int window_h;
    SDL_GetWindowSize(sota->window, &window_w, &window_h);
    SDL_assert(window_w == sota->settings.res.x);
    SDL_assert(window_h == sota->settings.res.y);
    if (sota->window) {
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "Window created\n");
        sota->renderer = SDL_CreateRenderer(sota->window, -1, SDL_RENDERER_TARGETTEXTURE);
        SDL_assert(sota->renderer);
        if (sota->renderer) {
            Utilities_DrawColor_Reset(sota->renderer);
            SDL_LogVerbose(SOTA_LOG_SYSTEM, "Renderer created\n");
        }
    }
#ifndef RENDER2WINDOW
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Rendering to sota->render_target\n");
    SDL_assert(sota->settings.res.x > 0);
    SDL_assert(sota->settings.res.y > 0);
    SDL_assert(sota->renderer != NULL);
    sota->render_target = SDL_CreateTexture(sota->renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, sota->settings.res.x, sota->settings.res.y);
    SDL_assert(sota->render_target != NULL);
    SDL_SetTextureBlendMode(sota->render_target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(sota->renderer, sota->render_target);
    SDL_assert(sota->render_target != NULL);
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

    sota->keyboardInputMap  = KeyboardInputMap_default;
    sota->gamepadInputMap   = GamepadInputMap_switch_pro;

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Loading pixelfonts\n");
    sota->pixelnours = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");
    PixelFont_Load(sota->pixelnours, sota->renderer, path);
    sota->pixelnours->y_offset = pixelfont_y_offset;

    sota->pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(sota->pixelnours_big, sota->renderer, path);
    sota->pixelnours_big->y_offset = pixelfont_big_y_offset;

    sota->pixelnours_tight = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_tight.png");
    PixelFont_Load(sota->pixelnours_tight, sota->renderer, path);

    /* Sprite init */
    b32 absolute = false;
    b32 isCursor = false;
    dstrect_funcs[absolute][isCursor = true]        = &Cursor_Dstrect_Relative;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Relative;
    dstrect_funcs[absolute = true][isCursor = true] = &Cursor_Dstrect_Absolute;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Absolute;

    /* --- Soundfx --- */
    sota->soundfx_cursor    = Soundfx_Load_Cursor();
    sota->soundfx_next_turn = Soundfx_Load_Next_Turn();

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
    fsm_Input_s[sota->state](sota);

    /* -- Set color -- */
    Utilities_DrawColor_Reset(sota->renderer);

    /* -- Checks -- */
    SDL_assert(sota->entity_mouse);

    sota->isrunning = true;
    return (sota);
}

int _Game_New_Events(void *data) {
    Game *IES = data;

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
    DTAB_INIT(IES->weapons_dtab, struct Weapon);
    DTAB_INIT(IES->items_dtab, struct Item);

    /* -- Alloc arrays of entities -- */
    IES->defendants    = DARR_INIT(IES->defendants,   tnecs_entity,  4);
    IES->patients      = DARR_INIT(IES->patients,     tnecs_entity,  4);
    IES->victims       = DARR_INIT(IES->victims,      tnecs_entity,  4);
    IES->deployed      = DARR_INIT(IES->deployed,     tnecs_entity,  4);
    IES->spectators    = DARR_INIT(IES->spectators,   tnecs_entity,  4);
    IES->auditors      = DARR_INIT(IES->auditors,     tnecs_entity,  4);
    IES->passives      = DARR_INIT(IES->passives,     tnecs_entity,  4);
    IES->openables     = DARR_INIT(IES->openables,    tnecs_entity,  4);
    IES->map_enemies   = DARR_INIT(IES->map_enemies,  tnecs_entity, 16);

    /* -- Alloc combat arrays -- */
    IES->combat_outcome.attacks = DARR_INIT(IES->combat_outcome.attacks, struct Combat_Attack,
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
    IES->world = tnecs_world_genesis();

    // Don't reuse entities.
    // If I forget to update an entity somewhere, it'll be invalid for sure.
    SDL_assert(IES->world->reuse_entities == false);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Components Registration\n");
    TNECS_REGISTER_COMPONENT(IES->world, Position);
    TNECS_REGISTER_COMPONENT(IES->world, Sprite);
    TNECS_REGISTER_COMPONENT(IES->world, Unit);
    TNECS_REGISTER_COMPONENT(IES->world, Boss);
    TNECS_REGISTER_COMPONENT(IES->world, Menu);
    TNECS_REGISTER_COMPONENT(IES->world, controllerGamepad);
    TNECS_REGISTER_COMPONENT(IES->world, controllerMouse);
    TNECS_REGISTER_COMPONENT(IES->world, controllerKeyboard);
    TNECS_REGISTER_COMPONENT(IES->world, controllerTouchpad);
    TNECS_REGISTER_COMPONENT(IES->world, Timer);
    TNECS_REGISTER_COMPONENT(IES->world, PopUp);
    TNECS_REGISTER_COMPONENT(IES->world, Slider);
    TNECS_REGISTER_COMPONENT(IES->world, Hover);
    TNECS_REGISTER_COMPONENT(IES->world, SliderOffscreen);
    TNECS_REGISTER_COMPONENT(IES->world, CursorFlag);
    TNECS_REGISTER_COMPONENT(IES->world, MouseFlag);
    TNECS_REGISTER_COMPONENT(IES->world, Text);
    TNECS_REGISTER_COMPONENT(IES->world, Breakable);
    TNECS_REGISTER_COMPONENT(IES->world, Door);
    TNECS_REGISTER_COMPONENT(IES->world, Chest);
    TNECS_REGISTER_COMPONENT(IES->world, Mobj_Link);
    TNECS_REGISTER_COMPONENT(IES->world, MapHPBar);
    TNECS_REGISTER_COMPONENT(IES->world, CombatAnimation);
    TNECS_REGISTER_COMPONENT(IES->world, MapAnimation);
    TNECS_REGISTER_COMPONENT(IES->world, UnitMoveAnimation);
    TNECS_REGISTER_COMPONENT(IES->world, RenderTop);
    TNECS_REGISTER_COMPONENT(IES->world, PixelFont);
    TNECS_REGISTER_COMPONENT(IES->world, AI);
    TNECS_REGISTER_COMPONENT(IES->world, Scene);
    TNECS_REGISTER_COMPONENT(IES->world, Cutscene);
    IES->timer_typeflag = TNECS_COMPONENT_NAME2TYPE(IES->world, Timer);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration\n");
    tnecs_world *world = IES->world;
    /* --- SYSTEM REGISTERING: FIRST COME FIRST SERVED ---*/

    /* -- Animating and sliding systems before drawing --  */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Scene,       1, Scene,       Position, Text, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Cutscene,    1, Cutscene,    Position, Text, Timer);

    TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_Sprite,   0, Sprite, Position, Slider);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_PopUp_Offscreen, 1, PopUp, Slider,
                                SliderOffscreen, Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Hover_Any,      0, Hover,  Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Sprite, 0, Sprite, Position, Timer);
    // Remove animated flag. Animated sprites must have a timer! Still sprites dont!

    /* -- Scrolling Text -- */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Scroll_Text,    0, PixelFont, Timer);

    /* -- Drawing -- */
    /* - for now only drawn sprites are map_units - */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer, MapHPBar);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer, MapHPBar, AI);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
                                Boss);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, MapHPBar, AI);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, MapHPBar, AI, Boss);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
                                UnitMoveAnimation);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, Timer, MapHPBar, AI,
                                UnitMoveAnimation, Boss);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite, MapHPBar);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   1, Unit, Position, Sprite);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Unit,   0, Unit, Position, Sprite, RenderTop);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_Boss_Icon,   0, Unit, Position, Sprite, Boss);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Map_HPBar, 0, Unit, Position, MapHPBar);

    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Menu,     0, Menu);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Text,     1, Text, Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Text_Timer, 0, Text, Position, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_PopUp,     0, PopUp, Position);
    /* - draw Cursor and Mouse last -> on top - */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Cursor,    0, Sprite,          Position, CursorFlag);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Draw_Mouse,     1, controllerMouse, Position, Sprite, MouseFlag);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Combat_onMap, 1, CombatAnimation, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Map_Animation, 1, MapAnimation, Position, Text, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Unit_Move_onMap, 0, UnitMoveAnimation, Position, Timer,
                                Unit);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration DONE\n");

    return (0);
}

void Game_Startup_Map(Game *IES) {
    // sota->settings.args.map_index
    SDL_Log("IMPLEMENT ME");
    exit(1);

    IES->state      = GAME_STATE_Gameplay_Map;
    IES->substate   = GAME_SUBSTATE_MENU;
}

void Game_Startup_Scene(Game *IES) {
    SDL_assert(IES              != NULL);
    SDL_assert(IES->pixelnours  != NULL);
    /* --- Get scene JSON file from filename --- */
    // TODO: differentiate between cutscene and talk scene
    s8 filename = Scene_Filename(IES->settings.args.scene);
    // SDL_Log("Loading Scene '%s'", filename.data);

    IES->scene      = TNECS_ENTITY_CREATE_wCOMPONENTS(IES->world, Scene);
    Scene *scene    = TNECS_GET_COMPONENT(IES->world, IES->scene, Scene);
    *scene = Scene_default;
    // TODO: Remove quit event on scene finish
    scene->event = event_Quit;

    scene->pixelnours = IES->pixelnours;
    Scene_Text_Box_Init(scene, IES->renderer);
    Scene_Texture_Create(scene, IES->renderer);

    jsonio_readJSON(filename, scene);
    IES->substate   = GAME_SUBSTATE_STANDBY;
    IES->state      = GAME_STATE_Scene_Talk;
    SDL_Log("POST jsonio_readJSON");
    Scene_Statement_Next(scene);
    SDL_Log("POST Scene_Statement_Next %d", scene->current_statement);
}

void Game_Startup_TitleScreen(Game *IES) {
    Game_titleScreen_Load(IES, IES->settings.args);
    Game_Mouse_Disable(IES);
    Game_cursorFocus_onMenu(IES);

    /* -- Checks -- */
    IES->state      = GAME_STATE_Title_Screen;
    IES->substate   = GAME_SUBSTATE_MENU;
}

void Game_Save_Copy(i16 from_ind,  i16 to_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    s8 filenameto       = s8_mut(SAVE_FOLDER);
    s8 filenamefrom     = s8_mut(SAVE_FOLDER);
    char tempto[DEFAULT_BUFFER_SIZE];
    char tempfrom[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(tempto,   DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", to_ind);
    stbsp_snprintf(tempfrom, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", from_ind);
    filenameto      = s8cat(filenameto,     s8_var(tempto));
    filenamefrom    = s8cat(filenamefrom,   s8_var(tempfrom));
    SDL_LogDebug(SOTA_LOG_APP, "copy saveJSON Game from %s to %s\n", filenamefrom, filenameto);
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
    RNG_readJSON(sota->s_xoshiro256ss, jRNG);
    RNG_Set_xoroshiro256ss(sota->s_xoshiro256ss[0],
                           sota->s_xoshiro256ss[1],
                           sota->s_xoshiro256ss[2],
                           sota->s_xoshiro256ss[3]);

    /* --- Convoy --- */
    cJSON *jconvoy = cJSON_GetObjectItem(json, "Convoy");
    Convoy_Clear(&sota->convoy);
    Convoy_readJSON(&sota->convoy, jconvoy);
    //     convoy.setWeapons(&weapons);

    /* --- Party --- */
    cJSON *jparty = cJSON_GetObjectItem(json, "Party");

    /* - Party filename may or may not be the current file - */
    cJSON   *jparty_filename    = cJSON_GetObjectItem(json, "filename");
    char    *party_filename     = cJSON_GetStringValue(jparty_filename);
    Party_Free(&sota->party);
    Party_Init(&sota->party);
    if (party_filename == NULL) {
        /* - If no filename in Party, Party is in the current file - */
        Party_readJSON(&sota->party, jparty);
    } else {
        /* - Reading party json - */
        SDL_assert(party_filename != NULL);
        SDL_Log("party_filename %s", party_filename);

        /* party filename should include folder */
        Party_Folder(&sota->party, "");
        jsonio_readJSON(s8_var(party_filename), &sota->party);
    }

    /* - Loading party units json - */
    Party_Load(&sota->party, sota, sota->weapons_dtab, sota->items_dtab);
    Party_Size(&sota->party);
    SDL_assert(sota->party.size > 0);

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

    /* --- RNG --- */
    cJSON *jRNG         = cJSON_CreateArray();
    RNG_Get_xoroshiro256ss(sota->s_xoshiro256ss);
    RNG_writeJSON(sota->s_xoshiro256ss, jRNG);

    /* --- Convoy --- */
    cJSON *jconvoy      = cJSON_CreateObject();
    Convoy_writeJSON(&sota->convoy, jconvoy);

    /* --- Party --- */
    cJSON *jparty       = cJSON_CreateObject();
    Party_writeJSON(&sota->party, jparty);

    /* --- Adding to parent JSON object --- */
    cJSON_AddItemToObject(json, "RNG",          jRNG);
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

void Game_loadJSON(struct Game *sota, i16 save_ind) {
    /* Checking save folder */
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));

    /* Creating load path */
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));

    /* Actual loading JSON save file */
    _Game_loadJSON(sota, filename);
}

void Game_saveJSON(struct Game *sota, i16 save_ind) {
    /* Making save folder if it doesn't exist */
    if (!PHYSFS_exists(SAVE_FOLDER))
        PHYSFS_mkdir(SAVE_FOLDER);

    /* Creating save path */
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));

    /* Actual saving game to JSON file */
    _Game_saveJSON(sota, filename);
}

/* --- State --- */
void Game_subState_Set(struct Game *sota,  i8 new_substate,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Substate set to %d because: %s", new_substate, reason);
    SDL_assert(new_substate > 0);
    SDL_assert(sota->substate != new_substate);
    sota->substate_previous = sota->substate;
    sota->substate = new_substate;
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game substate changed %d->%d: %s->%s",
                 sota->substate_previous, sota->substate,
                 gamesubStatenames[sota->substate_previous].data,
                 gamesubStatenames[sota->substate].data);
    if (new_substate == GAME_SUBSTATE_STANDBY)
        sota->cursor_diagonal = true;
    else
        sota->cursor_diagonal = false;

    if (fsm_Input_sGmpMap_ss[sota->substate] != NULL)
        fsm_Input_sGmpMap_ss[sota->substate](sota);
}

void Game_State_Set(struct Game *sota,  i8 new_state,  char *reason) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "State set to %d, because: %s", new_state, reason);
    SDL_assert(new_state > 0);
    SDL_assert(sota->state != new_state);
    sota->state_previous = sota->state;
    sota->state          = new_state;

    /* --- Set contextual inputs --- */
    if (fsm_Input_s[sota->state] != NULL)
        fsm_Input_s[sota->state](sota);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Game state changed %d->%d: %s->%s",
                 sota->state_previous, sota->state,
                 gameStatenames[sota->state_previous].data, gameStatenames[sota->state].data);
}

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota) {
    struct Position *cursor_position;
    cursor_position = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_position != NULL);
    if (!cursor_position->onTilemap)
        return;

    struct Sprite *cursor_sprite;
    cursor_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(cursor_sprite != NULL);

    float factor_max = (CAMERA_BOUNDS - CAMERA_BOUNDS_SCALE * sota->camera.zoom /
                        MAX_CAMERA_ZOOM);
    float factor_min = 1.0f - factor_max;

    int x = cursor_sprite->dstrect.x, y = cursor_sprite->dstrect.y;
    int w = cursor_sprite->dstrect.w, h = cursor_sprite->dstrect.h;
    int offset = CAMERA_SCROLL_SPEED * sota->camera.zoom / CAMERA_SCROLL_ZOOMFACTOR;

    if ((x + w / 2) >= (factor_max * sota->settings.res.x)) {
        sota->camera.offset.x   -= offset;
        sota->map->camera_moved = true;
    } else if ((x + w / 2) <= (factor_min * sota->settings.res.x)) {
        sota->camera.offset.x   += offset;
        sota->map->camera_moved = true;
    }
    if ((y + h / 2) >= (factor_max * sota->settings.res.y)) {
        sota->camera.offset.y   -= offset;
        sota->map->camera_moved = true;
    } else if ((y + h / 2) <= (factor_min * sota->settings.res.y)) {
        sota->camera.offset.y   += offset;
        sota->map->camera_moved = true;
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

    sota->runtime_ns += new_elapsedTime_ns + (delay > 0) * delay * SOTA_us;

}

/* --- FPS --- */
i64 Game_FPS_Delay(struct Game *sota, u64 elapsedTime_ns) {
    i64 delay       = 0;
    int fps_cap     = sota->settings.FPS.cap;    /* [s^-1] */
    int ff_cap      = sota->settings.FPS.ff_cap; /* [%]    */

    /* 0 frame delay if fast-forwarding (ff) without limit */
    if ((sota->fast_forward) && (ff_cap <= SOTA_100PERCENT)) {
        return (delay);
    }

    /* Compute delay for ff_cap */
    if ((sota->fast_forward) && (ff_cap > SOTA_100PERCENT)) {
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
    if (sota->entity_fps != 0)
        tnecs_entity_destroy(sota->world, sota->entity_fps);
    sota->entity_fps = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position, Text, Timer);

    /* -- Get timer -- */
    struct Timer *timer;
    timer  = TNECS_GET_COMPONENT(sota->world, sota->entity_fps, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    /* -- Get position -- */
    struct Position *position;
    position = TNECS_GET_COMPONENT(sota->world, sota->entity_fps, Position);
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
    struct Text *text = TNECS_GET_COMPONENT(sota->world, sota->entity_fps, Text);
    *text = Text_default;
    SDL_assert(text != NULL);
    text->pixelfont         = sota->pixelnours_big;
    text->onUpdate          = &Text_onUpdate_FPS;
    text->update_time_ns    = in_update_time_ns;
    Text_Set(text, "60.1", PIXELNOURS_BIG_Y_OFFSET);

    SDL_assert(sota->world->entity_typeflags[sota->entity_fps] ==
               TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Timer, Position, Text));
}

/* --- SETTINGS --- */
void Game_Brightness_Set(struct Game *sota, float bright) {
    bright = bright < SOTA_BRIGHTNESS_MIN ? SOTA_BRIGHTNESS_MIN : bright;
    bright = bright > SOTA_BRIGHTNESS_MAX ? SOTA_BRIGHTNESS_MAX : bright;
    SDL_SetWindowBrightness(sota->window, bright);
}

float Game_Brightness_Get(struct Game *sota) {
    return (SDL_GetWindowBrightness(sota->window));
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
    if (sota->music == NULL) {
        SDL_LogWarn(SOTA_LOG_AUDIO, "Sota has no song to play.");
        return;
    }

#ifndef DEBUG_NO_MUSIC
    if (!Mix_PlayingMusic())
        Mix_FadeInMusic(sota->music, -1, SOTA_MUSIC_FADEIN_ms);
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
    strncpy(sota->reason, "Preparation done. March!", sizeof(sota->reason));
    if (sota->state != GAME_STATE_Gameplay_Map)
        Game_State_Set(sota, GAME_STATE_Gameplay_Map, sota->reason);
    if (sota->substate != GAME_SUBSTATE_STANDBY)
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);

    /* -- Load map -- */
    Game_PopUp_Tile_Create(sota);
    Game_cursorFocus_onMap(sota);

    /* -- Destroy Deployment Menu -- */
    b32 destroy = true;
    u64 menu_num = DARR_NUM(sota->menu_stack);
    if (sota->menu_stack[menu_num - 1] == sota->deployment_menu) {
        Game_menuStack_Pop(sota, destroy);
    }

    /* -- Disable palette map during turn transition -- */
    Map_Palettemap_Autoset(sota->map, 0, TNECS_NULL);

    /* -- Set cursor position to first starting position -- */
    SDL_assert(sota                     != NULL);
    SDL_assert(sota->entity_cursor      != TNECS_NULL);
    struct Position *position = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(position                 != NULL);
    SDL_assert(sota->map                != NULL);
    SDL_assert(sota->map->start_pos     != NULL);
    SDL_assert(DARR_NUM(sota->map->start_pos) > 0);
    Position_Pos_Set(position, sota->map->start_pos[0].x, sota->map->start_pos[0].y);

    // Game_cursorFocus_onMap resets position to cursor_lastpos;
    // TODO: bool flag to disable Game_cursorFocus_onMap resetting cursor pos.
    sota->cursor_lastpos.x = sota->map->start_pos[0].x;
    sota->cursor_lastpos.y = sota->map->start_pos[0].y;



    /* -- Set popup_unit position -- */
    /* -- Set popup_tile position -- */
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point *pos = &cursor_pos->tilemap_pos;

    if (fsm_eCrsMvd_sGmpMap_ss[sota->substate] != NULL)
        fsm_eCrsMvd_sGmpMap_ss[sota->substate](sota, mover_entity, pos);

    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    SDL_assert(popup_ent != TNECS_NULL);

    Game_PopUp_Tile_Place(sota, *pos);

    Game_PopUp_Tile_Place(sota, *pos);


    /* -- Start turn transition -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}
