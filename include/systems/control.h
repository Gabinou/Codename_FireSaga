#ifndef SYSTEM_CONTROL_H
#define SYSTEM_CONTROL_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "tnecs.h"
#include "enums.h"
#include "debug.h"
#include "events.h"
#include "position.h"
#include "globals.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/touchpad.h"
#include "controller/mouse.h"

/* -- Systems -- */
extern void Control_Gamepad( tnecs_system_input_t *in_input);
extern void Control_Keyboard(tnecs_system_input_t *in_input);
extern void Control_Touchpad(tnecs_system_input_t *in_input);

/* -- Utilities -- */
extern void Control_Cursor_Moves(struct Game *s, struct Point cm, struct Point t,
                                 struct Point px, if32 ct);
extern void Control_Pressed(if8 b, if8 *l, if8 *p, bool o, if32 t,  if32 *c, u32 e, if32 m);
extern void Keyboard_Pressed(if8 b, if8 *p, if8 *pnum, if32 *cvt, u32 e,
                             struct controllerKeyboard *kb);
extern void Gamepad_Pressed(if8 b, if8 *p, if8 *pnum, if32 *cvt, u32 e,
                            struct controllerGamepad *gp);


#endif /* SYSTEM_CONTROL_H */