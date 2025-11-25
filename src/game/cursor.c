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
** game/cursor: Top level cursor management
**
*/
#include "item.h"
#include "nmath.h"
#include "slider.h"
#include "jsonio.h"
#include "sprite.h"
#include "octant.h"
#include "events.h"
#include "weapon.h"
#include "macros.h"
#include "globals.h"
#include "platform.h"
#include "position.h"
#include "utilities.h"
#include "filesystem.h"

#include "controller/mouse.h"
#include "controller/gamepad.h"
#include "controller/keyboard.h"

#include "game/map.h"
#include "game/game.h"
#include "game/cursor.h"

#include "map/map.h"

#include "menu/menu.h"
#include "menu/loadout_select.h"

/* -- Focus -- */
void Game_cursorFocus_onMap(Game *IES) {
    tnecs_E cursor = IES->cursor.entity;
    SDL_assert(cursor > TNECS_NULL);

    /* - Load square animated cursor - */
    Sprite *sprite = IES_GET_C(gl_world, cursor, Sprite);
    SDL_assert(sprite != NULL);
    char *path = PATH_JOIN( "assets", "GUI", "Cursor",
                            "mapcursors.png");
    Sprite_Load(sprite, path, IES->render.er);
    Spritesheet *ss = sprite->spritesheet;

    ss->frames[0] = IES->settings.cursor.frames;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = IES->settings.cursor.speed;

    sprite->scale.x = 1;
    sprite->scale.y = 1;

    /* - Setting tilesize - */
    Sprite_Tilesize_Set(sprite, IES->settings.tilesize);

    /* - Setting Slider - */
    Slider *slider = IES_GET_C(gl_world, cursor, Slider);
    SDL_assert(slider != NULL);
    slider->slidetype = SLIDETYPE_GEOMETRIC;

    /* - Set position to tilemap - */
    Position *position = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(position != NULL);
    position->onTilemap = true;

    Map *map = Game_Map(IES);
    _Position_Bounds_Set(position, 0, Map_col_len(map) - 1, 0, Map_row_len(map) - 1);

    /* -- Placing cursor on map -- */
    const Point *tilesize = Map_Tilesize(map);
    int row_len = Map_row_len(map);
    int col_len = Map_col_len(map);
    if (IES->flags.ismouse) {
        Point mouse_pos, tilemap_pos;
        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                           map->render.camera.offset.x, map->render.camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                           map->render.camera.offset.y, map->render.camera.zoom);
        tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, col_len - 1);
        tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, row_len - 1);
        position->tilemap_pos.x = tilemap_pos.x;
        position->tilemap_pos.y = tilemap_pos.y;
        _Position_Pos_Set(position, tilemap_pos.x, IES->cursor.lastpos.y);
    } else if (IES->flags.iscursor) {
        Position_Pos_Set(position, IES->cursor.lastpos);
    }

    SDL_Log("Game_cursorFocus_onMap");
    IES->cursor.lastpos.x = position->tilemap_pos.x;
    IES->cursor.lastpos.y = position->tilemap_pos.y;

    position->pixel_pos.x = sprite->tilesize[0] * (position->tilemap_pos.x - 1);
    position->pixel_pos.y = sprite->tilesize[1] * (position->tilemap_pos.y - 1);
    position->scale[0] = (float)tilesize->x;
    position->scale[1] = (float)tilesize->y;

    map->flags.update = true;
}

