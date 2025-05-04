
#include "game/cursor.h"
#include "game/game.h"
#include "menu/menu.h"
#include "controller/mouse.h"
#include "controller/gamepad.h"
#include "controller/keyboard.h"
#include "slider.h"
#include "sprite.h"
#include "position.h"
#include "octant.h"
#include "platform.h"
#include "events.h"
#include "nmath.h"
#include "macros.h"
#include "filesystem.h"
#include "utilities.h"
#include "map/map.h"
#include "menu/loadout_select.h"
#include "jsonio.h"

/* -- Focus -- */
void Game_cursorFocus_onMap(struct Game *sota) {
    tnecs_entity cursor = sota->entity_cursor;
    SDL_assert(cursor > TNECS_NULL);

    /* - Load square animated cursor - */
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, cursor, Sprite);
    SDL_assert(sprite != NULL);
    char *path = PATH_JOIN("..", "assets", "GUI", "Cursor", "mapcursors.png");
    Sprite_Load(sprite, path, sota->render.er);
    struct Spritesheet *ss = sprite->spritesheet;

    ss->frames[0] = sota->settings.cursor.frames;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = sota->settings.cursor.speed;

    sprite->scale.x = 1;
    sprite->scale.y = 1;

    /* - Setting tilesize - */
    Sprite_Tilesize_Set(sprite, sota->settings.tilesize);

    /* - Setting Slider - */
    struct Slider *slider = IES_GET_COMPONENT(sota->ecs.world, cursor, Slider);
    SDL_assert(slider != NULL);
    slider->slidetype = SLIDETYPE_GEOMETRIC;

    /* - Set position to tilemap - */
    struct Position *position = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(position != NULL);
    position->onTilemap = true;

    Position_Bounds_Set(position, 0, Map_col_len(sota->map) - 1, 0, Map_row_len(sota->map) - 1);

    /* -- Placing cursor on map -- */
    const Point *tilesize = Map_Tilesize(sota->map);
    int row_len = Map_row_len(sota->map);
    int col_len = Map_col_len(sota->map);
    if (sota->flags.ismouse) {
        struct Point mouse_pos, tilemap_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                           sota->camera.offset.x, sota->camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                           sota->camera.offset.y, sota->camera.zoom);
        tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, col_len - 1);
        tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, row_len - 1);
        position->tilemap_pos.x = tilemap_pos.x;
        position->tilemap_pos.y = tilemap_pos.y;
        Position_Pos_Set(position, tilemap_pos.x, sota->cursor.lastpos.y);
    } else if (sota->flags.iscursor) {
        Position_Pos_Set(position, sota->cursor.lastpos.x, sota->cursor.lastpos.y);
    }

    sota->cursor.lastpos.x = position->tilemap_pos.x;
    sota->cursor.lastpos.y = position->tilemap_pos.y;

    position->pixel_pos.x = sprite->tilesize[0] * (position->tilemap_pos.x - 1);
    position->pixel_pos.y = sprite->tilesize[1] * (position->tilemap_pos.y - 1);
    position->scale[0] = (float)tilesize->x;
    position->scale[1] = (float)tilesize->y;

    sota->map->flags.update = true;
}

void Game_cursorFocus_onMenu(struct Game *sota) {
    // focuses on menu on top of sota->menu_stack

    /* - Preliminaries - */
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);

    /* cursor */
    tnecs_entity cursor = sota->entity_cursor;
    SDL_assert(cursor != 0);
    struct Position *cursor_pos = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(cursor_pos != NULL);

    /* menu_stack top */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top = sota->menu_stack[stack_top];
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;

    /* sprite */
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, cursor, Sprite);
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
        struct Menu *mc_inv = IES_GET_COMPONENT(sota->ecs.world, menu, Menu);
        mc_inv->visible = false;
    }

    // TODO: load menu cursor.
    char *path = PATH_JOIN("..", "assets", "GUI", "Cursor", "mapcursors.png");
    Sprite_Load(sprite, path, sota->render.er);
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
    /* - Skip if Keyboard/Gamepad set sota->cursor.move -*/
    b32 skip = !((sota->cursor.move.x == 0) && (sota->cursor.move.y == 0));
    skip |= (menu == TNECS_NULL);
    skip |= !sota->flags.ismouse;
    skip |= sota->flags.iscursor;
    skip |= (sota->entity_mouse <= TNECS_NULL);
    skip |= (sota->entity_cursor <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SLIDING --- */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_position = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = IES_GET_COMPONENT(sota->ecs.world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (!(slidex || slidey)) {
        return;
    }

    /* -- Menu type -- */
    /* - menu pos - */
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, menu, Menu);

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
    sota->cursor.move.y = (mouse_pos.y > max_pos.y) - (mouse_pos.y < elem_pos.y);
    sota->cursor.move.x = (mouse_pos.x > max_pos.x) - (mouse_pos.x < elem_pos.x);
    /* - prioritize y - */
    if ((sota->cursor.move.x != 0) || (sota->cursor.move.x != 0)) {
        sota->moved_direction = Ternary_Direction(sota->cursor.move);
        // TODO: Call Menu_elem_move fsm function
        i8 new_elem = Menu_Elem_Move(mc, sota->moved_direction);

        if (new_elem != MENU_ELEM_NULL)
            sota->inputs.controller_code = CONTROLLER_MOUSE;
    }

}

