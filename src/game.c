#include "game.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_ds.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

SDL_Renderer * Game_renderer = NULL;
TTF_Font * Game_font = NULL;

struct Game Game_default = {
    .cursor_lastpos = {6, 6},

    .ismouse = false,
    .iscursor = true,
    .isrunning = false,
    .weapons = NULL,

    .menu_options =  NULL,
    .menu_stack = NULL,
    .menu_loaded = NULL,
    .menu_stack_num = 0,

    .entity_cursor = 0,
    .entity_mouse = 0,
    .entity_transition = 0,
    .entity_highlighted = 0,
    .entity_shadowed = 0,
    .entity_fps = 0,
    .selected_unit_entity = 0,
    .ent_unit_loaded = NULL,


    .units_loaded = NULL,
    .map_ptr = NULL,
    .window = NULL,
    .selected_unit_initial_position = {-1, -1},
    .selected_unit_moved_position = {-1, -1},
    // .settings = Settings_default,
    // tinymt32_t tinymt32,
    // .convoy = Convoy_default,
    // .camp = Camp_default,

    .chapter = -1,
    .state = 0,
};

void Game_makeTurntransition(struct Game * in_game) {
    SDL_Log("Game_makeTurntransition");
    if (in_game->entity_transition != 0) {
        tnecs_entity_destroy(in_game->world, in_game->entity_transition);
    }

    in_game->entity_transition = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Position, Sprite, Text);

    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_transition, Sprite);
    if (sprite_ptr != NULL) {
        sprite_ptr->visible = false;
    }
    struct Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_transition, Text);
    if (text_ptr != NULL) {
        text_ptr->visible = false;
        text_ptr->fontsize = in_game->settings.fontsize;
    }
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_transition, Position);
    if (sprite_ptr != NULL) {
        sprite_ptr->srcrect.x = 128;
        sprite_ptr->srcrect.y = 128;
        sprite_ptr->destrect.x = 128;
        sprite_ptr->destrect.y = 128;
    }
    // Sprite_Texture_Set(sprite_ptr, "assets//GUI//textbox.png");
}

void Game_setTurntransitiontext(struct Game * in_game, int8_t in_army) {
    SDL_Log("Game_setTurntransitiontext");
    Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_transition, Text);
    if (in_army > 0) {
        strcpy(text_ptr->text_line, armyNames[in_army]);
    } else {
        SDL_Log("in_army is invalid. Putting Default text.");
        strcpy(text_ptr->text_line, "Next ");
    }
}

void Game_FPS_Create(struct Game * in_game, float in_update_time) {
    SDL_Log("Game_FPS_Create");
    if (in_game->entity_fps != 0) {
        tnecs_entity_destroy(in_game->world, in_game->entity_fps);
    }
    in_game->entity_fps = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Position, Text, UpdateTimer);
    SDL_Log("Get timer");
    struct UpdateTimer * timer_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_fps, UpdateTimer);
    SDL_assert(timer_ptr != NULL);
    timer_ptr->update_time = in_update_time;
    timer_ptr->onUpdate = &onUpdate_FPS;

    SDL_Log("Get position");
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_fps, Position);
    SDL_assert(position_ptr != NULL);
    position_ptr->onTilemap = false;
    position_ptr->offset_px.x = 0;
    position_ptr->offset_px.y = 0;
    Position_Bounds_Set(position_ptr, 0, in_game->settings.res.x, 0, in_game->settings.res.y);
    // SDL_Log("FPS pos: %d %d ",  in_game->settings.FPS.pos.x, in_game->settings.FPS.pos.y);
    int16_t in_x = in_game->settings.res.x * 0.9f;
    int16_t in_y = in_game->settings.res.y * 0.01f;
    Position_Pos_Set(position_ptr, in_x, in_y);
    // SDL_Log("FPS pos: %d %d ",  position_ptr->pos->x, position_ptr->pos->y);
    // SDL_Log("FPS offset: %d %d ",  position_ptr->offset.x, position_ptr->offset.y);

    SDL_Log("Get Text");
    struct Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_fps, Text);
    SDL_assert(text_ptr != NULL);
    strcpy(text_ptr->text_line, "60.1");
    text_ptr->padding[0] = Text_default.padding[0];
    text_ptr->padding[1] = Text_default.padding[1];
    text_ptr->padding[2] = Text_default.padding[2];
    text_ptr->padding[3] = Text_default.padding[3];
    text_ptr->fontsize = Text_default.fontsize;
    text_ptr->spacingfactor = Text_default.spacingfactor;
    text_ptr->text_color = in_game->settings.FPS.textcolor;
    text_ptr->sizefactor[0] = in_game->settings.FPS.sizefactor[0]; // height
    text_ptr->sizefactor[1] = in_game->settings.FPS.sizefactor[1]; // width

    text_ptr->visible = in_game->settings.FPS.show;
    Text_Texture_Make(text_ptr);
    SDL_assert(in_game->world->entity_typeflags[in_game->entity_fps] == TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, UpdateTimer, Position, Text));
}

void Game_Menu_Disable_Entity(struct Game * in_game, tnecs_entity_t in_menu_entity) {
    SDL_Log("Game_Menu_Disable_Entity");
    SDL_assert(in_menu_entity > 0);
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_menu_entity, Menu);
    SDL_assert(menu_ptr != NULL);
    menu_ptr->enabled = false;
}

void Game_Menu_Disable(struct Game * in_game, int8_t in_menu) {
    SDL_Log("Game_Menu_Disable %d", in_menu);
    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_loaded[in_menu], Menu);
    SDL_assert(menu_ptr != NULL);
    menu_ptr->enabled = false;
}

void Game_Menu_Enable(struct Game * in_game, int8_t in_menu) {
    SDL_Log("Showing Menu %d", in_menu);

    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    // if (hmget(in_game->menus, in_menu) != 0) {
    //     Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, hmget(in_game->menus, in_menu), Text);
    //     Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, hmget(in_game->menus, in_menu), Sprite);

    //     sprite_ptr->visible = false;
    //     text_ptr->visible = false;

    // }
}

tnecs_entity_t Game_Menu_Get(struct Game * in_game, int8_t in_menu) {
    SDL_Log("Game_Menu_Get");
    tnecs_entity_t in_menu_entity = 0;
    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    in_menu_entity = in_game->menu_loaded[in_menu];
    return (in_menu_entity);
}

tnecs_entity_t Game_makeButton(struct Game * in_game, int8_t in_menu_option) {
    SDL_Log("Game_makeButton");
    tnecs_entity_t button = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Position, Text, Sprite);
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, button, Position);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, button, Sprite);
    struct Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, button, Text);

    position_ptr->onTilemap = true;
    Position_Bounds_Set(position_ptr, in_game->map_ptr->boundsmin.x, in_game->map_ptr->boundsmax.x, in_game->map_ptr->boundsmin.y, in_game->map_ptr->boundsmax.y);

    SDL_Color white = {255, 255, 255};
    text_ptr->text_color = white;
    text_ptr->visible = false;
    return (button);
}

void Game_Menu_Destroy(struct Game * in_game, int8_t in_menu) {
    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    tnecs_entity_destroy(in_game->world, in_game->menu_loaded[in_menu]);
    in_game->menu_loaded[in_menu] = 0;
}


void Game_menuStack_Push(struct Game * in_game, tnecs_entity_t in_menu_entity) {
    SDL_Log("Pushing to top of menu stack");
    SDL_assert(in_menu_entity > 0);
    arrpush(in_game->menu_stack, in_menu_entity);
    in_game->menu_stack_num++;
}

tnecs_entity_t Game_menuStack_Pop(struct Game * in_game, bool destroy) {
    SDL_Log("Popping top of menu stack");
    tnecs_entity_t menu_stack_top_entity = arrpop(in_game->menu_stack);
    in_game->menu_stack_num--;
    SDL_assert(menu_stack_top_entity > 0);
    if (destroy) {
        struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_stack_top_entity, Menu);
        in_game->menu_loaded[menu_ptr->id] = 0;
        tnecs_entity_destroy(in_game->world, menu_stack_top_entity);
        menu_stack_top_entity = 0;
    }
    return (menu_stack_top_entity);
}

void Game_Menu_Update(struct Game * in_game, int8_t in_menu) {
    SDL_Log("Updates menu: %d %s", in_menu, menuNames[in_menu]);
    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    if (in_game->menu_loaded[in_menu] == 0) {
        SDL_Log("menu %d %s is not loaded", in_menu, menuNames[in_menu]);
    } else {
        void * data_1 = NULL;
        void * data_2 = NULL;
        switch (in_menu) {
            default:
                break;
                // case MENU_UNIT_ITEMS:
                //     selected_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit, NULL);
                //     data_1 = selected_unit_ptr->_equipment;
                //      break;
        }
        in_game->menuContentMakers[in_menu](in_game, data_1, data_2);
        SDL_Log("Outside menuContentMakers");
        struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_loaded[in_menu], Menu);
        Menu_Cols_init(menu_ptr);
        Menu_cellsWidths_Compute(in_game->world, menu_ptr);
        Menu_rowHeight_Compute(in_game->world, menu_ptr);
        Menu_Width_Compute(menu_ptr);
        Menu_Height_Compute(menu_ptr);
        Menu_Patchsize_Compute(menu_ptr);
        Menu_Rects_Compute(menu_ptr);
        SDL_Log("Is menu enabled? %d", menu_ptr->enabled);
    }
}

void Game_Menu_Create(struct Game * in_game, int8_t in_menu) {
    SDL_Log("Create menu: %s", menuNames[in_menu]);
    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    if (in_game->menu_loaded[in_menu] != 0) {
        SDL_Log("menu %d is already loaded", in_menu);
    } else {
        in_game->menu_loaded[in_menu] = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Menu);
        struct Menu * temp_menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_loaded[in_menu], Menu);
        *temp_menu_ptr = Menu_default;
        temp_menu_ptr->texture_9patch = loadTexture(Game_renderer, in_game->filename_menu, false);
        temp_menu_ptr->font = in_game->menu_font;
        temp_menu_ptr->id = in_menu;
    }
    SDL_Log("in_game->menu_stack_num %d", in_game->menu_stack_num);
}

