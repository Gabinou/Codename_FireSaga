#ifndef CONTROLLERMOUSE_H
#define CONTROLLERMOUSE_H

#include <math.h>
#include <stdbool.h>
#include "macros.h"
#include "debug.h"
#include "tnecs.h"
#include "enums.h"
#include "nmath.h"
#include "events.h"
#include "SDL2/SDL.h"

extern struct MouseInputMap {
    uf8 accept;
    uf8 cancel;
    uf8 stats;
    uf8 trigger_right;
    uf8 trigger_left;
    uf8 minimap;
    uf8 faster;
    uf8 pause;

    uf8 accept_len;
    uf8 cancel_len;
    uf8 stats_len;
    uf8 menuright_len;
    uf8 menuleft_len;
    uf8 minimap_len;
    uf8 faster_len;
    uf8 pause_len;
} MouseInputMap_default;

typedef struct controllerMouse {
    struct MouseInputMap inputmap;
    if32 controller_type;
} controllerMouse;
extern struct controllerMouse controllerMouse_default;

extern void Mouse_checkButton(struct controllerMouse *mouse, u8 button);

#endif /* CONTROLLERMOUSE_H */
