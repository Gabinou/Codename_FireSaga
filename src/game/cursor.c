#include "game/cursor.h"

/* -- Focus -- */
void Game_cursorFocus_onMap(struct Game *sota) {
    tnecs_entity cursor = sota->entity_cursor;
    SDL_assert(cursor > TNECS_NULL);

    /* - Load square animated cursor - */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);
    SDL_assert(sprite != NULL);
    char *path = PATH_JOIN("..", "assets", "GUI", "Cursor", "mapcursors.png");
    Sprite_Load(sprite, path, sota->renderer);
    struct Spritesheet *ss = sprite->spritesheet;

    ss->frames[0] = sota->settings.cursor.frames;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = sota->settings.cursor.speed;

    // sprite->frames = sota->settings.cursor.frames;
    // sprite->frames_shadow = sota->settings.cursor.frames;
    // sprite->speed = sota->settings.cursor.speed;
    sprite->scale.x = 1;
    sprite->scale.y = 1;

    /* - Setting tilesize - */
    Sprite_Tilesize_Set(sprite, sota->settings.tilesize);

    /* - Setting Slider - */
    struct Slider *slider = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    SDL_assert(slider != NULL);
    slider->slidetype = SLIDETYPE_GEOMETRIC;

    /* - Set position to tilemap - */
    struct Position *position = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(position != NULL);
    position->onTilemap = true;
    Map_Bounds_Compute(sota->map);

    struct Point min = sota->map->boundsmin;
    struct Point max = sota->map->boundsmax;
    Position_Bounds_Set(position, min.x, max.x, min.y, max.y);

    /* -- Placing cursor on map -- */
    i32 map_tilesize[2] = {sota->map->tilesize[0], sota->map->tilesize[1]};
    int row_len = sota->map->row_len;
    int col_len = sota->map->col_len;
    if (sota->ismouse) {
        struct Point mouse_pos, tilemap_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, map_tilesize[0],
                                           sota->camera.offset.x, sota->camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, map_tilesize[1],
                                           sota->camera.offset.y, sota->camera.zoom);
        tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, col_len - 1);
        tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, row_len - 1);
        position->tilemap_pos.x = tilemap_pos.x;
        position->tilemap_pos.y = tilemap_pos.y;
    } else if (sota->iscursor) {
        int lastposx = sota->cursor_lastpos.x;
        int lastposy = sota->cursor_lastpos.y;
        position->tilemap_pos.x = int_inbounds(lastposx, 0, col_len - 1);
        position->tilemap_pos.y = int_inbounds(lastposy, 0, row_len - 1);
    }

    sota->cursor_lastpos.x = position->tilemap_pos.x;
    sota->cursor_lastpos.y = position->tilemap_pos.y;

    position->pixel_pos.x = sprite->tilesize[0] * (position->tilemap_pos.x - 1);
    position->pixel_pos.y = sprite->tilesize[1] * (position->tilemap_pos.y - 1);
    position->scale[0] = (float)map_tilesize[0];
    position->scale[1] = (float)map_tilesize[1];

    sota->map->update = true;
}

void Game_cursorFocus_onMenu(struct Game *sota) {
    // focuses on menu on top of sota->menu_stack

    /* - Preliminaries - */
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);

    /* cursor */
    tnecs_entity cursor = sota->entity_cursor;
    SDL_assert(cursor != 0);
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(cursor_pos != NULL);

    /* menu_stack top */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top = sota->menu_stack[stack_top];
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;

    /* sprite */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);
    SDL_assert(sprite != NULL);

    /* - Setting cursor - */
    cursor_pos->onTilemap = false;

    /* - cursor position on menu - */
    /* DESIGN: Reset cursor pos to elem 0 or keep elem in memory? Or something else? */
    switch (mc->type) {
        /* RESET */
        case MENU_TYPE_WEAPON_SELECT:
            mc->elem = LSM_ELEM_ITEM1;
            break;
        case MENU_TYPE_TRADE:
        case MENU_TYPE_ITEM_SELECT:
        case MENU_TYPE_PLAYER_SELECT:
            mc->elem = 0;
            break;
        /* NOT RESET */
        case MENU_TYPE_STATS:
        case MENU_TYPE_GROWTHS:
        default:
            break;
    }

    cursor_pos->pixel_pos.x = mc->elem_pos[mc->elem].x;
    cursor_pos->pixel_pos.y = mc->elem_pos[mc->elem].y;
    Cursor_Box_Offset(&cursor_pos->pixel_pos);

    /* disabling menues on stack bottom */
    for (int i = 0; i < stack_top; i++) {
        tnecs_entity menu = sota->menu_stack[i];
        struct Menu *mc_inv = TNECS_GET_COMPONENT(sota->world, menu, Menu);
        mc_inv->visible = false;
    }

    // TODO: load menu cursor.
    char *path = PATH_JOIN("..", "assets", "GUI", "Cursor", "mapcursors.png");
    Sprite_Load(sprite, path, sota->renderer);
    SDL_assert(sprite->spritesheet != NULL);
    struct Spritesheet *ss = sprite->spritesheet;
    SDL_assert(ss->frames != NULL);
    ss->frames[0] = sota->settings.cursor.frames;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = sota->settings.cursor.speed;

    sprite->scale.x = 8;
    sprite->scale.y = 8;
    Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
    Cursor_Dstrect_Absolute(sprite, &cursor_pos->pixel_pos, &sota->camera);
}