struct Combat_Forecast Game_Combat_Forecast(struct Game * in_game, struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos) {
    SDL_Log("Game_Combat_Forecast");
    in_game->combat_forecast.stats.attacker_computed = Unit_computedStats(attacker);
    in_game->combat_forecast.stats.defender_computed = Unit_computedStats(defender);

    in_game->combat_forecast.flow = Unit_Combat_Forecast_Flow(attacker, attacker_pos, defender, defender_pos);

    in_game->combat_forecast.stats.attacker_rates =  Unit_Combat_Forecast_Rates(attacker, defender);
    in_game->combat_forecast.stats.defender_rates =  Unit_Combat_Forecast_Rates(defender, attacker);

    in_game->combat_forecast.stats.attacker_damage = Unit_computeDamage(attacker, defender);
    in_game->combat_forecast.stats.defender_damage = Unit_computeDamage(defender, attacker);
    return (in_game->combat_forecast);
}

void Game_Combat_Outcome(struct Game * in_game, struct Unit * attacker, struct Unit * defender) {
    SDL_Log("Game_Combat_Outcome");
    uint8_t attack_num = 0;
    struct Combat_Outcome_Attack temp_attack;
    uint8_t current_brave = 1;

    current_brave = in_game->combat_forecast.flow.attacker_brave;
    while (current_brave-- > 0) {
        SDL_Log("%d \n \n", current_brave);
        temp_attack.hit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.attacker_rates.hit, GAME_RN_DOUBLE);
        temp_attack.crit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.attacker_rates.crit, GAME_RN_DOUBLE);
        temp_attack.aggressor = ATTACKER;
        in_game->outcome_combat[attack_num++] = temp_attack;
    }

    if (in_game->combat_forecast.flow.defender_retaliates) {
        current_brave = in_game->combat_forecast.flow.defender_brave;
        while (current_brave-- > 0) {
            SDL_Log("%d \n \n", current_brave);
            temp_attack.hit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.defender_rates.hit, GAME_RN_DOUBLE);
            temp_attack.crit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.defender_rates.crit, GAME_RN_DOUBLE);
            temp_attack.aggressor = DEFENDER;
            in_game->outcome_combat[attack_num++] = temp_attack;
        }
    }

    if (in_game->combat_forecast.flow.attacker_doubles) {
        current_brave = in_game->combat_forecast.flow.attacker_brave;
        while (current_brave-- > 0) {
            SDL_Log("%d \n \n", current_brave);
            temp_attack.hit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.attacker_rates.hit, GAME_RN_DOUBLE);
            temp_attack.crit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.attacker_rates.crit, GAME_RN_DOUBLE);
            temp_attack.aggressor = ATTACKER;
            in_game->outcome_combat[attack_num++] = temp_attack;
        }
    }

    if ((in_game->combat_forecast.flow.defender_retaliates) && (in_game->combat_forecast.flow.defender_doubles)) {
        current_brave = in_game->combat_forecast.flow.defender_brave;
        while (current_brave-- > 0) {
            SDL_Log("%d \n \n", current_brave);
            temp_attack.hit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.defender_rates.hit, GAME_RN_DOUBLE);
            temp_attack.crit = checkRate(&in_game->tinymt32, in_game->combat_forecast.stats.defender_rates.crit, GAME_RN_DOUBLE);
            temp_attack.aggressor = DEFENDER;
            in_game->outcome_combat[attack_num++] = temp_attack;
        }
    }
    in_game->Combat_Outcome_Attacks_num = attack_num;
}

void Game_Combat_Resolves(struct Game * in_game, struct Unit * attacker, struct Unit * defender) {
    SDL_Log("Game_Combat_Resolves");
    struct Combat_Forecast_Rates aggressor_rates;
    struct Unit * victim;
    uint8_t total_damage = 0;
    struct Damage damage;
    for (uint8_t i = 0; i < in_game->Combat_Outcome_Attacks_num; i++) {
        damage = in_game->outcome_combat[i].aggressor ? in_game->combat_forecast.stats.attacker_damage : in_game->combat_forecast.stats.defender_damage;
        victim = in_game->outcome_combat[i].aggressor ? attacker : defender;
        if (in_game->outcome_combat[i].hit & !in_game->outcome_combat[i].crit) {
            total_damage = equation_attack(damage.dmg[DMG_TYPE_PHYSICAL], damage.dmg[DMG_TYPE_PHYSICAL]);
        } else if (in_game->outcome_combat[i].hit & in_game->outcome_combat[i].crit) {
            total_damage = equation_attack(damage.dmg_crit[DMG_TYPE_PHYSICAL], damage.dmg_crit[DMG_TYPE_PHYSICAL]);
        }
        Unit_takesDamage(victim, total_damage);
        if (!attacker->is_alive | !defender->is_alive) {
            break;
        }
    }
}

void Game_Units_Unload(struct Game * in_game) {
    in_game->units_loaded = NULL;
    hmdefault(in_game->units_loaded, Unit_default);
}

void Game_Tiles_Unload(struct Game * in_game) {
    in_game->tiles_loaded = NULL;
    hmdefault(in_game->tiles_loaded, &Tile_default);
}

void Game_Map_Load(struct Game * in_game, const int16_t in_map_index) {
    SDL_Log("Loading Map index: %d \n", in_map_index);

    SDL_assert((in_map_index > CHAPTER_START) && (in_map_index < CHAPTER_END));
    SDL_Log("Associated map filename %s \n", mapFilenames[in_map_index]);
    Game_Map_Destroy(in_game);
    Game_Tiles_Unload(in_game);
    Game_Units_Unload(in_game);

    in_game->map_ptr = (struct Map *)SDL_malloc(sizeof(struct Map));
    *in_game->map_ptr = Map_default;
    in_game->map_ptr->tiles = in_game->tiles_loaded;
    arrsetcap(in_game->map_ptr->items_num, 8);
    arrsetcap(in_game->map_ptr->tilesindex, 20);
    arrsetcap(in_game->map_ptr->starting_positions, 20);
    arrsetcap(in_game->map_ptr->tilemap, DEFAULT_BUFFER_SIZE * DEFAULT_BUFFER_SIZE);
    arrsetcap(in_game->map_ptr->unitmap, DEFAULT_BUFFER_SIZE * DEFAULT_BUFFER_SIZE);
    arrsetcap(in_game->map_ptr->units_onfield, 50);
    arrsetcap(in_game->map_ptr->armies_onfield, 10);
    arrsetcap(in_game->map_ptr->arrivals, 30);
    arrsetcap(in_game->map_ptr->arrival_equipments, 30 * DEFAULT_EQUIPMENT_SIZE);

    Map_Renderer_Set(in_game->map_ptr, Game_renderer);
    Map_Danger_Load(in_game->map_ptr);
    Map_Tilesize_Set(in_game->map_ptr, in_game->settings.tilesize[0], in_game->settings.tilesize[1]);

    readJSON(mapFilenames[in_map_index], in_game->map_ptr);
    Map_tileTextures_Load(in_game->map_ptr);
    Map_Overlays_Init(in_game->map_ptr);
    Map_overlayTextures_load(in_game->map_ptr);
    in_game->map_ptr->mode_overlay = MAP_OVERLAY_HEAL + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK;
}

void Game_Map_Destroy(struct Game * in_game) {
    if (in_game->map_ptr != NULL) {
        free(in_game->map_ptr);
    }
}

void Game_Mouse_State_Set(struct Game * in_game, const int8_t in_menu) {
    SDL_Log("Game_Mouse_State_Set %d", in_menu);

    SDL_assert((in_menu > MENU_START) & (in_menu < MENU_END));
    int16_t * bounds;
    int16_t linespace;
    SDL_assert(in_game->entity_mouse != 0);
    struct Position * mouse_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Position);
    struct Text * mouse_text_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Text);

    switch (in_menu) {
        // case Gameplay_Map:
        //     mouse_position_ptr->scale[0] = in_game->settings.tilesize[0];
        //     mouse_position_ptr->scale[1] = in_game->settings.tilesize[1];
        //     break;

        // case MENU_Menu_Map:
        // case MENU_UNIT:
        //     linespace = 1;
        //     tnecs_entity_t current_menu = hmget(in_game->menus, in_menu);
        //     if (current_menu != 0) {
        //         const struct Text * menu_text_ptr = TNECS_GET_COMPONENT(in_game->world, current_menu, Text);
        //     }
        //     mouse_position_ptr->scale[0] = linespace;
        //     mouse_position_ptr->scale[1] = linespace;
        //     break;

        default:
            SDL_Log("in_menu is invalid");
    }
}

void Game_cursorFocus_onMap(struct Game * in_game) {
    SDL_Log("Game_cursorFocus_onMap");
    SDL_assert(in_game->entity_cursor != 0);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Sprite);
    SDL_assert(sprite_ptr != NULL);
    // Sprite_Rects_init(sprite_ptr, position_ptr->pixel_pos);
    sprite_ptr->animated = true;
    Sprite_Texture_Set(sprite_ptr, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"GUI"DIR_SEPARATOR"mapcursors.png");
    sprite_ptr->frames = in_game->settings.cursor.frames;
    sprite_ptr->speed = in_game->settings.cursor.speed;
    Sprite_tileSize_Set(sprite_ptr, in_game->settings.tilesize);
    Sprite_slideType_Set(sprite_ptr, SLIDETYPE_GEOMETRIC);

    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
    SDL_assert(position_ptr != NULL);
    position_ptr->onTilemap = true;
    if (in_game->map_ptr != NULL) {
        position_ptr->offset_px.x = in_game->map_ptr->offset_px.x;
        position_ptr->offset_px.y = in_game->map_ptr->offset_px.y;
    } else {
        position_ptr->offset_px.x = DEFAULT_TILEMAP_XOFFSET;
        position_ptr->offset_px.y = DEFAULT_TILEMAP_YOFFSET;
    }
    Map_Bounds_Compute(in_game->map_ptr);
    Position_Bounds_Set(position_ptr, in_game->map_ptr->boundsmin.x, in_game->map_ptr->boundsmax.x, in_game->map_ptr->boundsmin.y, in_game->map_ptr->boundsmax.y);
    SDL_Log("%d %d %d %d", in_game->map_ptr->boundsmin.x, in_game->map_ptr->boundsmax.x, in_game->map_ptr->boundsmin.y, in_game->map_ptr->boundsmax.y);

    Position_Pos_Set(position_ptr, in_game->cursor_lastpos.x, in_game->cursor_lastpos.y);
    position_ptr->scale[0] = (float)in_game->map_ptr->tilesize[0];
    position_ptr->scale[1] = (float)in_game->map_ptr->tilesize[1];
    position_ptr->periodic = false;
}


