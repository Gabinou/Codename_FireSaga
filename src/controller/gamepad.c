
#include "controller/gamepad.h"

struct GamepadInputMap GamepadInputMap_gamecube = {
    // NOTE:
    /*  1. L/R buttons -> TriggerLeft and TriggerRight  */
    /*  2. Z button    -> RightShoulder                 */
    .axis_left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    .axis_left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    .axis_right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    .axis_right_y       = SDL_CONTROLLER_AXIS_RIGHTY,

    .dpad_right         = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    .dpad_up            = SDL_CONTROLLER_BUTTON_DPAD_UP,
    .dpad_down          = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    .dpad_left          = SDL_CONTROLLER_BUTTON_DPAD_LEFT,

    .a                  = SDL_CONTROLLER_BUTTON_A,
    .b                  = SDL_CONTROLLER_BUTTON_B,
    .x                  = SDL_CONTROLLER_BUTTON_X,
    .y                  = SDL_CONTROLLER_BUTTON_Y,
    .trigger_right      = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    .trigger_left       = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    .start              = SDL_CONTROLLER_BUTTON_START,
    .options            = SDL_CONTROLLER_BUTTON_INVALID,
    .faster             = SDL_CONTROLLER_BUTTON_INVALID,
    .globalRange        = SDL_CONTROLLER_BUTTON_INVALID,

    .accept_button      = true,
    .cancel_button      = true,
    .stats_button       = true,
    .menuright_button   = false,
    .menuleft_button    = false,
    .minimap_button     = true,
    .faster_button      = true,
    .pause_button       = true,
    .globalRange_button = false,
};

struct GamepadInputMap GamepadInputMap_default = {
    .axis_left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    .axis_left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    .axis_right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    .axis_right_y       = SDL_CONTROLLER_AXIS_RIGHTY,

    .dpad_right         = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    .dpad_up            = SDL_CONTROLLER_BUTTON_DPAD_UP,
    .dpad_down          = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    .dpad_left          = SDL_CONTROLLER_BUTTON_DPAD_LEFT,

    .a                  = SDL_CONTROLLER_BUTTON_A,
    .b                  = SDL_CONTROLLER_BUTTON_B,
    .y                  = SDL_CONTROLLER_BUTTON_Y,
    .shoulder_right     = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    .shoulder_left      = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    .start              = SDL_CONTROLLER_BUTTON_START,
    .x                  = SDL_CONTROLLER_BUTTON_X,
    .options            = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    .faster             = SDL_CONTROLLER_BUTTON_START,
    .globalRange        = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,

    .accept_button      = true,
    .cancel_button      = true,
    .stats_button       = true,
    .menuright_button   = true,
    .menuleft_button    = true,
    .minimap_button     = true,
    .faster_button      = true,
    .pause_button       = true,
    .globalRange_button = false,
};

char button_names[SDL_CONTROLLER_BUTTON_MAX][BUTTON_NAME_MAX_LEN] = {
    "SDL_CONTROLLER_BUTTON_A",
    "SDL_CONTROLLER_BUTTON_B",
    "SDL_CONTROLLER_BUTTON_X",
    "SDL_CONTROLLER_BUTTON_Y",
    "SDL_CONTROLLER_BUTTON_BACK",
    "SDL_CONTROLLER_BUTTON_GUIDE",
    "SDL_CONTROLLER_BUTTON_START",
    "SDL_CONTROLLER_BUTTON_LEFTSTICK",
    "SDL_CONTROLLER_BUTTON_RIGHTSTICK",
    "SDL_CONTROLLER_BUTTON_LEFTSHOULDER",
    "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER",
    "SDL_CONTROLLER_BUTTON_DPAD_UP",
    "SDL_CONTROLLER_BUTTON_DPAD_DOWN",
    "SDL_CONTROLLER_BUTTON_DPAD_LEFT",
    "SDL_CONTROLLER_BUTTON_DPAD_RIGHT",
    "SDL_CONTROLLER_BUTTON_MISC1",
    "SDL_CONTROLLER_BUTTON_PADDLE1",
    "SDL_CONTROLLER_BUTTON_PADDLE2",
    "SDL_CONTROLLER_BUTTON_PADDLE3",
    "SDL_CONTROLLER_BUTTON_PADDLE4",
    "SDL_CONTROLLER_BUTTON_TOUCHPAD"
};