/* -- Follows mouse -- */
/* if the cursor is far away from elem box, move in the direction. */
void Game_CursorfollowsMouse_onMenu(struct Game *sota) {

    /* --- SKIPPING --- */
    tnecs_entity menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    /* - Skip if Keyboard/Gamepad set sota->cursor_move -*/
    bool skip = !((sota->cursor_move.x == 0) && (sota->cursor_move.y == 0));
    skip |= (menu == TNECS_NULL);
    skip |= !sota->ismouse;
    skip |= sota->iscursor;
    skip |= (sota->entity_mouse <= TNECS_NULL);
    skip |= (sota->entity_cursor <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SLIDING --- */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_position = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    bool slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    bool slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (!(slidex || slidey)) {
        return;
    }

    /* -- Menu type -- */
    /* - menu pos - */
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu, Menu);

    /* - elem pos & elem box - */
    SDL_assert(mc->elem < mc->elem_num);
    SDL_assert(mc->elem > MENU_ELEM_NULL);
    struct Point elem_pos = mc->elem_pos[mc->elem];
    struct Point elem_box = mc->elem_box[mc->elem];
    SDL_assert((elem_box.x > -1) && (elem_box.y > -1));
    SDL_assert((elem_pos.x > -1) && (elem_pos.y > -1));

    /* - mouse pos - */
    struct Point mouse_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    /* - cursor in box - */
    struct Point max_pos = {elem_pos.x + elem_box.x, elem_pos.y + elem_box.y};
    // is 1 if >, is -1 if <, otherwise 0
    sota->cursor_move.y = (mouse_pos.y > max_pos.y) - (mouse_pos.y < elem_pos.y);
    sota->cursor_move.x = (mouse_pos.x > max_pos.x) - (mouse_pos.x < elem_pos.x);
    /* - prioritize y - */
    if ((sota->cursor_move.x != 0) || (sota->cursor_move.x != 0)) {
        sota->moved_direction = Ternary_Direction(sota->cursor_move);
        i8 new_elem = Menu_Elem_Move(mc, sota->moved_direction);
        if (new_elem != MENU_ELEM_NULL)
            sota->controller_code = CONTROLLER_MOUSE;
    }

}

/* -- Moves -- */
void Game_Cursor_movedTime_Compute(struct Game *sota, uint64_t time_ns) {
    // Compute time that cursor has been moving
    if (sota->cursor_frame_moved)
        sota->cursor_moved_time_ms += time_ns / SOTA_us;
    else {
        sota->cursor_moved_time_ms = 0;
        sota->cursor_move.x = sota->cursor_move.y = 0;
    }
}

bool Game_isCursoronTilemap(struct Game *sota) {
    bool out = false;
    if (sota->ismouse) {
        tnecs_entity mouse = sota->entity_mouse;
        struct Point mouse_pos, tilemap_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, sota->map->tilesize[0],
                                           sota->camera.offset.x, sota->camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, sota->map->tilesize[1],
                                           sota->camera.offset.y, sota->camera.zoom);
        bool x_isIn = int_inbounds(tilemap_pos.x, 0, sota->map->col_len - 1);
        bool y_isIn = int_inbounds(tilemap_pos.y, 0, sota->map->row_len - 1);
        out = (x_isIn && y_isIn);
    }
    return (out);
}