/* -- Moves -- */
void Game_Cursor_movedTime_Compute(struct Game *sota, u64 time_ns) {
    // Compute time that cursor has been moving
    if (sota->cursor.frame_moved)
        sota->cursor.moved_time_ms += time_ns / SOTA_us;
    else {
        sota->cursor.moved_time_ms = 0;
        sota->cursor.move.x = sota->cursor.move.y = 0;
    }
}

b32 Game_isCursoronTilemap(struct Game *sota) {
    b32 out = false;
    if (sota->flags.ismouse) {
        struct Point mouse_pos, tilemap_pos;
        const Point *tilesize = Map_Tilesize(sota->map);

        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                           sota->camera.offset.x, sota->camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                           sota->camera.offset.y, sota->camera.zoom);
        b32 x_isIn = int_inbounds(tilemap_pos.x, 0, Map_col_len(sota->map) - 1);
        b32 y_isIn = int_inbounds(tilemap_pos.y, 0, Map_row_len(sota->map) - 1);
        out = (x_isIn && y_isIn);
    }
    return (out);
}

void Game_CursorfollowsMouse_onMap(struct Game *sota) {
    SDL_assert(sota != NULL);
    SDL_assert(sota->ecs.world != NULL);
    SDL_assert(sota->map != NULL);
    /* --- SKIPPING --- */
    /* - skip if any sota->cursor.move not 0 (other controller moved) - */
    b32 skip = ((sota->cursor.move.x != 0) || (sota->cursor.move.y != 0));
    skip |= !sota->flags.ismouse;
    skip |= sota->flags.iscursor;
    skip |= (sota->entity_mouse <= TNECS_NULL);
    skip |= (sota->entity_cursor <= TNECS_NULL);
    if (skip) {
        return;
    }

    /* - Skip if cursor is sliding - */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_position = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = IES_GET_COMPONENT(sota->ecs.world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    /* - Get cursor tile position - */
    struct Point mouse_pos, tilemap_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    const Point *tilesize = Map_Tilesize(sota->map);

    tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                       sota->camera.offset.x, sota->camera.zoom);
    tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                       sota->camera.offset.y, sota->camera.zoom);
    tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, Map_col_len(sota->map) - 1);
    tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, Map_row_len(sota->map) - 1);

    /* - Computing lastpos, move_data - */
    sota->cursor.lastpos.x = cursor_position->tilemap_pos.x;
    sota->cursor.lastpos.y = cursor_position->tilemap_pos.y;
    int tilemap_move_x = tilemap_pos.x - sota->cursor.lastpos.x;
    int tilemap_move_y = tilemap_pos.y - sota->cursor.lastpos.y;

    /* -- Scaling down cursor movement to one tile -- */
    int x_abs = abs(tilemap_move_x), y_abs = abs(tilemap_move_y);
    /* - Prioritize vertical movement - */
    if ((y_abs >= x_abs) && (y_abs > 0))
        sota->cursor.move.y = tilemap_move_y / y_abs;
    else if (x_abs > y_abs)
        sota->cursor.move.x = tilemap_move_x / x_abs;

    if ((sota->cursor.move.x != 0) || (sota->cursor.move.y != 0))
        sota->inputs.controller_code = CONTROLLER_MOUSE;

}
void Game_Cursor_Moves_onMenu(struct Game *sota) {
    /* For basically all states except Gameplay_map.standby */
    tnecs_entity menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    Game_Cursor_Moves_Straight(sota);
    b32 skip = ((sota->cursor.move.x == 0) && (sota->cursor.move.y == 0));
    skip |= (menu == TNECS_NULL);

    if (!skip)
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor.move,
                   &sota->inputs.controller_code);
}

void Game_Cursor_Moves_Straight(struct Game *sota) {
    // Reduces cursor movement to only straight.
    // Prioritizes y
    sota->cursor.move.x *= (sota->cursor.move.y == 0);
}

