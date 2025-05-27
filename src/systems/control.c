
#include "systems/control.h"
#include "controller/gamepad.h"
#include "controller/keyboard.h"
#include "game/game.h"
#include "game/cursor.h"
#include "events.h"
#include "slider.h"
#include "enums.h"
#include "map/map.h"
#include "position.h"
#include "sprite.h"

void Cursor_Scroll_Camera(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    Position *pos_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    Sprite   *sprite_arr = TNECS_COMPONENT_ARRAY(input, Sprite_ID);

    /* There should only be one cursor */
    SDL_assert(input->num_entities == 1);
    Position    *pos    = pos_arr;
    Sprite      *sprite = sprite_arr;

    if (!pos->onTilemap)
        return;

    /* -- Bounds -- */
    // Outside bounds, cursor scrolls the camera
    // More zoom -> cursor scrolls camera from farther away
    // factors are screen length ratio
    float factor_max = (CAMERA_BOUNDS - CAMERA_BOUNDS_SCALE * IES->map->render.camera.zoom /
                        MAX_CAMERA_ZOOM);
    float factor_min = 1.0f - factor_max;
    SDL_assert(factor_max < 1.0f);
    SDL_assert(factor_max > 0.0f);
    SDL_assert(factor_min < 1.0f);
    SDL_assert(factor_min > 0.0f);
    SDL_assert(factor_min < factor_max);

    Point bounds_max = {
        (factor_max * IES->settings.res.x),
        (factor_max * IES->settings.res.y)
    };
    Point bounds_min = {
        (factor_min * IES->settings.res.x),
        (factor_min * IES->settings.res.y)
    };

    /* -- Cursor center position -- */
    int x = sprite->dstrect.x, y = sprite->dstrect.y;
    int w = sprite->dstrect.w, h = sprite->dstrect.h;
    Point center = {(x + w / 2), (y + h / 2)};

    /* -- Scroll amount -- */
    // TODO: scrolling is TOO SLOW
    //      - Make scroll speed same as cursor speed.
    int scroll = CAMERA_SCROLL_SPEED * SOTA_DEZOOM(IES->map->render.camera.zoom,
                                                   CAMERA_SCROLL_ZOOMFACTOR);

    /* -- Does camera need to scroll? -- */
    if (center.x >= bounds_max.x) {
        IES->map->render.camera.offset.x   -= scroll;
        IES->map->flags.camera_moved = true;
    } else if (center.x <= bounds_min.x) {
        IES->map->render.camera.offset.x   += scroll;
        IES->map->flags.camera_moved = true;
    }
    if (center.y >= bounds_max.y) {
        IES->map->render.camera.offset.y   -= scroll;
        IES->map->flags.camera_moved = true;
    } else if (center.y <= bounds_min.y) {
        IES->map->render.camera.offset.y   += scroll;
        IES->map->flags.camera_moved = true;
    }
}

void Control_Cursor_Moves(struct Game *IES,    struct Point cursor_move,
                          struct Point target,  struct Point px_pos,
                          i32 controller_type) {
    /* - Immobile cursor - */
    if ((cursor_move.x == 0) && (cursor_move.y == 0)) {
        return;
    }

    IES->cursor.frame_moved = true;

    /* - Pause cursor after moving - */
    i32 min     = 0;
    i32 max     = CURSOR_FIRSTMOVE_PAUSE_ms;
    i32 moved   = IES->cursor.moved_time_ms;
    if ((moved > min) && (moved < max)) {
        return;
    }

    IES->cursor.move     = cursor_move;
    IES->inputs.controller_code = controller_type;
}

void Gamepad_Pressed(i8 SOTA_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                     u32 event, struct controllerGamepad *gp) {
    i32 theld       = gp->timeheld_button_ns;
    i32 min_held    = GAMEPAD_MINHELD_ns;
    b32 butblk      = gp->block_buttons;

    Control_Pressed(SOTA_b, press, pressed_num, controller_type,
                    event, butblk, theld, min_held);
}

void Keyboard_Pressed(i8 SOTA_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                      u32 event, struct controllerKeyboard *kb) {
    i32 theld       = kb->timeheld_button_ns;
    i32 min_held    = KEYBOARD_MINHELD_ns;
    b32 butblk      = kb->block_buttons;

    Control_Pressed(SOTA_b, press, pressed_num, controller_type,
                    event, butblk, theld, min_held);
}

void Control_Pressed(i8 SOTA_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                     u32 event, b32 block, i32 t_held_ns, i32 t_min_ns) {
    press[(*pressed_num)++] = SOTA_b;
    if (block) {
        return;
    }
    if (event <= 0) {
        return;
    }
    if ((t_min_ns <= 0) || (t_held_ns > t_min_ns)) {
        // NOTE: 1- User input is blocked in FSM.
        //       2- Control System always sends events.
        Event_Emit(__func__, SDL_USEREVENT, event, controller_type, NULL);
    }
}