void Game_cursorFocus_onMenu(struct Game * in_game) {
    // focuses on top of in_game->menu_stack.
    SDL_Log("Game_cursorFocus_onMenu");
    struct Menu * top_menu = NULL;

    SDL_assert(in_game->menu_stack_num > 0);
    SDL_assert(in_game->entity_cursor != 0);
    for (int8_t i = 0; i < in_game->menu_stack_num; i++) {
        SDL_Log("in_game->menu_stack_num, %d %d", in_game->menu_stack_num, i);
        top_menu = TNECS_GET_COMPONENT(in_game->world, in_game->menu_stack[i], Menu);
        if (top_menu != NULL) {
            top_menu->enabled = (i == (in_game->menu_stack_num - 1));
        }
    }

    SDL_assert(top_menu != NULL);
    SDL_Log("top_menu->enabled, %d", top_menu->enabled);

    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Sprite);
    SDL_assert(sprite_ptr != NULL);
    sprite_ptr->animated = false;
    sprite_ptr->tilesize[0] = top_menu->row_height;
    sprite_ptr->tilesize[1] = top_menu->col_widths[0];

    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
    SDL_assert(position_ptr != NULL);
    position_ptr->offset_px.x = top_menu->pos.x;
    position_ptr->offset_px.y = top_menu->pos.y;
    position_ptr->onTilemap = true;
    position_ptr->periodic = true;
    position_ptr->scale[0] = top_menu->col_widths[0];
    position_ptr->scale[1] = top_menu->row_height;
    Position_Bounds_Set(position_ptr, 0, top_menu->col_num - 1, 0, top_menu->row_num - 1);
    Position_Pos_Set(position_ptr, 0, 0);

    position_ptr->offset_px.x = top_menu->pos.x + top_menu->pad_menu.left + top_menu->pad_cell.left;
    position_ptr->offset_px.y = top_menu->pos.y + top_menu->pad_menu.top + top_menu->pad_menu.bottom + top_menu->pad_cell.top + top_menu->pad_cell.bottom;
}

void Game_Mouse_Create(struct Game * in_game) {
    SDL_Log("Game_Mouse_Create");
    Game_Mouse_Destroy(in_game);

    in_game->entity_mouse = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, controllerMouse, Position, Sprite);
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Position);
    SDL_assert(position_ptr != NULL);
    Position_Bounds_Set(position_ptr, -1000, 2000, -1000, 2000);

    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Sprite);
    SDL_assert(sprite_ptr != NULL);
    Sprite_Texture_Set(sprite_ptr, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"GUI"DIR_SEPARATOR"mousecursor.png");
}

void Game_Mouse_Enable(struct Game * in_game) {
    SDL_Log("Game_Mouse_Enable");
    in_game->ismouse = true;
    SDL_assert(in_game->entity_mouse != 0);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Sprite);
    SDL_assert(sprite_ptr != NULL);
    sprite_ptr->visible = true;
}

void Game_Mouse_Disable(struct Game * in_game) {
    SDL_Log("Game_Mouse_Disable");
    in_game->ismouse = false;
    SDL_assert(in_game->entity_mouse != 0);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Sprite);
    SDL_assert(sprite_ptr != NULL);
    sprite_ptr->visible = false;
}

void Game_Mouse_Destroy(struct Game * in_game) {
    SDL_Log("Game_Mouse_Destroy");
    SDL_assert(in_game->entity_mouse != 0);
    tnecs_entity_destroy(in_game->world, in_game->entity_mouse);
}

void Game_Cursor_Create(struct Game * in_game) {
    SDL_Log("Game_Cursor_Create");
    if (in_game->entity_mouse != 0) {
        Game_Cursor_Destroy(in_game);
    }

    in_game->entity_cursor = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Position, Sprite, controllerKeyboard, controllerGamepad, controllerTouchpad);
    SDL_assert(in_game->entity_cursor != 0);

    struct controllerKeyboard * keyboard_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerKeyboard);
    SDL_assert(keyboard_ptr != NULL);
    *keyboard_ptr = controllerKeyboard_default;
    keyboard_ptr->inputmap = &in_game->keyboardInputMap;

    struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);
    SDL_assert(gamepad_ptr != NULL);
    *gamepad_ptr = controllerGamepad_default;
    gamepad_ptr->inputmap = &in_game->gamepadInputMap;

    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Sprite);
    SDL_assert(sprite_ptr != NULL);
    *sprite_ptr = Sprite_default;
    struct Point temp_point;
    temp_point.x = 6;
    temp_point.y = 6;
    sprite_ptr->tilesize[0] = in_game->settings.tilesize[0];
    sprite_ptr->tilesize[1] = in_game->settings.tilesize[1];
    Sprite_Rects_init(sprite_ptr, temp_point);
    sprite_ptr->visible = true;
    sprite_ptr->animated = true;
    sprite_ptr->update_wait = CURSOR_SLIDEWAIT;

    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
    SDL_assert(position_ptr != NULL);
    *position_ptr = Position_default;
}

void Game_Cursor_Destroy(struct Game * in_game) {
    SDL_Log("Game_Cursor_Destroy");
    SDL_assert(in_game->entity_cursor != 0);
    tnecs_entity_destroy(in_game->world, in_game->entity_cursor);
}

void Game_Cursor_Enable(struct Game * in_game) {
    SDL_Log("Game_Cursor_Enable");
    in_game->iscursor = true;
    SDL_assert(in_game->entity_cursor != 0);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Sprite);
    SDL_assert(sprite_ptr != NULL);
    sprite_ptr->visible = true;
}

void Game_Cursor_Disable(struct Game * in_game) {
    SDL_Log("Game_Cursor_Disable");
    in_game->iscursor = false;
    SDL_assert(in_game->entity_cursor != 0);
    struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Sprite);
    SDL_assert(sprite_ptr != NULL);
    sprite_ptr->visible = false;
}

void Game_putPConMap(struct Game * in_game, int16_t * in_units, struct Point * in_pos_list, size_t load_num) {
    SDL_Log("Game_putPConMap\n");
    // Should be made into two functions:
    // createUnitEntity and putUnitEntityonMap.
    // Game_UnitEntity_Create();
    SDL_assert(in_game->map_ptr != NULL);
    char filename[DEFAULT_BUFFER_SIZE];

    for (int16_t i = 0; i < load_num; i++) {
        SDL_Log("Putting unit %s on map", unitNames[in_units[i]]);

        tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Unit, Position, Sprite);
        size_t typeflag_id1 = tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite));
        size_t typeflag_id2 = tnecs_typeflagid(in_game->world, in_game->world->entity_typeflags[temp_unit_ent]);
        SDL_Log("temp_unit_ent %d", temp_unit_ent);
        SDL_Log("typeflag %d", in_game->world->entity_typeflags[temp_unit_ent]);

        SDL_assert(in_game->world->entities[temp_unit_ent] == temp_unit_ent);
        SDL_assert(temp_unit_ent);
        SDL_assert(in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][in_game->world->num_entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))] - 1] == temp_unit_ent);
        SDL_assert(in_game->world->entity_typeflags[temp_unit_ent] == TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite));
        // SDL_Log("temp_unit_ent %d", temp_unit_ent);

        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Unit);
        SDL_assert(unit_ptr != NULL);
        memcpy(unit_ptr, &Unit_default, sizeof(Unit_default));
        *unit_ptr = in_game->party[in_units[i]];
        unit_ptr->weapons = &in_game->weapons;

        struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Position);
        SDL_assert(position_ptr != NULL);
        memcpy(position_ptr, &Position_default, sizeof(Position_default));
        position_ptr->onTilemap = true;
        Map_Bounds_Compute(in_game->map_ptr);
        Position_Bounds_Set(position_ptr, in_game->map_ptr->boundsmin.x, in_game->map_ptr->boundsmax.x, in_game->map_ptr->boundsmin.y, in_game->map_ptr->boundsmax.y);
        position_ptr->scale[0] = (float)in_game->settings.tilesize[0];
        position_ptr->scale[1] = (float)in_game->settings.tilesize[1];
        position_ptr->offset_px.x = in_game->map_ptr->offset_px.x;
        position_ptr->offset_px.y = in_game->map_ptr->offset_px.y;
        Position_Pos_Set(position_ptr, in_pos_list[i].x, in_pos_list[i].y);

        struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Sprite);
        SDL_assert(sprite_ptr != NULL);
        memcpy(sprite_ptr, &Sprite_default, sizeof(Sprite_default));
        strcpy(filename, "");
        strcat(filename, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"Units"DIR_SEPARATOR);
        strcat(filename, unitNames[in_units[i]]);
        strcat(filename, ".png");
        Sprite_Rects_init(sprite_ptr, in_pos_list[i]);
        Sprite_Texture_Set(sprite_ptr, filename);
        sprite_ptr->visible = true;

        SDL_assert(typeflag_id1 == typeflag_id2);
        SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][4]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][5]);
        Map_Unit_Put(in_game->map_ptr, in_game->world, in_pos_list[i].x, in_pos_list[i].y, temp_unit_ent);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][4]);
        SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][5]);
        SDL_assert(in_game->world->entities_bytype[typeflag_id2][(in_game->world->num_entities_bytype[typeflag_id2]) - 1] == temp_unit_ent);
    }
}

