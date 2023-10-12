#ifndef CONTROLLERKEYBOARD_H
#define CONTROLLERKEYBOARD_H

#include <math.h>
#include <stdbool.h>
#include "types.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "nmath.h"
#include "globals.h"
#include "utilities.h"
#include "SDL2/SDL.h"

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
    SDL_Scancode    trigger_right[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    trigger_left[SOTA_MAPPABLE_BUTTONS_NUM];

    uf8  dpad_right_len;
    uf8  dpad_up_len;
    uf8  dpad_left_len;
    uf8  dpad_down_len;
    uf8  a_len;
    uf8  b_len;
    uf8  x_len;
    uf8  y_len;
    uf8  start_len;
    uf8  shoulder_left_len;
    uf8  shoulder_right_len;
    uf8  trigger_right_len;
    uf8  trigger_left_len;

} KeyboardInputMap_default;

typedef struct controllerKeyboard {
    struct KeyboardInputMap *inputmap;

    if8     held_button[SOTA_INPUT_END];
    if8     held_move[SOTA_DIRECTIONS_MAIN_NUM];

    if32    controller_type;

    size_t  held_button_num;
    size_t  held_move_num;

    if32    timeheld_button_ns;
    if32    timeheld_move_ns;

    bool    block_buttons : 1;
    bool    block_move    : 1;
} controllerKeyboard;
extern struct controllerKeyboard controllerKeyboard_default;

/* -- Pressed button -- */
extern void Keyboard_Held(if8 *h, size_t *hn, if32 *t, if8 *p, size_t pn, if32 dt);
extern bool Keyboard_isPressed(struct controllerKeyboard *k, const uf8 *sa, int button);

#endif /* CONTROLLERKEYBOARD_H */