void Game_CursorfollowsMouse_onMap(struct Game *sota) {
    SDL_assert(sota != NULL);
    SDL_assert(sota->world != NULL);
    SDL_assert(sota->map != NULL);
    /* --- SKIPPING --- */
    /* - skip if any sota->cursor_move not 0 (other controller moved) - */
    bool skip = ((sota->cursor_move.x != 0) || (sota->cursor_move.y != 0));
    skip |= !sota->ismouse;
    skip |= sota->iscursor;
    skip |= (sota->entity_mouse <= TNECS_NULL);
    skip |= (sota->entity_cursor <= TNECS_NULL);
    if (skip) {
        return;
    }

    /* - Skip if cursor is sliding - */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_position = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    bool slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    bool slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    /* - Get cursor tile position - */
    struct Point mouse_pos, tilemap_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, sota->map->tilesize[0],
                                       sota->camera.offset.x, sota->camera.zoom);
    tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, sota->map->tilesize[1],
                                       sota->camera.offset.y, sota->camera.zoom);
    tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, sota->map->col_len - 1);
    tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, sota->map->row_len - 1);

    /* - Computing lastpos, move_data - */
    sota->cursor_lastpos.x = cursor_position->tilemap_pos.x;
    sota->cursor_lastpos.y = cursor_position->tilemap_pos.y;
    int tilemap_move_x = tilemap_pos.x - sota->cursor_lastpos.x;
    int tilemap_move_y = tilemap_pos.y - sota->cursor_lastpos.y;

    /* -- Scaling down cursor movement to one tile -- */
    int x_abs = abs(tilemap_move_x), y_abs = abs(tilemap_move_y);
    /* - Prioritize vertical movement - */
    if ((y_abs >= x_abs) && (y_abs > 0))
        sota->cursor_move.y = tilemap_move_y / y_abs;
    else if (x_abs > y_abs)
        sota->cursor_move.x = tilemap_move_x / x_abs;

    if ((sota->cursor_move.x != 0) || (sota->cursor_move.y != 0))
        sota->controller_code = CONTROLLER_MOUSE;

}
void Game_Cursor_Moves_onMenu(struct Game *sota) {
    // For basically all states except Gameplay_map.standby
    tnecs_entity menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    Game_Cursor_Moves_Straight(sota);
    bool skip = ((sota->cursor_move.x == 0) && (sota->cursor_move.y == 0));
    skip |= (menu == TNECS_NULL);

    if (!skip)
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor_move, &sota->controller_code);
}

void Game_Cursor_Moves_Straight(struct Game *sota) {
    // Reduces cursor movement to only straight.
    // Prioritizes y
    sota->cursor_move.x *= (sota->cursor_move.y == 0);
}

void Game_Cursor_Move_toCandidate(struct Game *sota) {
    // Set cursor to new candidate position
    tnecs_entity candidate = sota->candidates[sota->candidate];
    struct Position *candidate_pos, *cursor_pos;
    candidate_pos = TNECS_GET_COMPONENT(sota->world, candidate, Position);
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;
    SDL_Log("tilemap_pos.x %d %d", cursor_pos->tilemap_pos.x, cursor_pos->tilemap_pos.y);

}
void Game_Cursor_Next_Candidate(struct Game *sota) {
    tnecs_entity menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    Game_Cursor_Moves_Straight(sota);
    bool skip = ((sota->cursor_move.x == 0) && (sota->cursor_move.y == 0));
    skip |= (menu == TNECS_NULL);
    // TODO: move functionality to event?
    if (!skip)
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor_move,
                   &sota->controller_code);

}

