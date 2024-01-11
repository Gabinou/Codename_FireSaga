
#include "systems/control.h"

void Control_Cursor_Moves(struct Game *sota,    struct Point cursor_move,
                          struct Point target,  struct Point px_pos,
                          i32 controller_type) {
    /* - Immobile cursor - */
    if ((cursor_move.x == 0) && (cursor_move.y == 0)) {
        return;
    }

    sota->cursor_frame_moved = true;

    /* - Block sliding cursor - */
    if ((target.x != px_pos.x) || (target.y != px_pos.y)) {
        return;
    }

    /* - Pause cursor after moving - */
    i32 min     = 0;
    i32 max     = CURSOR_FIRSTMOVE_PAUSE_ms;
    i32 moved   = sota->cursor_moved_time_ms;
    if ((moved > min) && (moved < max)) {
        return;
    }

    sota->cursor_move     = cursor_move;
    sota->controller_code = controller_type;

}

void Gamepad_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                     u32 event, struct controllerGamepad *gp) {
    i32 theld       = gp->timeheld_button_ns;
    i32 min_held    = GAMEPAD_MINHELD_ns;
    bool butblk     = gp->block_buttons;

    Control_Pressed(sota_b, press, pressed_num, controller_type,
                    event, butblk, theld, min_held);
}

void Keyboard_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                      u32 event, struct controllerKeyboard *kb) {
    i32 theld       = kb->timeheld_button_ns;
    i32 min_held    = KEYBOARD_MINHELD_ns;
    bool butblk     = kb->block_buttons;

    Control_Pressed(sota_b, press, pressed_num, controller_type,
                    event, butblk, theld, min_held);
}

void Control_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
                     u32 event, bool block, i32 t_held_ns, i32 t_min_ns) {
    press[(*pressed_num)++] = sota_b;
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

void Control_Keyboard(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data != NULL);
    struct Point cursor_move = {0};

    /* -- Get game -- */
    struct Game *sota = input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Slider             *sld_arr      = TNECS_COMPONENTS_LIST(input, Slider);
    struct Position           *pos_arr      = TNECS_COMPONENTS_LIST(input, Position);
    struct controllerKeyboard *keyboard_arr = TNECS_COMPONENTS_LIST(input, controllerKeyboard);
    SDL_assert(sld_arr      != NULL);
    SDL_assert(pos_arr      != NULL);
    SDL_assert(keyboard_arr != NULL);

    for (int order = 0; order < input->num_entities; order++) {
        SDL_LogVerbose(SOTA_LOG_FPS, "%d", order);

        /* -- Preliminaries -- */
        struct controllerKeyboard *kb       = keyboard_arr + order;
        struct KeyboardInputMap   *im       = kb->inputmap;
        i32                       *ct       = &kb->controller_type;
        const u8                  *kb_state = SDL_GetKeyboardState(NULL);

        /* -- Keyboard button checking -- */
        i8 press[SOTA_BUTTON_END];
        i8 pnum          = 0;
        bool butblk      =  kb->block_buttons;
        size_t *mheld    = &kb->held_move_num;
        i32    *theld    = &kb->timeheld_button_ns;
        i32    *theld_ns = &kb->timeheld_move_ns;
        size_t *bheld    = &kb->held_button_num;

        for (int sota_b = SOTA_BUTTON_A; sota_b < SOTA_BUTTON_END; sota_b++) {
            if (Keyboard_isPressed(kb, kb_state, sota_b))
                Keyboard_Pressed(sota_b, press, &pnum, ct, sota->inputs[sota_b], kb);
        }

        Keyboard_Held(kb->held_button, bheld, theld, press, pnum, input->deltat);

        /* - Unblock keyboard - */
        if ((*theld) == 0)
            kb->block_buttons = false;

        /* -- Keyboard move checking -- */
        i8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
        i8 pmove_num = 0;
        bool right = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_RIGHT);
        bool up    = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_UP);
        bool left  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_LEFT);
        bool down  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_DOWN);

        /* - Collapse diagonals to one of 4 main directions - */
        if (up && !down) {
            cursor_move.y       = -1;
            pmove[pmove_num++]  = SOTA_INPUT_UP;
        } else if (down && !up) {
            cursor_move.y       =  1;
            pmove[pmove_num++]  = SOTA_INPUT_DOWN;
        }
        if (left && !right) {
            cursor_move.x       = -1;
            pmove[pmove_num++]  = SOTA_INPUT_LEFT;
        } else if (right && !left) {
            cursor_move.x       =  1;
            pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
        }

        Keyboard_Held(kb->held_move, mheld, theld_ns, press, pnum, input->deltat);

        // #ifdef SOTA_MUSIC_TOGGLE
        // if (kb_state[SOTA_MUSIC_TOGGLE]) {
        //     i32 held_ns = kb->timeheld_button_ns;
        //     if (held_ns < 1000)
        //         Event_Emit(__func__, SDL_USEREVENT, event_Music_Toggle, NULL, NULL);
        // }
        // #endif /* SOTA_MUSIC_TOGGLE */

        // #ifdef SOTA_INTERACTIVE_RELOAD
        // if (kb_state[SOTA_INTERACTIVE_RELOAD])
        //     Event_Emit(__func__, SDL_USEREVENT, event_Reload, NULL, NULL);
        // #endif /* SOTA_INTERACTIVE_RELOAD */

        struct Point target    = sld_arr[order].target;
        struct Point pixel_pos = pos_arr[order].pixel_pos;
        Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, *ct);
    }
}

void Control_Gamepad(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data != NULL);
    struct Point cursor_move = {0};

    /* -- Get game -- */
    struct Game *sota = input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Slider            *sld_arr       = TNECS_COMPONENTS_LIST(input, Slider);
    struct Position          *pos_arr       = TNECS_COMPONENTS_LIST(input, Position);
    struct controllerGamepad *gamepad_arr   = TNECS_COMPONENTS_LIST(input, controllerGamepad);

    for (int order = 0; order < input->num_entities; order++) {
        SDL_LogVerbose(SOTA_LOG_FPS, "%d", order);
        struct controllerGamepad *gp = gamepad_arr + order;

        /* -- Preliminaries -- */
        i8 press[SOTA_BUTTON_END];
        i8 pnum = 0;
        size_t  *mheld    = &gp->held_move_num;
        i32     *theld_ns = &gp->timeheld_move_ns;
        i32     *theld    = &gp->timeheld_button_ns;
        size_t  *bheld    = &gp->held_button_num;

        /* -- Gamepad button checking -- */
        for (int sota_b = SOTA_BUTTON_A; sota_b <= SOTA_BUTTON_TRIGGER_RIGHT; sota_b++) {
            if (Gamepad_isPressed(gp, sota_b))
                Gamepad_Pressed(sota_b, press, &pnum, &gp->controller_type, sota->inputs[sota_b], gp);
        }

        Gamepad_Held(gp->held_button, bheld, theld, press, pnum, input->deltat);

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

        Gamepad_Held(gp->held_move, mheld, theld_ns, pmove, pmove_num, input->deltat);

        struct Point target    = sld_arr[order].target;
        struct Point pixel_pos = pos_arr[order].pixel_pos;
        Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, gp->controller_type);
    }
}

void Control_Touchpad(tnecs_system_input *input) {

}