void Control_Keyboard(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* --- PRELIMINARIES --- */
    SDL_assert(IES != NULL);
    struct Point cursor_move = {0};

    Slider   *slider_arr = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    Position *pos_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    controllerKeyboard   *kb_arr     = TNECS_COMPONENT_ARRAY(input, controllerKeyboard_ID);

    /* There should only be one cursor */
    SDL_assert(input->num_entities == 1);
    Slider             *sld = slider_arr;
    Position           *pos = pos_arr;
    controllerKeyboard *kb  = kb_arr;

    /* -- Preliminaries -- */
    i32                       *ct       = &kb->controller_type;
    const u8                  *kb_state = SDL_GetKeyboardState(NULL);

    /* -- Keyboard button checking -- */
    i8 press[SOTA_BUTTON_END];
    i8 pnum          = 0;
    size_t *mheld    = &kb->held_move_num;
    i32    *theld    = &kb->timeheld_button_ns;
    i32    *theld_ns = &kb->timeheld_move_ns;
    size_t *bheld    = &kb->held_button_num;

    for (int SOTA_b = SOTA_BUTTON_A; SOTA_b < SOTA_BUTTON_END; SOTA_b++) {
        if (Keyboard_isPressed(kb, kb_state, SOTA_b))
            Keyboard_Pressed(SOTA_b, press, &pnum, ct, IES->inputs.arr[SOTA_b], kb);
    }
    u64 updateTime_ns = SOTA_ns / IES->settings.FPS.cap;
    Keyboard_Held(kb->held_button, bheld, theld, press, pnum, updateTime_ns);
    /* - Unblock keyboard - */
    if ((*theld) == 0)
        kb->block_buttons = false;

    /* -- Keyboard move checking -- */
    // i8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
    // i8 pmove_num = 0;
    b32 right = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_RIGHT);
    b32 up    = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_UP);
    b32 left  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_LEFT);
    b32 down  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_DOWN);

    /* - Collapse diagonals to one of 4 main directions - */
    if (up && !down) {
        cursor_move.y       = -1;
        // pmove[pmove_num++]  = SOTA_INPUT_UP;
    } else if (down && !up) {
        cursor_move.y       =  1;
        // pmove[pmove_num++]  = SOTA_INPUT_DOWN;
    }
    if (left && !right) {
        cursor_move.x       = -1;
        // pmove[pmove_num++]  = SOTA_INPUT_LEFT;
    } else if (right && !left) {
        cursor_move.x       =  1;
        // pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
    }

    Keyboard_Held(kb->held_move, mheld, theld_ns, press, pnum, updateTime_ns);

    struct Point target    = sld->target;
    struct Point pixel_pos = pos->pixel_pos;
    Control_Cursor_Moves(IES, cursor_move, target, pixel_pos, *ct);
}

void Control_Gamepad(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* --- PRELIMINARIES --- */
    struct Point cursor_move = {0};

    Slider   *slider_arr = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    Position *pos_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    controllerGamepad   *gp_arr     = TNECS_COMPONENT_ARRAY(input, controllerGamepad_ID);

    /* There should only be one cursor */
    SDL_assert(input->num_entities == 1);
    Slider             *sld = slider_arr;
    Position           *pos = pos_arr;
    controllerGamepad  *gp  = gp_arr;

    /* -- Preliminaries -- */
    i8 press[SOTA_BUTTON_END];
    i8 pnum = 0;
    size_t  *mheld    = &gp->held_move_num;
    i32     *theld_ns = &gp->timeheld_move_ns;
    i32     *theld    = &gp->timeheld_button_ns;
    size_t  *bheld    = &gp->held_button_num;

    /* -- Gamepad button checking -- */
    for (int SOTA_b = SOTA_BUTTON_A; SOTA_b <= SOTA_BUTTON_TRIGGER_RIGHT; SOTA_b++) {
        if (Gamepad_isPressed(gp, SOTA_b))
            Gamepad_Pressed(SOTA_b, press, &pnum, &gp->controller_type, IES->inputs.arr[SOTA_b], gp);
    }

    u64 updateTime_ns = SOTA_ns / IES->settings.FPS.cap;
    Gamepad_Held(gp->held_button, bheld, theld, press, pnum, updateTime_ns);

    /* - Unblock gamepad - */
    if (gp->timeheld_button_ns == 0)
        gp->block_buttons = false;

    /* -- Gamepad move checking -- */
    i8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
    i8 pmove_num = 0;

    cursor_move = Gamepad_Joystick_Direction(gp);
    if (cursor_move.x > 0) {
        pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
    } else if (cursor_move.x < 0) {
        pmove[pmove_num++]  = SOTA_INPUT_LEFT;
    }
    /* - Up/Down axis - */
    if  (cursor_move.y > 0) {
        pmove[pmove_num++]  = SOTA_INPUT_DOWN;
    } else if  (cursor_move.y < 0)  {
        pmove[pmove_num++]  = SOTA_INPUT_UP;
    }

    /* - Move buttons - */
    /* Priority of move buttons over joystick */
    if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_RIGHT)) {
        cursor_move.x       =  1;
        pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
    } else if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_LEFT)) {
        cursor_move.x       = -1;
        pmove[pmove_num++]  = SOTA_INPUT_LEFT;
    }
    if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_UP)) {
        cursor_move.y       = -1;
        pmove[pmove_num++]  = SOTA_INPUT_UP;
    } else if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_DOWN)) {
        cursor_move.y       =  1;
        pmove[pmove_num++]  = SOTA_INPUT_DOWN;
    }

    Gamepad_Held(gp->held_move, mheld, theld_ns, pmove, pmove_num, updateTime_ns);

    struct Point target    = sld->target;
    struct Point pixel_pos = pos->pixel_pos;
    Control_Cursor_Moves(IES, cursor_move, target, pixel_pos, gp->controller_type);
}

