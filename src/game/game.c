#include "game/game.h"

#define STB_SPRINTF_IMPLEMENTATION
#ifndef STB_SPRINTF_IMPLEMENTATION
    #include "stb_sprintf.h"
#endif /* STB_SPRINTF_IMPLEMENTATION */

struct Game Game_default = {
    .cursor_lastpos         = {1, 1},
    .moved_direction        = SOTA_DIRECTION_NULL,
    .ismouse                = false,
    .iscursor               = true,
    .isrunning              = false,
    .items_dtab             = NULL,
    .weapons_dtab           = NULL,
    .runtime_ns             = 0,
    .combat_outcome         = {0},
    .filename_menu          = {0},
    .menu_options_dtab      = NULL,
    .menu_stack             = NULL,
    .player_select_menus    = {0},
    .combat_forecast        = {0},
    .popups                 = {TNECS_NULL},
    .stats_menu             = TNECS_NULL,
    .pre_combat_popup       = TNECS_NULL,
    .growths_menu           = TNECS_NULL,
    .deployment_menu        = TNECS_NULL,

    .cursor_move            = {0},
    .entity_mouse           = TNECS_NULL,
    .ent_unit_loaded        = NULL,
    .entity_cursor          = 0,
    .entity_transition      = 0,
    .entity_highlighted     = 0,
    .entity_shadowed        = 0,
    .entity_fps             = 0,
    .selected_unit_entity   = 0,

    .isShadow               = false,

    .ai_timer               = TNECS_NULL,

    .party                  = {0},
    .party_id_stack         = {0},

    // .menu_font       = NULL,
    .menu_pixelfont     = NULL,

    .map           = NULL,
    .window        = NULL,
    .units_loaded  = NULL,
    .selected_unit_moved_position   = {-1, -1},
    .selected_unit_initial_position = {-1, -1},
    // .convoy = Convoy_default,
    // .camp = Camp_default,

    .aggressor  = TNECS_NULL, // combat
    .defendant  = TNECS_NULL, // combat
    .defendants = NULL,       // combat
    .patients   = NULL,       // staff
    .victims    = NULL,       // rescue
    .deployed   = NULL,       // rescue
    .spectators = NULL,       // dance
    .auditors   = NULL,       // talk
    .passives   = NULL,       // trade
    .openables  = NULL,       // doors and chests

    .cursor_diagonal    = false,

    .inputs = {0},

    .ai_state   = {0},
    /* --- Music --- */
    .music              = NULL,
    .soundfx_cursor     = NULL,
    .soundfx_next_turn  = NULL,

    .chapter            = -1,
    .state              = GAME_STATE_Title_Screen,
    .substate           = GAME_SUBSTATE_MENU,
    .state_previous     = GAME_STATE_START,
    .substate_previous  = GAME_SUBSTATE_START,
    .fast_forward       = false,
};

/* --- Constructors/Destructors --- */

