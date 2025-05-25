
#ifndef SYSTEM_CONTROL_H
#define SYSTEM_CONTROL_H

#include "tnecs.h"
#include "types.h"

void Control_Gamepad(     tnecs_input *input);
void Control_Touchpad(    tnecs_input *input);
void Control_Keyboard(    tnecs_input *input);
void Cursor_Scroll_Camera(tnecs_input *input);
void Control_Pressed(i8 IES_b,
                     i8 *press,
                     i8 *pressed_num,
                     i32 *controller_type,
                     u32 event,
                     b32 block,
                     i32 t_held_ns,
                     i32 t_min_ns);
#endif /* SYSTEM_CONTROL_H */