void Game_cursorFocus_onMenu(Game *IES) {
    // focuses on menu on top of IES->menus.stack
    /* - Preliminaries - */

    SDL_assert(DARR_NUM(IES->menus.stack) > 0);

    /* cursor */
    tnecs_E cursor = IES->cursor.entity;
    SDL_assert(cursor != 0);
    Position *cursor_pos = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(cursor_pos != NULL);

    /* menu_stack top */
    int stack_top       = DARR_NUM(IES->menus.stack) - 1;
    tnecs_E menu_top    = IES->menus.stack[stack_top];
    Menu *mc     = IES_GET_C(gl_world, menu_top, Menu);
    SDL_assert(mc           != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;

    /* sprite */
    Sprite *sprite = IES_GET_C(gl_world, cursor, Sprite);
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
            mc->elem = 0;
            break;
        /* NOT RESET */
        case MENU_TYPE_ITEM_SELECT:
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
        tnecs_E menu = IES->menus.stack[i];
        Menu *mc_inv = IES_GET_C(gl_world, menu, Menu);
        mc_inv->visible = false;
    }

    // TODO: load menu cursor.
    char *path = PATH_JOIN( "assets", "GUI", "Cursor",
                            "mapcursors.png");
    Sprite_Load(sprite, path, IES->render.er);
    SDL_assert(sprite->spritesheet != NULL);
    Spritesheet *ss = sprite->spritesheet;
    SDL_assert(ss->frames != NULL);
    ss->frames[0] = IES->settings.cursor.frames;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = IES->settings.cursor.speed;

    Map *map = Game_Map(IES);
    sprite->scale.x = 8;
    sprite->scale.y = 8;
    Sprite_Tilesize_Set(sprite, IES->settings.tilesize);
    if (map != NULL) {
        Cursor_Dstrect_Absolute(sprite, &cursor_pos->pixel_pos, &map->render.camera);
    } else {
        Camera camera = Camera_default;
        Cursor_Dstrect_Absolute(sprite, &cursor_pos->pixel_pos, &camera);
    }
    mc->visible = true;
}

/* -- Follows mouse -- */
/* if the cursor is far away from elem box, move in the direction. */
void Game_CursorfollowsMouse_onMenu(Game *IES) {
    /* --- SKIPPING --- */
    tnecs_E menu = IES->menus.stack[DARR_NUM(IES->menus.stack) - 1];
    /* - Skip if Keyboard/Gamepad set IES->cursor.move -*/
    b32 skip = !((IES->cursor.move.x == 0) && (IES->cursor.move.y == 0));
    skip |= (menu == TNECS_NULL);
    skip |= !IES->flags.ismouse;
    skip |= IES->flags.iscursor;
    skip |= (IES->mouse.entity <= TNECS_NULL);
    skip |= (IES->cursor.entity <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SLIDING --- */
    tnecs_E cursor = IES->cursor.entity;
    Position *cursor_position = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    Slider *cursor_slider = IES_GET_C(gl_world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    // SDL_Log("slider %d %d", cursor_slider->target.x, cursor_slider->target.y);
    // SDL_Log("pos %d %d", cursor_position->pixel_pos.x, cursor_position->pixel_pos.y);
    // if (!(slidex || slidey)) {
    if (slidex || slidey) {
        return;
    }

    /* -- Menu type -- */
    /* - menu pos - */
    Menu *mc = IES_GET_C(gl_world, menu, Menu);

    /* - elem pos & elem box - */
    SDL_assert(mc->elem < mc->elem_num);
    SDL_assert(mc->elem > MENU_ELEM_NULL);
    Point elem_pos = mc->elem_pos[mc->elem];
    Point elem_box = mc->elem_box[mc->elem];
    SDL_assert((elem_box.x > -1) && (elem_box.y > -1));
    SDL_assert((elem_pos.x > -1) && (elem_pos.y > -1));

    /* - mouse pos - */
    Point mouse_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

    /* - cursor in box - */
    Point max_pos = {elem_pos.x + elem_box.x, elem_pos.y + elem_box.y};
    // is 1 if >, is -1 if <, otherwise 0
    Point move;
    move.y = (mouse_pos.y > max_pos.y) - (mouse_pos.y < elem_pos.y);
    move.x = (mouse_pos.x > max_pos.x) - (mouse_pos.x < elem_pos.x);

    i32 direction   = Ternary_Direction_Straight(Ternary_Direction(move));
    i32 old_elem    = Menu_Elem(mc);
    i32 num_elem    = Menu_Elem_Num(mc);
    if ((old_elem == 0) && (direction == SOTA_DIRECTION_TOP)) {
        // Menu element is on the edge.
        // Mouse shouldn't make cursor move periodically
        return;
    }

    if ((old_elem == (num_elem - 1)) && (direction == SOTA_DIRECTION_BOTTOM)) {
        // Menu element is on the edge.
        // Mouse shouldn't make cursor move periodically
        return;
    }

    IES->cursor.move               = move;
    IES->cursor.moved_direction    = direction;
    IES->inputs.controller_type    = CONTROLLER_MOUSE;
}

/* -- Moves -- */
void Game_Cursor_movedTime_Compute(Game *IES, u64 time_ns) {
    // Compute time that cursor has been moving
    if (IES->cursor.frame_moved)
        IES->cursor.moved_time_ms += time_ns / SOTA_us;
    else {
        IES->cursor.moved_time_ms = 0;
        IES->cursor.move.x = IES->cursor.move.y = 0;
    }
}

b32 Game_isCursoronTilemap(Game *IES) {
    b32 out = false;
    if (IES->flags.ismouse) {
        Map *map = Game_Map(IES);
        Point mouse_pos, tilemap_pos;
        const Point *tilesize = Map_Tilesize(map);

        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                           map->render.camera.offset.x, map->render.camera.zoom);
        tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                           map->render.camera.offset.y, map->render.camera.zoom);
        b32 x_isIn = int_inbounds(tilemap_pos.x, 0, Map_col_len(map) - 1);
        b32 y_isIn = int_inbounds(tilemap_pos.y, 0, Map_row_len(map) - 1);
        out = (x_isIn && y_isIn);
    }
    return (out);
}

