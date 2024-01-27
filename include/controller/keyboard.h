#ifndef CONTROLLERKEYBOARD_H
#define CONTROLLERKEYBOARD_H

#include <math.h>
#include <stdbool.h>
#include "types.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "nmath.h"
#include "utilities.h"
#include "controller/gamepad.h"
#include "SDL2/SDL.h"

#define SOTA_INTERACTIVE_RELOAD SDL_SCANCODE_SPACE

#define SOTA_MUSIC_TOGGLE SDL_SCANCODE_M

extern struct KeyboardInputMap {
    /* Physical dpad -> Keyboard equivalent */
    SDL_Scancode    dpad_right[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_up[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_down[SOTA_MAPPABLE_BUTTONS_NUM];

    /* Physical buttons -> Keyboard equivalent */
    SDL_Scancode    a[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    b[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    y[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    x[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    start[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    shoulder_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    shoulder_right[SOTA_MAPPABLE_BUTTONS_NUM];

    /* Physical triggers -> Keyboard equivalent */
    /* Triggers  [0, INT_FAST16_MAX] -> [0, 32767] */
    SDL_Scancode    trigger_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    trigger_right[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    m[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    space[SOTA_MAPPABLE_BUTTONS_NUM];

    u8  dpad_right_len;
    u8  dpad_up_len;
    u8  dpad_left_len;
    u8  dpad_down_len;
    u8  a_len;
    u8  b_len;
    u8  x_len;
    u8  y_len;
    u8  start_len;
    u8  shoulder_left_len;
    u8  shoulder_right_len;
    u8  trigger_right_len;
    u8  trigger_left_len;
    u8  m_len;
    u8  space_len;

} KeyboardInputMap_default;

typedef struct controllerKeyboard {
    struct KeyboardInputMap *inputmap;

    i8     held_button[SOTA_INPUT_END];
    i8     held_move[SOTA_DIRECTIONS_MAIN_NUM];

    i32    controller_type;

    size_t  held_button_num;
    size_t  held_move_num;

    i32    timeheld_button_ns;
    i32    timeheld_move_ns;

    bool    block_buttons : 1;
    bool    block_move    : 1;
} controllerKeyboard;
extern struct controllerKeyboard controllerKeyboard_default;

/* -- Pressed button -- */
void Keyboard_Held(i8 *h, size_t *hn, i32 *t, i8 *p, size_t pn, i32 dt);
bool Keyboard_isPressed(struct controllerKeyboard *k, const u8 *sa, int button);

#endif /* CONTROLLERKEYBOARD_H */
