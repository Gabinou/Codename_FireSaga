
#include "systems/control.h"

void Control_Cursor_Moves(struct Game *sota, struct Point cursor_move, struct Point target,
                          struct Point px_pos, if32 controller_type) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Immobile cursor - */
    if ((cursor_move.x == 0) && (cursor_move.y == 0)) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    sota->cursor_frame_moved = true;

    /* - Block sliding cursor - */
    if ((target.x != px_pos.x) || (target.y != px_pos.y)) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* - Pause cursor after moving - */
    int min = 0;
    int max = CURSOR_FIRSTMOVE_PAUSE_ms;
    if ((sota->cursor_moved_time_ms > min) && (sota->cursor_moved_time_ms < max)) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    sota->cursor_move     = cursor_move;
    sota->controller_code = controller_type;

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Gamepad_Pressed(if8 sota_b, if8 *press, if8 *pressed_num, if32 *controller_type,
                     u32 event, struct controllerGamepad *gp) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if32 theld      = gp->timeheld_button_ns;
    if32 min_held   = GAMEPAD_MINHELD_ns;
    bool butblk     = gp->block_buttons;

    Control_Pressed(sota_b, press, pressed_num, butblk, theld,
                    controller_type, event, min_held);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Keyboard_Pressed(if8 sota_b, if8 *press, if8 *pressed_num, if32 *controller_type,
                      u32 event, struct controllerKeyboard *kb) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if32 theld      = kb->timeheld_button_ns;
    if32 min_held   = KEYBOARD_MINHELD_ns;
    bool butblk     = kb->block_buttons;

    Control_Pressed(sota_b, press, pressed_num, butblk, theld,
                    controller_type, event, min_held);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Control_Pressed(if8 sota_b, if8 *press, if8 *pressed_num, bool block, if32 t_held_ns,
                     if32 *controller_type, u32 event, if32 t_min_ns) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    press[(*pressed_num)++] = sota_b;
    if (block) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    if (event <= 0)
        return;
    // TODO: fsm for button events
    if ((t_min_ns <= 0) || (t_held_ns > t_min_ns))
        Event_Emit(__func__, SDL_USEREVENT, event, controller_type, NULL);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Control_Keyboard(tnecs_system_input_t *input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
        SOTA_Log_FPS("%d", order);

        /* -- Preliminaries -- */
        struct controllerKeyboard *kb       = keyboard_arr + order;
        struct KeyboardInputMap   *im       = kb->inputmap;
        if32                      *ct       = &kb->controller_type;
        const uf8                 *kb_state = SDL_GetKeyboardState(NULL);

        /* -- Keyboard button checking -- */
        if8 press[SOTA_BUTTON_END];
        if8 pnum         = 0;
        bool butblk      =  kb->block_buttons;
        size_t *mheld    = &kb->held_move_num;
        if32   *theld    = &kb->timeheld_button_ns;
        if32   *theld_ns = &kb->timeheld_move_ns;
        size_t *bheld    = &kb->held_button_num;

        for (int b = SOTA_INPUT_ACCEPT; b <= SOTA_INPUT_GLOBALRANGE; b++) {
            if (Keyboard_isPressed(kb, kb_state, b))
                Keyboard_Pressed(b, press, &pnum, ct, btn_ev[b], kb);
        }

        Keyboard_Held(kb->held_button, bheld, theld, press, pnum, input->deltat);

        /* - Unblock keyboard - */
        if ((*theld) == 0)
            kb->block_buttons = false;

        /* -- Keyboard move checking -- */
        if8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
        if8 pmove_num = 0;
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

        struct Point target    = sld_arr[order].target;
        struct Point pixel_pos = pos_arr[order].pixel_pos;
        Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, *ct);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Control_Gamepad(tnecs_system_input_t *input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
        SOTA_Log_FPS("%d", order);
        struct controllerGamepad *gp = gamepad_arr + order;

        /* -- Preliminaries -- */
        if8 press[SOTA_BUTTON_END];
        if8 pnum = 0;
        size_t  *mheld    = &gp->held_move_num;
        if32    *theld_ns = &gp->timeheld_move_ns;
        if32    *theld    = &gp->timeheld_button_ns;
        size_t  *bheld    = &gp->held_button_num;

        /* -- Gamepad button checking -- */
        // TODO: use controller buttons
        for (int sota_b = SOTA_BUTTON_A; sota_b < SOTA_BUTTON_END; sota_b++) {
            int sdl_button = sdl_buttons[sota_b];
            if (Gamepad_isPressed(gp, sdl_button))
                Gamepad_Pressed(sota_b, press, &pnum, &gp->controller_type, btn_ev[sota_b], gp);
        }

        Gamepad_Held(gp->held_button, bheld, theld, press, pnum, input->deltat);

        /* - Unblock gamepad - */
        if (gp->timeheld_button_ns == 0)
            gp->block_buttons = false;

        /* -- Gamepad move checking -- */
        if8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
        if8 pmove_num = 0;

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
        if (Gamepad_isPressed(gp, SOTA_INPUT_RIGHT)) {
            cursor_move.x       =  1;
            pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
        } else if (Gamepad_isPressed(gp, SOTA_INPUT_LEFT)) {
            cursor_move.x       = -1;
            pmove[pmove_num++]  = SOTA_INPUT_LEFT;
        }
        if (Gamepad_isPressed(gp, SOTA_INPUT_UP)) {
            cursor_move.y       = -1;
            pmove[pmove_num++]  = SOTA_INPUT_UP;
        } else if (Gamepad_isPressed(gp, SOTA_INPUT_DOWN)) {
            cursor_move.y       =  1;
            pmove[pmove_num++]  = SOTA_INPUT_DOWN;
        }

        Gamepad_Held(gp->held_move, mheld, theld_ns, pmove, pmove_num, input->deltat);

        struct Point target    = sld_arr[order].target;
        struct Point pixel_pos = pos_arr[order].pixel_pos;
        Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, gp->controller_type);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Control_Touchpad(tnecs_system_input_t *input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