void Game_Free(struct Game *sota) {
    Game_Cursor_Free(sota);
    Game_PopUp_Tile_Free(sota);
    Game_Mouse_Free(sota);
    Game_menuStack_Free(sota);
    Game_PlayerSelectMenus_Free(sota);

    if (sota->map_enemies != NULL) {
        DARR_FREE(sota->map_enemies);
        sota->map_enemies = NULL;
    }

    if (sota->combat_outcome.attacks != NULL) {
        DARR_FREE(sota->combat_outcome.attacks);
        sota->combat_outcome.attacks = NULL;
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
        SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Control");
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

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Map");

    Game_Map_Reinforcements_Free(sota);
    if ((sota->map != NULL) && (sota->map->unitmap != NULL))
        Map_Units_Free(sota->map);
    Game_Map_Free(sota);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Camera");
    // if (sota->camera != NULL)
    // SDL_free(sota->camera);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Events");
    Events_Data_Free();
    Events_Names_Free();
    Events_Receivers_Free();
    #ifndef SOTA_OPENGL
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Renderer");
    if (sota->render_target != NULL)
        SDL_DestroyTexture(sota->render_target);
    if (sota->renderer != NULL) {
        SDL_DestroyRenderer(sota->renderer); /* free renderer before window */
    }
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free Window");
    if (sota->window != NULL)
        SDL_DestroyWindow(sota->window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    Mix_Quit();
    SDL_Quit();
    #else /* SOTA_OPENGL */
    #endif /* SOTA_OPENGL */
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Free tnecs world");
    if (sota->world != NULL) {
        tnecs_world_destroy(sota->world); /* crashes */
    }
    Game_Items_Free(  &sota->items_dtab);
    Game_Weapons_Free(&sota->weapons_dtab);
    if (sota->units_loaded != NULL) {
        SDL_free(sota->units_loaded);
        sota->units_loaded = NULL;
    }
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

void Game_Init(struct Game *sota) {
    *sota = Game_default;
    sota->settings = Settings_default;
    SDL_assert(sota->settings.FPS.cap > 0);
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Init game");
    i16 flags = 0;
    sota->filename_menu = s8_literal(PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png"));

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Init game");
    /* init weapons_dtab */
    if (sota->weapons_dtab != NULL) {
        DTAB_FREE(sota->weapons_dtab);
        sota->weapons_dtab = NULL;
    }
    DTAB_INIT(sota->weapons_dtab, struct Weapon);

    /* init items_dtab */
    if (sota->items_dtab != NULL) {
        DTAB_FREE(sota->items_dtab);
        sota->items_dtab = NULL;
    }
    DTAB_INIT(sota->items_dtab, struct Item);

    sota->map_enemies = DARR_INIT(sota->map_enemies, tnecs_entity, 16);
    if (sota->settings.fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;
    sota->units_loaded = SDL_calloc(UNIT_ID_END, sizeof(* sota->units_loaded));

    sota->camera.offset.x = DEFAULT_CAMERA_XOFFSET;
    sota->camera.offset.y = DEFAULT_CAMERA_YOFFSET;
    sota->camera.zoom     = DEFAULT_CAMERA_ZOOM;

    #ifndef SOTA_OPENGL
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Firesaga: Window Initialization");
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        /*  NOTE: --- SDL_INIT LEAKS A LOT OF MEMORY --- */
        //  Ex:     reachable 224,425 bytes in 1,445 blocks
        //          suppressed 16 bytes in 1 blocks
        //          It may be cause of X11.
        //      Someone on Stack overflow said his NVIDIA drivers leak 10Mb.
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }

    /* Initialize SDL_mixer */
    struct Music_settings music = sota->settings.music_settings;
    if (Mix_OpenAudio(music.frequency, music.format, music.channels, music.sample_size) < 0) {
        SDL_LogCritical(SOTA_LOG_SYSTEM, "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                        Mix_GetError());
        exit(1);
    }

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
    #endif
    #else
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
    // char *path_mapping = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
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
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Allocating space for events\n");
    Events_Data_Malloc();
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Initializing user events\n");
    Events_Names_Declare();
    Events_Names_Alloc();
    Events_Receivers_Declare();
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Initializing Menus\n");
    Game_Menus_Init(sota);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Genesis of tnecs\n");
    sota->world = tnecs_world_genesis();
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "Components Registration\n");
    TNECS_REGISTER_COMPONENT(sota->world, Position);
    TNECS_REGISTER_COMPONENT(sota->world, Sprite);
    TNECS_REGISTER_COMPONENT(sota->world, Unit);
    TNECS_REGISTER_COMPONENT(sota->world, Boss);
    TNECS_REGISTER_COMPONENT(sota->world, Menu);
    TNECS_REGISTER_COMPONENT(sota->world, controllerGamepad);
    TNECS_REGISTER_COMPONENT(sota->world, controllerMouse);
    TNECS_REGISTER_COMPONENT(sota->world, controllerKeyboard);
    TNECS_REGISTER_COMPONENT(sota->world, controllerTouchpad);
    TNECS_REGISTER_COMPONENT(sota->world, Timer);
    TNECS_REGISTER_COMPONENT(sota->world, PopUp);
    TNECS_REGISTER_COMPONENT(sota->world, Slider);
    TNECS_REGISTER_COMPONENT(sota->world, Hover);
    TNECS_REGISTER_COMPONENT(sota->world, SliderOffscreen);
    TNECS_REGISTER_COMPONENT(sota->world, CursorFlag);
    TNECS_REGISTER_COMPONENT(sota->world, MouseFlag);
    TNECS_REGISTER_COMPONENT(sota->world, Text);
    TNECS_REGISTER_COMPONENT(sota->world, Breakable);
    TNECS_REGISTER_COMPONENT(sota->world, Door);
    TNECS_REGISTER_COMPONENT(sota->world, Chest);
    TNECS_REGISTER_COMPONENT(sota->world, Mobj_Link);
    TNECS_REGISTER_COMPONENT(sota->world, MapHPBar);
    TNECS_REGISTER_COMPONENT(sota->world, CombatAnimation);
    TNECS_REGISTER_COMPONENT(sota->world, MapAnimation);
    TNECS_REGISTER_COMPONENT(sota->world, UnitMoveAnimation);
    TNECS_REGISTER_COMPONENT(sota->world, RenderTop);
    TNECS_REGISTER_COMPONENT(sota->world, PixelFont);
    TNECS_REGISTER_COMPONENT(sota->world, AI);
    sota->timer_typeflag = TNECS_COMPONENT_NAME2TYPE(sota->world, Timer);

    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration\n");
    tnecs_world *world = sota->world;
    /* --- SYSTEM REGISTERING: FIRST COME FIRST SERVED ---*/
    /* -- Control systems ran first --  */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Keyboard, 0, Position, Sprite, controllerKeyboard);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Gamepad,  0, Position, Sprite, controllerGamepad);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Touchpad, 0, Position, Sprite, controllerTouchpad);

    /* -- Animating and sliding systems before drawing --  */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_Sprite,   0, Sprite, Position, Slider);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Slide_PopUp_Offscreen, 1, PopUp, Slider,
                                SliderOffscreen, Position);
    /* - no sliders without offscreen yet -  */
    // TNECS_REGISTER_SYSTEM_wEXCL(sota->world, slidePopUp, 0, PopUp, Slider, Position);
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
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Turn_Transition, 1, MapAnimation, Position, Text, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Animate_Unit_Move_onMap, 0, UnitMoveAnimation, Position, Timer,
                                Unit);
    SDL_LogVerbose(SOTA_LOG_SYSTEM, "System Registration DONE\n");

    sota->isrunning = true;
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
    bool absolute = false;
    bool isCursor = false;
    dstrect_funcs[absolute][isCursor = true]        = &Cursor_Dstrect_Relative;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Relative;
    dstrect_funcs[absolute = true][isCursor = true] = &Cursor_Dstrect_Absolute;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Absolute;

    /* --- Alloc arrays of entities --- */
    sota->defendants   = DARR_INIT(sota->defendants, tnecs_entity, 4);
    sota->patients     = DARR_INIT(sota->patients,   tnecs_entity, 4);
    sota->victims      = DARR_INIT(sota->victims,    tnecs_entity, 4);
    sota->deployed     = DARR_INIT(sota->deployed,   tnecs_entity, 4);
    sota->spectators   = DARR_INIT(sota->spectators, tnecs_entity, 4);
    sota->auditors     = DARR_INIT(sota->auditors,   tnecs_entity, 4);
    sota->passives     = DARR_INIT(sota->passives,   tnecs_entity, 4);
    sota->openables    = DARR_INIT(sota->openables,  tnecs_entity, 4);

    /* --- Alloc combat arrays --- */
    sota->combat_outcome.attacks = DARR_INIT(sota->combat_outcome.attacks, struct Combat_Attack,
                                             SOTA_COMBAT_MAX_ATTACKS);

    /* --- Set default contextual inputs --- */
    fsm_Input_s[sota->state](sota);

    /* --- Soundfx --- */
    sota->soundfx_cursor    = Soundfx_Load_Cursor();
    sota->soundfx_next_turn = Soundfx_Load_Next_Turn();
}

