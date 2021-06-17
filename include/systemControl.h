#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "tnecs.h"
#include "enums.h"
#include "events.h"
#include "position.h"
#include "controllerKeyboard.h"
#include "controllerGamepad.h"
#include "controllerTouchpad.h"
#include "controllerMouse.h"

// Forward declarations
extern void Event_Emit(uint32_t in_event_type, int32_t in_event_code, void * in_data1, void * in_data2);

extern struct Point * cursor_move;
extern int32_t * controller_keyboard;
extern int32_t * controller_gamepad;
extern int32_t * controller_touchpad;
extern int32_t * controller_mouse;

extern void controlMalloc();
extern void controlFree();

extern void controlMouse(tnecs_system_input_t * in_input);
extern void controlKeyboard(tnecs_system_input_t * in_input);
extern void controlGamepad(tnecs_system_input_t * in_input);
extern void controlTouchpad(tnecs_system_input_t * in_input);

#endif /* SYSTEMCONTROL_H */