void Game_mapArrivals_Load(struct Game * in_game) {
    SDL_Log("Game_mapArrivals_Load\n");

    SDL_assert(in_game->map_ptr != NULL);
    char filename[DEFAULT_BUFFER_SIZE];
    char unitname[DEFAULT_BUFFER_SIZE];
    SDL_Log("1Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
    SDL_Log("1Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
    SDL_Log("1Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
    SDL_Log("1Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);

    for (int16_t i = 0; i < in_game->map_ptr->arrivals_num; i++) {
        if (in_game->map_ptr->arrivals[i].turn == in_game->map_ptr->turn) {
            size_t current_num = in_game->world->num_entities_bytype[TNECS_COMPONENT_NAMES2TYPEFLAGID(in_game->world, Unit, Position, Sprite)];
            SDL_Log("2Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
            SDL_Log("2Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
            SDL_Log("2Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
            SDL_Log("2Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
            tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Unit, Position, Sprite); // no... THIS LINE FUCKS UP THE ENTITIES BY TYPE.
            SDL_Log("3Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
            SDL_Log("3Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
            SDL_Log("3Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
            SDL_Log("3Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
            size_t typeflag_id1 = tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite));
            size_t typeflag_id2 = tnecs_typeflagid(in_game->world, in_game->world->entity_typeflags[temp_unit_ent]);
            SDL_assert(typeflag_id1 == typeflag_id2);
            SDL_assert(in_game->world->num_entities_bytype[TNECS_COMPONENT_NAMES2TYPEFLAGID(in_game->world, Unit, Position, Sprite)] > current_num);

            SDL_Log("temp_unit_ent %d", temp_unit_ent);
            SDL_Log("typeflag %d", in_game->world->entity_typeflags[temp_unit_ent]);

            SDL_assert(in_game->world->entities[temp_unit_ent] == temp_unit_ent);
            SDL_assert(temp_unit_ent);
            SDL_assert(in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][in_game->world->num_entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))] - 1] == temp_unit_ent);
            SDL_Log("entity_typeflag %d", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][in_game->world->num_entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))] - 1]);
            SDL_assert(in_game->world->entity_typeflags[temp_unit_ent] == TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite));


            struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Unit);
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
            SDL_assert(unit_ptr != NULL);
            *unit_ptr = Unit_default;
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
            // SDL_memcpy(unit_ptr, &Unit_default, sizeof(Unit));
            unit_ptr->weapons = &in_game->weapons;
            strncpy(unitname, "", 1);
            // strcat(unitname, ".."DIR_SEPARATOR"units"DIR_SEPARATOR);
            SDL_Log("arrival id %d", in_game->map_ptr->arrivals[i].id);
            strcat(unitname, unitNames[in_game->map_ptr->arrivals[i].id]);
            strcat(unitname, ".json");
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
            readJSON(unitname, unit_ptr); // this line fucks up the world.
            SDL_Log("post readJSON");
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
            SDL_Log("post assert");
            unit_ptr->army = in_game->map_ptr->arrivals[i].army;
            if (unit_ptr->_equipment[UNIT_HAND_RIGHT].id > 0) {
                Unit_equips(unit_ptr, UNIT_HAND_RIGHT);
            }
            if (unit_ptr->_equipment[UNIT_HAND_LEFT].id > 0) {
                Unit_equips(unit_ptr, UNIT_HAND_LEFT);
            }
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);

            struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Position);
            SDL_assert(position_ptr != NULL);
            *position_ptr = Position_default;
            // SDL_memcpy(position_ptr, &Position_default, sizeof(Position));
            position_ptr->onTilemap = true;
            Position_Bounds_Set(position_ptr, in_game->map_ptr->boundsmin.x, in_game->map_ptr->boundsmax.x, in_game->map_ptr->boundsmin.y, in_game->map_ptr->boundsmax.y);
            position_ptr->scale[0] = (float)in_game->settings.tilesize[0];
            position_ptr->scale[1] = (float)in_game->settings.tilesize[1];
            position_ptr->offset_px.x = in_game->map_ptr->offset_px.x;
            position_ptr->offset_px.y = in_game->map_ptr->offset_px.y;
            Position_Pos_Set(position_ptr, in_game->map_ptr->arrivals[i].position.x, in_game->map_ptr->arrivals[i].position.y);
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);

            struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, temp_unit_ent, Sprite);
            SDL_assert(sprite_ptr != NULL);
            *sprite_ptr = Sprite_default;
            // SDL_memcpy(sprite_ptr, &Sprite_default, sizeof(Sprite));
            strcpy(filename, "");
            strcat(filename, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"Units"DIR_SEPARATOR);
            strcat(filename, unitNames[in_game->map_ptr->arrivals[i].id]);
            strcat(filename, ".png");
            Sprite_Rects_init(sprite_ptr, in_game->map_ptr->arrivals[i].position);
            Sprite_Texture_Set(sprite_ptr, filename);
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);


            SDL_assert(in_game->world->entity_typeflags[temp_unit_ent] == TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite));
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);
            Map_Unit_Put(in_game->map_ptr, in_game->world, position_ptr->tilemap_pos.x, position_ptr->tilemap_pos.y, temp_unit_ent);
            SDL_assert(in_game->world->entities_bytype[typeflag_id1][(in_game->world->num_entities_bytype[typeflag_id1]) - 1] == temp_unit_ent);

            // in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][in_game->world->num_entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))] - 1] = temp_unit_ent;

        }
    }
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][4]);
    SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[tnecs_typeflagid(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][5]);
}

void Game_Party_Load(struct Game * in_game, int16_t * unit_ids, size_t load_num) {
    SDL_Log("Loading units\n");
    char * filename = SDL_malloc(DEFAULT_BUFFER_SIZE);
    struct Unit temp_unit;
    Game_Party_Clear(in_game);

    for (size_t i = 0; i < load_num; i++) {
        SDL_assert((unit_ids[i] > 0) && (unit_ids[i] < UNIT_NAME_PC_END));
        temp_unit = Unit_default;
        strcpy(filename, "units"PHYSFS_SEPARATOR);
        strcat(filename, unitNames[unit_ids[i]]);
        strcat(filename, ".json");
        temp_unit.weapons = &in_game->weapons;
        readJSON(filename, &temp_unit);
        in_game->party[unit_ids[i]] = temp_unit;
    }
}

void Game_Party_Clear(struct Game * in_game) {
    for (size_t i = 0; i < MAX_PARTY_SIZE; i++) {
        in_game->party[i] = Unit_default;
    }
}

void Game_Party_Unload(struct Game * in_game, int16_t * to_unload_ids, size_t unload_num) {
    for (size_t i = 0; i < unload_num; i++) {
        for (size_t j = 0; j < MAX_PARTY_SIZE; j++) {
            if (in_game->party[j]._id == to_unload_ids[i]) {
                in_game->party[j] = Unit_default;
            }
        }
    }
}

void Game_init(struct Game * in_game) {
    in_game->settings = Settings_default;
    int16_t flags = 0;

    strcpy(in_game->filename_menu, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"GUI"DIR_SEPARATOR"menu8px.png");
    if (in_game->menu_loaded != NULL) {
        arrfree(in_game->menu_loaded);
    }
    arrsetlen(in_game->menu_loaded, MENU_END);
    hmdefault(in_game->weapons, Weapon_default);

    if (in_game->settings.fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    Game_Menu_init(in_game);

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
        exit(ERROR_TTFInitFail);
    }

    Game_font = TTF_OpenFont(".."DIR_SEPARATOR"fonts"DIR_SEPARATOR"arial.ttf", in_game->settings.fontsize);
    in_game->menu_font = TTF_OpenFont(".."DIR_SEPARATOR"fonts"DIR_SEPARATOR"arial.ttf", in_game->settings.fontsize);

    SDL_assert(Game_font != NULL);
    // SDL_Log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());

    SDL_assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    SDL_Log("SDL subsystems initialized.\n");
    in_game->window = SDL_CreateWindow(in_game->settings.title, in_game->settings.pos.x, in_game->settings.pos.y, in_game->settings.res.x, in_game->settings.res.x, flags);

    if (in_game->window) {
        SDL_Log("Window created\n");
        Game_renderer = SDL_CreateRenderer(in_game->window, -1, 0);

        if (Game_renderer) {
            SDL_SetRenderDrawColor(Game_renderer, 255, 255, 255, 255);
            SDL_Log("Renderer created\n");
        }
    }

    SDL_Log("Allocating space for globals\n");
    controlMalloc();
    Events_Data_Malloc();

    SDL_Log("Initializing user events\n");
    Events_Names_Declare();
    Events_Receivers_Declare();

    SDL_Log("Initializing Menu Options\n");
    Menu_Options_init();

    SDL_Log("Genesis of tnecs\n");
    in_game->world = tnecs_world_genesis();
    SDL_assert(in_game->world->len_entities_bytype[0] > 0);

    SDL_Log("Components Registration\n");
    TNECS_REGISTER_COMPONENT(in_game->world, Position);             // 1
    TNECS_REGISTER_COMPONENT(in_game->world, Sprite);               // 2
    TNECS_REGISTER_COMPONENT(in_game->world, Unit);                 // 4
    TNECS_REGISTER_COMPONENT(in_game->world, Item);                 // 8
    TNECS_REGISTER_COMPONENT(in_game->world, Weapon);               // 16
    TNECS_REGISTER_COMPONENT(in_game->world, Text);                 // 32
    TNECS_REGISTER_COMPONENT(in_game->world, Menu);                 // 64
    TNECS_REGISTER_COMPONENT(in_game->world, controllerGamepad);    // 128
    TNECS_REGISTER_COMPONENT(in_game->world, controllerMouse);      // 256
    TNECS_REGISTER_COMPONENT(in_game->world, controllerKeyboard);   // 512
    TNECS_REGISTER_COMPONENT(in_game->world, controllerTouchpad);   // 1024
    TNECS_REGISTER_COMPONENT(in_game->world, UpdateTimer);          // 2048
    TNECS_REGISTER_COMPONENT(in_game->world, RenderTimer);          // 5096
    SDL_Log("System Registration\n");
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, drawSprite, 0, Sprite, Position);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, drawCursor, 0, Sprite, Position, controllerGamepad, controllerKeyboard, controllerTouchpad);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, drawMouse, 0, Sprite, Position, controllerMouse);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, drawMenu, 0, Menu);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, drawText, 0, Text, Position, UpdateTimer);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, controlMouse, 0, Sprite, Position, controllerMouse);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, controlKeyboard, 0, Position, Sprite, controllerKeyboard);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, controlGamepad, 0, Position, Sprite, controllerGamepad);
    TNECS_REGISTER_SYSTEM_WEXCL(in_game->world, controlTouchpad, 0, Position, Sprite, controllerTouchpad);

    in_game->isrunning = true;
    in_game->keyboardInputMap = KeyboardInputMap_default;
    in_game->gamepadInputMap = GamepadInputMap_default;
    in_game->mouseInputMap = MouseInputMap_default;
}