// TODO: Rename
void Game_Startup(struct Game *sota, struct Input_Arguments in_args) {
    strncpy(sota->reason, "Initial sota startup", sizeof(sota->reason));

    /* -- Load Cursor and mouse -- */
    SDL_ShowCursor(SDL_DISABLE); // for default cursor.
    Game_FPS_Create(sota, SOTA_FPS_UPDATE_ns);
    Game_Cursor_Create(sota);
    Game_Mouse_Create(sota);

    /* -- Load Title -- */
    Game_titleScreen_Load(sota, in_args);
    Game_Mouse_Disable(sota);
    Game_cursorFocus_onMenu(sota);

    /* -- Load Background -- */
    SDL_assert(sota->state      == GAME_STATE_Title_Screen);
    SDL_assert(sota->substate   == GAME_SUBSTATE_MENU);
}

void Game_Save_Copy( i16 from_ind,  i16 to_ind) {
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

void Game_loadJSON(struct Game *sota,  i16 save_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));
    cJSON *json         = jsonio_parseJSON(filename);
    // readJSON_narrative(json, &sota->narrative);
    cJSON *jRN          = cJSON_GetObjectItem(json, "RN");
    cJSON *jRN_s1       = cJSON_GetObjectItem(jRN, "s1");
    cJSON *jRN_s2       = cJSON_GetObjectItem(jRN, "s2");
    cJSON *jRN_s3       = cJSON_GetObjectItem(jRN, "s3");
    cJSON *jRN_s4       = cJSON_GetObjectItem(jRN, "s4");
    u64 s1              = cJSON_GetNumberValue(jRN_s1);
    u64 s2              = cJSON_GetNumberValue(jRN_s2);
    u64 s3              = cJSON_GetNumberValue(jRN_s3);
    u64 s4              = cJSON_GetNumberValue(jRN_s4);
    RNG_Set_xoroshiro256ss(s1, s2, s3, s4);

    cJSON *jconvoy = cJSON_GetObjectItem(json, "Convoy");
    Convoy_Clear(&sota->convoy);
    Convoy_readJSON(&sota->convoy, jconvoy);
    //     convoy.setWeapons(&weapons);
    cJSON *jparty = cJSON_GetObjectItem(json, "Party");
    Game_Party_Clear(sota);
    // cJSON * jcamp = cJSON_GetObjectItem(json, "Camp");
    // Camp_readJSON(&sota->camp, jcamp);
    cJSON *junit = cJSON_GetObjectItem(jparty, "Unit");
    struct Unit temp_unit;
    sota->party_size = 0;
    while (junit != NULL) {
        temp_unit = Unit_default;
        Unit_readJSON(&temp_unit, junit);
        sota->party[sota->party_size] = temp_unit;
        sota->party_size++;
        junit = junit->next;
    }
    cJSON_Delete(json);
}

