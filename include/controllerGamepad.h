#ifndef CONTROLLERGAMEPAD_H
#define CONTROLLERGAMEPAD_H

#include <math.h>
#include "map.h"
#include "utilities.h"

extern SDL_Renderer * Game_renderer;

typedef struct controllerGamepad {
    SDL_GameController * controller;
    struct GamepadInputMap * inputmap;
    int32_t * controller_indices; // joystick indices that are supported by the game controller interface
    // used controller should always be zero.
    int16_t joystick_deadzone;

    int8_t * held_move;
    int8_t * held_button;
    size_t held_move_num;
    size_t held_button_num;

    bool block_buttons;
    bool block_move;

    float timeheld_move;
    float timeheld_button;
} controllerGamepad;
extern struct controllerGamepad controllerGamepad_default;

extern bool Gamepad_isPressed(struct controllerGamepad * in_gamepad, SDL_GameControllerButton * in_map, size_t mapped_len);
extern void Gamepad_setController(struct controllerGamepad * in_gamepad, int32_t in_index);
extern void Gamepad_removeController(struct controllerGamepad * in_gamepad, int32_t in_index);
extern void Gamepad_addController(struct controllerGamepad * in_gamepad, int32_t in_joystick);
extern void Gamepad_checkMove(struct controllerGamepad * in_gamepad, int8_t * in_pressed, size_t pressed_num, float dt);
extern void Gamepad_checkButton(struct controllerGamepad * in_gamepad, int8_t * in_pressed, size_t pressed_num, float dt);


#endif /* CONTROLLERGAMEPAD_H */