void Game_startup(struct Game * in_game, struct Input_Arguments in_args) {

    switch (in_args.startup_mode) {
        case STARTUP_SOUND_ROOM:
        case STARTUP_SCENE_ROOM:
        case STARTUP_SUPPORT_ROOM:
        case STARTUP_BATTLECONVOS_ROOM:
        case STARTUP_OTHERCONVOS_ROOM:
        case STARTUP_TITLESCREEN:
            break;
        case STARTUP_TESTING:
            SDL_Log("STARTUP_TESTING ");

            // SDL_ShowCursor(SDL_DISABLE); // for default cursor.
            Game_FPS_Create(in_game, DEFAULT_TEXT_UPDATETIME);
            Game_Cursor_Create(in_game);
            // Game_Mouse_Create(in_game);

            SDL_Log("Loading in test Map\n");
            strncpy(in_game->reason, "for testing", sizeof(in_game->reason));
            Game_State_Set(in_game, GAME_STATE_Gameplay_Map, in_game->reason);
            strncpy(in_game->reason, "on init state to GAME_STATE_Gameplay_Map substate is idle", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
            Game_Map_Load(in_game, CHAPTER_TEST);
            // Game_Mouse_State_Set(in_game, MENU_MAP);
            Game_cursorFocus_onMap(in_game);

            SDL_Log("Loading in test party\n");
            int16_t * unit_inds = NULL;
            arrput(unit_inds, UNIT_NAME_SILOU);
            Game_Party_Load(in_game, unit_inds, 1);
            struct Point * positions_list = NULL;
            struct Point temp_point = {.x = 6, .y = 6};
            arrput(positions_list, temp_point);
            Game_putPConMap(in_game, unit_inds, positions_list, 1);
            SDL_assert(in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0] == 3);


            SDL_Log("Loading in arrivals\n");
            Game_mapArrivals_Load(in_game);
            SDL_assert(in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1] == 4);
            SDL_assert(in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2] == 5);
            SDL_assert(in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3] == 6);
            SDL_assert(in_game->world->num_entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))] == 4);
            SDL_Log("Num_Entity with unit position Sprite %d\n", in_game->world->num_entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][0]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][1]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][2]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][3]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][4]);
            SDL_Log("Entity with unit position Sprite %d\n", in_game->world->entities_bytype[TNECS_TYPEFLAGID(in_game->world, TNECS_COMPONENT_NAMES2TYPEFLAG(in_game->world, Unit, Position, Sprite))][5]);
            break;
        default:
            SDL_Log("Startup mode is invalid.\n");
            exit(ERROR_InvalidStartup);
            break;
    }
}

