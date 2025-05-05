#ifndef CONTROLLERKEYBOARD_H
#define CONTROLLERKEYBOARD_H

#include <math.h>
#include <stdbool.h>
#include "types.h"
#include "enums.h"

typedef struct controllerKeyboard {
    struct KeyboardInputMap *inputmap;

    i8     held_button[SOTA_INPUT_END];
    i8     held_move[SOTA_DIRECTIONS_MAIN_NUM];

    i32    controller_type;

    size_t  held_button_num;
    size_t  held_move_num;

    i32    timeheld_button_ns;
    i32    timeheld_move_ns;

    b32    block_buttons : 1;
    b32    block_move    : 1;
} controllerKeyboard;
extern const struct controllerKeyboard controllerKeyboard_default;

/* -- Pressed button -- */
void Keyboard_Held(i8 *h, size_t *hn, i32 *t, i8 *p, size_t pn, i32 dt);
b32 Keyboard_isPressed(struct controllerKeyboard *k, const u8 *sa, int button);

#endif /* CONTROLLERKEYBOARD_H */
