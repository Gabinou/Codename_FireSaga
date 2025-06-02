
#include "controller/gamepad.h"
#include "structs.h"
#include "utilities.h"

/* -- GLOSSARY -- */
// - sota_button:   index to physical button in GamepadInputMap
// - mapped_button: user determining what the physical button should do

/* Gamecube controller */
const struct GamepadInputMap GamepadInputMap_gamecube = {
    /*  1. L/R buttons -> TriggerLeft and TriggerRight  */
    /*  2. Z button    -> RightShoulder                 */
    /*<sota_button>     = <mapped_button> */
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
    .start              = SDL_CONTROLLER_BUTTON_START,
    .shoulder_left      = SDL_CONTROLLER_BUTTON_INVALID,
    .shoulder_right     = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

    .trigger_left       = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    .trigger_right      = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
};

/* Switch Pro controller */
const struct GamepadInputMap GamepadInputMap_switch_pro = {
    /*<sota_button>     = <mapped_button> */
    .axis_left_x        = SDL_CONTROLLER_AXIS_LEFTX,
    .axis_left_y        = SDL_CONTROLLER_AXIS_LEFTY,
    .axis_right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
    .axis_right_y       = SDL_CONTROLLER_AXIS_RIGHTY,

    .dpad_right         = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
    .dpad_up            = SDL_CONTROLLER_BUTTON_DPAD_UP,
    .dpad_down          = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    .dpad_left          = SDL_CONTROLLER_BUTTON_DPAD_DOWN,

    /* Switch pro controller should flip a/b and x/y... */
    .a                  = SDL_CONTROLLER_BUTTON_B,
    .b                  = SDL_CONTROLLER_BUTTON_A,
    .x                  = SDL_CONTROLLER_BUTTON_Y,
    .y                  = SDL_CONTROLLER_BUTTON_X,
    .start              = SDL_CONTROLLER_BUTTON_START,
    .shoulder_left      = SDL_CONTROLLER_BUTTON_INVALID,
    .shoulder_right     = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

    .trigger_left       = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    .trigger_right      = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
};

/* Generic gamepad */
const struct GamepadInputMap GamepadInputMap_default = {
    /*<sota_button>     = <mapped_button> */
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
    .x                  = SDL_CONTROLLER_BUTTON_X,
    .start              = SDL_CONTROLLER_BUTTON_START,
    .shoulder_left      = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    .shoulder_right     = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

    .trigger_left       = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    .trigger_right      = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
};

