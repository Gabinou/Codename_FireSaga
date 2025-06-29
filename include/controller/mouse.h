#ifndef CONTROLLERMOUSE_H
#define CONTROLLERMOUSE_H

#include "enums.h"

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
extern const struct MouseInputMap MouseInputMap_default;

typedef struct controllerMouse {
    i32 controller_type;
    struct MouseInputMap inputmap;
} controllerMouse;
extern const struct controllerMouse controllerMouse_default;

void Mouse_checkButton(struct controllerMouse *mouse, u8 button);

#endif /* CONTROLLERMOUSE_H */
