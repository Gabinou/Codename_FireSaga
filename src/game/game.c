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
    .combat_attacks         = NULL,

    .menu_options_dtab      = NULL,
    .menu_stack             = NULL,
    .player_select_menus    = {0},
    .popups                 = {TNECS_NULL},
    .stats_menu             = TNECS_NULL,
    .pre_combat_menu        = TNECS_NULL,
    .GM_menu                = TNECS_NULL,
    .map_animation          = TNECS_NULL,

    .cursor_move            = {0},
    .entity_mouse           = TNECS_NULL,
    .ent_unit_loaded        = NULL,
    .entity_cursor          = 0,
    .entity_transition      = 0,
    .entity_highlighted     = 0,
    .entity_shadowed        = 0,
    .entity_fps             = 0,
    .selected_unit_entity   = 0,

    .isShadow = false,

    // .menu_font       = NULL,
    .menu_pixelfont     = NULL,

    .map           = NULL,
    .window        = NULL,
    .units_loaded  = NULL,
    .selected_unit_moved_position   = {-1, -1},
    .selected_unit_initial_position = {-1, -1},
    // tinymt32_t tinymt32,
    // .convoy = Convoy_default,
    // .camp = Camp_default,

    .aggressor  = TNECS_NULL, // combat
    .defendant  = TNECS_NULL, // combat
    .defendants = NULL,       // combat
    .patients   = NULL,       // staff
    .victims    = NULL,       // rescue
    .spectators = NULL,       // dance
    .auditors   = NULL,       // talk
    .passives   = NULL,       // trade
    .openables  = NULL,       // doors and chests

    .cursor_diagonal    = false,

    .inputs = {0},


    .chapter            = -1,
    .state              = GAME_STATE_Title_Screen,
    .substate           = GAME_SUBSTATE_MENU,
    .state_previous     = GAME_STATE_START,
    .substate_previous  = GAME_SUBSTATE_START,
    .fast_forward       = false,
};