void Game_CursorfollowsMouse_onMap(Game *IES) {
    // SDL_Log(__func__);
    SDL_assert(IES     != NULL);
    SDL_assert(gl_world != NULL);
    Map *map = Game_Map(IES);
    SDL_assert(map != NULL);
    /* --- SKIPPING --- */
    /* - skip if any IES->cursor.move not 0 (other controller moved) - */
    b32 skip = ((IES->cursor.move.x != 0) || (IES->cursor.move.y != 0));
    skip |= !IES->flags.ismouse;
    skip |= IES->flags.iscursor;
    skip |= (IES->mouse.entity <= TNECS_NULL);
    skip |= (IES->cursor.entity <= TNECS_NULL);
    if (skip) {
        return;
    }

    /* - Skip if cursor is sliding - */
    tnecs_E cursor = IES->cursor.entity;
    Position *cursor_position = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    Slider *cursor_slider = IES_GET_C(gl_world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    /* - Get cursor tile position - */
    Point mouse_pos, tilemap_pos;
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    const Point *tilesize = Map_Tilesize(map);

    tilemap_pos.x = SOTA_PIXEL2TILEMAP(mouse_pos.x, tilesize->x,
                                       map->render.camera.offset.x, map->render.camera.zoom);
    tilemap_pos.y = SOTA_PIXEL2TILEMAP(mouse_pos.y, tilesize->y,
                                       map->render.camera.offset.y, map->render.camera.zoom);
    tilemap_pos.x = int_inbounds(tilemap_pos.x, 0, Map_col_len(map) - 1);
    tilemap_pos.y = int_inbounds(tilemap_pos.y, 0, Map_row_len(map) - 1);

    /* - Computing lastpos, move_data - */
    // SDL_Log("Game_CursorfollowsMouse_onMap");
    // SDL_Log("IES->cursor.lastpos.x %d %d",
    //         IES->cursor.lastpos.x,
    //         IES->cursor.lastpos.y);
    // IES->cursor.lastpos.x = cursor_position->tilemap_pos.x;
    // IES->cursor.lastpos.y = cursor_position->tilemap_pos.y;
    int tilemap_move_x = tilemap_pos.x - cursor_position->tilemap_pos.x;
    int tilemap_move_y = tilemap_pos.y - cursor_position->tilemap_pos.y;

    /* -- Scaling down cursor movement to one tile -- */
    int x_abs = abs(tilemap_move_x);
    int y_abs = abs(tilemap_move_y);

    /* - Prioritize vertical movement - */
    if ((y_abs >= x_abs) && (y_abs > 0)) {
        IES->cursor.move.y = tilemap_move_y / y_abs;
    } else if (x_abs > y_abs) {
        IES->cursor.move.x = tilemap_move_x / x_abs;
    }
    // SDL_Log("IES->cursor.move %d %d",
    // IES->cursor.move.x, IES->cursor.move.y);

    if ((IES->cursor.move.x != 0) || (IES->cursor.move.y != 0)) {
        Point   *data1 = IES_calloc(1, sizeof(*data1));
        i32     *data2 = IES_calloc(1, sizeof(*data2));
        *data1 = IES->cursor.move;
        IES->inputs.controller_type = CONTROLLER_MOUSE;
        *data2 = IES->inputs.controller_type;

        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Moves,
                    data1, data2);
    }

    IES->cursor.move.x = 0;
    IES->cursor.move.y = 0;
}

