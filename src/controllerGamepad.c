
#include "controllerGamepad.h"
#include "stb_ds.h"

struct controllerGamepad controllerGamepad_default = {
    .inputmap = NULL,
    .controller = NULL,
    .controller_indices = NULL, // joystick indices that are supported by the game controller interface
    // used controller should always be zero.

    .joystick_deadzone = 8000,
    .held_move = NULL,
    .held_button = NULL,
    .timeheld_move = 0.0f,
    .timeheld_button = 0.0f,

    .block_buttons = 0,
    .block_move = 0,
};

void controllerGamepadModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, controllerGamepadModule);
    ECS_COMPONENT(in_world, controllerGamepad);
    ECS_SET_COMPONENT(controllerGamepad);
    ECS_EXPORT_COMPONENT(controllerGamepad);
}

bool Gamepad_isPressed(struct controllerGamepad * in_gamepad, SDL_GameControllerButton * in_map, size_t mapped_len) {
    bool out = false;
    for (size_t i = 0; i < mapped_len; ++i) {
        if (SDL_GameControllerGetButton(in_gamepad->controller, in_map[i])) {
            out = true;
        }
    }
    return (out);
}

void Gamepad_setController(struct controllerGamepad * in_gamepad, int32_t in_index) {
    if (arrlen(in_gamepad->controller_indices) > 0) {
        in_gamepad->controller = SDL_GameControllerOpen(in_gamepad->controller_indices[in_index]);
    }
}

void Gamepad_removeController(struct controllerGamepad * in_gamepad, int32_t in_index) {
    // This is weird. SHould remove controller work like this?
    SDL_Log("Removing controller %d", in_index);
    SDL_assert(SDL_IsGameController(in_gamepad->controller_indices[in_index]));
    SDL_GameControllerClose(in_gamepad->controller);
    arrdel(in_gamepad->controller_indices, in_index);
    in_gamepad->controller = NULL;
    Gamepad_setController(in_gamepad, 0);
}

void Gamepad_addController(struct controllerGamepad * in_gamepad, int32_t in_joystick) {
    SDL_Log("Adding controller %d", in_joystick);
    SDL_assert(SDL_IsGameController(in_joystick));
    SDL_assert(!in_gamepad->controller);
    SDL_assert(in_gamepad->controller = SDL_GameControllerOpen(in_joystick));
    arrput(in_gamepad->controller_indices, in_joystick);
}

void Gamepad_checkMove(struct controllerGamepad * in_gamepad, int8_t * in_pressed, size_t pressed_num, float dt) {
    bool arrequal = false;
    if ((in_gamepad->held_button_num == pressed_num)  && (pressed_num != 0)) {
        for (uint8_t i = 0; i < pressed_num; i++) {
            if (in_gamepad->held_button[i] != in_pressed[i]) {
                goto BREAK;
            }
        }
        arrequal = true;
    }
BREAK:
    {}
    if (arrequal) {
        in_gamepad->timeheld_move += dt;
    } else {
        in_gamepad->held_move = in_pressed;
        in_gamepad->held_move_num = pressed_num;
        in_gamepad->timeheld_move = 0.0f;
    }
}

void Gamepad_checkButton(struct controllerGamepad * in_gamepad, int8_t * in_pressed, size_t pressed_num, float dt) {
    // SDL_Log("dt %f in_gamepad->timeheld_button %f", dt, in_gamepad->timeheld_button);
    bool arrequal = false;
    if ((in_gamepad->held_button_num == pressed_num)  && (pressed_num != 0)) {
        for (uint8_t i = 0; i < pressed_num; i++) {
            if (in_gamepad->held_button[i] != in_pressed[i]) {
                goto BREAK;
            }
        }
        arrequal = true;
    }
BREAK:
    {}
    if (arrequal) {
        in_gamepad->timeheld_button += dt;
    } else {
        in_gamepad->held_button = in_pressed;
        in_gamepad->held_button_num = pressed_num;
        in_gamepad->timeheld_button = 0.0f;
    }
}