/* --- Constructors/Destructors --- */
void Game_Free(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Cursor_Free(sota);
    Game_PopUp_Tile_Free(sota);
    Game_Mouse_Free(sota);
    Game_menuStack_Free(sota);
    Game_PlayerSelectMenus_Free(sota);
    if (sota->map_enemies != NULL) {
        DARR_FREE(sota->map_enemies);
        sota->map_enemies = NULL;
    }

    if (sota->combat_attacks != NULL) {
        DARR_FREE(sota->combat_attacks);
        sota->combat_attacks = NULL;
    }

    PixelFont_Free(sota->pixelnours,       true);
    PixelFont_Free(sota->pixelnours_big,   true);
    PixelFont_Free(sota->pixelnours_tight, true);

    if (sota->stats_menu > TNECS_NULL) {
        struct MenuComponent *mc;
        mc = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, MenuComponent);
        if (mc->data != NULL) {
            struct StatsMenu *stats_menu = mc->data;
            if (mc->n9patch.texture != NULL)
                SDL_DestroyTexture(mc->n9patch.texture);
            StatsMenu_Free(stats_menu);
            mc->data = NULL;
        }
    }

    if (sota->item_select_menu > TNECS_NULL) {
        struct MenuComponent *mc;
        mc = TNECS_GET_COMPONENT(sota->world, sota->item_select_menu, MenuComponent);
        if (mc->data != NULL) {
            struct LoadoutSelectMenu *ism = mc->data;
            LoadoutSelectMenu_Free(ism);
            free(ism);
            mc->data = NULL;
        }
    }

    if (sota->renderer)
        SOTA_Log_Debug("Free Control");
    // controlFree();
    SOTA_Log_Debug("Free Map");
    Game_UnitsonMap_Free(sota);
    Game_Map_Reinforcements_Free(sota);
    Game_Map_Free(sota);
    SOTA_Log_Debug("Free Camera");
    // if (sota->camera != NULL)
    // SDL_free(sota->camera);
    SOTA_Log_Debug("Free Events");
    Events_Data_Free();
    Events_Names_Free();
    Events_Receivers_Free();
    #ifndef SOTA_OPENGL
    SOTA_Log_Debug("Free Renderer");
    if (sota->render_target != NULL)
        SDL_DestroyTexture(sota->render_target);
    if (sota->renderer != NULL) {
        SDL_DestroyRenderer(sota->renderer); /* free renderer before window */
    }
    SOTA_Log_Debug("Free Window");
    if (sota->window != NULL)
        SDL_DestroyWindow(sota->window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();
    #else /* SOTA_OPENGL */
    #endif /* SOTA_OPENGL */
    SOTA_Log_Debug("Free tnecs world");
    if (sota->world != NULL) {
        tnecs_world_destroy(sota->world); /* crashes */
    }
    Game_Items_Free(  sota->items_dtab);
    Game_Weapons_Free(sota->weapons_dtab);
    if (sota->units_loaded != NULL) {
        SDL_free(sota->units_loaded);
        sota->units_loaded = NULL;
    }
    // if (sota->font != NULL) {
    //     TTF_CloseFont(sota->font);
    //     sota->font = NULL;
    // }
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

    TTF_Quit();
    SOTA_Log_Debug("Game cleaned.");
    SDL_free(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
struct Game *Game_Init() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Allocing Game");
    struct Game *out_game;
    out_game    = SDL_malloc(sizeof(struct Game));
    *out_game   = Game_default;
    out_game->settings = Settings_default;
    SDL_assert(out_game->settings.FPS.cap > 0);
    SOTA_Log_Debug("Init game");
    if16 flags = 0;
    strcpy(out_game->filename_menu, PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png"));
    /* init weapons_dtab */
    if (out_game->weapons_dtab != NULL) {
        DTAB_FREE(out_game->weapons_dtab);
        out_game->weapons_dtab = NULL;
    }
    DTAB_INIT(out_game->weapons_dtab, struct Weapon);

    /* init items_dtab */
    if (out_game->items_dtab != NULL) {
        DTAB_FREE(out_game->items_dtab);
        out_game->items_dtab = NULL;
    }
    DTAB_INIT(out_game->items_dtab, struct Item);

    out_game->map_enemies = DARR_INIT(out_game->map_enemies, tnecs_entity_t, 16);
    if (out_game->settings.fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;
    out_game->units_loaded = SDL_calloc(UNIT_ID_END, sizeof(* out_game->units_loaded));
    out_game->camera.offset.x = DEFAULT_CAMERA_XOFFSET;
    out_game->camera.offset.y = DEFAULT_CAMERA_YOFFSET;
    out_game->camera.zoom = DEFAULT_CAMERA_ZOOM;

    // SOTA_Log_Debug("Firesaga: Font Initialization");
    // if (TTF_Init() == -1) {
    //     SOTA_Log_Debug("TTF_Init: %s\n", TTF_GetError());
    //     exit(ERROR_TTFInitFail);
    // }

    // out_game->font = TTF_OpenFont(PATH_JOIN("..", "fonts", "arial.ttf"), out_game->settings.fontsize);
    // SDL_assert(out_game->font != NULL);
    // SOTA_Log_Debug("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());

    #ifndef SOTA_OPENGL
    SOTA_Log_Debug("Firesaga: Window Initialization");
    int success = SDL_Init(SDL_INIT_EVERYTHING);
    //  --- SDL_INIT LEAKS A LOT OF MEMORY ---
    //     reachable 224,425 bytes in 1,445 blocks
    //     suppressed 16 bytes in 1 blocks
    //     It may be cause of X11.
    //     Someone on Stack overflow said his NVIDIA drivers leak 10Mb.
    SDL_assert(success == 0);
    out_game->window = SDL_CreateWindow(out_game->settings.title, out_game->settings.pos.x,
                                        out_game->settings.pos.y, out_game->settings.res.x, out_game->settings.res.y, flags);
    // SDL_assert(SDL_ISPIXELFORMAT_INDEXED(SDL_GetWindowPixelFormat(out_game->window)));
    SDL_GetWindowSize(out_game->window, &out_game->window_w, &out_game->window_h);
    SDL_assert(out_game->window_w == out_game->settings.res.x);
    SDL_assert(out_game->window_h == out_game->settings.res.y);
    if (out_game->window) {
        SOTA_Log_Debug("Window created\n");
        out_game->renderer = SDL_CreateRenderer(out_game->window, -1, SDL_RENDERER_TARGETTEXTURE);
        SDL_assert(out_game->renderer);
        if (out_game->renderer) {
            Utilities_DrawColor_Reset(out_game->renderer);
            SOTA_Log_Debug("Renderer created\n");
        }
    }
    #ifndef RENDER2WINDOW
    SOTA_Log_Debug("Rendering to sota->render_target\n");
    SDL_assert(out_game->settings.res.x > 0);
    SDL_assert(out_game->settings.res.y > 0);
    SDL_assert(sota->renderer != NULL);
    sota->render_target = SDL_CreateTexture(sota->renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, out_game->settings.res.x, out_game->settings.res.y);
    SDL_assert(sota->render_target != NULL);
    SDL_SetTextureBlendMode(sota->render_target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(sota->renderer, sota->render_target);
    SDL_assert(sota->render_target != NULL);
    #endif
    #else
    SOTA_Log_Debug("Firesaga: OpenGL Window Initialization");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // TODO: set better window flags
    out_game->gl_window = SDL_CreateWindow(out_game->settings.title, out_game->settings.pos.x,
                                           out_game->settings.pos.y, out_game->settings.res.x, out_game->settings.res.y,
                                           flags | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_assert(out_game->gl_window);
    out_game->gl_context = SDL_GL_CreateContext(out_game->gl_window);
    // glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0)
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    // Initialize OpenGL
    if (!initGL()) {
        printf("Unable to initialize OpenGL!\n");
        success = false;
    }
    #endif /* SOTA_OPENGL */
    char *path_mapping = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
    char *temp_base = SDL_GetBasePath();
    SDL_assert(DEFAULT_BUFFER_SIZE >= strlen(temp_base));
    strncpy(path_mapping, temp_base, DEFAULT_BUFFER_SIZE);
    free(temp_base);
    path_mapping = nstr_Path_Remove_Top(path_mapping, DIR_SEPARATOR[0]);
    const char *separator = PHYSFS_getDirSeparator();
    strcat(path_mapping, separator);
    strcat(path_mapping, DIR_SEPARATOR"gamecontrollerdb.txt");
    SOTA_Log_Debug("Path to gamecontrollerdb: %s\n", path_mapping);
    if (SDL_GameControllerAddMappingsFromFile(path_mapping) < 0)
        SOTA_Log_Debug("gamecontrollerdb.txt not found!\n");
    free(path_mapping);
    SOTA_Log_Debug("Allocating space for globals\n");
    SOTA_Log_Debug("Allocating space for events\n");
    Events_Data_Malloc();
    SOTA_Log_Debug("Initializing user events\n");
    Events_Names_Declare();
    Events_Names_Alloc();
    Events_Receivers_Declare();
    SOTA_Log_Debug("Initializing Menus\n");
    Game_Menus_Init(out_game);
    SOTA_Log_Debug("Genesis of tnecs\n");
    out_game->world = tnecs_world_genesis();
    SOTA_Log_Debug("Components Registration\n");
    TNECS_REGISTER_COMPONENT(out_game->world, Position);
    TNECS_REGISTER_COMPONENT(out_game->world, Sprite);
    TNECS_REGISTER_COMPONENT(out_game->world, Unit);
    // TNECS_REGISTER_COMPONENT(out_game->world, Text_TTF);
    TNECS_REGISTER_COMPONENT(out_game->world, MenuComponent);
    TNECS_REGISTER_COMPONENT(out_game->world, controllerGamepad);
    TNECS_REGISTER_COMPONENT(out_game->world, controllerMouse);
    TNECS_REGISTER_COMPONENT(out_game->world, controllerKeyboard);
    TNECS_REGISTER_COMPONENT(out_game->world, controllerTouchpad);
    TNECS_REGISTER_COMPONENT(out_game->world, Timer);
    TNECS_REGISTER_COMPONENT(out_game->world, PopUp);
    TNECS_REGISTER_COMPONENT(out_game->world, Slider);
    TNECS_REGISTER_COMPONENT(out_game->world, Hover);
    TNECS_REGISTER_COMPONENT(out_game->world, SliderOffscreen);
    TNECS_REGISTER_COMPONENT(out_game->world, CursorFlag);
    TNECS_REGISTER_COMPONENT(out_game->world, MouseFlag);
    TNECS_REGISTER_COMPONENT(out_game->world, Text);
    TNECS_REGISTER_COMPONENT(out_game->world, Breakable);
    TNECS_REGISTER_COMPONENT(out_game->world, Door);
    TNECS_REGISTER_COMPONENT(out_game->world, Chest);
    TNECS_REGISTER_COMPONENT(out_game->world, Mobj_Link);
    TNECS_REGISTER_COMPONENT(out_game->world, MapHPBar);
    TNECS_REGISTER_COMPONENT(out_game->world, CombatAnimation);
    TNECS_REGISTER_COMPONENT(out_game->world, RenderTop);
    TNECS_REGISTER_COMPONENT(out_game->world, PixelFont);
    out_game->timer_typeflag = TNECS_COMPONENT_NAME2TYPE(out_game->world, Timer);

    SOTA_Log_Debug("System Registration\n");
    tnecs_world_t *world = out_game->world;
    /* --- SYSTEM REGISTERING: FIRST COME FIRST SERVED ---*/
    /* -- Control systems ran first --  */
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Keyboard, 0, Position, Sprite, controllerKeyboard);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Gamepad,  0, Position, Sprite, controllerGamepad);
    TNECS_REGISTER_SYSTEM_wEXCL(world, Control_Touchpad, 0, Position, Sprite, controllerTouchpad);

    /* -- Animating and sliding systems before drawing --  */
    TNECS_REGISTER_SYSTEM_wEXCL(world, slideSprite,   0, Sprite, Position, Slider);
    TNECS_REGISTER_SYSTEM_wEXCL(world, slidePopUpOffscreen, 1, PopUp, Slider,
                                SliderOffscreen, Position);
    /* - no sliders without offscreen yet -  */
    // TNECS_REGISTER_SYSTEM_wEXCL(out_game->world, slidePopUp, 0, PopUp, Slider, Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, hoverAny,      0, Hover,  Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, animateSprite, 0, Sprite, Position, Timer);
    // Remove animated flag. Animated sprites must have a timer! Still sprites dont!

    /* -- Scrolling Text -- */
    TNECS_REGISTER_SYSTEM_wEXCL(world, scrollText,    0, PixelFont, Timer);

    /* -- Drawing -- */
    /* - for now only drawn sprites are map_units - */
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMapUnit,   1, Unit, Position, Sprite, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMapUnit,   1, Unit, Position, Sprite, Timer, MapHPBar);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMapUnit,   1, Unit, Position, Sprite, MapHPBar);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMapUnit,   1, Unit, Position, Sprite);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMapUnit,   0, Unit, Position, Sprite, RenderTop);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMap_HPBar, 0, Unit, Position, MapHPBar);

    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMenu,      0, MenuComponent);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawText,      1, Text, Position);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawTextTimer, 1, Text, Position, Timer);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawPopUp,     0, PopUp, Position);
    /* - draw Cursor and Mouse last -> on top - */
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawCursor,    0, Sprite,          Position, CursorFlag);
    TNECS_REGISTER_SYSTEM_wEXCL(world, drawMouse,     1, controllerMouse, Position, Sprite, MouseFlag);
    SOTA_Log_Debug("System Registration DONE\n");

    out_game->isrunning = true;
    out_game->keyboardInputMap  = KeyboardInputMap_default;
    out_game->gamepadInputMap   = GamepadInputMap_gamecube;
    SOTA_Log_Debug("Loading pixelfonts\n");
    out_game->pixelnours = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours.png");
    PixelFont_Load(out_game->pixelnours, out_game->renderer, path);
    out_game->pixelnours->y_offset = pixelfont_y_offset;

    out_game->pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_Big.png");
    PixelFont_Load(out_game->pixelnours_big, out_game->renderer, path);
    out_game->pixelnours_big->y_offset = pixelfont_big_y_offset;

    out_game->pixelnours_tight = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_tight.png");
    PixelFont_Load(out_game->pixelnours_tight, out_game->renderer, path);

    /* Sprite init */
    bool absolute = false;
    bool isCursor = false;
    dstrect_funcs[absolute][isCursor = true]        = &Cursor_Dstrect_Relative;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Relative;
    dstrect_funcs[absolute = true][isCursor = true] = &Cursor_Dstrect_Absolute;
    dstrect_funcs[absolute][isCursor = false]       = &Sprite_Dstrect_Absolute;

    /* --- Alloc arrays of entities --- */
    out_game->defendants   = DARR_INIT(out_game->defendants, tnecs_entity_t, 4);
    out_game->patients     = DARR_INIT(out_game->patients,   tnecs_entity_t, 4);
    out_game->victims      = DARR_INIT(out_game->victims,    tnecs_entity_t, 4);
    out_game->spectators   = DARR_INIT(out_game->spectators, tnecs_entity_t, 4);
    out_game->auditors     = DARR_INIT(out_game->auditors,   tnecs_entity_t, 4);
    out_game->passives     = DARR_INIT(out_game->passives,   tnecs_entity_t, 4);
    out_game->openables    = DARR_INIT(out_game->openables,  tnecs_entity_t, 4);

    /* --- Alloc combat arrays --- */
    out_game->combat_attacks = DARR_INIT(out_game->combat_attacks, struct Combat_Attack,
                                         SOTA_COMBAT_MAX_ATTACKS);

    /* --- Set default contextual inputs --- */
    fsm_Input_s[out_game->state](out_game);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_game);
}

