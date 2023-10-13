#ifndef CONTROLLERGAMEPAD_H
#define CONTROLLERGAMEPAD_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "nmath.h"
#include "debug.h"
#include "utilities.h"
#include "macros.h"
#include "globals.h"
#include "SDL2/SDL.h"

/* -- How to present controller remapping to players? -- */
// -> Use gameplay.standby sota_inputs as labels

struct GamepadInputMap {
    /* Physical joysticks -> no user change */
    /* Joysticks: [INT_FAST16_MIN, INT_FAST16_MAX] -> [-32768, 32767] */
    SDL_GameControllerAxis    axis_left_x;
    SDL_GameControllerAxis    axis_left_y;
    SDL_GameControllerAxis    axis_right_x;
    SDL_GameControllerAxis    axis_right_y;

    /* Physical dpad -> no user change */
    SDL_GameControllerButton  dpad_right;
    SDL_GameControllerButton  dpad_up;
    SDL_GameControllerButton  dpad_left;
    SDL_GameControllerButton  dpad_down;

    /* Physical buttons -> user can change */
    SDL_GameControllerButton  a;
    SDL_GameControllerButton  b;
    SDL_GameControllerButton  x;
    SDL_GameControllerButton  y;
    SDL_GameControllerButton  start;
    SDL_GameControllerButton  shoulder_left;
    SDL_GameControllerButton  shoulder_right;

    /* Physical triggers -> user can change */
    /* Triggers  [0, INT_FAST16_MAX] -> [0, 32767] */
    SDL_GameControllerButton  trigger_left;
    SDL_GameControllerButton  trigger_right;
};

extern struct GamepadInputMap GamepadInputMap_default;
extern struct GamepadInputMap GamepadInputMap_gamecube;

typedef struct controllerGamepad {
    SDL_GameController     **controllers;
    struct GamepadInputMap  *inputmap;

    if32       *joystick_instances;
    if32        controller_type;
    int         controllers_num;
    int         controllers_len;

    if16        deadzone_joystick;
    if16        deadzone_trigger;

    if8         held_move[SOTA_DIRECTIONS_MAIN_NUM];
    if8         held_button[SOTA_INPUT_END];

    size_t      held_move_num;
    size_t      held_button_num;

    if32        timeheld_move_ns;
    if32        timeheld_button_ns;
    char      **sdl_button_names;

    bool        block_buttons   : 1;
    bool        block_move      : 1;
} controllerGamepad;
extern struct controllerGamepad controllerGamepad_default;

enum GAMEPAD_BUTTON_NAMES {
    SOTA_BUTTON_NAME_MAX_LEN    = 36,
    SDL_BUTTON_NAME_MAX_LEN     = 36,
    AXIS_NAME_MAX_LEN           = 33,
};
extern char sdl_button_names[SDL_CONTROLLER_BUTTON_MAX][SDL_BUTTON_NAME_MAX_LEN];
extern char sdl_axis_names[SDL_CONTROLLER_AXIS_MAX][AXIS_NAME_MAX_LEN];
extern char sota_button_names[SOTA_BUTTON_END][SOTA_BUTTON_NAME_MAX_LEN]

/* --- Constructors/Destructors --- */
extern void Gamepad_Init(struct controllerGamepad *g);
extern void Gamepad_Free(struct controllerGamepad *g);

/* -- Controller management -- */
extern void Gamepad_Realloc(struct controllerGamepad *g);
extern void Gamepad_addController(   struct controllerGamepad *g, if32 j);
extern void Gamepad_removeController(struct controllerGamepad *g, if32 i);

/* -- Pressed button -- */
extern void Gamepad_Held(if8 *h, size_t *hn, if32 *t, if8 *p, size_t pn, if32 dt);
extern bool Gamepad_isPressed(   struct controllerGamepad *g,  int sb);
extern bool Gamepad_ButtonorAxis(struct controllerGamepad *g,  int b, int i, bool is);
extern struct Point Gamepad_Joystick_Direction(struct controllerGamepad *g);

#endif /* CONTROLLERGAMEPAD_H */