void Control_Touchpad(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);
}

const fsm_main_t fsm_cfollows_s[GAME_STATE_NUM] = {
    /* STATE_START         */ NULL,
    /* STATE_Combat        */ NULL,
    /* STATE_Scene_Talk    */ NULL,
    /* STATE_Cutscene      */ NULL,
    /* STATE_Gameplay_Map  */ &fsm_cfollows_sGmpMap,
    /* STATE_Gameplay_Camp */ NULL,
    /* STATE_Preparation   */ NULL,
    /* STATE_Title_Screen  */ NULL,
    /* STATE_Animation     */ NULL,
};

const fsm_main_t fsm_cfollows_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* SUBSTATE_START          */ NULL,
    /* SUBSTATE_MAP_MINIMAP    */ NULL,
    /* SUBSTATE_MENU           */ &fsm_cfollows_sGmpMap_ssMenu,
    /* SUBSTATE_MAP_UNIT_MOVES */ &fsm_cfollows_sGmpMap_ssMapUnitMv,
    /* SUBSTATE_MAP_COMBAT     */ NULL,
    /* SUBSTATE_MAP_NPCTURN    */ NULL,
    /* SUBSTATE_SAVING         */ NULL,
    /* SUBSTATE_STANDBY        */ &fsm_cfollows_sGmpMap_ssStby,
    /* SUBSTATE_MAP_CANDIDATES */ NULL,
    /* SUBSTATE_CUTSCENE       */ NULL,
    /* SUBSTATE_MAP_ANIMATION  */ NULL,
};

void fsm_cfollows_sGmpMap(            Game *IES) {
    fsm_cfollows_sGmpMap_ss[Game_Substate_Current(IES)](IES);
}
void fsm_cfollows_sTtlScrn(             Game *IES) {
    Game_CursorfollowsMouse_onMenu(IES);
}
void fsm_cfollows_sGmpMap_ssMenu(     Game *IES) {
    Game_CursorfollowsMouse_onMenu(IES);
}
void fsm_cfollows_sGmpMap_ssStby(     Game *IES) {
    Game_CursorfollowsMouse_onMap(IES);
}
void fsm_cfollows_sGmpMap_ssMapUnitMv(Game *IES) {
    Game_CursorfollowsMouse_onMap(IES);
}

void Cursor_Follows_Mouse(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);
    if (fsm_cfollows_s[Game_State_Current(IES)] != NULL) {
        fsm_cfollows_s[Game_State_Current(IES)](IES);
    }
    // Game_CursorfollowsMouse_onMenu
    //  - sGmpMap_ssMenu
    //  - sTtlScrn
    // Game_CursorfollowsMouse_onMenu(IES);
    // Game_CursorfollowsMouse_onMap
    //  - sGmpMap_ssMapUnitMv
    //  - sGmpMap_ssStby
    // Game_CursorfollowsMouse_onMap(IES);
}

const fsm_main_t fsm_cmoves_s[GAME_STATE_NUM] = {
    /* STATE_START         */ NULL,
    /* STATE_Combat        */ NULL,
    /* STATE_Scene_Talk    */ NULL,
    /* STATE_Cutscene      */ NULL,
    /* STATE_Gameplay_Map  */ &fsm_cmoves_sGmpMap,
    /* STATE_Gameplay_Camp */ NULL,
    /* STATE_Preparation   */ &fsm_cmoves_sPrep,
    /* STATE_Title_Screen  */ &fsm_cmoves_sTtlScrn,
    /* STATE_Animation     */ NULL,
};