void Game_Cursor_Moves_onMenu(Game *IES) {
    /* For basically all states except Gameplay_map.standby */
    tnecs_E menu = IES->menus.stack[DARR_NUM(IES->menus.stack) - 1];
    Game_Cursor_Moves_Straight(IES);
    b32 skip = ((IES->cursor.move.x == 0) && (IES->cursor.move.y == 0));
    skip |= (menu == TNECS_NULL);

    if (!skip) {
        Point   *data1 = IES_calloc(1, sizeof(*data1));
        i32     *data2 = IES_calloc(1, sizeof(*data2));
        *data1 = IES->cursor.move;
        *data2 = IES->inputs.controller_type;

        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Moves,
                    data1, data2);
    }
}

void Game_Cursor_Moves_Straight(Game *IES) {
    // Reduces cursor movement to only straight.
    // Prioritizes y
    IES->cursor.move.x *= (IES->cursor.move.y == 0);
}

void Game_Cursor_Move_toCandidate(Game *IES) {
    /* Set cursor to new candidate position */
    tnecs_E candidate = IES->targets.candidates[IES->targets.order];
    Position *candidate_pos, *cursor_pos;
    candidate_pos   = IES_GET_C(gl_world, candidate, Position);
    cursor_pos      = IES_GET_C(gl_world, IES->cursor.entity, Position);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;
}

void Game_Cursor_Next_Candidate(Game *IES) {
    Game_Cursor_Moves_Straight(IES);
    b32 skip = ((IES->cursor.move.x == 0) && (IES->cursor.move.y == 0));

    if (skip)
        return;

    Point   *data1 = IES_calloc(1, sizeof(*data1));
    i32     *data2 = IES_calloc(1, sizeof(*data2));
    *data1 = IES->cursor.move;
    *data2 = IES->inputs.controller_type;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Cursor_Moves,
                data1, data2);
}

void Game_Cursor_Moves_onMap(Game *IES) {
    /* --- SKIPPING --- */
    /* - skip if any IES->cursor.move 0 - */
    b32 skip = ((IES->cursor.move.x == 0) && (IES->cursor.move.y == 0));
    skip |= (IES->cursor.entity <= TNECS_NULL);

    if (skip) {
        return;
    }

    /* --- SENDING --- */
    tnecs_E cursor = IES->cursor.entity;
    Position *position;
    position = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(position != NULL);

    /* -- Cursor_Moves should not be sent WHEN: -- */
    b32 canSend = true;
    //    1- cursor is in sliding animation.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE

    /* - Skip if cursor is sliding - */
    Position *cursor_position = IES_GET_C(gl_world, cursor, Position);
    SDL_assert(cursor_position != NULL);
    Slider *cursor_slider = IES_GET_C(gl_world, cursor, Slider);
    SDL_assert(cursor_slider != NULL);

    b32 slidex = (cursor_slider->target.x != cursor_position->pixel_pos.x);
    b32 slidey = (cursor_slider->target.y != cursor_position->pixel_pos.y);
    if (slidex || slidey) {
        return;
    }

    //    2- cursor is blocked by CURSOR_FIRSTMOVE_PAUSE.
    //       Game_CursorfollowsMouse_onMap TAKES CHARGE
    //    3- cursor is stuck on the edges of the map.
    if (!IES->cursor.diagonal)
        Game_Cursor_Moves_Straight(IES);
    int cx = IES->cursor.move.x, cy = IES->cursor.move.y;
    int tx = position->tilemap_pos.x, ty = position->tilemap_pos.y;
    Map *map = Game_Map(IES);
    int cl = Map_col_len(map), rl = Map_row_len(map);
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
    if (Game_Substate_Current(IES) == GAME_SUBSTATE_MAP_UNIT_MOVES) {
        // SDL_Log("IES->cursor.move %d %d", IES->cursor.move.x, IES->cursor.move.y);
        int nx = int_inbounds(tx + cx, 0, Map_col_len(map) - 1);
        int ny = int_inbounds(ty + cy, 0, Map_row_len(map) - 1);
        canSend &= (map->darrs.movemap[ny * cl + nx] >= NMATH_MOVEMAP_MOVEABLEMIN);
    }
#endif /* INFINITE_MOVE_ALL */
    if (canSend) {
        Point   *data1 = IES_calloc(1, sizeof(*data1));
        i32     *data2 = IES_calloc(1, sizeof(*data2));
        *data1 = IES->cursor.move;
        *data2 = IES->inputs.controller_type;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Moves,
                    data1, data2);
    }
}