void Game_Save_Copy(const int16_t from_ind, const int16_t to_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filenameto[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char filenamefrom[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char tempto[DEFAULT_BUFFER_SIZE];
    char tempfrom[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(tempto, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", to_ind);
    stbsp_snprintf(tempfrom, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", from_ind);
    strcat(filenameto, tempto);
    strcat(filenamefrom, tempfrom);
    SDL_Log("copy saveJSON Game from %s to %s\n", filenamefrom, filenameto);
    PHYSFS_file * pfrom = PHYSFS_openRead(filenamefrom);

    SDL_assert(pfrom != NULL);
    PHYSFS_file * pto = PHYSFS_openWrite(filenameto);
    int16_t len = PHYSFS_fileLength(pfrom);
    char longbuffer[len];
    PHYSFS_readBytes(pfrom, longbuffer, len);
    PHYSFS_writeBytes(pto, longbuffer, len);
    PHYSFS_close(pfrom);
    PHYSFS_close(pto);
}

void Game_Save_Delete(const int16_t save_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    SDL_Log("Deleting Game: %s\n", filename);
    PHYSFS_delete(filename);
}

void Game_loadJSON(struct Game * in_game, const int16_t save_ind) {
    SDL_assert(PHYSFS_exists(SAVE_FOLDER));
    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    cJSON * json = parseJSON(filename);
    // readJSON_narrative(json, &in_game->narrative);

    cJSON * jRN = cJSON_GetObjectItem(json, "RN");
    cJSON * jRN_status = cJSON_GetObjectItem(jRN, "Status");
    cJSON * jRN_mat1 = cJSON_GetObjectItem(jRN, "mat1");
    cJSON * jRN_mat2 = cJSON_GetObjectItem(jRN, "mat2");
    cJSON * jRN_tmat = cJSON_GetObjectItem(jRN, "tmat");

    in_game->tinymt32.mat1 = cJSON_GetNumberValue(jRN_mat1);
    in_game->tinymt32.mat2 = cJSON_GetNumberValue(jRN_mat2);
    in_game->tinymt32.tmat = cJSON_GetNumberValue(jRN_tmat);

    cJSON * jelement;
    int16_t i = 0;
    cJSON_ArrayForEach(jelement, jRN_status) {
        if ((i >= 0) && (i < 4)) {
            in_game->tinymt32.status[i] = cJSON_GetNumberValue(jelement);
            i++;
        }
    }

    cJSON * jconvoy = cJSON_GetObjectItem(json, "Convoy");
    Convoy_clear(&in_game->convoy);
    Convoy_readJSON(&in_game->convoy, jconvoy);
    //     convoy.setWeapons(&weapons);
    cJSON * jparty = cJSON_GetObjectItem(json, "Party");
    Game_Party_Clear(in_game);
    // cJSON * jcamp = cJSON_GetObjectItem(json, "Camp");
    // Camp_readJSON(&in_game->camp, jcamp);

    cJSON * junit = cJSON_GetObjectItem(jparty, "Unit");
    struct Unit temp_unit;

    in_game->party_size = 0;
    while (junit != NULL) {
        temp_unit = Unit_default;
        Unit_readJSON(&temp_unit, junit);
        in_game->party[in_game->party_size] = temp_unit;
        in_game->party_size++;
        junit = junit->next;
    }
}



void Game_saveJSON(struct Game * in_game, const int16_t save_ind) {
    if (!PHYSFS_exists(SAVE_FOLDER)) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }

    char filename[DEFAULT_BUFFER_SIZE] = SAVE_FOLDER;
    char temp[DEFAULT_BUFFER_SIZE];
    stbsp_snprintf(temp, DEFAULT_BUFFER_SIZE, DIR_SEPARATOR"save%04d.bsav", save_ind);
    strcat(filename, temp);
    SDL_Log("saveJSON Game to: %s\n", filename);
    PHYSFS_delete(filename);
    PHYSFS_file * fp = PHYSFS_openWrite(filename);

    SDL_assert(fp);
    cJSON * json = cJSON_CreateObject();
    // writeJSON_narrative(json, &in_game->narrative);
    cJSON * jparty = cJSON_CreateObject();
    cJSON * jconvoy = cJSON_CreateObject();
    // cJSON * jcamp = cJSON_CreateObject();
    cJSON * junit;

    cJSON * jRN = cJSON_CreateObject();
    cJSON * jRN_status = cJSON_CreateArray();
    cJSON * jRN_mat1 = cJSON_CreateNumber(in_game->tinymt32.mat1);
    cJSON * jRN_mat2 = cJSON_CreateNumber(in_game->tinymt32.mat2);
    cJSON * jRN_tmat = cJSON_CreateNumber(in_game->tinymt32.tmat);
    cJSON * jtemp;
    // SDL_Log("RnStatus[0] %d", tinymt32.status[0]);

    jtemp = cJSON_CreateNumber(in_game->tinymt32.status[0]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(in_game->tinymt32.status[1]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(in_game->tinymt32.status[2]);
    cJSON_AddItemToArray(jRN_status, jtemp);
    jtemp = cJSON_CreateNumber(in_game->tinymt32.status[3]);
    cJSON_AddItemToArray(jRN_status, jtemp);

    for (uint8_t i = 0; i < in_game->party_size; i++) {
        junit = cJSON_CreateObject();
        Unit_writeJSON(&in_game->party[i], junit);
        cJSON_AddItemToObject(jparty, "Unit", junit);
    }

    cJSON_AddItemToObject(jRN, "Status", jRN_status);
    cJSON_AddItemToObject(jRN, "mat1", jRN_mat1);
    cJSON_AddItemToObject(jRN, "mat2", jRN_mat2);
    cJSON_AddItemToObject(jRN, "tmat", jRN_tmat);
    cJSON_AddItemToObject(json, "RN", jRN);
    cJSON_AddItemToObject(json, "Party", jparty);
    Convoy_writeJSON(&in_game->convoy, jconvoy);
    // cJSON_AddItemToObject(json, "Camp", jcamp);
    // Camp_writeJSON(&in_game->camp, jcamp);
    cJSON_AddItemToObject(json, "Convoy", jconvoy);
    printJSON(fp, json);
    PHYSFS_close(fp);
    cJSON_Delete(json);
}

extern void Game_subState_Set(struct Game * in_game, const int8_t new_substate, const char * reason) {
    SDL_Log("Game_subState_Set to %d, because:", new_substate);
    SDL_Log("%s", reason);
    SDL_assert(new_substate > 0);
    SDL_assert(in_game->substate != new_substate);
    in_game->substate = new_substate;
    SDL_Log("New game substate: %s", gamesubStatenames[in_game->substate]);
}

extern void Game_State_Set(struct Game * in_game, const int8_t new_state, const char * reason) {
    SDL_Log("Game_State_Set to %d, for this reason:", new_state);
    SDL_Log("%s", reason);
    SDL_assert(new_state > 0);
    SDL_assert(in_game->state != new_state);
    in_game->state = new_state;
    SDL_Log("New game state: %s", gamesubStatenames[in_game->state]);
}

void Game_clean(struct Game * in_game) {
    controlFree();
    Events_Data_Free();
    SDL_DestroyWindow(in_game->window);
    SDL_DestroyRenderer(Game_renderer);
    SDL_Quit();
    tnecs_world_destroy(in_game->world);
    SDL_Log("Game cleaned.");
}

void Game_getDefenders(struct Game * in_game, tnecs_entity_t attacker_ent) {
    struct Unit * attacker_unit_ptr = TNECS_GET_COMPONENT(in_game->world, attacker_ent, Unit);
    struct Position * attacker_position_ptr = TNECS_GET_COMPONENT(in_game->world, attacker_ent, Position);
    if (in_game->defenders_ent != NULL) {
        arrfree(in_game->defenders_ent);
    }
    in_game->num_defenders = 0;

    int_tile_t * costmapp = NULL;
    arrsetlen(costmapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t * movemapp = NULL;
    arrsetlen(movemapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t * attackmapp = NULL;
    arrsetlen(attackmapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t move = 0;
    struct Point start;
    int_tile_t * range;

    SDL_assert(attacker_position_ptr != NULL);
    struct Point attacker_pos = attacker_position_ptr->tilemap_pos;
    struct Point attacker_offset = attacker_position_ptr->offset_px;
    start.x = attacker_pos.x - attacker_offset.x;
    start.y = attacker_pos.y - attacker_offset.y;

    SDL_assert(attacker_unit_ptr != NULL);
    range = Unit_computeRange(attacker_unit_ptr);
    move = Unit_effectiveStats(attacker_unit_ptr).move;

    costmapp = Map_Costmap_Movement_Compute(in_game->map_ptr, in_game->world, attacker_ent);
    movemapp = Pathfinding_Map_Move(costmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, start, move, POINTS_MATRIX);
    attackmapp = Pathfinding_Map_Attack(movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, move, range, POINTS_LIST, MOVETILE_INCLUDE);

    int16_t temp_row = 0;
    int16_t temp_col = 0;
    tnecs_entity_t temp_defender_ent = 0;
    struct Unit * temp_defender_unit_ptr = NULL;

    for (uint8_t i = 0; i < arrlen(attackmapp) / 2; i++) {
        temp_col = attackmapp[i * TWO_D + 0];
        temp_row = attackmapp[i * TWO_D + 1];
        temp_defender_ent = Map_Unit_Get(in_game->map_ptr, temp_row, temp_col);

        if (temp_defender_ent != 0) {
            temp_defender_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_defender_ent, Unit);
            if (utilities_army2alignment(attacker_unit_ptr->army) != utilities_army2alignment(temp_defender_unit_ptr->army)) {
                arrput(in_game->defenders_ent, temp_defender_ent);
                in_game->num_defenders++;
            }
        }
    }
}


void Game_getAudience(struct Game * in_game, tnecs_entity_t moved_ent) {
    struct Unit * moved_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Unit);
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Position);
    if (in_game->spectators_ent != NULL) {
        arrfree(in_game->spectators_ent);
    }
    in_game->num_spectators = 0;
    struct Point neighbor;
    tnecs_entity_t temp_spectator_ent = 0;
    struct Unit * temp_spectator_unit_ptr;
    for (int32_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        neighbor.x = equation_inbounds(position_ptr->tilemap_pos.x + cycle4_mppm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        neighbor.y = equation_inbounds(position_ptr->tilemap_pos.y + cycle4_ppmm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        if ((neighbor.x != position_ptr->tilemap_pos.x) & (neighbor.y != position_ptr->tilemap_pos.y)) {
            temp_spectator_ent = Map_Unit_Get(in_game->map_ptr, neighbor.x, neighbor.y);
            if (temp_spectator_ent != 0) {
                temp_spectator_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_spectator_ent, Unit);
                if (temp_spectator_unit_ptr->waits) {
                    arrput(in_game->spectators_ent, temp_spectator_ent);
                    in_game->num_spectators++;
                }
            }
        }
    }
}

void Game_getTalkers(struct Game * in_game, tnecs_entity_t moved_ent) {
    struct Unit * moved_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Unit);
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Position);
    if (in_game->talkers_ent != NULL) {
        arrfree(in_game->talkers_ent);
    }
    in_game->num_talkers = 0;
    struct Point neighbor;
    tnecs_entity_t temp_talker_ent = 0;
    struct Unit * temp_talker_unit_ptr;
    for (int32_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        neighbor.x = equation_inbounds(position_ptr->tilemap_pos.x + cycle4_mppm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        neighbor.y = equation_inbounds(position_ptr->tilemap_pos.y + cycle4_ppmm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        if ((neighbor.x != position_ptr->tilemap_pos.x) & (neighbor.y != position_ptr->tilemap_pos.y)) {
            temp_talker_ent = Map_Unit_Get(in_game->map_ptr, neighbor.x, neighbor.y);
            if (temp_talker_ent != 0) {
                temp_talker_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_talker_ent, Unit);
                if (temp_talker_unit_ptr->talkable > 0) {
                    arrput(in_game->talkers_ent, temp_talker_ent);
                    in_game->num_talkers++;
                }
            }
        }
    }
}

void Game_getPatients(struct Game * in_game, tnecs_entity_t healer_ent) {
    struct Unit * healer_unit_ptr = TNECS_GET_COMPONENT(in_game->world, healer_ent, Unit);
    struct Position * healer_position_ptr = TNECS_GET_COMPONENT(in_game->world, healer_ent, Position);
    if (in_game->patients_ent != NULL) {
        arrfree(in_game->patients_ent);
    }
    in_game->num_patients = 0;

    int_tile_t * costmapp = NULL;
    arrsetlen(costmapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t * movemapp = NULL;
    arrsetlen(movemapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t * healmapp = NULL;
    arrsetlen(healmapp, in_game->map_ptr->row_len * in_game->map_ptr->col_len);
    int_tile_t move = 0;
    struct Point start;
    int_tile_t * range;

    SDL_assert(healer_position_ptr != NULL);
    struct Point healer_pos = healer_position_ptr->tilemap_pos;
    struct Point healer_offset = healer_position_ptr->offset_px;
    start.x = healer_pos.x - healer_offset.x;
    start.y = healer_pos.y - healer_offset.y;

    SDL_assert(healer_unit_ptr != NULL);
    range = Unit_computeRange(healer_unit_ptr);
    move = Unit_effectiveStats(healer_unit_ptr).move;

    costmapp = Map_Costmap_Movement_Compute(in_game->map_ptr, in_game->world, healer_ent);
    movemapp = Pathfinding_Map_Move(costmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, start, move, POINTS_MATRIX);
    healmapp = Pathfinding_Map_Attack(movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, move, range, POINTS_LIST, MOVETILE_INCLUDE);

    int_tile_t temp_row = 0;
    int_tile_t temp_col = 0;
    tnecs_entity_t temp_patient_ent = 0;
    struct Unit * temp_patient_unit_ptr = NULL;

    for (uint8_t i = 0; i < arrlen(healmapp) / 2; i++) {
        temp_col = healmapp[i * TWO_D + 0];
        temp_row = healmapp[i * TWO_D + 1];
        temp_patient_ent = Map_Unit_Get(in_game->map_ptr, temp_row, temp_col);

        if (temp_patient_ent != 0) {
            temp_patient_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_patient_ent, Unit);
            if (utilities_army2alignment(healer_unit_ptr->army) != utilities_army2alignment(temp_patient_unit_ptr->army)) {
                arrput(in_game->defenders_ent, temp_patient_ent);
                in_game->num_defenders++;
            }
        }
    }
    arrfree(costmapp);
    arrfree(movemapp);
    arrfree(healmapp);
}

void Game_getTraders(struct Game * in_game, tnecs_entity_t moved_ent) {
    struct Unit * moved_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Unit);
    struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, moved_ent, Position);
    if (in_game->traders_ent != NULL) {
        arrfree(in_game->traders_ent);
    }
    in_game->num_traders = 0;
    struct Point neighbor;
    tnecs_entity_t temp_trader_ent = 0;
    struct Unit * temp_trader_unit_ptr;
    for (int32_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        neighbor.x = equation_inbounds(position_ptr->tilemap_pos.x + cycle4_mppm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        neighbor.y = equation_inbounds(position_ptr->tilemap_pos.y + cycle4_ppmm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        if ((neighbor.x != position_ptr->tilemap_pos.x) & (neighbor.y != position_ptr->tilemap_pos.y)) {
            temp_trader_ent = Map_Unit_Get(in_game->map_ptr, neighbor.x, neighbor.y);
            if (temp_trader_ent != 0) {
                temp_trader_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_trader_ent, Unit);
                if (utilities_army2alignment(moved_ptr->army) == utilities_army2alignment(temp_trader_unit_ptr->army)) {
                    arrput(in_game->traders_ent, temp_trader_ent);
                    in_game->num_traders++;
                }
            }
        }
    }
}

void Game_getRescuees(struct Game * in_game, tnecs_entity_t rescuer_ent) {
    struct Unit * rescuer_unit_ptr = TNECS_GET_COMPONENT(in_game->world, rescuer_ent, Unit);
    struct Position * rescuer_position_ptr = TNECS_GET_COMPONENT(in_game->world, rescuer_ent, Position);
    if (in_game->rescuees_ent != NULL) {
        arrfree(in_game->rescuees_ent);
    }
    in_game->num_rescuees = 0;
    struct Point neighbor;
    int32_t index[2] = {-1, 1};
    tnecs_entity_t temp_rescuee_ent = 0;
    struct Unit * temp_rescuee_unit_ptr = NULL;
    for (int32_t i = 0; i < SQUARE_NEIGHBOURS; i++) {
        neighbor.x = equation_inbounds(rescuer_position_ptr->tilemap_pos.x + cycle4_mppm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        neighbor.y = equation_inbounds(rescuer_position_ptr->tilemap_pos.y + cycle4_ppmm(i), in_game->map_ptr->row_len, in_game->map_ptr->col_len);
        if ((neighbor.x != rescuer_position_ptr->tilemap_pos.x) & (neighbor.y != rescuer_position_ptr->tilemap_pos.y)) {
            temp_rescuee_ent = Map_Unit_Get(in_game->map_ptr, neighbor.x, neighbor.y);
            if (temp_rescuee_ent != 0) {
                temp_rescuee_unit_ptr = TNECS_GET_COMPONENT(in_game->world, temp_rescuee_ent, Unit);
                if (utilities_army2alignment(rescuer_unit_ptr->army) == utilities_army2alignment(temp_rescuee_unit_ptr->army)) {
                    if (equation_canCarry(Unit_effectiveStats(rescuer_unit_ptr).con, Unit_effectiveStats(temp_rescuee_unit_ptr).con)) {
                        arrput(in_game->rescuees_ent, temp_rescuee_ent);
                        in_game->num_rescuees++;
                    }
                }
            }
        }
    }
}

void Game_canSeize(struct Game * in_game, tnecs_entity_t moved_ent) {

}

void Game_subStateSwitch_onMenupop(struct Game * in_game, int8_t menu_popped) {
    // if (in_game->menu_stack_num == 0) {
    //     strncpy(in_game->reason, "Last menu on stack was popped", sizeof(in_game->reason));
    //     Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
    //     Game_cursorFocus_onMap(in_game);
    // } else {
    SDL_assert((menu_popped > MENU_START) & (menu_popped < MENU_END));
    switch (menu_popped) {
        case MENU_UNIT_ACTION:
            strncpy(in_game->reason, "MENU_UNIT_ACTION was popped", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_MAP_UNIT_MOVES, in_game->reason);
            break;
        case MENU_MAP_ACTION:
            strncpy(in_game->reason, "MENU_MAP_ACTION was popped", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
            break;
        case MENU_UNIT_ITEMS:
            SDL_Log("MENU_UNIT_ITEMS was popped");
            strncpy(in_game->reason, "MENU_UNIT_ITEMS was popped", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
            break;
        default:
            strncpy(in_game->reason, "invalid menu was popped, but menu still exists", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
            break;
    }
    // if (in_game->menu_stack_num == 0) {
    // Game_cursorFocus_onMenu(in_game);
}

// // loss conditions
// bool essentialDied(Map * in_map, const Narrative * in_narrative) {
//     std_vector<uint16_t> essentials = in_map->getEssentials();

//     for (int16_t i = 0; i < essentials.size(); i++) {
//         if (in_narrative->death[essentials[i]]) {
//             return (true);
//         }
//     }

//     return (false);
// }

// // win conditions
// bool escaped(Map * in_map) {
//     return (false);
// }

// bool Seized(Map * in_map) {
//     //Maybe not necessary? maybe win is called when the throne is seized?
//     return (in_map->getSeized());
// }

// bool talked() {
//     return (false);
// }

// bool bossDied(Map * in_map) {
//     return (in_map->getBossDeath());
// }

// bool rout(Map * in_map) {
//     return ((in_map->getnumArrivals() < 1));
// }

// bool areAlive() {
//     return (false);
// }

// bool positionCondition(Unit * in_unit, Map_condition * in_mcond) {
//     // Called by the game everytime a unit moves.
//     if (in_mcond->army > 0) {
//         if (in_mcond->army != in_unit->getArmy()) {
//             return (false);
//         }
//     }

//     if (in_mcond->unit > 0) {
//         if (in_mcond->unit != in_unit->getid()) {
//             return (false);
//         }
//     }

//     // int16_t * unitpos = in_unit->getPos();

//     // if (unitpos[0] < std_max(0, (int16_t)in_mcond->colmin)) {
//     //     return(false);
//     // }
//     // if (unitpos[0] > std_min(255, (int16_t)in_mcond->colmax)) {
//     //     return(false);
//     // }
//     // if (unitpos[1] < std_max(0, (int16_t)in_mcond->rowmin)) {
//     //     return(false);
//     // }
//     // if (unitpos[1] > std_min(255, (int16_t)in_mcond->rowmax)) {
//     //     return(false);
//     // }
//     return (true);
// }

void Game_menuOptions_Update(struct Game * in_game, tnecs_entity_t * in_entity, char * in_name) {

}

void Game_menuOptions_Create(struct Game * in_game, tnecs_entity_t * in_entity, char * in_name) {
    SDL_Log("Game_menuOptions_Create");
    if (*in_entity == 0) {
        *in_entity = TNECS_ENTITY_CREATE_WCOMPONENTS(in_game->world, Position, Text, RenderTimer);
    }

    struct Text * text_ptr = TNECS_GET_COMPONENT(in_game->world, *in_entity, Text);
    SDL_assert(text_ptr !=  NULL);
    text_ptr->visible = true;
    text_ptr->fontsize = in_game->settings.fontsize;

    SDL_Color white = {255, 255, 255};
    text_ptr->text_color = white;

    text_ptr->padding[0] = Text_default.padding[0];
    text_ptr->padding[1] = Text_default.padding[1];
    text_ptr->padding[2] = Text_default.padding[2];
    text_ptr->padding[3] = Text_default.padding[3];
    text_ptr->fontsize = Text_default.fontsize;
    text_ptr->spacingfactor = Text_default.spacingfactor;

    text_ptr->text_color = white;

    text_ptr->sizefactor[0] = 1; // height, width
    text_ptr->sizefactor[1] = 1; // height, width

    strcpy(text_ptr->text_line, in_name);
    SDL_Log("Menu_option_Line: %s %s", in_name, text_ptr->text_line);
}

void Game_Menu_LocationfromCursor(struct Game * in_game, tnecs_entity_t in_menu_entity) {
    SDL_Log("Game_Menu_LocationfromCursor");
    struct Menu * menu_ptr = NULL;
    struct Position * cursor_pos_ptr = NULL;

    SDL_assert(in_game->entity_cursor);
    SDL_assert(in_menu_entity > 0);

    cursor_pos_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
    menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_menu_entity, Menu);
    SDL_assert(menu_ptr != NULL);
    if (cursor_pos_ptr != NULL) {
        if (cursor_pos_ptr->onTilemap) {
            menu_ptr->pos.x = cursor_pos_ptr->tilemap_pos.x * in_game->map_ptr->tilesize[0];
            menu_ptr->pos.y = cursor_pos_ptr->tilemap_pos.y * in_game->map_ptr->tilesize[0];
        } else {
            menu_ptr->pos.x = cursor_pos_ptr->pixel_pos.x;
            menu_ptr->pos.y = cursor_pos_ptr->pixel_pos.y;
        }
    } else {
        // why this value?
        menu_ptr->pos.x = in_game->settings.res.x / 2;
        menu_ptr->pos.y = in_game->settings.res.y / 2;
    }
}

void Game_Menu_LocationfromUnit(struct Game * in_game, tnecs_entity_t in_menu_entity, tnecs_entity_t in_unit_entity) {
    SDL_Log("Game_Menu_LocationfromUnit");
    struct Menu * menu_ptr = NULL;
    struct Position * unit_pos_ptr = NULL;

    SDL_assert(in_unit_entity > 0);
    SDL_assert(in_menu_entity > 0);
    unit_pos_ptr = TNECS_GET_COMPONENT(in_game->world, in_unit_entity, Position);
    menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_menu_entity, Menu);
    SDL_assert(menu_ptr != NULL);
    if (unit_pos_ptr != NULL) {
        if (unit_pos_ptr->onTilemap) {
            menu_ptr->pos.x = unit_pos_ptr->tilemap_pos.x * in_game->map_ptr->tilesize[0];
            menu_ptr->pos.y = unit_pos_ptr->tilemap_pos.y * in_game->map_ptr->tilesize[0];
        } else {
            menu_ptr->pos.x = unit_pos_ptr->pixel_pos.x;
            menu_ptr->pos.y = unit_pos_ptr->pixel_pos.y;
        }
    } else {
        // why this value?
        menu_ptr->pos.x = in_game->settings.res.x / 2;
        menu_ptr->pos.y = in_game->settings.res.y / 2;
    }
}


void makeContent_MENU_UNIT_ACTION(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_UNIT_ACTION");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_UNIT_ACTION];
    struct Position * unit_pos_ptr = NULL;
    SDL_assert(in_game->selected_unit_entity != 0);
    SDL_assert(menu_entity != 0);
    unit_pos_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Position);
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    menu_ptr->col_num = 1;
    menu_ptr->row_num = 0;
    arrfree(menu_ptr->menuoptions);

    MenuOption_ITEMS.pad_text = MenuOption_default.pad_text;
    MenuOption_ITEMS.pad_icon = MenuOption_default.pad_icon;
    MenuOption_ITEMS.pad_cell = MenuOption_default.pad_cell;
    Game_menuOptions_Create(in_game, &MenuOption_ITEMS.ent_text, menuOptionnames[MENU_OPTION_ITEMS]);
    arrput(menu_ptr->menuoptions, MenuOption_ITEMS);
    menu_ptr->row_num++;

    if (in_game->num_traders > 0) {
        MenuOption_TRADE.pad_text = MenuOption_default.pad_text;
        MenuOption_TRADE.pad_icon = MenuOption_default.pad_icon;
        MenuOption_TRADE.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_TRADE.ent_text, menuOptionnames[MENU_OPTION_TRADE]);
        arrput(menu_ptr->menuoptions, MenuOption_TRADE);
        menu_ptr->row_num++;
    }
    bool seize = false;
    if (seize) {
        MenuOption_SEIZE.pad_text = MenuOption_default.pad_text;
        MenuOption_SEIZE.pad_icon = MenuOption_default.pad_icon;
        MenuOption_SEIZE.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_SEIZE.ent_text, menuOptionnames[MENU_OPTION_SEIZE]);
        arrput(menu_ptr->menuoptions, MenuOption_SEIZE);
        menu_ptr->row_num++;
    }
    if (in_game->num_talkers > 0) {
        MenuOption_TALK.pad_text = MenuOption_default.pad_text;
        MenuOption_TALK.pad_icon = MenuOption_default.pad_icon;
        MenuOption_TALK.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_TALK.ent_text, menuOptionnames[MENU_OPTION_TALK]);
        arrput(menu_ptr->menuoptions, MenuOption_TALK);
        menu_ptr->row_num++;
    }
    if (in_game->num_defenders > 0) {
        MenuOption_ATTACK.pad_text = MenuOption_default.pad_text;
        MenuOption_ATTACK.pad_icon = MenuOption_default.pad_icon;
        MenuOption_ATTACK.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_ATTACK.ent_text, menuOptionnames[MENU_OPTION_ATTACK]);
        arrput(menu_ptr->menuoptions, MenuOption_ATTACK);
        menu_ptr->row_num++;
    }
    if (in_game->num_patients > 0) {
        MenuOption_STAFF.pad_text = MenuOption_default.pad_text;
        MenuOption_STAFF.pad_icon = MenuOption_default.pad_icon;
        MenuOption_STAFF.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_STAFF.ent_text, menuOptionnames[MENU_OPTION_STAFF]);
        arrput(menu_ptr->menuoptions, MenuOption_STAFF);
        menu_ptr->row_num++;
    }
    if (in_game->num_spectators > 0) {
        MenuOption_DANCE.pad_text = MenuOption_default.pad_text;
        MenuOption_DANCE.pad_icon = MenuOption_default.pad_icon;
        MenuOption_DANCE.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_DANCE.ent_text, menuOptionnames[MENU_OPTION_DANCE]);
        arrput(menu_ptr->menuoptions, MenuOption_DANCE);
        menu_ptr->row_num++;
    }
    if (in_game->num_rescuees > 0) {
        MenuOption_RESCUE.pad_text = MenuOption_default.pad_text;
        MenuOption_RESCUE.pad_icon = MenuOption_default.pad_icon;
        MenuOption_RESCUE.pad_cell = MenuOption_default.pad_cell;
        Game_menuOptions_Create(in_game, &MenuOption_RESCUE.ent_text, menuOptionnames[MENU_OPTION_RESCUE]);
        arrput(menu_ptr->menuoptions, MenuOption_RESCUE);
        menu_ptr->row_num++;
    }
    MenuOption_WAIT.pad_text = MenuOption_default.pad_text;
    MenuOption_WAIT.pad_icon = MenuOption_default.pad_icon;
    MenuOption_WAIT.pad_cell = MenuOption_default.pad_cell;
    Game_menuOptions_Create(in_game, &MenuOption_WAIT.ent_text, menuOptionnames[MENU_OPTION_WAIT]);
    arrput(menu_ptr->menuoptions, MenuOption_WAIT);
    menu_ptr->row_num++;

    Game_Menu_LocationfromUnit(in_game, menu_entity, in_game->selected_unit_entity);
}

void makeContent_MENU_UNIT_ITEMS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_UNIT_ITEMS");

    struct Menu * menu_ptr = NULL;
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_UNIT_ITEMS];
    struct Unit * unit_ptr = NULL;
    char item_name [DEFAULT_BUFFER_SIZE];

    SDL_assert(in_game->selected_unit_entity != 0);
    SDL_assert(menu_entity != 0);
    unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
    struct Inventory_item * equipment = unit_ptr->_equipment;
    menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    menu_ptr->col_num = 1;
    menu_ptr->row_num = 0;
    struct MenuOption temp_option;
    struct Text * text_ptr = NULL;
    for (uint8_t i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        SDL_Log("item number %d", i);
        if ((equipment[i].id > 0) & (in_game->weapons != NULL)) {
            SDL_Log("item id %d", equipment[i].id);
            temp_option = MenuOption_default;
            strncpy(item_name, hmget(in_game->weapons, equipment[i].id).item->name, sizeof(item_name));
            Game_menuOptions_Create(in_game, &temp_option.ent_text, item_name);
            text_ptr = TNECS_GET_COMPONENT(in_game->world, temp_option.ent_text, Text);
            strncpy(text_ptr->text_line, item_name, sizeof(text_ptr->text_line));
            arrput(menu_ptr->menuoptions, temp_option);
            menu_ptr->row_num++;
        }
    }
    Game_Menu_LocationfromUnit(in_game, menu_entity, in_game->selected_unit_entity);
}