const char sdl_button_names[SDL_CONTROLLER_BUTTON_MAX][SDL_BUTTON_NAME_MAX_LEN] = {
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

const char sota_button_names[SOTA_BUTTON_END][SOTA_BUTTON_NAME_MAX_LEN] = {
    "SOTA_BUTTON_DPAD_RIGHT",
    "SOTA_BUTTON_DPAD_UP",
    "SOTA_BUTTON_DPAD_DOWN",
    "SOTA_BUTTON_DPAD_LEFT",
    "SOTA_BUTTON_A",
    "SOTA_BUTTON_B",
    "SOTA_BUTTON_X",
    "SOTA_BUTTON_Y",
    "SOTA_BUTTON_START",
    "SOTA_BUTTON_SHOULDER_LEFT",
    "SOTA_BUTTON_SHOULDER_RIGHT",
    "SOTA_BUTTON_TRIGGER_LEFT",
    "SOTA_BUTTON_TRIGGER_RIGHT",
    "SOTA_BUTTON_KEYBOARD_M",
    "SOTA_BUTTON_KEYBOARD_SPACE",
};

const char sdl_axis_names[SDL_CONTROLLER_AXIS_MAX][AXIS_NAME_MAX_LEN] = {
    "SDL_CONTROLLER_AXIS_LEFTX",
    "SDL_CONTROLLER_AXIS_LEFTY",
    "SDL_CONTROLLER_AXIS_RIGHTX",
    "SDL_CONTROLLER_AXIS_RIGHTY",
    "SDL_CONTROLLER_AXIS_TRIGGERLEFT",
    "SDL_CONTROLLER_AXIS_TRIGGERRIGHT",
};

const char sota_input_names[SOTA_INPUT_END][SOTA_INPUT_NAME_MAX_LEN] = {
#define REGISTER_ENUM(x, y) "SOTA_INPUT_##x",
#include "names/input.h"
};
#undef REGISTER_ENUM

const struct controllerGamepad controllerGamepad_default = {
    .controller_type    = CONTROLLER_GAMEPAD,
    .controllers_len    =    2,
    .deadzone_joystick  = 8000,
    .deadzone_trigger   = 8000,
};

/* --- Constructors/Destructors --- */
void Gamepad_Init_tnecs(void *voidgp) {
    Gamepad_Init(voidgp);
}

void Gamepad_Free_tnecs(void *voidgp) {
    Gamepad_Free(voidgp);
}

void Gamepad_Init(struct controllerGamepad *gp) {
    Gamepad_Free(gp);
    gp->controllers_num     = controllerGamepad_default.controllers_num;
    gp->controllers_len     = controllerGamepad_default.controllers_len;
    gp->controllers         = SDL_calloc(gp->controllers_len, sizeof(*gp->controllers));
    SDL_assert(gp->controllers != NULL);
    gp->joystick_instances  = SDL_calloc(gp->controllers_len, sizeof(*gp->joystick_instances));
    SDL_assert(gp->joystick_instances != NULL);
}

void Gamepad_Free(struct controllerGamepad *gp) {
    if (gp->controllers != NULL) {
        SDL_free(gp->controllers);
        gp->controllers = NULL;
    }
    if (gp->joystick_instances != NULL) {
        SDL_free(gp->joystick_instances);
        gp->joystick_instances = NULL;
    }
}

b32 Gamepad_ButtonorAxis(struct controllerGamepad *gp, SDL_GameControllerButton sdl_button, int i,
                         b32 isbutton) {
    SDL_GameController *controller  = gp->controllers[i];
    b32 out;
    if (isbutton) {
        out = SDL_GameControllerGetButton(controller, sdl_button);
        // if (out)
        // SDL_Log("Gamepad pressing %s", sdl_button_names[sdl_button]);
    } else {
        out = SDL_GameControllerGetAxis(controller, sdl_button) > gp->deadzone_trigger;
        // if (out)
        // SDL_Log("Gamepad pressing %s", sdl_axis_names[sdl_button]);
    }

    return (out);
}
// Note: input button index in GamepadInputMap
b32 Gamepad_isPressed(struct controllerGamepad *gp, int sota_button) {
    /* -- Preliminaries -- */
    SDL_GameControllerButton *map = &gp->inputmap->dpad_right;
    SDL_GameControllerButton sdl_button = map[sota_button];

    /* -- Check if button/axis is pressed, -- */
    b32 isbutton = (sdl_button != SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    isbutton     &= (sdl_button != SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

    b32 out = false;
    for (int i = 0; i < gp->controllers_num; i++) {
        out |= Gamepad_ButtonorAxis(gp, sdl_button, i, isbutton);
    }
    return (out);
}

struct Point Gamepad_Joystick_Direction(struct controllerGamepad *gp) {
    i32    jdead     =  gp->deadzone_joystick;
    struct Point cursor_move = {0};
    struct GamepadInputMap      *im = gp->inputmap;
    for (int i = 0; i < gp->controllers_num; i++) {
        SDL_GameController *controller  = gp->controllers[i];

        Sint16 axis_left_x  = SDL_GameControllerGetAxis(controller, im->axis_left_x);
        Sint16 axis_left_y  = SDL_GameControllerGetAxis(controller, im->axis_left_y);
        Sint16 axis_right_x = SDL_GameControllerGetAxis(controller, im->axis_right_x);
        Sint16 axis_right_y = SDL_GameControllerGetAxis(controller, im->axis_right_y);

        /* - Left/Right axis - */
        b32 pressed = false;
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

    return (cursor_move);
}

void Gamepad_removeController(struct controllerGamepad *gp, i32 joystick_instance) {
    SDL_Log("remove joystick %d", joystick_instance);
    for (int i = 0; i < gp->controllers_num; i++) {
        if (joystick_instance == gp->joystick_instances[i]) {
            SDL_GameControllerClose(gp->controllers[i]);
            gp->controllers[i]          = NULL;
            gp->joystick_instances[i]   = -1;
            int num_to_move = gp->controllers_num-- - i;
            if (num_to_move > 0) {
                SDL_GameController **conts = gp->controllers;
                int bytesize = num_to_move * sizeof(*conts);
                memmove(conts + i - 1, conts + i, bytesize);

                i32 *joyst = gp->joystick_instances;
                bytesize = num_to_move * sizeof(*gp->joystick_instances);
                memmove(joyst + i - 1, joyst + i, bytesize);
            }
        }
    }
}

void _Gamepad_Realloc(struct controllerGamepad *gp) {
    SDL_assert(gp->controllers        != NULL);
    SDL_assert(gp->joystick_instances != NULL);
    SDL_assert(gp->controllers_len    > 0);
    gp->controllers_len *= 2;

    int bytesize            = gp->controllers_len * sizeof(*gp->controllers);
    SDL_assert(bytesize > 0);
    gp->controllers         = realloc(gp->controllers,          bytesize);

    bytesize                = gp->controllers_len * sizeof(*gp->joystick_instances);
    SDL_assert(bytesize > 0);
    gp->joystick_instances  = realloc(gp->joystick_instances,   bytesize);

    SDL_assert(gp->controllers        != NULL);
    SDL_assert(gp->joystick_instances != NULL);

}

void Gamepad_addController(struct controllerGamepad *gp, i32 joystick_device) {
    SDL_Log("add joystick %d", joystick_device);
    SDL_assert(SDL_IsGameController(joystick_device));
    if ((gp->controllers_num + 1) >= gp->controllers_len)
        _Gamepad_Realloc(gp);

    SDL_GameController *controllers = gp->controllers[gp->controllers_num];
    gp->controllers[gp->controllers_num]        = SDL_GameControllerOpen(joystick_device);
    SDL_Joystick *joystick                      = SDL_GameControllerGetJoystick(controllers);
    gp->joystick_instances[gp->controllers_num] = SDL_JoystickInstanceID(joystick);
    gp->controllers_num++;
}

void Gamepad_Held(i8 *held, size_t *h_num, i32 *held_ns, i8 *pressed, size_t p_num, i32 dt_ns) {
    SDL_assert(p_num < SOTA_INPUT_END);
    SDL_assert(p_num >= 0);
    b32 arrequal = false;
    if ((*h_num == p_num) && (p_num != 0))
        arrequal = i8_all_equal(held, pressed, p_num);

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
}
