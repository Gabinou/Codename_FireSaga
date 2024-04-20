#ifndef SYSTEM_CONTROL_H
#define SYSTEM_CONTROL_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "tnecs.h"
#include "enums.h"
#include "debug.h"
#include "events.h"
#include "position.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/touchpad.h"
#include "controller/mouse.h"

// /* -- Systems -- */
// void Control_Gamepad( tnecs_system_input *input);
// void Control_Keyboard(tnecs_system_input *input);
// void Control_Touchpad(tnecs_system_input *input);

// /* -- Utilities -- */
// void Control_Cursor_Moves(struct Game *s, struct Point cm, struct Point t,
//                           struct Point px, i32 ct);

// void Control_Pressed( i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
//                       bool o, i32 t, i32 m);
// void Gamepad_Pressed( i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
//                       struct controllerGamepad  *gp);
// void Keyboard_Pressed(i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
//                       struct controllerKeyboard *kb);


#endif /* SYSTEM_CONTROL_H */