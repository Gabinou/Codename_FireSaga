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
    SDL_Scancode    moveright[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    moveup[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    moveleft[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    movedown[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    accept[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    cancel[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    minimap[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    menuright[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    menuleft[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    pause[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    stats[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    options[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    faster[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    globalRange[SOTA_MAPPABLE_BUTTONS_NUM];

    uf8  moveright_len;
    uf8  moveup_len;
    uf8  moveleft_len;
    uf8  movedown_len;
    uf8  accept_len;
    uf8  cancel_len;
    uf8  minimap_len;
    uf8  menuright_len;
    uf8  menuleft_len;
    uf8  pause_len;
    uf8  stats_len;
    uf8  options_len;
    uf8  faster_len;
    uf8  globalRange_len;

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