void Game_saveJSON(struct Game *sota,  i16 save_ind) {
    if (!PHYSFS_exists(SAVE_FOLDER))
        PHYSFS_mkdir(SAVE_FOLDER);
    s8 filename = s8_mut(SAVE_FOLDER);
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    filename = s8cat(filename, s8_var(temp));
    SDL_LogDebug(SOTA_LOG_APP, "saveJSON Game to: %s\n", filename.data);
    PHYSFS_delete(filename.data);
    PHYSFS_file *fp = PHYSFS_openWrite(filename.data);
    SDL_assert(fp);
    cJSON *json         = cJSON_CreateObject();
    // writeJSON_narrative(json, &sota->narrative);
    cJSON *jparty       = cJSON_CreateObject();
    cJSON *jconvoy      = cJSON_CreateObject();
    // cJSON * jcamp = cJSON_CreateObject();
    cJSON *junit;
    cJSON *jRN          = cJSON_CreateObject();
    cJSON *jRN_status   = cJSON_CreateArray();

    // TODO: save xoshiro256ss state
    RNG_Get_xoroshiro256ss(sota->s_xoshiro256ss);
    cJSON *jRN_s1     = cJSON_CreateNumber(sota->s_xoshiro256ss[0]);
    cJSON *jRN_s2     = cJSON_CreateNumber(sota->s_xoshiro256ss[0]);
    cJSON *jRN_s3     = cJSON_CreateNumber(sota->s_xoshiro256ss[0]);
    cJSON *jRN_s4     = cJSON_CreateNumber(sota->s_xoshiro256ss[0]);

    for (u8 i = 0; i < sota->party_size; i++) {
        junit = cJSON_CreateObject();
        Unit_writeJSON(&sota->party[i], junit);
        cJSON_AddItemToObject(jparty, "Unit", junit);
    }
    cJSON_AddItemToObject(jRN,  "s1",       jRN_s1);
    cJSON_AddItemToObject(jRN,  "s2",       jRN_s2);
    cJSON_AddItemToObject(jRN,  "s3",       jRN_s3);
    cJSON_AddItemToObject(jRN,  "s4",       jRN_s4);
    cJSON_AddItemToObject(json, "RN",       jRN);
    cJSON_AddItemToObject(json, "Party",    jparty);
    Convoy_writeJSON(&sota->convoy, jconvoy);
    // cJSON_AddItemToObject(json, "Camp", jcamp);
    // Camp_writeJSON(&sota->camp, jcamp);
    cJSON_AddItemToObject(json, "Convoy", jconvoy);
    jsonio_Print(fp, json);
    PHYSFS_close(fp);
    cJSON_Delete(json);
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

    /* --- Set default contextual inputs --- */
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
