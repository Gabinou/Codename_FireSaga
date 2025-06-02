#ifndef CONTROLLERGAMEPAD_H
#define CONTROLLERGAMEPAD_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "debug.h"
#include "SDL.h"

/* -- How to present controller remapping to players? -- */
// -> Use gameplay.standby sota_inputs as labels

extern const struct GamepadInputMap GamepadInputMap_default;
extern const struct GamepadInputMap GamepadInputMap_gamecube;
extern const struct GamepadInputMap GamepadInputMap_switch_pro;

typedef struct controllerGamepad {
    SDL_GameController     **controllers;
    struct GamepadInputMap  *inputmap;

    i32        *joystick_instances;
    i32         controller_type;
    int         controllers_num;
    int         controllers_len;

    i16         deadzone_joystick;
    i16         deadzone_trigger;

    i8          held_move[SOTA_DIRECTIONS_MAIN_NUM];
    i8          held_button[SOTA_INPUT_END];

    size_t      held_move_num;
    size_t      held_button_num;

    i32         timeheld_move_ns;
    i32         timeheld_button_ns;
    char      **sdl_button_names;

    b32        block_buttons   : 1;
    b32        block_move      : 1;
} controllerGamepad;
extern const struct controllerGamepad controllerGamepad_default;

enum GAMEPAD_BUTTON_NAMES {
    SOTA_INPUT_NAME_MAX_LEN     = 23,
    SOTA_BUTTON_NAME_MAX_LEN    = 27,
    SDL_BUTTON_NAME_MAX_LEN     = 36,
    AXIS_NAME_MAX_LEN           = 33,
};
extern const char sdl_button_names[SDL_CONTROLLER_BUTTON_MAX][SDL_BUTTON_NAME_MAX_LEN];
extern const char sdl_axis_names[SDL_CONTROLLER_AXIS_MAX][AXIS_NAME_MAX_LEN];
extern const char sota_button_names[SOTA_BUTTON_END][SOTA_BUTTON_NAME_MAX_LEN];
extern const char sota_input_names[SOTA_INPUT_END][SOTA_INPUT_NAME_MAX_LEN];

/* --- Constructors/Destructors --- */
void Gamepad_Init(   struct controllerGamepad *g);
void Gamepad_Free(   struct controllerGamepad *g);
void Gamepad_Init_tnecs(void                  *g);
void Gamepad_Free_tnecs(void                  *g);

void _Gamepad_Realloc(struct controllerGamepad *g);

/* -- Controller management -- */
void Gamepad_addController(   struct controllerGamepad *g, i32 j);
void Gamepad_removeController(struct controllerGamepad *g, i32 i);

/* -- Pressed button -- */
void Gamepad_Held(i8 *h, size_t *hn, i32 *t, i8 *p, size_t pn, i32 dt);
b32 Gamepad_isPressed(   struct controllerGamepad *g,  int sb);
b32 Gamepad_ButtonorAxis(struct controllerGamepad *g,  SDL_GameControllerButton b, int i, b32 is);
struct Point Gamepad_Joystick_Direction(struct controllerGamepad *g);

#endif /* CONTROLLERGAMEPAD_H */