void Game_Cursor_Move_toCandidate(struct Game *sota) {
    /* Set cursor to new candidate position */
    tnecs_entity candidate = sota->candidates[sota->candidate];
    struct Position *candidate_pos, *cursor_pos;
    candidate_pos   = IES_GET_COMPONENT(sota->ecs.world, candidate, Position);
    cursor_pos      = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Position);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;
}

void Game_Cursor_Next_Candidate(struct Game *sota) {
    Game_Cursor_Moves_Straight(sota);
    b32 skip = ((sota->cursor.move.x == 0) && (sota->cursor.move.y == 0));

    if (skip)
        return;

    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor.move,
               &sota->inputs.controller_code);
}

void Game_Cursor_Moves_onMap(struct Game *sota) {
    /* --- SKIPPING --- */
    /* - skip if any sota->cursor.move 0 - */
    b32 skip = ((sota->cursor.move.x == 0) && (sota->cursor.move.y == 0));
    skip |= (sota->entity_cursor <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SENDING --- */
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *position;
    position = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(position != NULL);

    /* -- Cursor_Moves should not be sent WHEN: -- */
    b32 canSend = true;
    //    1- cursor is in sliding animation.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE

    /* - Skip if cursor is sliding - */
    struct Position *cursor_position = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    struct Slider *cursor_slider = IES_GET_COMPONENT(sota->ecs.world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    //    2- cursor is blocked by CURSOR_FIRSTMOVE_PAUSE.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE
    //    3- cursor is stuck on the edges of the map.
    if (!sota->cursor.diagonal)
        Game_Cursor_Moves_Straight(sota);
    int cx = sota->cursor.move.x, cy = sota->cursor.move.y;
    int tx = position->tilemap_pos.x, ty = position->tilemap_pos.y;
    int cl = Map_col_len(sota->map), rl = Map_row_len(sota->map);
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
    if (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_UNIT_MOVES) {
        // SDL_Log("sota->cursor.move %d %d", sota->cursor.move.x, sota->cursor.move.y);
        int nx = int_inbounds(tx + cx, 0, Map_col_len(sota->map) - 1);
        int ny = int_inbounds(ty + cy, 0, Map_row_len(sota->map) - 1);
        canSend &= (sota->map->darrs.movemap[ny * cl + nx] >= NMATH_MOVEMAP_MOVEABLEMIN);
    }
#endif /* INFINITE_MOVE_ALL */
    if (canSend)
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moves, &sota->cursor.move,
                   &sota->inputs.controller_code);

}

/* --- Cursor --- */
void Game_Cursor_Create(struct Game *sota) {
    Game_Cursor_Free(sota);
    sota->entity_cursor = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Slider_ID,
                                                          Position_ID, Sprite_ID,
                                                          controllerKeyboard_ID,
                                                          controllerGamepad_ID,
                                                          controllerTouchpad_ID,
                                                          Timer_ID, CursorFlag_ID);
    SDL_assert(sota->entity_cursor != 0);
    struct controllerKeyboard *keyboard;
    keyboard = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, controllerKeyboard);
    SDL_assert(keyboard != NULL);
    *keyboard = controllerKeyboard_default;
    keyboard->inputmap = &sota->inputs.keyboardInputMap;
    struct controllerGamepad *gamepad;
    gamepad = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, controllerGamepad);
    SDL_assert(gamepad != NULL);

    *gamepad = controllerGamepad_default;
    Gamepad_Init(gamepad);
    gamepad->inputmap = &sota->inputs.gamepadInputMap;

    struct Slider *slider;
    slider = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Slider);
    SDL_assert(slider != NULL);
    *slider = Slider_default;
    Slider_Ratio_Set(slider, SOTA_CURSOR_RATIO, SOTA_CURSOR_RATIO);
    slider->slidetype                   = SLIDETYPE_GEOMETRIC;

    /* -- sprite -- */
    struct Timer *timer;
    timer = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    sprite->tilesize[0] = sota->settings.tilesize[0];
    sprite->tilesize[1] = sota->settings.tilesize[1];
    sprite->visible = true;
    // sprite->sprite_type = DESTRECT_FUNCTION_CURSOR;

    SDL_assert(sprite->spritesheet == NULL);
    sprite->spritesheet = SDL_malloc(sizeof(*sprite->spritesheet));
    SDL_assert(sprite->spritesheet != NULL);

    *sprite->spritesheet    = Spritesheet_default;
    struct Spritesheet *ss  = sprite->spritesheet;
    ss->loop_num    = 1;
    ss->speeds      = SDL_malloc(ss->loop_num * sizeof(*ss->speeds));
    ss->frames      = SDL_malloc(ss->loop_num * sizeof(*ss->frames));
    ss->loops_pos   = SDL_calloc(ss->loop_num,  sizeof(*ss->loops_pos));
    ss->loop_modes  = SDL_malloc(ss->loop_num * sizeof(*ss->loop_modes));

    ss->frames[0] = sota->settings.cursor.frames;
    ss->speeds[0] = SDL_malloc(ss->frames[0] * sizeof(**ss->speeds));
    ss->loop_modes[0] = LOOPING_PINGPONG;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = sota->settings.cursor.speed;
    SDL_assert(ss->current_loop == 0);

    Cursor_Rects_Init(sprite);
    SDL_assert(sprite->srcrect.w == 64);
    SDL_assert(sprite->srcrect.h == 64);
    struct Position *position = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Position);
    SDL_assert(position != NULL);
    *position = Position_default;
}

