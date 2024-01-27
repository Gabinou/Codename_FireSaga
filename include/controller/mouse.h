#ifndef CONTROLLERMOUSE_H
#define CONTROLLERMOUSE_H

// #include <math.h>
// #include <stdbool.h>
#include "macros.h"
#include "debug.h"
#include "tnecs.h"
#include "enums.h"
// #include "nmath.h"
#include "events.h"
#include "SDL2/SDL.h"

struct MouseInputMap {
    u8 accept;
    u8 cancel;
    u8 stats;
    u8 trigger_right;
    u8 trigger_left;
    u8 minimap;
    u8 faster;
    u8 pause;

    u8 accept_len;
    u8 cancel_len;
    u8 stats_len;
    u8 menuright_len;
    u8 menuleft_len;
    u8 minimap_len;
    u8 faster_len;
    u8 pause_len;
};
extern struct MouseInputMap MouseInputMap_default;

typedef struct controllerMouse {
    struct MouseInputMap inputmap;
    i32 controller_type;
} controllerMouse;
extern struct controllerMouse controllerMouse_default;

void Mouse_checkButton(struct controllerMouse *mouse, u8 button);

#endif /* CONTROLLERMOUSE_H */