char axis_names[SDL_CONTROLLER_AXIS_MAX][AXIS_NAME_MAX_LEN] = {
    "SDL_CONTROLLER_AXIS_LEFTX",
    "SDL_CONTROLLER_AXIS_LEFTY",
    "SDL_CONTROLLER_AXIS_RIGHTX",
    "SDL_CONTROLLER_AXIS_RIGHTY",
    "SDL_CONTROLLER_AXIS_TRIGGERLEFT",
    "SDL_CONTROLLER_AXIS_TRIGGERRIGHT",
};


struct controllerGamepad controllerGamepad_default = {
    .inputmap           = NULL,
    .controllers        = NULL,
    .joystick_instances = NULL,
    .controller_type    = CONTROLLER_GAMEPAD,

    .controllers_num    =    0,
    .controllers_len    =    2,

    .deadzone_joystick  = 8000,
    .deadzone_trigger   = 8000,
    .timeheld_move_ns   =    0,
    .timeheld_button_ns =    0,

    .block_buttons      =    0,
    .block_move         =    0,
};

/* --- Constructors/Destructors --- */
void Gamepad_Init(struct controllerGamepad *gp) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Gamepad_Free(gp);
    gp->controllers_num     = controllerGamepad_default.controllers_num;
    gp->controllers_len     = controllerGamepad_default.controllers_len;
    gp->controllers         = calloc(gp->controllers_len, sizeof(*gp->controllers));
    gp->joystick_instances  = calloc(gp->controllers_len, sizeof(*gp->joystick_instances));

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Gamepad_Free(struct controllerGamepad *gp) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (gp->controllers != NULL) {
        free(gp->controllers);
        gp->controllers = NULL;
    }
    if (gp->joystick_instances != NULL) {
        free(gp->joystick_instances);
        gp->joystick_instances = NULL;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Gamepad_ButtonorAxis(struct controllerGamepad *gp, int button, int i, bool isbutton) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_GameController *controller  = gp->controllers[i];
    bool out;
    if (isbutton) {
        out = SDL_GameControllerGetButton(controller, button);
        if (out)
            SDL_Log("Gamepad pressing %s", button_names[button]);
    } else {
        out = SDL_GameControllerGetAxis(controller, button) > gp->deadzone_trigger;
        if (out)
            SDL_Log("Gamepad pressing %s", axis_names[button]);
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

bool Gamepad_isPressed(struct controllerGamepad *gp, int sota_button) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    struct GamepadInputMap *map = gp->inputmap;
    int pure_button = (sota_button - SOTA_INPUT_DPAD_END); /* button index excluding dpad */

    /* -- Get mapped button from button index -- */
    SDL_GameControllerButton button = *(&map->dpad_right + sota_button);

    /* -- Check if button/axis is pressed, -- */
    /* EXCLUDES JOYSTICKS */
    bool *flags   = &map->accept_button;
    bool isbutton = (pure_button < 0) ? true : *(flags + pure_button);
    bool out = false;
    for (int i = 0; i < gp->controllers_num; i++) {
        out |= Gamepad_ButtonorAxis(gp, button, i, isbutton);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

struct Point Gamepad_Joystick_Direction(struct controllerGamepad *gp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if32    jdead     =  gp->deadzone_joystick;
    struct Point cursor_move = {0};
    struct GamepadInputMap      *im = gp->inputmap;
    for (int i = 0; i < gp->controllers_num; i++) {
        SDL_GameController *controller  = gp->controllers[i];

        Sint16 axis_left_x        = SDL_GameControllerGetAxis(controller, im->axis_left_x);
        Sint16 axis_left_y        = SDL_GameControllerGetAxis(controller, im->axis_left_y);
        Sint16 axis_right_x      = SDL_GameControllerGetAxis(controller, im->axis_right_x);
        Sint16 axis_right_y      = SDL_GameControllerGetAxis(controller, im->axis_right_y);

        /* - Left/Right axis - */
        bool pressed = false;
        if          ((axis_left_x >  jdead) || (axis_right_x >  jdead)) {
            cursor_move.x = 1;
            pressed = true;
        } else if   ((axis_left_x < -jdead) || (axis_right_x < -jdead)) {
            cursor_move.x = -1;
            pressed = true;
        }
        /* - Up/Down axis - */
        if          ((axis_left_y >  jdead) || (axis_right_y >  jdead)) {
            cursor_move.y = 1;
            pressed = true;
        } else if   ((axis_left_y < -jdead) || (axis_right_y < -jdead))  {
            cursor_move.y = -1;
            pressed = true;
        }
        if (pressed)
            break;
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (cursor_move);
}


void Gamepad_removeController(struct controllerGamepad *gp, if32 joystick_instance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("remove joystick %ld", joystick_instance);
    for (int i = 0; i < gp->controllers_num; i++) {
        if (joystick_instance == gp->joystick_instances[i]) {
            SDL_GameControllerClose(gp->controllers[i]);
            gp->controllers[i]           = NULL;
            gp->joystick_instances[i]   = -1;
            int num_to_move = gp->controllers_num-- - i;
            if (num_to_move > 0) {
                int bytesize = num_to_move * sizeof(*gp->controllers);
                memmove(gp->controllers        + i - 1, gp->controllers        + i, bytesize);
                bytesize = num_to_move * sizeof(*gp->joystick_instances);
                memmove(gp->joystick_instances + i - 1, gp->joystick_instances + i, bytesize);
            }
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Gamepad_Realloc(struct controllerGamepad *gp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(gp->controllers        != NULL);
    SDL_assert(gp->joystick_instances != NULL);
    SDL_assert(gp->controllers_len    > 0);
    gp->controllers_len *= 2;

    gp->controllers         = realloc(gp->controllers, gp->controllers_len * sizeof(*gp->controllers));
    gp->joystick_instances  = realloc(gp->joystick_instances,
                                      gp->controllers_len * sizeof(*gp->joystick_instances));

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Gamepad_addController(struct controllerGamepad *gp, if32 joystick_device) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("add joystick %ld", joystick_device);
    SDL_assert(SDL_IsGameController(joystick_device));
    if ((gp->controllers_num + 1) >= gp->controllers_len)
        Gamepad_Realloc(gp);

    gp->controllers[gp->controllers_num]        = SDL_GameControllerOpen(joystick_device);
    SDL_Joystick *joystick                      = SDL_GameControllerGetJoystick(
                                                          gp->controllers[gp->controllers_num]);
    gp->joystick_instances[gp->controllers_num] = SDL_JoystickInstanceID(joystick);
    gp->controllers_num++;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Gamepad_Held(if8 *held, size_t *h_num, if32 *held_ns, if8 *pressed, size_t p_num, if32 dt_ns) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(p_num < SOTA_INPUT_END);
    SDL_assert(p_num >= 0);
    bool arrequal = false;
    if ((*h_num == p_num) && (p_num != 0))
        arrequal = if8_all_equal(held, pressed, p_num);

    if (arrequal) {
        /* - Keep pressing same buttons - */
        *held_ns   += dt_ns;
    } else if (p_num > 0) {
        /* - Keep pressing different buttons - */
        memcpy(held, pressed, p_num * sizeof(*held));
        *h_num      = p_num;
        *held_ns   += dt_ns; /* Changing pressed buttons still counts as holding */
    } else if (p_num == 0) {
        /* - Don't press any buttons - */
        *held_ns    = 0;
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