void Game_Cursor_Free(struct Game *sota) {
    if (sota->entity_cursor != 0) {
        struct controllerGamepad *gamepad;
        gamepad = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, controllerGamepad);
        SDL_assert(gamepad != NULL);
        Gamepad_Free(gamepad);

        struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Sprite);
        SDL_assert(sprite != NULL);
        Sprite_Free(sprite);

        tnecs_entity_destroy(sota->ecs.world, sota->entity_cursor);
    }
}

void Game_Cursor_Enable(struct Game *sota) {
    sota->flags.iscursor = true;
    sota->inputs.controller_code = CONTROLLER_KEYBOARD;
    SDL_assert(sota->entity_cursor != 0);
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Cursor_Disable(struct Game *sota) {
    sota->flags.iscursor = false;
    SDL_assert(sota->entity_cursor != 0);
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
#ifndef DEBUG_CURSOR_SHOW_wDISABLED
    sprite->visible = false;
#endif
}

/* --- Mouse --- */
void Game_Mouse_Create(struct Game *sota) {
    Game_Mouse_Free(sota);
    sota->entity_mouse = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, controllerMouse_ID,
                                                         Position_ID, Sprite_ID, MouseFlag_ID);
    struct Position *position;
    position = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, Position);
    struct controllerMouse *mouse;
    mouse = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, controllerMouse);
    mouse->inputmap = MouseInputMap_default;

    position->onTilemap = false;
    SDL_assert(position != NULL);
    Position_Bounds_Set(position, -1000, 2000, -1000, 2000);
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    Sprite_Load(sprite, PATH_JOIN("..", "assets", "GUI", "Cursor", "mousecursor.png"), sota->render.er);
    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->surface != NULL);
    SDL_assert(sprite->texture != NULL);
    sprite->visible = false;
    sprite->flip = SDL_FLIP_NONE;

    struct Spritesheet *ss = sprite->spritesheet;
    ss->loop_num      = 1;
    ss->frames        = SDL_malloc(ss->loop_num * sizeof(*ss->frames));
    ss->speeds        = SDL_malloc(ss->loop_num * sizeof(*ss->speeds));
    ss->loops_pos     = SDL_calloc(ss->loop_num,  sizeof(*ss->loops_pos));
    ss->loop_modes    = SDL_malloc(ss->loop_num * sizeof(*ss->loop_modes));
    ss->loop_modes[0] = LOOPING_INVALID;

    ss->frames[0]     = 1;
    ss->speeds[0]     = SDL_calloc(ss->frames[0], sizeof(**ss->speeds));
    SDL_assert(ss->current_loop == 0);

    sprite->srcrect.w = MOUSE_DEFAULT_SRCRECT_W;
    sprite->srcrect.h = MOUSE_DEFAULT_SRCRECT_H;
    sprite->dstrect.w = MOUSE_DEFAULT_DSTRECT_W;
    sprite->dstrect.h = MOUSE_DEFAULT_DSTRECT_H;
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);
}

void Game_Mouse_Enable(struct Game *sota) {
    sota->flags.ismouse = true;
    sota->inputs.controller_code = CONTROLLER_MOUSE;
    SDL_assert(sota->entity_mouse != 0);
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Mouse_Disable(struct Game *sota) {
    sota->flags.ismouse = false;
    sota->inputs.controller_code = CONTROLLER_KEYBOARD;
    SDL_assert(sota->entity_mouse != 0);
    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = false;
}

void Game_Mouse_Free(struct Game *sota) {
    if (sota->entity_mouse == TNECS_NULL)
        return;

    struct Sprite *sprite = IES_GET_COMPONENT(sota->ecs.world, sota->entity_mouse, Sprite);
    SDL_assert(sprite != NULL);
    Sprite_Free(sprite);
    tnecs_entity_destroy(sota->ecs.world, sota->entity_mouse);
    sota->entity_mouse = TNECS_NULL;
}