void makeContent_MENU_COMBAT_FORECAST(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_COMBAT_FORECAST");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_COMBAT_FORECAST];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    tnecs_entity_t * attacker = (tnecs_entity_t *) data_1;
    tnecs_entity_t * defenders = (tnecs_entity_t *) data_2;
}


void makeContent_MENU_COMBAT_FORECAST_CHOOSE_DEFENDERS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_COMBAT_FORECAST_CHOOSE_DEFENDERS");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_COMBAT_FORECAST_CHOOSE_DEFENDERS];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    tnecs_entity_t * attacker = (tnecs_entity_t *) data_1;
    tnecs_entity_t * defenders = (tnecs_entity_t *) data_2;
}

void makeContent_MENU_COMBAT_FORECAST_CHOOSE_WEAPONS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_COMBAT_FORECAST_CHOOSE_WEAPONS");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_COMBAT_FORECAST_CHOOSE_WEAPONS];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    Text * text_ptr = NULL;
    struct MenuOption temp_option;
    struct Unit * attacker = TNECS_GET_COMPONENT(in_game->world, in_game->attacker_ent, Unit);
    for (uint8_t i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (attacker->_equipment[i].id != 0) {
            temp_option = MenuOption_default;
            TNECS_ADD_COMPONENT(in_game->world, temp_option.ent_text, Text);
            text_ptr = TNECS_GET_COMPONENT(in_game->world, temp_option.ent_text, Text);

            if (in_game->weapons != NULL) {
                // strncpy(in_game, "Last menu on stack was popped", sizeof(in_game->reason));
                strcpy(text_ptr->text_line, hmget(in_game->weapons, attacker->_equipment[i].id).item->name);
            }
            arrput(menu_ptr->menuoptions, temp_option);
            menu_ptr->row_num++;
        }
    }
}