// TODO: Rename
void Game_Startup(struct Game *sota, struct Input_Arguments in_args) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Save_Copy(const if16 from_ind, const if16 to_ind) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filenameto[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char filenamefrom[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char tempto[DEFAULT_BUFFER_SIZE];
    char tempfrom[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(tempto,   DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", to_ind);
    stbsp_snprintf(tempfrom, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", from_ind);
    strcat(filenameto, tempto);
    strcat(filenamefrom, tempfrom);
    SOTA_Log_Debug("copy saveJSON Game from %s to %s\n", filenamefrom, filenameto);
    PHYSFS_file *pfrom = PHYSFS_openRead(filenamefrom);
    SDL_assert(pfrom != NULL);
    PHYSFS_file *pto = PHYSFS_openWrite(filenameto);
    if16 len = PHYSFS_fileLength(pfrom);
    char longbuffer[len];
    PHYSFS_readBytes(pfrom, longbuffer, len);
    PHYSFS_writeBytes(pto, longbuffer, len);
    PHYSFS_close(pfrom);
    PHYSFS_close(pto);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Save_Delete(const if16 save_ind) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    SOTA_Log_Debug("Deleting Game: %s\n", filename);
    PHYSFS_delete(filename);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_loadJSON(struct Game *sota, const if16 save_ind) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    cJSON *json         = jsonio_parseJSON(filename);
    // readJSON_narrative(json, &sota->narrative);
    cJSON *jRN          = cJSON_GetObjectItem(json, "RN");
    cJSON *jRN_status   = cJSON_GetObjectItem(jRN, "Status");
    cJSON *jRN_mat1     = cJSON_GetObjectItem(jRN, "mat1");
    cJSON *jRN_mat2     = cJSON_GetObjectItem(jRN, "mat2");
    cJSON *jRN_tmat     = cJSON_GetObjectItem(jRN, "tmat");
    sota->tinymt32.mat1 = cJSON_GetNumberValue(jRN_mat1);
    sota->tinymt32.mat2 = cJSON_GetNumberValue(jRN_mat2);
    sota->tinymt32.tmat = cJSON_GetNumberValue(jRN_tmat);
    cJSON *jelement;
    if16 i = 0;
    cJSON_ArrayForEach(jelement, jRN_status) {
        if ((i >= 0) && (i < 4)) {
            sota->tinymt32.status[i] = cJSON_GetNumberValue(jelement);
            i++;
        }
    }
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_saveJSON(struct Game *sota, const if16 save_ind) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (!PHYSFS_exists(SAVE_FOLDER))
        PHYSFS_mkdir(SAVE_FOLDER);
    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    SOTA_Log_Debug("saveJSON Game to: %s\n", filename);
    PHYSFS_delete(filename);
    PHYSFS_file *fp = PHYSFS_openWrite(filename);
    SDL_assert(fp);
    cJSON *json         = cJSON_CreateObject();
    // writeJSON_narrative(json, &sota->narrative);
    cJSON *jparty       = cJSON_CreateObject();
    cJSON *jconvoy      = cJSON_CreateObject();
    // cJSON * jcamp = cJSON_CreateObject();
    cJSON *junit;
    cJSON *jRN          = cJSON_CreateObject();
    cJSON *jRN_status   = cJSON_CreateArray();
    cJSON *jRN_mat1     = cJSON_CreateNumber(sota->tinymt32.mat1);
    cJSON *jRN_mat2     = cJSON_CreateNumber(sota->tinymt32.mat2);
    cJSON *jRN_tmat     = cJSON_CreateNumber(sota->tinymt32.tmat);
    cJSON *jtemp;
    // SOTA_Log_Debug("RnStatus[0] %d", tinymt32.status[0]);
    jtemp = cJSON_CreateNumber(sota->tinymt32.status[0]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(sota->tinymt32.status[1]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(sota->tinymt32.status[2]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(sota->tinymt32.status[3]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    for (uf8 i = 0; i < sota->party_size; i++) {
        junit = cJSON_CreateObject();
        Unit_writeJSON(&sota->party[i], junit);
        cJSON_AddItemToObject(jparty, "Unit", junit);
    }
    cJSON_AddItemToObject(jRN,  "Status",   jRN_status);
    cJSON_AddItemToObject(jRN,  "mat1",     jRN_mat1);
    cJSON_AddItemToObject(jRN,  "mat2",     jRN_mat2);
    cJSON_AddItemToObject(jRN,  "tmat",     jRN_tmat);
    cJSON_AddItemToObject(json, "RN",       jRN);
    cJSON_AddItemToObject(json, "Party",    jparty);
    Convoy_writeJSON(&sota->convoy, jconvoy);
    // cJSON_AddItemToObject(json, "Camp", jcamp);
    // Camp_writeJSON(&sota->camp, jcamp);
    cJSON_AddItemToObject(json, "Convoy", jconvoy);
    Filesystem_printJSON(fp, json);
    PHYSFS_close(fp);
    cJSON_Delete(json);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- State --- */
void Game_subState_Set(struct Game *sota, const if8 new_substate, const char *reason) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Substate set to %d because: %s", new_substate, reason);
    SOTA_Log_Debug("%s", reason);
    SDL_assert(new_substate > 0);
    SDL_assert(sota->substate != new_substate);
    sota->substate_previous = sota->substate;
    sota->substate = new_substate;
    SOTA_Log_Debug("Game substate changed %d->%d: %s->%s", sota->substate_previous,
                   sota->substate, gamesubStatenames[sota->substate_previous],
                   gamesubStatenames[sota->substate]);
    if (new_substate == GAME_SUBSTATE_STANDBY)
        sota->cursor_diagonal = true;
    else
        sota->cursor_diagonal = false;

    if (fsm_Input_sGmpMap_ss[sota->substate] != NULL)
        fsm_Input_sGmpMap_ss[sota->substate](sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_State_Set(struct Game *sota, const if8 new_state, const char *reason) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("State set to %d, because: %s", new_state, reason);
    SOTA_Log_Debug("%s", reason);
    SDL_assert(new_state > 0);
    SDL_assert(sota->state != new_state);
    sota->state_previous = sota->state;
    sota->state          = new_state;

    /* --- Set default contextual inputs --- */
    if (fsm_Input_s[sota->state] != NULL)
        fsm_Input_s[sota->state](sota);

    SOTA_Log_Debug("Game state changed %d->%d: %s->%s", sota->state_previous, sota->state,
                   gameStatenames[sota->state_previous], gameStatenames[sota->state]);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Position *cursor_position;
    cursor_position = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_position != NULL);
    if (!cursor_position->absolute) {
        const struct Sprite *cursor_sprite;
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Time --- */
void Game_Delay(struct Game *sota, i64 delay_ms, u64 currentTime_ns,
                u64 elapsedTime_ns) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota != NULL);

    /* - Skip if negative delay - */
    if (delay_ms < 0) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
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

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- FPS --- */
i64 Game_FPS_Delay(struct Game *sota, u64 elapsedTime_ns) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i64 delay       = 0;
    int fps_cap     = sota->settings.FPS.cap;    /* [s^-1] */
    int ff_cap      = sota->settings.FPS.ff_cap; /* [%]    */

    /* 0 frame delay if fast-forwarding (ff) without limit */
    if ((sota->fast_forward) && (ff_cap <= SOTA_100PERCENT)) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (delay);
    }

    /* Compute delay for ff_cap */
    if ((sota->fast_forward) && (ff_cap > SOTA_100PERCENT)) {
        int ff_fps_cap = fps_cap * ff_cap / SOTA_100PERCENT;
        delay = ceil((1000.0f / ff_fps_cap) - (elapsedTime_ns / 1e6));
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (delay);
    }

    /* Compute delay for no ff */
    if (((elapsedTime_ns * fps_cap) < 1e9) || (elapsedTime_ns == 0))
        delay = ceil((1000.0f / fps_cap) - (elapsedTime_ns / 1e6));

    SDL_assert(delay >= 0);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (delay);
}


void Game_FPS_Create(struct Game *sota, i64 in_update_time_ns) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->entity_fps != 0)
        tnecs_entity_destroy(sota->world, sota->entity_fps);
    sota->entity_fps = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position, Text, Timer);

    /* -- Get timer -- */
    struct Timer *timer;
    timer  = TNECS_GET_COMPONENT(sota->world, sota->entity_fps, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    /* -- Get position -- */
    struct Position *position = TNECS_GET_COMPONENT(sota->world, sota->entity_fps, Position);
    *position = Position_default;

    SDL_assert(position != NULL);
    position->onTilemap = false;
    Position_Bounds_Set(position, 0, sota->settings.res.x, 0, sota->settings.res.y);
    if16 in_x = sota->settings.res.x * 0.92f;
    if16 in_y = sota->settings.res.y * 0.02f;
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
    Text_Set(text, "60.1");

    SDL_assert(sota->world->entity_typeflags[sota->entity_fps] ==
               TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Timer, Position, Text));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