/* --- Cursor --- */
void Game_Cursor_Create(Game *IES) {
    Game_Cursor_Free(IES);
    IES->cursor.entity = IES_E_CREATE_wC(  gl_world,
                                           Slider_ID,
                                           Position_ID,
                                           Sprite_ID,
                                           controllerKeyboard_ID,
                                           controllerGamepad_ID,
                                           controllerTouchpad_ID,
                                           Timer_ID,
                                           CursorFlag_ID);
    SDL_assert(IES->cursor.entity != 0);
    controllerKeyboard *keyboard;
    keyboard = IES_GET_C(gl_world, IES->cursor.entity, controllerKeyboard);
    SDL_assert(keyboard != NULL);
    *keyboard = controllerKeyboard_default;
    keyboard->inputmap = &IES->inputs.keyboardInputMap;
    controllerGamepad *gamepad;
    gamepad = IES_GET_C(gl_world, IES->cursor.entity, controllerGamepad);
    SDL_assert(gamepad != NULL);

    *gamepad = controllerGamepad_default;
    Gamepad_Init(gamepad);
    gamepad->inputmap = &IES->inputs.gamepadInputMap;

    Slider *slider;
    slider = IES_GET_C(gl_world, IES->cursor.entity, Slider);
    SDL_assert(slider != NULL);
    *slider = Slider_default;
    Slider_Ratio_Set(slider, SOTA_CURSOR_RATIO, SOTA_CURSOR_RATIO);
    slider->slidetype                   = SLIDETYPE_GEOMETRIC;

    /* -- sprite -- */
    Timer *timer;
    timer = IES_GET_C(gl_world, IES->cursor.entity, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    Sprite *sprite = IES_GET_C(gl_world, IES->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    sprite->tilesize[0] = IES->settings.tilesize[0];
    sprite->tilesize[1] = IES->settings.tilesize[1];
    sprite->visible = true;
    // sprite->sprite_type = DESTRECT_FUNCTION_CURSOR;

    SDL_assert(sprite->spritesheet == NULL);
    sprite->spritesheet = SDL_malloc(sizeof(*sprite->spritesheet));
    SDL_assert(sprite->spritesheet != NULL);

    *sprite->spritesheet    = Spritesheet_default;
    Spritesheet *ss  = sprite->spritesheet;
    ss->loop_num    = 1;
    ss->speeds      = SDL_malloc(ss->loop_num * sizeof(*ss->speeds));
    ss->frames      = SDL_malloc(ss->loop_num * sizeof(*ss->frames));
    ss->loops_pos   = SDL_calloc(ss->loop_num,  sizeof(*ss->loops_pos));
    ss->loop_modes  = SDL_malloc(ss->loop_num * sizeof(*ss->loop_modes));

    ss->frames[0] = IES->settings.cursor.frames;
    ss->speeds[0] = SDL_malloc(ss->frames[0] * sizeof(**ss->speeds));
    ss->loop_modes[0] = LOOPING_PINGPONG;
    for (int i = 0; i < ss->frames[0]; ++i)
        ss->speeds[0][i] = IES->settings.cursor.speed;
    SDL_assert(ss->current_loop == 0);

    Cursor_Rects_Init(sprite);
    SDL_assert(sprite->srcrect.w == 64);
    SDL_assert(sprite->srcrect.h == 64);
    Position *position = IES_GET_C(gl_world, IES->cursor.entity, Position);
    SDL_assert(position != NULL);
    *position = Position_default;
}

void Game_Cursor_Free(Game *IES) {
    if (IES->cursor.entity != TNECS_NULL) {
        tnecs_E_destroy(gl_world, IES->cursor.entity);
    }
}

void Game_Cursor_Enable(Game *IES) {
    IES->flags.iscursor = true;
    IES->inputs.controller_type = CONTROLLER_KEYBOARD;
    SDL_assert(IES->cursor.entity != 0);
    Sprite *sprite = IES_GET_C(gl_world, IES->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Cursor_Disable(Game *IES) {
    IES->flags.iscursor = false;
    SDL_assert(IES->cursor.entity != 0);
    Sprite *sprite = IES_GET_C(gl_world, IES->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
#ifndef DEBUG_CURSOR_SHOW_wDISABLED
    sprite->visible = false;
#endif
}

/* --- Mouse --- */
void Game_Mouse_Create(Game *IES) {
    Game_Mouse_Free(IES);
    IES->mouse.entity = IES_E_CREATE_wC(gl_world, controllerMouse_ID,
                                        Position_ID, Sprite_ID, MouseFlag_ID);
    Position *position;
    position = IES_GET_C(gl_world, IES->mouse.entity, Position);
    controllerMouse *mouse;
    mouse = IES_GET_C(gl_world, IES->mouse.entity, controllerMouse);
    *mouse = controllerMouse_default;
    mouse->inputmap = MouseInputMap_default;

    position->onTilemap = false;
    SDL_assert(position != NULL);
    _Position_Bounds_Set(position, -1000, 2000, -1000, 2000);
    Sprite *sprite = IES_GET_C(gl_world, IES->mouse.entity, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    Sprite_Load(sprite, PATH_JOIN("assets", "GUI", "Cursor", "mousecursor.png"), IES->render.er);
    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->surface != NULL);
    SDL_assert(sprite->texture != NULL);
    sprite->visible = false;
    sprite->flip = SDL_FLIP_NONE;

    Spritesheet *ss = sprite->spritesheet;
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

void Game_Mouse_Enable(Game *IES) {
    IES->flags.ismouse = true;
    IES->inputs.controller_type = CONTROLLER_MOUSE;
    SDL_assert(IES->mouse.entity != 0);
    Sprite *sprite = IES_GET_C(gl_world, IES->mouse.entity, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = true;
}

void Game_Mouse_Disable(Game *IES) {
    IES->flags.ismouse = false;
    IES->inputs.controller_type = CONTROLLER_KEYBOARD;
    SDL_assert(IES->mouse.entity != 0);
    Sprite *sprite = IES_GET_C(gl_world, IES->mouse.entity, Sprite);
    SDL_assert(sprite->dstrect.w);
    SDL_assert(sprite->dstrect.h);

    SDL_assert(sprite != NULL);
    sprite->visible = false;
}

void Game_Mouse_Free(Game *IES) {
    if (IES->mouse.entity == TNECS_NULL)
        return;

    Sprite *sprite = IES_GET_C(gl_world, IES->mouse.entity, Sprite);
    SDL_assert(sprite != NULL);
    Sprite_Free(sprite);
    tnecs_E_destroy(gl_world, IES->mouse.entity);
    IES->mouse.entity = TNECS_NULL;
}

Point *Game_Lastpos_P(Game *IES) {
    return (&IES->cursor.lastpos);
}

Point Game_Lastpos_R(const Game *IES) {
    return (IES->cursor.lastpos);
}

void Game_Lastpos_W(Game *IES, Point pos) {
    IES->cursor.lastpos = pos;
}

