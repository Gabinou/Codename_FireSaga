
#include "controllerKeyboard.h"

struct controllerKeyboard controllerKeyboard_default = {
    .held_button = NULL,
    .held_move = NULL,
    .timeheld_button = 0.0f,
    .timeheld_move = 0.0f,
    .block_buttons = false,
    .block_move = false,
    .tilesize = {0, 0},
    .inputmap = NULL,
};

void Keyboard_checkMove(struct controllerKeyboard * in_keyboard, int8_t * in_pressed, size_t pressed_num, float dt) {
    bool arrequal = false;
    if ((in_keyboard->held_button_num == pressed_num)  && (pressed_num != 0)) {
        for (uint8_t i = 0; i < pressed_num; i++) {
            if (in_keyboard->held_move[i] != in_pressed[i]) {
                goto BREAK;
            }
        }
        arrequal = true;
    }
BREAK:
    {}
    if (arrequal) {
        in_keyboard->timeheld_button += dt;
    } else {
        in_keyboard->held_move = in_pressed;
        in_keyboard->held_move_num = pressed_num;
        in_keyboard->timeheld_move = 0.0f;
    }
}

void Keyboard_checkButton(struct controllerKeyboard * in_keyboard, int8_t * in_pressed, size_t pressed_num, float dt) {
    bool arrequal = false;
    if ((in_keyboard->held_button_num == pressed_num)  && (pressed_num != 0)) {
        for (uint8_t i = 0; i < pressed_num; i++) {
            if (in_keyboard->held_button[i] != in_pressed[i]) {
                goto BREAK;
            }
        }
        arrequal = true;
    }
BREAK:
    {}
    if (arrequal) {
        in_keyboard->timeheld_button += dt;
    } else {
        in_keyboard->held_button = in_pressed;
        in_keyboard->held_button_num = pressed_num;
        in_keyboard->timeheld_button = 0.0f;
    }
}

bool Keyboard_isPressed(struct controllerKeyboard * in_keyboard, const uint8_t * state_array, SDL_Scancode * to_find, size_t mapped_len) {
    // SDL_Log("Keyboard_isPressed");
    bool out = false;
    for (uint16_t i = 0; i < mapped_len; i++) {
        if (state_array[to_find[i]]) {
            out = true;
        }
    }
    return (out);
}