void makeContent_MENU_RESCUE(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_RESCUE");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_RESCUE];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    struct MenuOption temp_option;
    for (uint8_t i = 0; i < in_game->num_rescuees; i++) {
        temp_option = MenuOption_default;
        temp_option.data = & in_game->rescuees_ent[i];
    }
}
void makeContent_MENU_UNITS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_UNITS");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_UNITS];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
}

void makeContent_MENU_OPTIONS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_OPTIONS");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_OPTIONS];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
}

void makeContent_MENU_DANCE(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_DANCE");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_DANCE];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    struct MenuOption temp_option;
    for (uint8_t i = 0; i < in_game->num_spectators; i++) {
        temp_option = MenuOption_default;
        temp_option.data = & in_game->spectators_ent[i];
    }
}

void makeContent_MENU_SHOP(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_SHOP");
    // Shop can be an armory, an item shop, a Convoy upgrade shop (carpenter?)
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_SHOP];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    *menu_ptr = Menu_default;
}

void makeContent_MENU_STAFF(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_CONVOY");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_STAFF];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    struct MenuOption temp_option;
    for (uint8_t i = 0; i < in_game->num_patients; i++) {
        temp_option = MenuOption_default;
        temp_option.data = &in_game->patients_ent[i];
    }
}

void makeContent_MENU_CONVOY(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_CONVOY");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_CONVOY];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
}

void makeContent_MENU_MAP_ACTION(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_MAP_ACTION");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_MAP_ACTION];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    menu_ptr->col_num = 1;
    menu_ptr->row_num = 0;
    arrfree(menu_ptr->menuoptions);

    MenuOption_GLOBAL_RANGE.pad_text = MenuOption_default.pad_text;
    MenuOption_GLOBAL_RANGE.pad_icon = MenuOption_default.pad_icon;
    MenuOption_GLOBAL_RANGE.pad_cell = MenuOption_default.pad_cell;
    Game_menuOptions_Create(in_game, &MenuOption_GLOBAL_RANGE.ent_text, menuOptionnames[MENU_OPTION_GLOBAL_RANGE]);
    arrput(menu_ptr->menuoptions, MenuOption_GLOBAL_RANGE);
    menu_ptr->row_num++;

    MenuOption_END_TURN.pad_text = MenuOption_default.pad_text;
    MenuOption_END_TURN.pad_icon = MenuOption_default.pad_icon;
    MenuOption_END_TURN.pad_cell = MenuOption_default.pad_cell;
    Game_menuOptions_Create(in_game, &MenuOption_END_TURN.ent_text, menuOptionnames[MENU_OPTION_END_TURN]);
    arrput(menu_ptr->menuoptions, MenuOption_END_TURN);
    menu_ptr->row_num++;

    Game_Menu_LocationfromCursor(in_game, menu_entity);
}

void makeContent_MENU_TRADE(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_TRADE");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_TRADE];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
}

void makeContent_MENU_UNIT_STATS(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_UNIT_STATS");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_UNIT_STATS];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
}

void makeContent_MENU_TALK(struct Game * in_game, void * data_1, void * data_2) {
    SDL_Log("makeContent_MENU_TALK");
    tnecs_entity_t menu_entity = in_game->menu_loaded[MENU_TALK];
    struct Menu * menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity, Menu);
    struct MenuOption temp_option;
    for (uint8_t i = 0; i < in_game->num_talkers; i++) {
        temp_option = MenuOption_default;
        temp_option.data = &in_game->talkers_ent[i];
    }
}

bool Game_Menu_init(struct Game * in_game) {
    bool crashed = 0;
    if (in_game->menuContentMakers != NULL) {
        arrfree(in_game->menuContentMakers);
    }
    arrput(in_game->menuContentMakers, NULL);
#define REGISTER_ENUM(x) arrput(in_game->menuContentMakers, &makeContent_MENU_##x);
#include "names/menu.h"
#undef REGISTER_ENUM
    return (crashed);
}