void Game_Cursor_Moves_onMap(struct Game *sota) {
    /* --- SKIPPING --- */
    /* - skip if any sota->cursor_move 0 - */
    bool skip = ((sota->cursor_move.x == 0) && (sota->cursor_move.y == 0));
    skip |= (sota->entity_cursor <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SENDING --- */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *position;
    position = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(position != NULL);

    /* -- Cursor_Moves should not be sent WHEN: -- */
    bool canSend = true;
    //    1- cursor is in sliding animation.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE

    /* - Skip if cursor is sliding - */
    struct Position *cursor_position = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    bool slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    bool slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    //    2- cursor is blocked by CURSOR_FIRSTMOVE_PAUSE.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE
    //    3- cursor is stuck on the edges of the map.
    if (!sota->cursor_diagonal)
        Game_Cursor_Moves_Straight(sota);
    int cx = sota->cursor_move.x, cy = sota->cursor_move.y;
    int tx = position->tilemap_pos.x, ty = position->tilemap_pos.y;
    int cl = sota->map->col_len, rl = sota->map->row_len;
    canSend &= !((cx == -1)  && (cy == 0) && (tx == 0)); /* x- */
    canSend &= !((cx == 0)  && (cy == -1) && (ty == 0)); /* y- */
    canSend &= !((cx == 1)  && (cy == 0) && (tx == (cl - 1))); /* x+ */
    canSend &= !((cx == 0)  && (cy == 1) && (ty == (rl - 1))); /* y+ */
    //    4- cursor is stuck on the corners of the map.
    canSend &= !((cx == 1)  && (cy == 1) && (tx == (cl - 1)) && (ty == (rl - 1))); /* x+y+ */
    canSend &= !((cx == -1)  && (cy == 1) && (tx == 0) && (ty == (rl - 1))); /* x-y+ */
    canSend &= !((cx == 1)  && (cy == -1) && (tx == (cl - 1)) && (ty == 0)); /* x+y- */
    canSend &= !((cx == -1) && (cy == -1) && (tx == 0) && (ty == 0)); /* x-y- */
    #ifndef INFINITE_MOVE_ALL
    //    5- cursor is NOT in movemap IN map_unit move SUBSTATE
    if (sota->substate == GAME_SUBSTATE_MAP_UNIT_MOVES) {
        // SDL_Log("sota->cursor_move %d %d", sota->cursor_move.x, sota->cursor_move.y);
        int nx = int_inbounds(tx + cx, 0, sota->map->col_len - 1);
        int ny = int_inbounds(ty + cy, 0, sota->map->row_len - 1);
        canSend &= (sota->map->movemap[ny * cl + nx] >= NMATH_MOVEMAP_MOVEABLEMIN);
    }
    #endif /* INFINITE_MOVE_ALL */
    if (canSend)
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor_move,
                   &sota->controller_code);

}

/* --- Cursor --- */
void Game_Cursor_Create(struct Game *sota) {
    Game_Cursor_Free(sota);
    sota->entity_cursor = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Slider,
                                                          Position, Sprite,
                                                          controllerKeyboard,
                                                          controllerGamepad,
                                                          controllerTouchpad,
                                                          Timer, CursorFlag);
    SDL_assert(sota->entity_cursor != 0);
    struct controllerKeyboard *keyboard;
    keyboard = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
    SDL_assert(keyboard != NULL);
    *keyboard = controllerKeyboard_default;
    keyboard->inputmap = &sota->keyboardInputMap;
    struct controllerGamepad *gamepad;
    gamepad = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
    SDL_assert(gamepad != NULL);

    *gamepad = controllerGamepad_default;
    Gamepad_Init(gamepad);
    gamepad->inputmap = &sota->gamepadInputMap;

    struct Slider *slider;
    slider = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Slider);
    SDL_assert(slider != NULL);
    *slider = Slider_default;
    slider->slidefactors[DIMENSION_X] = SOTA_CURSOR_SLIDEFACTOR;
    slider->slidefactors[DIMENSION_Y] = SOTA_CURSOR_SLIDEFACTOR;
    slider->update_wait_ns = CURSOR_SLIDEWAIT_ns;
    slider->slidetype = SLIDETYPE_GEOMETRIC;

    /* -- sprite -- */
    struct Timer *timer;
    timer = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Timer);
    SDL_assert(timer != NULL);
    * timer = Timer_default;

    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    sprite->tilesize[0] = sota->settings.tilesize[0];
    sprite->tilesize[1] = sota->settings.tilesize[1];
    sprite->visible = true;
    // sprite->sprite_type = DESTRECT_FUNCTION_CURSOR;

    SDL_assert(sprite->spritesheet == NULL);
    sprite->spritesheet = malloc(sizeof(*sprite->spritesheet));
    SDL_assert(sprite->spritesheet != NULL);

    *sprite->spritesheet    = Spritesheet_default;
    struct Spritesheet *ss  = sprite->spritesheet;
    ss->loop_num    = 1;
    ss->speeds      = malloc(ss->loop_num * sizeof(*ss->speeds));
    ss->frames      = malloc(ss->loop_num * sizeof(*ss->frames));
    ss->loops_pos   = calloc(ss->loop_num,  sizeof(*ss->loops_pos));
    ss->loop_modes  = malloc(ss->loop_num * sizeof(*ss->loop_modes));

    ss->frames[0] = sota->settings.cursor.frames;
    ss->speeds[0] = malloc(ss->frames[0] * sizeof(**ss->speeds));
    ss->loop_modes[0] = LOOPING_PINGPONG;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = sota->settings.cursor.speed;
    SDL_assert(ss->current_loop == 0);

    Cursor_Rects_Init(sprite);
    SDL_assert(sprite->srcrect.w == 64);
    SDL_assert(sprite->srcrect.h == 64);
    struct Position *position = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(position != NULL);
    *position = Position_default;
}