const fsm_main_t fsm_cmoves_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* SUBSTATE_START          */ NULL,
    /* SUBSTATE_MAP_MINIMAP    */ NULL,
    /* SUBSTATE_MENU           */ &fsm_cmoves_sGmpMap_ssMenu,
    /* SUBSTATE_MAP_UNIT_MOVES */ &fsm_cmoves_sGmpMap_ssMapUnitMv,
    /* SUBSTATE_MAP_COMBAT     */ NULL,
    /* SUBSTATE_MAP_NPCTURN    */ NULL,
    /* SUBSTATE_SAVING         */ NULL,
    /* SUBSTATE_STANDBY        */ &fsm_cmoves_sGmpMap_ssStby,
    /* SUBSTATE_MAP_CANDIDATES */ &fsm_cmoves_sGmpMap_ssMapCndt,
    /* SUBSTATE_CUTSCENE       */ NULL,
    /* SUBSTATE_MAP_ANIMATION  */ NULL,
};

extern const fsm_main_t fsm_cmoves_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* SUBSTATE_START          */ NULL,
    /* SUBSTATE_MAP_MINIMAP    */ NULL,
    /* SUBSTATE_MENU           */ &fsm_cmoves_sPrep_ssMenu,
    /* SUBSTATE_MAP_UNIT_MOVES */ NULL,
    /* SUBSTATE_MAP_COMBAT     */ NULL,
    /* SUBSTATE_MAP_NPCTURN    */ NULL,
    /* SUBSTATE_SAVING         */ NULL,
    /* SUBSTATE_STANDBY        */ NULL,
    /* SUBSTATE_MAP_CANDIDATES */ &fsm_cmoves_sPrep_ssMapCndt,
    /* SUBSTATE_CUTSCENE       */ NULL,
    /* SUBSTATE_MAP_ANIMATION  */ NULL,
};

void fsm_cmoves_sGmpMap(              Game *IES) {
    SDL_Log(__func__);
    if (fsm_cmoves_sGmpMap_ss[Game_Substate_Current(IES)] != NULL) {
        fsm_cmoves_sGmpMap_ss[Game_Substate_Current(IES)](IES);
    }
}
void fsm_cmoves_sPrep(                Game *IES) {
    SDL_Log(__func__);
    SDL_Log("Game_Substate_Current(IES) %d %d", GAME_SUBSTATE_MAP_CANDIDATES,
            Game_Substate_Current(IES));

    if (fsm_cmoves_sPrep_ss[Game_Substate_Current(IES)] != NULL) {
        fsm_cmoves_sPrep_ss[Game_Substate_Current(IES)](IES);

    }
}
void fsm_cmoves_sTtlScrn(             Game *IES) {
    SDL_Log(__func__);
    Game_Cursor_Moves_onMenu(IES);
}
void fsm_cmoves_sGmpMap_ssMenu(       Game *IES) {
    SDL_Log(__func__);
    Game_Cursor_Moves_onMenu(IES);
}
void fsm_cmoves_sGmpMap_ssStby(       Game *IES) {
    Game_Cursor_Moves_onMap(IES);
}
void fsm_cmoves_sGmpMap_ssMapUnitMv(  Game *IES) {
    Game_Cursor_Moves_onMap(IES);
}
void fsm_cmoves_sPrep_ssMenu(    Game *IES) {
    Game_Cursor_Moves_onMenu(IES);
}
void fsm_cmoves_sPrep_ssMapCndt(    Game *IES) {
    SDL_Log(__func__);
    // TODO: should be Game_Cursor_Moves_onMenu ???
    Game_Cursor_Next_Candidate(IES);
}
void fsm_cmoves_sGmpMap_ssMapCndt(  Game *IES) {
    Game_Cursor_Next_Candidate(IES);
}

void Cursor_Moves(        tnecs_input *input) {
    // SDL_Log(__func__);
    Game *IES = input->data;
    SDL_assert(IES != NULL);
    SDL_Log("Game_State_Current(IES) %d %d", GAME_STATE_Preparation, Game_State_Current(IES));
    if (fsm_cmoves_s[Game_State_Current(IES)] != NULL) {
        fsm_cmoves_s[Game_State_Current(IES)](IES);
    }
    // Game_Cursor_Moves_onMenu:
    //  - sGmpMap_ssMenu
    //  - sTtlScrn
    // Game_Cursor_Moves_onMenu(IES);
    // Game_Cursor_Moves_onMap:
    //  - sGmpMap_ssMapUnitMv
    //  - sGmpMap_ssStby
    // Game_Cursor_Moves_onMap(IES);
}