void Game_Cursor_Free(struct Game *sota) {
    if (sota->entity_cursor != 0) {
        struct controllerGamepad *gamepad;
        gamepad = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
        SDL_assert(gamepad != NULL);
        Gamepad_Free(gamepad);

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
        SDL_assert(sprite != NULL);
        Sprite_Free(sprite);

        tnecs_entity_destroy(sota->world, sota->entity_cursor);
    }
}

void Game_Cursor_Enable(struct Game *sota) {
    sota->iscursor = true;
    sota->controller_code = CONTROLLER_KEYBOARD;
    SDL_assert(sota->entity_cursor != 0);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Cursor_Disable(struct Game *sota) {
    sota->iscursor = false;
    SDL_assert(sota->entity_cursor != 0);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    #ifndef DEBUG_CURSOR_SHOW_wDISABLED
    sprite->visible = false;
    #endif
}

/* --- Mouse --- */
void Game_Mouse_Create(struct Game *sota) {
    Game_Mouse_Free(sota);
    sota->entity_mouse = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, controllerMouse,
                                                         Position, Sprite, MouseFlag);
    struct Position *position;
    position = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Position);
    struct controllerMouse *mouse;
    mouse = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, controllerMouse);
    mouse->inputmap = MouseInputMap_default;

    position->onTilemap = false;
    SDL_assert(position != NULL);
    Position_Bounds_Set(position, -1000, 2000, -1000, 2000);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    *sprite = Sprite_default;
    SDL_assert(sprite != NULL);
    Sprite_Load(sprite, PATH_JOIN("..", "assets", "GUI", "Cursor", "mousecursor.png"), sota->renderer);
    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->surface != NULL);
    SDL_assert(sprite->texture != NULL);
    sprite->visible = false;
    sprite->flip = SDL_FLIP_NONE;

    *sprite->spritesheet = Spritesheet_default;

    struct Spritesheet *ss = sprite->spritesheet;
    ss->loop_num      = 1;
    ss->frames        = malloc(ss->loop_num * sizeof(*ss->frames));
    ss->speeds        = malloc(ss->loop_num * sizeof(*ss->speeds));
    ss->loops_pos     = calloc(ss->loop_num,  sizeof(*ss->loops_pos));
    ss->loop_modes    = malloc(ss->loop_num * sizeof(*ss->loop_modes));
    ss->loop_modes[0] = LOOPING_INVALID;

    ss->frames[0]     = 1;
    ss->speeds[0]     = calloc(ss->frames[0], sizeof(**ss->speeds));
    SDL_assert(ss->current_loop == 0);

    sprite->srcrect.w = MOUSE_DEFAULT_SRCRECT_W;
    sprite->srcrect.h = MOUSE_DEFAULT_SRCRECT_H;
    sprite->dstrect.w = MOUSE_DEFAULT_DSTRECT_W;
    sprite->dstrect.h = MOUSE_DEFAULT_DSTRECT_H;
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

}

void Game_Mouse_Enable(struct Game *sota) {
    sota->ismouse = true;
    sota->controller_code = CONTROLLER_MOUSE;
    SDL_assert(sota->entity_mouse != 0);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Mouse_Disable(struct Game *sota) {
    sota->ismouse = false;
    sota->controller_code = CONTROLLER_KEYBOARD;
    SDL_assert(sota->entity_mouse != 0);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = false;
}

void Game_Mouse_Free(struct Game *sota) {
    if (sota->entity_mouse > 0) {
        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
        if (sprite != NULL)
            Sprite_Free(sprite);
        tnecs_entity_destroy(sota->world, sota->entity_mouse);
    }
}
